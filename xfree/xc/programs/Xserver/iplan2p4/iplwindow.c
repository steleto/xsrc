/* $XFree86: xc/programs/Xserver/iplan2p4/iplwindow.c,v 3.0 1996/08/18 01:55:13 dawes Exp $ */
/* $XConsortium: iplwindow.c,v 5.22 94/04/17 20:29:07 dpw Exp $ */
/***********************************************************

Copyright (c) 1987  X Consortium

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL THE
X CONSORTIUM BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN
AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

Except as contained in this notice, the name of the X Consortium shall not be
used in advertising or otherwise to promote the sale, use or other dealings
in this Software without prior written authorization from the X Consortium.


Copyright 1987 by Digital Equipment Corporation, Maynard, Massachusetts.

                        All Rights Reserved

Permission to use, copy, modify, and distribute this software and its 
documentation for any purpose and without fee is hereby granted, 
provided that the above copyright notice appear in all copies and that
both that copyright notice and this permission notice appear in 
supporting documentation, and that the name of Digital not be
used in advertising or publicity pertaining to distribution of the
software without specific, written prior permission.  

DIGITAL DISCLAIMS ALL WARRANTIES WITH REGARD TO THIS SOFTWARE, INCLUDING
ALL IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS, IN NO EVENT SHALL
DIGITAL BE LIABLE FOR ANY SPECIAL, INDIRECT OR CONSEQUENTIAL DAMAGES OR
ANY DAMAGES WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS,
WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION,
ARISING OUT OF OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS
SOFTWARE.

******************************************************************/

/* Modified nov 94 by Martin Schaller (Martin_Schaller@maus.r.de) for use with
interleaved planes */

#include "X.h"
#include "scrnintstr.h"
#include "windowstr.h"
#include "ipl.h"
#include "mistruct.h"
#include "regionstr.h"
#include "iplmskbits.h"

extern WindowPtr *WindowTable;

Bool
iplCreateWindow(pWin)
    WindowPtr pWin;
{
    iplPrivWin *pPrivWin;

    pPrivWin = iplGetWindowPrivate(pWin);
    pPrivWin->pRotatedBorder = NullPixmap;
    pPrivWin->pRotatedBackground = NullPixmap;
    pPrivWin->fastBackground = FALSE;
    pPrivWin->fastBorder = FALSE;
    pPrivWin->oldRotate.x = 0;
    pPrivWin->oldRotate.y = 0;

#ifdef PIXMAP_PER_WINDOW
    /* Setup pointer to Screen pixmap */
    pWin->devPrivates[frameWindowPrivateIndex].ptr =
	(pointer) iplGetScreenPixmap(pWin->drawable.pScreen);
#endif

    return TRUE;
}

Bool
iplDestroyWindow(pWin)
    WindowPtr pWin;
{
    iplPrivWin *pPrivWin;

    pPrivWin = iplGetWindowPrivate(pWin);

    if (pPrivWin->pRotatedBorder)
	(*pWin->drawable.pScreen->DestroyPixmap)(pPrivWin->pRotatedBorder);
    if (pPrivWin->pRotatedBackground)
	(*pWin->drawable.pScreen->DestroyPixmap)(pPrivWin->pRotatedBackground);
    return(TRUE);
}

/*ARGSUSED*/
Bool
iplMapWindow(pWindow)
    WindowPtr pWindow;
{
    return(TRUE);
}

