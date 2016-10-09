/* kdrive-config.h.in: not at all generated.                      -*- c -*-
 */

#ifndef _KDRIVE_CONFIG_H_
#define _KDRIVE_CONFIG_H_

#include <dix-config.h>
#include <xkb-config.h>

/* Building kdrive server. */
#define KDRIVESERVER

/* Include framebuffer support in X servers */
#undef KDRIVEFBDEV

/* Enable touchscreen support */
#undef TOUCHSCREEN

/* Support tslib touchscreen abstraction library */
#undef TSLIB

/* Support KDrive kbd driver */
#undef KDRIVE_KBD

/* Support KDrive mouse driver */
#undef KDRIVE_MOUSE

/* Support KDrive evdev driver */
#undef KDRIVE_EVDEV

/* Verbose debugging output hilarity */
#undef DEBUG

/* Have the backtrace() function. */
#undef HAVE_BACKTRACE

/* Have execinfo.h for backtrace(). */
#undef HAVE_EXECINFO_H

/* Support RANDR extension */
#define RANDR 1

#endif /* _KDRIVE_CONFIG_H_ */
