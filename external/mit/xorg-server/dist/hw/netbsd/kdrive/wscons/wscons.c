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
#include <unistd.h>
#include "kdrive.h"
#include "wskbd.h"
#include "wsmouse.h"

static int wsconsInit(void);
static void wsconsEnable(void);
static void wsconsDisable(void);
static void wsconsFini(void);
static void wsconsBell(int volume, int pitch, int duration);

static KdOsFuncs wsconsFuncs = {
	.Init       = wsconsInit,
	.Enable     = wsconsEnable,
	.SpecialKey = 0,
	.Disable    = wsconsDisable,
	.Fini       = wsconsFini,
	.pollEvents = 0,
	.Bell       = wsconsBell,
};

void
KdOsAddInputDrivers(void)
{
	KdAddKeyboardDriver(&wskbdDriver);
	KdAddPointerDriver(&wsmouseDriver);
}

void
OsVendorInit(void)
{
    KdOsInit(&wsconsFuncs);
}

static int
wsconsInit(void)
{
	return 1;
}

static void
wsconsEnable(void)
{
}

static void
wsconsDisable(void)
{
}

static void
wsconsFini(void)
{
}

/* ARGSUSED */
static void
wsconsBell(int volume, int pitch, int duration)
{
}