/* (x, y) is the upper left corner of the window on the screen 
   do we really need to pass this?  (is it a;ready in pWin->absCorner?)
   we only do the rotation for pixmaps that are 32 bits wide (padded
or otherwise.)
   iplChangeWindowAttributes() has already put a copy of the pixmap
in pPrivWin->pRotated*
*/
/*ARGSUSED*/
Bool
iplPositionWindow(pWin, x, y)
    WindowPtr pWin;
    int x, y;
{
    iplPrivWin *pPrivWin;
    int setxy = 0;

    pPrivWin = iplGetWindowPrivate(pWin);
    if (pWin->backgroundState == BackgroundPixmap && pPrivWin->fastBackground)
    {
	iplXRotatePixmap(pPrivWin->pRotatedBackground,
		      pWin->drawable.x - pPrivWin->oldRotate.x);
	iplYRotatePixmap(pPrivWin->pRotatedBackground,
		      pWin->drawable.y - pPrivWin->oldRotate.y);
	setxy = 1;
    }

    if (!pWin->borderIsPixel &&	pPrivWin->fastBorder)
    {
	while (pWin->backgroundState == ParentRelative)
	    pWin = pWin->parent;
	iplXRotatePixmap(pPrivWin->pRotatedBorder,
		      pWin->drawable.x - pPrivWin->oldRotate.x);
	iplYRotatePixmap(pPrivWin->pRotatedBorder,
		      pWin->drawable.y - pPrivWin->oldRotate.y);
	setxy = 1;
    }
    if (setxy)
    {
	pPrivWin->oldRotate.x = pWin->drawable.x;
	pPrivWin->oldRotate.y = pWin->drawable.y;
    }
    return (TRUE);
}

/*ARGSUSED*/
Bool
iplUnmapWindow(pWindow)
    WindowPtr pWindow;
{
    return (TRUE);
}

/* UNCLEAN!
   this code calls the bitblt helper code directly.

   iplCopyWindow copies only the parts of the destination that are
visible in the source.
*/


void 
iplCopyWindow(pWin, ptOldOrg, prgnSrc)
    WindowPtr pWin;
    DDXPointRec ptOldOrg;
    RegionPtr prgnSrc;
{
    DDXPointPtr pptSrc;
    register DDXPointPtr ppt;
    RegionRec rgnDst;
    register BoxPtr pbox;
    register int dx, dy;
    register int i, nbox;
    WindowPtr pwinRoot;

    pwinRoot = WindowTable[pWin->drawable.pScreen->myNum];

    REGION_INIT(pWin->drawable.pScreen, &rgnDst, NullBox, 0);

    dx = ptOldOrg.x - pWin->drawable.x;
    dy = ptOldOrg.y - pWin->drawable.y;
    REGION_TRANSLATE(pWin->drawable.pScreen, prgnSrc, -dx, -dy);
    REGION_INTERSECT(pWin->drawable.pScreen, &rgnDst, &pWin->borderClip, prgnSrc);

    pbox = REGION_RECTS(&rgnDst);
    nbox = REGION_NUM_RECTS(&rgnDst);
    if(!nbox || !(pptSrc = (DDXPointPtr )ALLOCATE_LOCAL(nbox * sizeof(DDXPointRec))))
    {
	REGION_UNINIT(pWin->drawable.pScreen, &rgnDst);
	return;
    }
    ppt = pptSrc;

    for (i = nbox; --i >= 0; ppt++, pbox++)
    {
	ppt->x = pbox->x1 + dx;
	ppt->y = pbox->y1 + dy;
    }

    iplDoBitbltCopy((DrawablePtr)pwinRoot, (DrawablePtr)pwinRoot,
		GXcopy, &rgnDst, pptSrc, ~0L);
    DEALLOCATE_LOCAL(pptSrc);
    REGION_UNINIT(pWin->drawable.pScreen, &rgnDst);
}



