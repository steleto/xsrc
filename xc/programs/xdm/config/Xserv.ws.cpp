XCOMM $XConsortium: Xserv.ws.cpp,v 1.3 93/09/28 14:30:30 gildea Exp $
XCOMM
XCOMM
XCOMM $XFree86: xc/programs/xdm/config/Xserv.ws.cpp,v 1.1.1.1.12.2 1998/10/04 15:23:14 hohndel Exp $
XCOMM
XCOMM Xservers file, workstation prototype
XCOMM
XCOMM This file should contain an entry to start the server on the
XCOMM local display; if you have more than one display (not screen),
XCOMM you can add entries to the list (one per line).  If you also
XCOMM have some X terminals connected which do not support XDMCP,
XCOMM you can add them here as well.  Each X terminal line should
XCOMM look like:
XCOMM	XTerminalName:0 foreign
XCOMM
:0 local BINDIR/X DEFAULTVT
