/*-
 * Copyright (c) 2014 The NetBSD Foundation, Inc.
 * All rights reserved.
 *
 * This code is derived from software contributed to The NetBSD Foundation
 * by TOYOKURA Atsushi.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE NETBSD FOUNDATION, INC. AND CONTRIBUTORS
 * ``AS IS'' AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED
 * TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL THE FOUNDATION OR CONTRIBUTORS
 * BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */
#include <unistd.h>
#include <errno.h>
#include <sys/ioctl.h>
#include <sys/mman.h>
#include <sys/time.h>
#include <sys/types.h>
#include <dev/wscons/wsconsio.h>

#ifdef HAVE_CONFIG_H
#include <kdrive-config.h>
#endif
#include "wsfb.h"

typedef struct _wsfbPriv {
	int                       fd;
	struct wsdisplayio_fbinfo fbi;
	unsigned char             *fbmem;
	size_t                    fbmem_len;
} wsfbPriv;

typedef struct _wsfbScrPriv {
	Rotation randr;
	Bool shadow;
} wsfbScrPriv;

char *wsfbDevicePath = NULL;

static Bool wsfbSetShadow(ScreenPtr pScreen);

/* Open the framebuffer device. */
static int
wsfbOpen(char *dev)
{
	if (dev == NULL)
		dev = WSFB_DEFAULT_DEV;

	return open(dev, O_RDWR | O_NONBLOCK | O_EXCL);
}

static Bool
wsfbGetFbinfo(int fd, struct wsdisplayio_fbinfo *fbi)
{
	struct wsdisplay_fbinfo info;
	int wstype, linebytes;

	if (ioctl(fd, WSDISPLAYIO_GET_FBINFO, fbi) != 0) {
		ErrorF("ioctl(WSDISPLAYIO_GET_FBINFO) failed, "
		       "falling back to old method\n");

		if (ioctl(fd, WSDISPLAYIO_GINFO, &info) == -1) {
			ErrorF("ioctl WSDISPLAY_GINFO: %s\n",
			       strerror(errno));
			return FALSE;
		}
		if (ioctl(fd, WSDISPLAYIO_GTYPE, &wstype) == -1) {
			ErrorF("ioctl WSDISPLAY_GTYPE: %s\n",
			       strerror(errno));
			return FALSE;
		}
		if (ioctl(fd, WSDISPLAYIO_LINEBYTES, &linebytes) == -1) {
			ErrorF("ioctl WSDISPLAYIO_LINEBYTES: %s\n",
			       strerror(errno));
			return FALSE;
		}
		
		/* ok, fake up a new style fbinfo */
		fbi->fbi_width        = info.width;
		fbi->fbi_height       = info.height;
		fbi->fbi_stride       = linebytes;
		fbi->fbi_bitsperpixel = info.depth;

		if (info.depth > 16) {
			struct _fbi_rgbmasks *fbi_rgbmasks;

			fbi->fbi_pixeltype = WSFB_RGB;
			fbi_rgbmasks = &fbi->fbi_subtype.fbi_rgbmasks;
			switch (wstype) {
			case WSDISPLAY_TYPE_SUN24:
			case WSDISPLAY_TYPE_SUNCG12:
			case WSDISPLAY_TYPE_SUNCG14:
			case WSDISPLAY_TYPE_SUNTCX:
			case WSDISPLAY_TYPE_SUNFFB:
			case WSDISPLAY_TYPE_XVR1000:
			case WSDISPLAY_TYPE_VC4:
				fbi_rgbmasks->red_offset   = 0;
				fbi_rgbmasks->red_size     = 8;
				fbi_rgbmasks->green_offset = 8;
				fbi_rgbmasks->green_size   = 8;
				fbi_rgbmasks->blue_offset  = 16;
				fbi_rgbmasks->blue_size    = 8;
				break;
			default:
				fbi_rgbmasks->red_offset   = 16;
				fbi_rgbmasks->red_size     = 8;
				fbi_rgbmasks->green_offset = 8;
				fbi_rgbmasks->green_size   = 8;
				fbi_rgbmasks->blue_offset  = 0;
				fbi_rgbmasks->blue_size    = 8;
			}
			fbi_rgbmasks->alpha_offset = 0;
			fbi_rgbmasks->alpha_size = 0;
		} else if (info.depth == 16) {
			struct _fbi_rgbmasks *fbi_rgbmasks;

			fbi->fbi_pixeltype = WSFB_RGB;
			fbi_rgbmasks = &fbi->fbi_subtype.fbi_rgbmasks;
			switch (wstype) {
			default:
				fbi_rgbmasks->red_offset   = 11;
				fbi_rgbmasks->red_size     = 5;
				fbi_rgbmasks->green_offset = 5;
				fbi_rgbmasks->green_size   = 6;
				fbi_rgbmasks->blue_offset  = 0;
				fbi_rgbmasks->blue_size    = 5;
			}
			fbi_rgbmasks->alpha_offset = 0;
			fbi_rgbmasks->alpha_size = 0;
		} else if (info.depth <= 8) {
			fbi->fbi_pixeltype = WSFB_CI;
			fbi->fbi_subtype.fbi_cmapinfo.cmap_entries =
			    info.cmsize;
		}
		fbi->fbi_flags = 0;
		fbi->fbi_fbsize = linebytes * info.height;
	}

	return TRUE;
}
	
