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
#include "wsmouse.h"

#define NUMEVENTS 16		/* max # of ws events to read at once */

typedef struct _wsmousePriv {
	int fd;
	struct wscons_event lastEvent;
	int lastEventFraction;
	int ax, ay, az;
	int lastButtons;
} wsmousePriv;

static Status wsmouseInit(KdPointerInfo *pi);
static Status wsmouseEnable(KdPointerInfo *pi);
static void wsmouseDisable(KdPointerInfo *pi);
static void wsmouseFini(KdPointerInfo *pi);

static int wsmouseOpen(const char *dev);
static Status wsmouseInitialize(KdPointerInfo *pi, wsmousePriv *priv);
static void wsmouseRead(int port, void *closure);

int wsmouseDevicePath;

KdPointerDriver wsmouseDriver = {
	.name    = "wsmouse",
	.Init    = wsmouseInit,
	.Enable  = wsmouseEnable,
	.Disable = wsmouseDisable,
	.Fini    = wsmouseFini
};

static Status
wsmouseInit(KdPointerInfo *pi)
{
	wsmousePriv *priv = NULL;

	if (!pi)
		goto fail;

	priv = (wsmousePriv *)calloc(1, sizeof(wsmousePriv));
	if (!priv)
		goto fail;

	if (wsmouseInitialize(pi, priv) != Success)
		goto fail;

	pi->driverPrivate = priv;
	return Success;

fail:
	
	wsmouseFini(pi);
	return !Success;
}

static Status
wsmouseEnable(KdPointerInfo *pi)
{
	wsmousePriv *priv = pi->driverPrivate;

	if (!KdRegisterFd(priv->fd, wsmouseRead, pi))
		return BadAlloc;

	return Success;
}

static void
wsmouseDisable(KdPointerInfo *pi)
{
	wsmousePriv *priv = pi->driverPrivate;

	KdUnregisterFd (pi, priv->fd, TRUE);
}

static void
wsmouseFini(KdPointerInfo *pi)
{
	wsmousePriv *priv;
	
	if (!pi)
		return;

	priv = (wsmousePriv *)pi->driverPrivate;
	if (priv) {
		if (priv->fd >= 0) {
			close(priv->fd);
		}

		free(priv);
		pi->driverPrivate = NULL;
	}
	
}

static int
wsmouseOpen(const char *dev)
{
	int fd = -1;
	int version = WSMOUSE_EVENT_VERSION;

	fd = open(dev, O_RDWR | O_NONBLOCK);
	if (fd < 0) {
		ErrorF("%s: cannot open input device\n", dev);
		goto fail;
	}

	if (ioctl(fd, WSMOUSEIO_SETVERSION, &version) != 0) {
		ErrorF("%s: cannot set wsmouse event version\n", dev);
		goto fail;
	}

	return fd;

fail:
	if (fd >= 0) {
		close(fd);
	}
	return -1;
}

/* ARGSUSED */
static Status
wsmouseInitialize(KdPointerInfo *pi, wsmousePriv *priv)
{
	priv->fd = wsmouseOpen("/dev/wsmouse");
	if (priv->fd < 0)
		goto fail;

	return Success;

fail:
	if (priv->fd >= 0)
		close(priv->fd);

	priv->fd = -1;
	return !Success;
}

/* ARGSUSED */
static void
wsmouseRead(int port, void *closure)
{
	KdPointerInfo *pi = closure;
	wsmousePriv *priv = pi->driverPrivate;
	struct wscons_event eventList[NUMEVENTS];
	int fraction = priv->lastEventFraction;
	int buttons = priv->lastButtons;
	ssize_t n, c, i;
	unsigned char *pBuf;

	pBuf = (unsigned char *)eventList;

	if (fraction > 0)
		memcpy(pBuf, (void *)&priv->lastEvent, fraction);

	c = read(priv->fd, &pBuf[fraction], sizeof(eventList) - fraction);
	if (c < 0)
		return;

	n = c + fraction;

	buttons = priv->lastButtons;
	for (i = 0; i < n / sizeof(struct wscons_event); i++) {
		switch(eventList[i].type) {
		case WSCONS_EVENT_MOUSE_UP:
			buttons &= ~(1 << eventList[i].value);
			KdEnqueuePointerEvent(pi, buttons | KD_MOUSE_DELTA,
					      0, 0, 0);
			break;
		case WSCONS_EVENT_MOUSE_DOWN:
			buttons |= (1 << eventList[i].value);
			KdEnqueuePointerEvent(pi, buttons | KD_MOUSE_DELTA,
					      0, 0, 0);
			break;
		case WSCONS_EVENT_MOUSE_DELTA_X:
			KdEnqueuePointerEvent(pi, buttons | KD_MOUSE_DELTA,
					      eventList[i].value, 0, 0);
			break;
		case WSCONS_EVENT_MOUSE_DELTA_Y:
			KdEnqueuePointerEvent(pi, buttons | KD_MOUSE_DELTA,
					      0, -eventList[i].value, 0);
			break;
		case WSCONS_EVENT_MOUSE_ABSOLUTE_X:
			if (eventList[i].value == 4095) 
				break;
			priv->ax = eventList[i].value;
			KdEnqueuePointerEvent(pi, buttons,
					      priv->ax, priv->ay, priv->az);
			break;
		case WSCONS_EVENT_MOUSE_ABSOLUTE_Y:
			priv->ay = eventList[i].value;
			KdEnqueuePointerEvent(pi, buttons,
					      priv->ax, priv->ay, priv->az);
			break;
		case WSCONS_EVENT_MOUSE_DELTA_Z:
			KdEnqueuePointerEvent(pi, buttons | KD_MOUSE_DELTA,
					      0, 0, eventList[i].value);
			break;
		case WSCONS_EVENT_MOUSE_ABSOLUTE_Z:
			/* ignore those */
			break;
		case WSCONS_EVENT_MOUSE_DELTA_W:
			ErrorF("Relative W %d\n", eventList[i].value);
			break;
		default:
			ErrorF("bad wsmouse event type=%d\n",
			       eventList[i].type);
			break;
		}
	}

	fraction = n - i * sizeof(struct wscons_event);
	if (fraction > 0)
		memcpy(&priv->lastEvent, &eventList[i], fraction);

	priv->lastEventFraction = fraction;
	priv->lastButtons = buttons;
}
