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

#include <errno.h>
#include <termios.h>
#include <kdrive.h>
#include <sys/ioctl.h>
#include <dev/wscons/wsconsio.h>
#include <xkbsrv.h>
#include "wskbd.h"
#include "wskeymap.h"

typedef struct _wskbdPriv {
	int fd;
	struct termios kbdtty;
}  wskbdPriv;


static Bool wskbdInit(KdKeyboardInfo *ki);
static Bool wskbdEnable(KdKeyboardInfo *ki);
static void wskbdLeds(KdKeyboardInfo *ki, int leds);
static void wskbdDisable(KdKeyboardInfo *ki);
static void wskbdFini(KdKeyboardInfo *ki);

static Bool wskbdInitialize(KdKeyboardInfo *ki, wskbdPriv *priv);
static int wskbdOpen(const char *dev);
static void wskbdRead(int fd, void *closure);


KdKeyboardDriver wskbdDriver = {
	.name    = "wskbd",
	.Init    = wskbdInit,
	.Enable  = wskbdEnable,
	.Leds    = wskbdLeds,
	.Disable = wskbdDisable,
	.Fini    = wskbdFini
};

int wskbdDevicePath;


static Bool
wskbdInit(KdKeyboardInfo *ki)
{
	wskbdPriv *priv = NULL;

	if (!ki)
		goto fail;

	free(ki->path);
	ki->path = strdup("wskbd");
	free(ki->name);
	ki->name = strdup("generic keyboard support in wscons");

	priv = (wskbdPriv *)calloc(1, sizeof(wskbdPriv));
	if (!priv)
		goto fail;

	if (!wskbdInitialize(ki, priv))
		goto fail;

	ki->driverPrivate = priv;
	return Success;

fail:
	wskbdFini(ki);
	return !Success;
}

static Bool
wskbdEnable(KdKeyboardInfo *ki)
{
	wskbdPriv *priv = ki->driverPrivate;
	struct termios tty;
	KeySym keymap[KD_MAX_LENGTH  * KD_MAX_WIDTH];
	KeySymsRec keySyms = {
		.map        = keymap,
		.minKeyCode = 0,
		.maxKeyCode = KD_MAX_LENGTH - 1,
		.mapWidth   = KD_MAX_WIDTH
	};
	CARD8 modMap[KD_MAX_LENGTH];
	int option;

	tty = priv->kbdtty;
	tty.c_iflag = IGNPAR | IGNBRK;
	tty.c_oflag = 0;
	tty.c_cflag = CREAD | CS8;
	tty.c_lflag = 0;
	tty.c_cc[VTIME] = 0;
	tty.c_cc[VMIN] = 1;
	cfsetispeed(&tty, 9600);
	cfsetospeed(&tty, 9600);
	if (tcsetattr(priv->fd, TCSANOW, &tty) < 0)
		ErrorF("wskbdEnable: tcsetattr: %s\n", strerror(errno));

	if (!wskeymapInitKeySymRec(priv->fd, &keySyms))
		goto fail;

	if (!wskeymapInitModMap(&keySyms, modMap))
		goto fail;

	XkbApplyMappingChange(ki->dixdev, &keySyms, keySyms.minKeyCode,
	    keySyms.maxKeyCode - keySyms.minKeyCode + 1, modMap,
	    serverClient);

//	option = WSKBD_RAW;
//	ioctl(priv->fd, WSKBDIO_SETMODE, &option);

	ki->minScanCode = keySyms.minKeyCode;
	ki->maxScanCode = keySyms.maxKeyCode;

//	ki->minScanCode = KD_MIN_KEYCODE;
//	ki->maxScanCode = KD_MAX_KEYCODE;

	KdRegisterFd(priv->fd, wskbdRead, ki);

	return Success;

fail:
	return !Success;
}

/* ARGSUSED */
static void
wskbdLeds(KdKeyboardInfo *ki, int leds)
{
}

static void
wskbdDisable(KdKeyboardInfo *ki)
{
	wskbdPriv *priv = ki->driverPrivate;
	int option;

	option = WSKBD_TRANSLATED;
	ioctl(priv->fd, WSKBDIO_SETMODE, &option);

	tcsetattr(priv->fd, TCSANOW, &priv->kbdtty);

	KdUnregisterFd(ki, priv->fd, FALSE);
}


static void
wskbdFini(KdKeyboardInfo *ki)
{
	wskbdPriv *priv = NULL;

	if (!ki)
		return;

	priv = ki->driverPrivate;
	if (priv) {
		if (priv->fd >= 0)
			close(priv->fd);
		free(ki->driverPrivate);
		ki->driverPrivate = NULL;
	}
}

/* ARGSUSED */
static Bool
wskbdInitialize(KdKeyboardInfo *ki, wskbdPriv *priv)
{
	priv->fd = wskbdOpen("/dev/wskbd");
	if (priv->fd < 0)
		goto fail;

	tcgetattr(priv->fd, &priv->kbdtty);

        ki->xkbRules   = "xorg";
        ki->xkbModel   = "sl-c3x00";
//        ki->xkbLayout  = "us";
//        ki->xkbVariant = NULL;
//        ki->xkbOptions = NULL;

	return TRUE;

fail:
	if (priv->fd >= 0)
		close(priv->fd);

	priv->fd = -1;
	return FALSE;
}

static int
wskbdOpen(const char *dev)
{
	int fd = -1;

	fd = open(dev, O_RDONLY | O_NONBLOCK | O_EXCL);
	if (fd < 0) {
		ErrorF("wskbd: cannot open \"%s\"\n", dev);
		goto fail;
	}
	
#ifdef WSKBDIO_SETVERSION
	int version = WSKBDIO_EVENT_VERSION;
	if (ioctl(fd, WSKBDIO_SETVERSION, &version) == -1) {
		ErrorF("wskbd: cannot set version\n");
		goto fail;
	}
#endif

	return fd;

fail:
	if (fd >= 0)
		close(fd);

	return -1;
}

/* ARGSUSED */
static void
wskbdRead(int fd, void *closure)
{
	KdKeyboardInfo *ki = closure;
	wskbdPriv *priv = ki->driverPrivate;
	struct wscons_event events[64];
	int code;
	int i;
	ssize_t n;

	if ((n = read(priv->fd, events, sizeof(events))) > 0) {
		ErrorF("wskbd: read size=%d\n", n);
		n /= sizeof(struct wscons_event);

		for (i = 0; i < n; i++) {
			code = events[i].value + KD_MIN_KEYCODE;
//			code = wskeymapGetScancode(events[i].value) + KD_MIN_KEYCODE;
			switch (events[i].type) {
			case WSCONS_EVENT_KEY_UP:
				KdEnqueueKeyboardEvent(closure, code, 1);
				break;
			case WSCONS_EVENT_KEY_DOWN:
				KdEnqueueKeyboardEvent(closure, code, 0);
				break;
			default:
				ErrorF("wskbd: unknown type=%d\n",
				       events[i].value);
				break;
			}
			ErrorF("wskbd: type=%d value=%d\n",
			       events[i].type, events[i].value);
		}
	}
}