/* Map the framebuffer's memory. */
static pointer
wsfbMmap(size_t len, off_t off, int fd)
{
	int pagemask, mapsize;
	caddr_t addr;
	pointer mapaddr;

	pagemask = getpagesize() - 1;
	mapsize = ((int)len + pagemask) & ~pagemask;
	addr = 0;

	mapaddr =
	    mmap(addr, mapsize, PROT_READ | PROT_WRITE, MAP_SHARED, fd, off);

	if (mapaddr == MAP_FAILED)
		mapaddr = NULL;

	return mapaddr;
}

static Bool
wsfbInitialize(KdCardInfo *card, wsfbPriv *priv)
{
	priv->fd = wsfbOpen(wsfbDevicePath);
	if (priv->fd < 0) {
		ErrorF("Error opening wsdisplay framebuffer %s: %s\n",
		    wsfbDevicePath, strerror(errno));
		goto fail;
	}

	if (!wsfbGetFbinfo(priv->fd, &priv->fbi)) {
		ErrorF("Error ioctl(WSDISPLAYIO_GET_FBINFO) failed.\n");
		goto fail;
	}

	return TRUE;

fail:
	if (priv->fbmem != NULL)
		munmap(priv->fbmem, priv->fbmem_len);
	if (priv->fd >= 0)
		close(priv->fd);

	return FALSE;
}

Bool
wsfbCardInit(KdCardInfo *card)
{
	wsfbPriv *priv = NULL;

	priv = (wsfbPriv *)calloc(1, sizeof(wsfbPriv));
	if (!priv)
		goto fail;

	if (!wsfbInitialize(card, priv))
		goto fail;

	card->driver = priv;
	return TRUE;

fail:
	free(priv);
	return FALSE;
}

static Bool
wsfbMapFramebuffer(KdScreenInfo *screen, wsfbScrPriv *scrpriv)
{
	wsfbPriv *priv = screen->card->driver;
	KdPointerMatrix	m;

	if (scrpriv->randr != RR_Rotate_0)
		scrpriv->shadow = TRUE;
	else
		scrpriv->shadow = FALSE;

	KdComputePointerMatrix(&m, scrpriv->randr, screen->width,
			       screen->height);

	KdSetPointerMatrix(&m);

	screen->width  = priv->fbi.fbi_width;
	screen->height = priv->fbi.fbi_height;

	ErrorF("%s[%d] randr=0x%x width/height=%d/%d\n", __func__, __LINE__,
	       scrpriv->randr, screen->width, screen->height);
	if (scrpriv->shadow) {
		if (!KdShadowFbAlloc(screen,
		    scrpriv->randr & (RR_Rotate_90 | RR_Rotate_270)))
			return FALSE;
	} else {
		screen->fb.byteStride = priv->fbi.fbi_stride;
		screen->fb.pixelStride =
		    priv->fbi.fbi_stride * 8 / priv->fbi.fbi_bitsperpixel;
		screen->fb.frameBuffer = (CARD8 *)(priv->fbmem);
	}
	
	ErrorF("%s[%d] success\n", __func__, __LINE__);
	return TRUE;
}

