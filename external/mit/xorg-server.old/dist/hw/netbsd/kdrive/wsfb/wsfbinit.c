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

#ifdef HAVE_CONFIG_H
#include <kdrive-config.h>
#endif
#include <kdrive.h>
#include "wsfb.h"

static KdCardFuncs wsfbFuncs = {
	.cardinit         = wsfbCardInit,
	.scrinit          = wsfbScreenInit,
	.initScreen       = wsfbInitScreen,
	.finishInitScreen = wsfbFinishInitScreen,
	.createRes        = wsfbCreateResources,
	.preserve         = wsfbPreserve,
	.enable           = wsfbEnable,
	.dpms             = wsfbDPMS,
	.disable          = wsfbDisable,
	.restore          = wsfbRestore,
	.scrfini          = wsfbScreenFini,
	.cardfini         = wsfbCardFini,

	.initCursor       = NULL,
	.enableCursor     = NULL,
	.disableCursor    = NULL,
	.finiCursor       = NULL,
	.recolorCursor    = NULL,

	.initAccel        = NULL,
	.enableAccel      = NULL,
	.disableAccel     = NULL,
	.finiAccel        = NULL,

	.getColors        = wsfbGetColors,
	.putColors        = wsfbPutColors,
};

void
InitCard(char *name)
{
	KdCardInfoAdd(&wsfbFuncs, 0);
}

void
InitOutput(ScreenInfo *screenInfo, int argc, char **argv)
{
	KdInitOutput(screenInfo, argc, argv);
}

void
InitInput(int argc, char **argv)
{
	KdOsAddInputDrivers();
	KdInitInput();
}

void
CloseInput(void)
{
}

#ifdef DDXBEFORERESET
void
ddxBeforeReset(void)
{
}
#endif

void
ddxUseMsg(void)
{
	KdUseMsg();
	ErrorF("\n");
	ErrorF("Xwsfb Device Usage:\n");
	ErrorF("-fb path         Wsfb device to use. Defaults to %s\n",
	       WSFB_DEFAULT_DEV);
	ErrorF("\n");
}

int
ddxProcessArgument(int argc, char **argv, int i)
{
	if (!strcmp(argv[i], "-fb"))
	{
		if (i + 1 < argc)
		{
			wsfbDevicePath = argv[i + 1];
			return 2;
		}
		UseMsg();
		exit(1);
	}

	return KdProcessArgument(argc, argv, i);
}