/* swap in correct PaintWindow* routine.  If we can use a fast output
routine (i.e. the pixmap is paddable to 32 bits), also pre-rotate a copy
of it in devPrivates[iplWindowPrivateIndex].ptr.
*/
Bool
iplChangeWindowAttributes(pWin, mask)
    WindowPtr pWin;
    unsigned long mask;
{
    register unsigned long index;
    register iplPrivWin *pPrivWin;
    int width;
    WindowPtr	pBgWin;

    pPrivWin = iplGetWindowPrivate(pWin);

    /*
     * When background state changes from ParentRelative and
     * we had previously rotated the fast border pixmap to match
     * the parent relative origin, rerotate to match window
     */
    if (mask & (CWBackPixmap | CWBackPixel) &&
	pWin->backgroundState != ParentRelative &&
	pPrivWin->fastBorder &&
	(pPrivWin->oldRotate.x != pWin->drawable.x ||
	 pPrivWin->oldRotate.y != pWin->drawable.y))
    {
	iplXRotatePixmap(pPrivWin->pRotatedBorder,
		      pWin->drawable.x - pPrivWin->oldRotate.x);
	iplYRotatePixmap(pPrivWin->pRotatedBorder,
		      pWin->drawable.y - pPrivWin->oldRotate.y);
	pPrivWin->oldRotate.x = pWin->drawable.x;
	pPrivWin->oldRotate.y = pWin->drawable.y;
    }
    while(mask)
    {
	index = lowbit (mask);
	mask &= ~index;
	switch(index)
	{
	case CWBackPixmap:
	    if (pWin->backgroundState == None)
	    {
		pPrivWin->fastBackground = FALSE;
	    }
	    else if (pWin->backgroundState == ParentRelative)
	    {
		pPrivWin->fastBackground = FALSE;
		/* Rotate border to match parent origin */
		if (pPrivWin->pRotatedBorder) {
		    for (pBgWin = pWin->parent;
			 pBgWin->backgroundState == ParentRelative;
			 pBgWin = pBgWin->parent);
		    iplXRotatePixmap(pPrivWin->pRotatedBorder,
				  pBgWin->drawable.x - pPrivWin->oldRotate.x);
		    iplYRotatePixmap(pPrivWin->pRotatedBorder,
				  pBgWin->drawable.y - pPrivWin->oldRotate.y);
		    pPrivWin->oldRotate.x = pBgWin->drawable.x;
		    pPrivWin->oldRotate.y = pBgWin->drawable.y;
		}
	    }
	    else if (((width = (pWin->background.pixmap->drawable.width))
		      <= INTER_PGSZ) && !(width & (width - 1)))
	    {
		iplCopyRotatePixmap(pWin->background.pixmap,
				    &pPrivWin->pRotatedBackground,
				    pWin->drawable.x,
				    pWin->drawable.y);
		if (pPrivWin->pRotatedBackground)
		{
		    pPrivWin->fastBackground = TRUE;
		    pPrivWin->oldRotate.x = pWin->drawable.x;
		    pPrivWin->oldRotate.y = pWin->drawable.y;
		}
		else
		{
		    pPrivWin->fastBackground = FALSE;
		}
	    }
	    else
	    {
		pPrivWin->fastBackground = FALSE;
	    }
	    break;

	case CWBackPixel:
	    pPrivWin->fastBackground = FALSE;
	    break;

	case CWBorderPixmap:
	    if (((width = (pWin->border.pixmap->drawable.width)) <= INTER_PGSZ) &&
		!(width & (width - 1)))
	    {
		for (pBgWin = pWin;
		     pBgWin->backgroundState == ParentRelative;
		     pBgWin = pBgWin->parent);
		iplCopyRotatePixmap(pWin->border.pixmap,
				    &pPrivWin->pRotatedBorder,
				    pBgWin->drawable.x,
				    pBgWin->drawable.y);
		if (pPrivWin->pRotatedBorder)
		{
		    pPrivWin->fastBorder = TRUE;
		    pPrivWin->oldRotate.x = pBgWin->drawable.x;
		    pPrivWin->oldRotate.y = pBgWin->drawable.y;
		}
		else
		{
		    pPrivWin->fastBorder = FALSE;
		}
	    }
	    else
	    {
		pPrivWin->fastBorder = FALSE;
	    }
	    break;
	 case CWBorderPixel:
	    pPrivWin->fastBorder = FALSE;
	    break;
	}
    }
    return (TRUE);
}