static Bool
wsfbUnmapFramebuffer(KdScreenInfo *screen)
{
	KdShadowFbFree(screen);
	return TRUE;
}

#define Mask(offset, size)      (((1 << size) - 1) << offset)

static Bool
wsfbScreenInitialize(KdScreenInfo *screen, wsfbScrPriv *scrpriv)
{
	wsfbPriv *priv = screen->card->driver;
	struct wsdisplay_fbinfo fbinfo;
	struct _fbi_rgbmasks *m;
	Pixel allbits;
	int wsmode = WSDISPLAYIO_MODE_DUMBFB;
	int depth;

	screen->width  = priv->fbi.fbi_width;
	screen->height = priv->fbi.fbi_height;

	switch (priv->fbi.fbi_pixeltype) {
	case WSFB_RGB:
		m = &priv->fbi.fbi_subtype.fbi_rgbmasks;
		screen->fb.visuals   = (1 << TrueColor);
		screen->fb.redMask   = Mask(m->red_offset,   m->red_size);
		screen->fb.greenMask = Mask(m->green_offset, m->green_size);
		screen->fb.blueMask  = Mask(m->blue_offset,  m->blue_size);

		allbits = screen->fb.redMask | screen->fb.greenMask |
			  screen->fb.blueMask;
		depth = 32;
		while (depth && !(allbits & (1 << (depth - 1))))
			depth--;
		break;

	case WSFB_GREYSCALE:
	case WSFB_CI:
	default:
		break;
	}

	screen->fb.depth        = depth;
	screen->fb.bitsPerPixel = priv->fbi.fbi_bitsperpixel;

	scrpriv->randr = screen->randr;

	/* Switch to graphics mode - required before mmap. */
	if (ioctl(priv->fd, WSDISPLAYIO_SMODE, &wsmode) == -1) {
		ErrorF("ioctl WSDISPLAYIO_SMODE: %s\n",
		       strerror(errno));
		return FALSE;
	}
	
	priv->fbmem = wsfbMmap(priv->fbi.fbi_fbsize, 0, priv->fd);
	if (priv->fbmem == NULL) {
		ErrorF("Error mmap wsdisplay frmae buffer fail.\n");
		return FALSE;
	}
	priv->fbmem_len = priv->fbi.fbi_fbsize;

	return wsfbMapFramebuffer(screen, scrpriv);
}

static Bool
wsfbCreateColormap(ColormapPtr pmap)
{
	ScreenPtr pScreen = pmap->pScreen;
	KdScreenPriv(pScreen);
	wsfbPriv *priv = pScreenPriv->card->driver;

	switch (priv->fbi.fbi_pixeltype) {
	case WSFB_GREYSCALE:
	case WSFB_CI:
		return FALSE;

	case WSFB_RGB:
	default:
		return fbInitializeColormap(pmap);
	}
}

Bool
wsfbScreenInit(KdScreenInfo *screen)
{
	wsfbScrPriv *scrpriv = NULL;

	scrpriv = (wsfbScrPriv *)calloc(1, sizeof(wsfbScrPriv));
	if (!scrpriv)
		goto fail;

	if (!wsfbScreenInitialize(screen, scrpriv))
		goto fail;

	screen->driver = scrpriv;
	return TRUE;

fail:
	free(scrpriv);
	return FALSE;
}

Bool
wsfbInitScreen(ScreenPtr pScreen)
{
	pScreen->CreateColormap = wsfbCreateColormap;
	return TRUE;
}

#ifdef RANDR
static Bool
wsfbRandRGetInfo(ScreenPtr pScreen, Rotation *rotations)
{
	KdScreenPriv(pScreen);
	KdScreenInfo *screen = pScreenPriv->screen;
	wsfbScrPriv *scrpriv = screen->driver;
	RRScreenSizePtr pSize;
	Rotation randr;
	int n;

	*rotations = RR_Rotate_All | RR_Reflect_All;

	ErrorF("%s[%d] start\n", __func__, __LINE__);

	for (n = 0; n < pScreen->numDepths; n++)
		if (pScreen->allowedDepths[n].numVids)
			break;
	if (n == pScreen->numDepths)
		return FALSE;

	ErrorF("%s[%d]\n", __func__, __LINE__);

	pSize = RRRegisterSize(pScreen, screen->width, screen->height,
			       screen->width_mm, screen->height_mm);

	randr = KdSubRotation(scrpriv->randr, screen->randr);

	RRSetCurrentConfig(pScreen, randr, 0, pSize);

	ErrorF("%s[%d] end\n", __func__, __LINE__);

	return TRUE;
}

