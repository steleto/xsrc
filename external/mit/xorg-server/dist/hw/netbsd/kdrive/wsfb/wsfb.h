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

#ifndef _WSFB_H_
#define _WSFB_H_

#include <sys/time.h>
#include <dev/wscons/wsconsio.h>
#include "kdrive.h"
#include "screenint.h"

#if defined(__NetBSD__)
#define WSFB_DEFAULT_DEV "/dev/ttyE0"
#else
#define WSFB_DEFAULT_DEV "/dev/ttyC0"
#endif

extern char *wsfbDevicePath;

Bool wsfbCardInit(KdCardInfo *card);
Bool wsfbScreenInit(KdScreenInfo *screen);
Bool wsfbInitScreen(ScreenPtr pScreen);
Bool wsfbFinishInitScreen(ScreenPtr pScreen);
Bool wsfbCreateResources(ScreenPtr pScreen);
void wsfbPreserve(KdCardInfo *card);
Bool wsfbEnable(ScreenPtr pScreen);
Bool wsfbDPMS(ScreenPtr pScreen, int mode);
void wsfbDisable(ScreenPtr pScreen);
void wsfbRestore(KdCardInfo *card);
void wsfbScreenFini(KdScreenInfo *screen);
void wsfbCardFini(KdCardInfo *card);

void wsfbGetColors(ScreenPtr pScreen, int n, xColorItem *pdefs);
void wsfbPutColors(ScreenPtr pScreen, int n, xColorItem *pdefs);

#endif /* _WSFB_H_ */