static void
wsfbSetScreenSizes(ScreenPtr pScreen)
{
	KdScreenPriv(pScreen);
	KdScreenInfo *screen = pScreenPriv->screen;
	wsfbScrPriv *scrpriv = screen->driver;
	wsfbPriv *priv = screen->card->driver;

	if (scrpriv->randr & (RR_Rotate_0 | RR_Rotate_180)) {
		pScreen->width    = priv->fbi.fbi_width;
		pScreen->height   = priv->fbi.fbi_height;
		pScreen->mmWidth  = screen->width_mm;
		pScreen->mmHeight = screen->height_mm;
	} else {
		pScreen->width    = priv->fbi.fbi_height;
		pScreen->height   = priv->fbi.fbi_width;
		pScreen->mmWidth  = screen->height_mm;
		pScreen->mmHeight = screen->width_mm;
	}
}

static Bool
wsfbRandRSetConfig(ScreenPtr pScreen, Rotation randr, int rate,
    RRScreenSizePtr pSize)
{
	KdScreenPriv(pScreen);
	KdScreenInfo *screen = pScreenPriv->screen;
	wsfbScrPriv *scrpriv = screen->driver;
	Bool wasEnabled = pScreenPriv->enabled;
	wsfbScrPriv oldscr;
	int oldwidth, oldheight, oldmmwidth, oldmmheight;
	int newwidth, newheight;

	ErrorF("%s[%d] start randr=0x%x rate=%d s->r=0x%x w/h=%d/%d\n",
	       __func__, __LINE__,
	       randr, rate, screen->randr, pSize->width, pSize->height);

	if (screen->randr & (RR_Rotate_0 | RR_Rotate_180)) {
		newwidth  = pSize->width;
		newheight = pSize->height;
	} else {
		newwidth  = pSize->height;
		newheight = pSize->width;
	}

	ErrorF("%s[%d] new w/h=%d/%d e=%d\n", __func__, __LINE__,
	       screen->randr, newwidth, newheight, wasEnabled);

	if (wasEnabled)
		KdDisableScreen(pScreen);

	oldscr = *scrpriv;

	oldwidth    = screen->width;
	oldheight   = screen->height;
	oldmmwidth  = pScreen->mmWidth;
	oldmmheight = pScreen->mmHeight;

	/*
	 * Set new configuration
	 */

	scrpriv->randr = KdAddRotation(screen->randr, randr);

	wsfbUnmapFramebuffer(screen);

	if (!wsfbMapFramebuffer(screen, scrpriv))
		goto bail4;

	KdShadowUnset(screen->pScreen);

	if (!wsfbSetShadow (screen->pScreen))
		goto bail4;

	wsfbSetScreenSizes(screen->pScreen);

	/*
	 * Set frame buffer mapping
	 */
	(*pScreen->ModifyPixmapHeader)(fbGetScreenPixmap(pScreen),
	     pScreen->width, pScreen->height, screen->fb.depth,
	     screen->fb.bitsPerPixel, screen->fb.byteStride,
	     screen->fb.frameBuffer);

	/* set the subpixel order */

	KdSetSubpixelOrder(pScreen, scrpriv->randr);
	if (wasEnabled)
		KdEnableScreen(pScreen);

	ErrorF("%s[%d] end\n", __func__, __LINE__);
	return TRUE;

bail4:
	wsfbUnmapFramebuffer(screen);
	*scrpriv = oldscr;
	(void)wsfbMapFramebuffer(screen, scrpriv);
	pScreen->width    = oldwidth;
	pScreen->height   = oldheight;
	pScreen->mmWidth  = oldmmwidth;
	pScreen->mmHeight = oldmmheight;

	if (wasEnabled)
		KdEnableScreen(pScreen);
	return FALSE;
}

static Bool
wsfbRandRInit(ScreenPtr pScreen)
{
	rrScrPrivPtr    pScrPriv;

	ErrorF("%s[%d] start\n", __func__, __LINE__);
	if (!RRScreenInit(pScreen))
		return FALSE;

	pScrPriv = rrGetScrPriv(pScreen);
	pScrPriv->rrGetInfo   = wsfbRandRGetInfo;
	pScrPriv->rrSetConfig = wsfbRandRSetConfig;

	ErrorF("%s[%d] end\n", __func__, __LINE__);
	return TRUE;
}
#endif

Bool
wsfbFinishInitScreen(ScreenPtr pScreen)
{
	if (!shadowSetup(pScreen))
		return FALSE;
	
	ErrorF("%s[%d] before RandR\n", __func__, __LINE__);
#ifdef RANDR
	if (!wsfbRandRInit(pScreen))
		return FALSE;
#endif
	ErrorF("%s[%d] after RandR\n", __func__, __LINE__);

	return TRUE;
}

static void *
wsfbWindowLinear(ScreenPtr pScreen, CARD32 row, CARD32 offset, int mode,
     CARD32 *size, void *closure)
{
	KdScreenPriv(pScreen);
	wsfbPriv *priv = pScreenPriv->card->driver;

	if (!pScreenPriv->enabled)
		return 0;
	*size = priv->fbi.fbi_stride;
	return (CARD8 *)priv->fbmem + row * priv->fbi.fbi_stride + offset;
}

static Bool
wsfbSetShadow(ScreenPtr pScreen)
{
    KdScreenPriv(pScreen);
    KdScreenInfo *screen = pScreenPriv->screen;
    wsfbScrPriv *scrpriv = screen->driver;
    wsfbPriv *priv = screen->card->driver;
    ShadowUpdateProc update;
    ShadowWindowProc window;
    int useYX = 0;

#ifdef __arm__
	/* Use variant copy routines that always read left to right in the
	   shadow framebuffer.  Reading vertical strips is exceptionally
	   slow on XScale due to cache effects.  */
	useYX = 1;
#endif

	window = wsfbWindowLinear;
	update = 0;

	if (scrpriv->randr)
		if (priv->fbi.fbi_bitsperpixel == 16)
			switch (scrpriv->randr) {
			case RR_Rotate_90:
				if (useYX)
					update = shadowUpdateRotate16_90YX;
				else
					update =  shadowUpdateRotate16_90;
				break;
			case RR_Rotate_180:
				update = shadowUpdateRotate16_180;
				break;
			case RR_Rotate_270:
				if (useYX)
					update = shadowUpdateRotate16_270YX;
				else
					update =  shadowUpdateRotate16_270;
				break;
			default:
				update = shadowUpdateRotate16;
				break;
			}
		else
			update = shadowUpdateRotatePacked;
	else
		update = shadowUpdatePacked;

	return KdShadowSet(pScreen, scrpriv->randr, update, window);
}

Bool
wsfbCreateResources(ScreenPtr pScreen)
{
	return wsfbSetShadow(pScreen);
}

void
wsfbPreserve(KdCardInfo *card)
{
}

Bool
wsfbEnable(ScreenPtr pScreen)
{
	return TRUE;
}

Bool
wsfbDPMS(ScreenPtr pScreen, int mode)
{
	return TRUE;
}

void
wsfbDisable(ScreenPtr pScreen)
{
}

void
wsfbRestore(KdCardInfo *card)
{
}

void
wsfbScreenFini(KdScreenInfo *screen)
{
	wsfbPriv *priv = screen->card->driver;
	int wsmode;

	/* Restore the text mode. */
	wsmode = WSDISPLAYIO_MODE_EMUL;
	if (ioctl(priv->fd, WSDISPLAYIO_SMODE, &wsmode) == -1)
		ErrorF("error setting text mode %s\n", strerror(errno));
}

void
wsfbCardFini(KdCardInfo *card)
{
	wsfbPriv *priv = (wsfbPriv *)card->driver;

	munmap(priv->fbmem, priv->fbmem_len);
	close(priv->fd);
	free(priv);
}

void wsfbGetColors(ScreenPtr pScreen, int n, xColorItem *pdefs)
{
}

void wsfbPutColors(ScreenPtr pScreen, int n, xColorItem *pdefs)
{
}
