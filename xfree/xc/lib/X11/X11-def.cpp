#ifndef __CYGWIN__
LIBRARY X11
#else
LIBRARY libX11
#endif
VERSION LIBRARY_VERSION
EXPORTS
	XSaveContext   
	XFindContext   
	XDeleteContext   
	XListDepths   
	XrmParseCommand   
	Xpermalloc   
	_XrmInternalStringToQuark   
	XrmStringToQuark   
	XrmPermStringToQuark   
	XrmUniqueQuark   
	XrmQuarkToString   
	XAllocColorCells   
	XAllocColorPlanes   
	XAllowEvents   
	XAutoRepeatOn   
	XAutoRepeatOff   
	XSetWindowBackground   
	XSetWindowBorderWidth   
	XBell   
	XSetWindowBorder   
	XEnableAccessControl   
	XDisableAccessControl   
	XSetAccessControl   
	XSetCloseDownMode   
	XChangeActivePointerGrab   
	XSetWindowColormap   
	XChangeGC   
	XChangeKeyboardControl   
	XChangePointerControl   
	XChangeProperty   
	XChangeSaveSet   
	XAddToSaveSet   
	XRemoveFromSaveSet   
	XChangeWindowAttributes   
	XResizeWindow   
	XCheckIfEvent   
	XCheckMaskEvent   
	XCheckWindowEvent   
	XCheckTypedEvent   
	XCheckTypedWindowEvent   
	XCirculateSubwindows   
	XCirculateSubwindowsDown   
	XCirculateSubwindowsUp   
	XCloseDisplay   
	XClearWindow   
	XClearArea   
	XMoveResizeWindow   
	_XConnectDisplay   
	_XDisconnectDisplay   
	_XSendClientPrefix   
	XSetAuthorization   
	XConvertSelection   
	XCopyArea   
	XCopyColormapAndFree   
	XCopyGC   
	XCopyPlane   
	XCreateBitmapFromData   
	XCreateColormap   
	XCreatePixmapCursor   
	XCreateGC   
	_XUpdateGCCache   
	_XFlushGCCache   
	XFlushGC   
	XGContextFromGC   
	XCreateGlyphCursor   
	XCreatePixmapFromBitmapData   
	XCreatePixmap   
	XCreateSimpleWindow   
	XCreateFontCursor   
	XDefineCursor   
	XDeleteProperty   
	XDestroySubwindows   
	XDestroyWindow   
	XDisplayName   
	XDrawArc   
	XDrawArcs   
	XDrawLine   
	XDrawLines   
	XDrawPoint   
	XDrawPoints   
	XDrawRectangle   
	XDrawRectangles   
	XDrawSegments   
	XGetErrorText   
	XGetErrorDatabaseText   
	XSetErrorHandler   
	XSetIOErrorHandler   
	_XEventToWire   
	XFetchName   
	XGetIconName   
	XFillArc   
	XFillArcs   
	XFillPolygon   
	XFillRectangle   
	XFillRectangles   
	XFlush   
	XLoadQueryFont   
	XFreeFont   
	XQueryFont   
	XListFonts   
	XFreeFontNames   
	XListFontsWithInfo   
	XFreeFontInfo   
	XFreeColormap   
	XFreeColors   
	XFreeCursor   
	_XFreeExtData   
	XFreeGC   
	XFreePixmap   
	XActivateScreenSaver   
	XResetScreenSaver   
	XForceScreenSaver   
	XSetArcMode   
	XSetFillRule   
	XSetFillStyle   
	XSetGraphicsExposures   
	XSetSubwindowMode   
	XGeometry   
	XGetAtomName   
	XAllocNamedColor   
	XGetDefault   
	XGetFontPath   
	XFreeFontPath   
	XGetFontProperty   
	XGetGeometry   
	XAllocColor   
	XGetSizeHints   
	XGetWMHints   
	XGetZoomHints   
	XGetNormalHints   
	XGetIconSizes   
	XGetCommand   
	XGetTransientForHint   
	XGetClassHint   
	XGetInputFocus   
	XGetImage   
	XGetSubImage   
	XGetKeyboardControl   
	XGetMotionEvents   
	XGetPointerControl   
	XGetPointerMapping   
	XGetKeyboardMapping   
	XGetWindowProperty   
	XGetSelectionOwner   
	XGetScreenSaver   
	XGetStandardColormap   
	XGetWindowAttributes   
	XGrabButton   
	XGrabKey   
	XGrabKeyboard   
	XGrabPointer   
	XGrabServer   
	XAddHost   
	XRemoveHost   
	XAddHosts   
	XRemoveHosts   
	XIfEvent   
	XCreateImage   
	_XSetImage   
	_XInitImageFuncPtrs   
	XDrawImageString   
	XDrawImageString16   
	XInitExtension   
	XAddExtension   
	XEHeadOfExtensionList   
	XAddToExtensionList   
	XFindOnExtensionList   
	XESetCreateGC   
	XESetCopyGC   
	XESetFlushGC   
	XESetFreeGC   
	XESetCreateFont   
	XESetFreeFont   
	XESetCloseDisplay   
	XESetWireToEvent   
	XESetEventToWire   
	XESetWireToError   
	XESetError   
	XESetErrorString   
	XESetPrintErrorValues   
	XInstallColormap   
	XInternAtom   
	XKeycodeToKeysym   
	XKeysymToKeycode   
	XLookupKeysym   
	XRefreshKeyboardMapping   
	XLookupString   
	XRebindKeysym   
	XKeysymToString   
	XKillClient   
	XListHosts   
	XListInstalledColormaps   
	XListProperties   
	XListExtensions   
	XFreeExtensionList   
	XLoadFont   
	XLookupColor   
	XLowerWindow   
	XConnectionNumber   
	XRootWindow   
	XDefaultScreen   
	XDefaultRootWindow   
	XDefaultVisual   
	XDefaultGC   
	XBlackPixel   
	XWhitePixel   
	XAllPlanes   
	XQLength   
	XDisplayWidth   
	XDisplayHeight   
	XDisplayWidthMM   
	XDisplayHeightMM   
	XDisplayPlanes   
	XDisplayCells   
	XScreenCount   
	XServerVendor   
	XProtocolVersion   
	XProtocolRevision   
	XVendorRelease   
	XDisplayString   
	XDefaultDepth   
	XDefaultColormap   
	XBitmapUnit   
	XBitmapBitOrder   
	XBitmapPad   
	XImageByteOrder   
	XNextRequest   
	XLastKnownRequestProcessed   
	XScreenOfDisplay   
	XDefaultScreenOfDisplay   
	XDisplayOfScreen   
	XRootWindowOfScreen   
	XBlackPixelOfScreen   
	XWhitePixelOfScreen   
	XDefaultColormapOfScreen   
	XDefaultDepthOfScreen   
	XDefaultGCOfScreen   
	XDefaultVisualOfScreen   
	XWidthOfScreen   
	XHeightOfScreen   
	XWidthMMOfScreen   
	XHeightMMOfScreen   
	XPlanesOfScreen   
	XCellsOfScreen   
	XMinCmapsOfScreen   
	XMaxCmapsOfScreen   
	XDoesSaveUnders   
	XDoesBackingStore   
	XEventMaskOfScreen   
	XScreenNumberOfScreen   
	XDestroyImage   
	XGetPixel   
	XPutPixel   
	XSubImage   
	XAddPixel   
	XNoOp   
	XMapRaised   
	XMapSubwindows   
	XMapWindow   
	XMaskEvent   
	XMaxRequestSize   
	XResourceManagerString   
	XDisplayMotionBufferSize   
	XDisplayKeycodes   
	XVisualIDFromVisual   
	XGetModifierMapping   
	XSetModifierMapping   
	XNewModifiermap   
	XFreeModifiermap   
	XInsertModifiermapEntry   
	XDeleteModifiermapEntry   
	XMoveWindow   
	XNextEvent   
	XOpenDisplay   
	_XFreeDisplayStructure   
	XParseColor   
	XParseGeometry   
	XPeekEvent   
	XPeekIfEvent   
	XEventsQueued   
	XPending   
	XSetWindowBackgroundPixmap   
	XSetWindowBorderPixmap   
	XPolygonRegion   
	XDrawText   
	XDrawText16   
	XPutBackEvent   
	_XReverse_Bytes   
	XPutImage   
	XQueryBestSize   
	XQueryColor   
	XQueryColors   
	XQueryBestCursor   
	XQueryExtension   
	XQueryKeymap   
	XQueryPointer   
	XQueryBestStipple   
	XQueryTextExtents16   
	XQueryTextExtents   
	XQueryBestTile   
	XQueryTree   
	XRaiseWindow   
	XReadBitmapFile   
	XRecolorCursor   
	XConfigureWindow   
	XCreateRegion   
	XClipBox   
	XUnionRectWithRegion   
	XSetRegion   
	XDestroyRegion   
	XOffsetRegion   
	XShrinkRegion   
	XIntersectRegion   
	XUnionRegion   
	XSubtractRegion   
	XXorRegion   
	XEmptyRegion   
	XEqualRegion   
	XPointInRegion   
	XRectInRegion   
	XReparentWindow   
	XRestackWindows   
	XRotateWindowProperties   
	XScreenResourceString   
	XSelectInput   
	XSendEvent   
	XSetBackground   
	XSetClipRectangles   
	_XSetClipRectangles   
	XSetClipMask   
	XSetClipOrigin   
	XSetDashes   
	XSetFontPath   
	XSetFont   
	XSetForeground   
	XSetFunction   
	XSetSizeHints   
	XSetWMHints   
	XSetZoomHints   
	XSetNormalHints   
	XSetIconSizes   
	XSetCommand   
	XSetStandardProperties   
	XSetTransientForHint   
	XSetClassHint   
	XSetInputFocus   
	XSetLineAttributes   
	XSetPlaneMask   
	XSetPointerMapping   
	XChangeKeyboardMapping   
	XSetSelectionOwner   
	XSetScreenSaver   
	XSetState   
	XSetStipple   
	XSetStandardColormap   
	XSetTile   
	XSetTSOrigin   
	XRotateBuffers   
	XFetchBuffer   
	XFetchBytes   
	XStoreBuffer   
	XStoreBytes  
	XStoreColor   
	XStoreColors   
	XStoreNamedColor   
	XStoreName   
	XSetIconName   
	_XInitKeysymDB   
	XStringToKeysym   
	XSync   
	_XSyncFunction   
	XSynchronize   
	XSetAfterFunction   
	XDrawString   
	XDrawString16   
	XTextExtents   
	XTextWidth   
	XTextExtents16   
	XTextWidth16   
	XTranslateCoordinates   
	XUndefineCursor   
	XUngrabButton   
	XUngrabKeyboard   
	XUngrabKey   
	XUngrabPointer   
	XUngrabServer   
	XUninstallColormap   
	XUnloadFont   
	XUnmapSubwindows   
	XUnmapWindow   
	XGetVisualInfo   
	XMatchVisualInfo   
	XWarpPointer   
	XWindowEvent   
	XCreateWindow   
	_XProcessWindowAttributes   
	XWriteBitmapFile   
	_XFlush   
	_XEventsQueued   
	_XReadEvents   
	_XRead   
	_XReadPad   
	_XSend   
	_XAllocID   
	_XSetLastRequestRead   
	_XReply   
	_XEatData   
	_XEnq   
	_XUnknownWireEvent   
	_XUnknownNativeEvent   
	_XWireToEvent   
	_XDefaultIOError   
	_XDefaultError   
	_XDefaultWireError   
	_XError   
	_XIOError   
	_XAllocScratch   
	_XVIDtoVisual   
	XFree   
	_XGetHostname   
	_XScreenOfWindow   
	_XAllocIDs   
	_XAllocTemp   
	_XFreeTemp   
	XrmInitialize   
	XrmGetDatabase   
	XrmSetDatabase   
	XrmStringToQuarkList   
	XrmStringToBindingQuarkList   
	XrmCombineDatabase   
	XrmMergeDatabases   
	XrmQPutResource   
	XrmPutResource   
	XrmQPutStringResource   
	XrmPutStringResource   
	XrmPutLineResource   
	XrmGetStringDatabase   
	XrmGetFileDatabase   
	XrmCombineFileDatabase   
	XrmEnumerateDatabase   
	XrmPutFileDatabase   
	XrmQGetSearchList   
	XrmQGetSearchResource   
	XrmQGetResource   
	XrmGetResource   
	XrmLocaleOfDatabase   
	XrmDestroyDatabase   
	XGetGCValues   
	XGetWMSizeHints   
	XGetWMNormalHints   
	XGetRGBColormaps   
	XGetTextProperty   
	XGetWMName   
	XGetWMIconName   
	XGetWMClientMachine   
	XGetWMColormapWindows   
	XGetWMProtocols   
	XIconifyWindow   
	XListPixmapFormats   
	XAllocSizeHints   
	XAllocStandardColormap   
	XAllocWMHints   
	XAllocClassHint   
	XAllocIconSize   
	XReconfigureWMWindow   
	XSetWMSizeHints   
	XSetWMNormalHints   
	XSetRGBColormaps   
	XSetTextProperty   
	XSetWMName   
	XSetWMIconName   
	XSetWMClientMachine   
	XSetWMColormapWindows   
	XSetWMProperties   
	XSetWMProtocols   
	XStringListToTextProperty   
	XTextPropertyToStringList   
	XFreeStringList   
	XWMGeometry   
	XWithdrawWindow   
	XauDisposeAuth   
	XauGetBestAuthByAddr   
	XauFileName   
	XauReadAuth   
	XcmsCIELabToCIEXYZ   
	XcmsCIEXYZToCIELab   
	XcmsCIELabClipab   
	XcmsCIELabClipL   
	XcmsCIELabClipLab   
	XcmsCIELabQueryMinL   
	XcmsCIELabQueryMaxC   
	XcmsCIELabQueryMaxL   
	_XcmsCIELabQueryMaxLCRGB   
	XcmsCIELabQueryMaxLC   
	XcmsCIELabWhiteShiftColors   
	XcmsCIELuvToCIEuvY   
	XcmsCIEuvYToCIELuv   
	XcmsCIELuvClipuv   
	XcmsCIELuvClipL   
	XcmsCIELuvClipLuv   
	XcmsCIELuvQueryMinL   
	XcmsCIELuvQueryMaxC   
	XcmsCIELuvQueryMaxL   
	_XcmsCIELuvQueryMaxLCRGB   
	XcmsCIELuvQueryMaxLC   
	XcmsCIELuvWhiteShiftColors   
	XcmsCIEuvYToCIEXYZ   
	XcmsCIEXYZToCIEuvY   
	XcmsCIExyYToCIEXYZ   
	XcmsCIEXYZToCIExyY   
	XcmsTekHVCToCIEuvY   
	XcmsCIEuvYToTekHVC   
	_XcmsTekHVC_CheckModify   
	XcmsTekHVCClipC   
	XcmsTekHVCClipV   
	XcmsTekHVCClipVC   
	XcmsTekHVCQueryMinV   
	XcmsTekHVCQueryMaxC   
	XcmsTekHVCQueryMaxV   
	_XcmsTekHVCQueryMaxVCRGB   
	XcmsTekHVCQueryMaxVC   
	XcmsTekHVCQueryMaxVSamples   
	XcmsTekHVCWhiteShiftColors   
	XcmsAddColorSpace   
	XcmsAddFunctionSet   
	XcmsAllocNamedColor   
	XcmsAllocColor   
	XcmsCreateCCC   
	XcmsDefaultCCC   
	XcmsFreeCCC   
	_XcmsAddCmapRec   
	_XcmsCopyCmapRecAndFree   
	_XcmsDeleteCmapRec   
	XcmsCCCOfColormap   
	_XcmsCopyISOLatin1Lowered   
	_XcmsResolveColorString   
	_XcmsConvertColorsWithWhitePt   
	_XcmsEqualWhitePts   
	_XcmsDIConvertColors   
	_XcmsDDConvertColors   
	XcmsConvertColors   
	_XcmsRegFormatOfPrefix   
	XcmsFormatOfPrefix   
	_XcmsCopyPointerArray   
	_XcmsFreePointerArray   
	_XcmsPushPointerArray   
	_XcmsInitDefaultCCCs   
	_XcmsInitScrnInfo   
	_XcmsFreeIntensityMaps   
	_XcmsGetIntensityMap   
	_XcmsGetTableType0   
	_XcmsGetTableType1   
	_XcmsValueCmp   
	_XcmsIntensityCmp   
	_XcmsValueInterpolation   
	_XcmsIntensityInterpolation   
	_XcmsTableSearch   
	_XcmsMatVec   
	XcmsCIEXYZToRGBi   
	XcmsRGBiToCIEXYZ   
	XcmsRGBiToRGB   
	XcmsRGBToRGBi   
	_XcmsLRGB_InitScrnDefault   
	XcmsLookupColor   
	_XcmsCubeRoot   
	_XcmsSquareRoot   
	XcmsDisplayOfCCC   
	XcmsVisualOfCCC   
	XcmsScreenNumberOfCCC   
	XcmsScreenWhitePointOfCCC   
	XcmsClientWhitePointOfCCC   
	XcmsPrefixOfFormat   
	_XcmsGetElement   
	_XcmsGetProperty   
	XcmsQueryBlack   
	XcmsQueryBlue   
	XcmsQueryGreen   
	XcmsQueryRed   
	XcmsQueryWhite   
	XcmsQueryColor   
	XcmsQueryColors   
	XcmsSetWhitePoint   
	XcmsSetCompressionProc   
	XcmsSetWhiteAdjustProc   
	_XcmsSetGetColors   
	XcmsStoreColor   
	XcmsStoreColors   
	_XcmsCosine   
	_XcmsSine   
	_XcmsArcTangent   
	_XcmsRGB_to_XColor   
	_XColor_to_XcmsRGB   
	_XcmsResolveColor   
	_XcmsUnresolveColor   
	_XUnresolveColor   
	_XParseBaseFontNameList   
	XCreateFontSet   
	XFontsOfFontSet   
	XBaseFontNameListOfFontSet   
	XLocaleOfFontSet   
	XContextDependentDrawing   
	XExtentsOfFontSet   
	XFreeFontSet   
	XVaCreateNestedList   
	XGetIMValues   
	XCreateIC   
	XDestroyIC   
	XGetICValues   
	XSetICValues   
	XSetICFocus   
	XUnsetICFocus   
	XIMOfIC   
	XmbResetIC   
	XwcResetIC   
	XmbLookupString   
	XwcLookupString   
	_XIMCompileResourceList   
	_XCopyToArg   
	XOpenIM   
	XCloseIM   
	XDisplayOfIM   
	XLocaleOfIM   
	XSetLocaleModifiers   
	XSupportsLocale   
	_XlcValidModSyntax   
	_XlcDefaultMapModifiers   
	_XlcAddLoader   
	_XlcCurrentLC   
	XmbSetWMProperties   
	XmbDrawText   
	XmbDrawString   
	XmbDrawImageString   
	XmbTextEscapement   
	XmbTextExtents   
	XmbTextPerCharExtents   
	XwcDrawText   
	XwcDrawString   
	XwcDrawImageString   
	XwcTextEscapement   
	XwcTextExtents   
	XwcTextPerCharExtents   
	XFilterEvent   
	_XRegisterFilterByMask   
	_XRegisterFilterByType   
	_XUnregisterFilter   
	XDefaultString   
	_XlcDefaultLoader   
	_XrmInitParseInfo   
	XmbTextPropertyToTextList   
	XmbTextListToTextProperty   
	XwcTextPropertyToTextList   
	XwcFreeStringList   
	XwcTextListToTextProperty   
	_X11TransConnectDisplay   
	XGetAtomNames   
	XInitImage   
	XESetBeforeFlush   
	_XFreeAtomTable   
	_XUpdateAtomCache   
	XInternAtoms   
	_XKeyInitialize   
	XConvertCase   
	_XTranslateKey   
	_XTranslateKeySym   
	_XKeysymToModifiers   
	XExtendedMaxRequestSize   
	XReadBitmapFileData   
	_XAsyncErrorHandler   
	_XDeqAsyncHandler   
	_XGetAsyncReply   
	_XGetAsyncData   
	_XPollfdCacheInit   
	_XPollfdCacheAdd   
	_XPollfdCacheDel   
	_XRegisterInternalConnection   
	_XUnregisterInternalConnection   
	XInternalConnectionNumbers   
	XProcessInternalConnection   
	XAddConnectionWatch   
	XRemoveConnectionWatch   
	_XDeq   
	_XcmsCIEXYZ_ValidSpec   
	_XcmsCIEuvY_ValidSpec   
	XcmsSetCCCOfColormap   
	XDirectionalDependentDrawing   
	XContextualDrawing   
	XSetIMValues   
	XRegisterIMInstantiateCallback   
	XUnregisterIMInstantiateCallback   
	XCreateOC   
	XDestroyOC   
	XOMOfOC   
	XSetOCValues   
	XGetOCValues   
	XOpenOM   
	XCloseOM   
	XSetOMValues   
	XGetOMValues   
	XDisplayOfOM   
	XLocaleOfOM   
	_XlcRemoveLoader   
	_XOpenLC   
	_XCloseLC   
	_XlcCopyFromArg   
	_XlcCopyToArg   
	_XlcCountVaList   
	_XlcVaToArgList   
	_XlcCompileResourceList   
	_XlcGetValues   
	_XlcSetValues   
	XInitThreads   
	XLockDisplay   
	XUnlockDisplay   
	_X11TransFreeConnInfo   
	_X11TransOpenCOTSClient   
	_X11TransOpenCLTSClient   
	_X11TransSetOption  
	_X11TransConnect   
	_X11TransBytesReadable   
	_X11TransRead   
	_X11TransWrite   
	_X11TransReadv   
	_X11TransWritev   
	_X11TransDisconnect   
	_X11TransClose   
	_X11TransCloseForCloning   
	_X11TransIsLocal   
	_X11TransGetMyAddr   
	_X11TransGetPeerAddr   
	_X11TransGetConnectionNumber   
	_X11TransGetHostname   
	_X11TransConvertAddress   
	_XimCbDispatch   
	_XimLookupMBText   
	_XimLookupWCText   
	_XimRegisterFilter   
	_XimUnregisterFilter   
	_XimReregisterFilter   
	_XimRegisterServerFilter   
	_XimUnregisterServerFilter   
	_XimProtoCreateIC   
	_XimCheckDataSize   
	_XimSetHeader   
	_XimProtoIMFree   
	_XimProtoOpenIM   
	_XimICOfXICID   
	_XimSetEventMaskCallback   
	_XimSync   
	_XimProcSyncReply   
	_XimRespSyncReply   
	_XimSyncCallback   
	_XimForwardEvent   
	_XimForwardEventCallback   
	_XimRegisterTriggerKeysCallback   
	_XimGetWindowEventmask   
	_XimTriggerNotify   
	_XimFreeCommitInfo   
	_XimCommitCallback   
	_XimProcError   
	_XimErrorCallback   
	_XimError   
	_Ximctstombs   
	_Ximctstowcs   
	_XimProtoMbLookupString   
	_XimProtoWcLookupString   
	_XimRegProtoIntrCallback   
	_XimFreeProtoIntrCallback   
	_XimDispatchInit   
	_XimProtoEventToWire   
	_XimProtoWireToEvent   
	_XimExtension   
	_XimRegisterIMInstantiateCallback   
	_XimUnRegisterIMInstantiateCallback   
	_XimResetIMInstantiateCallback   
	_XimDestroyIMStructureList   
	_XimServerDestroy   
	_XInitIM   
	_XimLocalFilter   
	_XimLocalGetICValues   
	_XimLocalCreateIC   
	_XimCheckIfLocalProcessing   
	_XimLocalIMFree   
	_XimLocalGetIMValues   
	_XimLocalSetIMValues   
	_XimLocalOpenIM   
	_XimLocalMbLookupString   
	_XimLocalWcLookupString   
	_XimParseStringFile   
	_XimSetICValueData   
	_XimLocalSetICValues   
	_XimSetIMResourceList   
	_XimSetICResourceList   
	_XimSetInnerIMResourceList   
	_XimSetInnerICResourceList   
	_XimCheckCreateICValues   
	_XimGetResourceListRec   
	_XimSetIMMode   
	_XimCheckIMMode   
	_XimSetICMode   
	_XimCheckICMode   
	_XimSetLocalIMDefaults   
	_XimSetICDefaults   
	_XimDecodeLocalIMAttr   
	_XimDecodeLocalICAttr   
	_XimCheckLocalInputStyle   
	_XimEncodeLocalIMAttr   
	_XimEncodeLocalICAttr   
	_XimGetCurrentIMValues   
	_XimSetCurrentIMValues   
	_XimGetCurrentICValues   
	_XimSetCurrentICValues   
	_XimInitialResourceInfo   
	_XimDecodeIMATTRIBUTE   
	_XimDecodeICATTRIBUTE   
	_XimEncodeIMATTRIBUTE   
	_XimEncodeICATTRIBUTE   
	_XimGetAttributeID   
	_XimThaiFilter   
	_XimThaiCreateIC   
	_XimCheckIfThaiProcessing   
	_XimThaiOpenIM   
	_XimThaiIMFree   
	_XimThaiCloseIM   
	_XimXConf   
	_XimConnect   
	_XimShutdown   
	_XimFlush   
	_XimFilterWaitEvent   
	_XimFreeTransIntrCallback   
	_XimTransFilterWaitEvent   
	_XimTransInternalConnection   
	_XimTransFlush   
	_XimTransConf   
	_XlcParseCharSet   
	_XlcInitCTInfo   
	_XlcGetCSValues   
	_XlcGetCharSet   
	_XlcAddCharSet   
	_XlcCreateDefaultCharSet   
	_XlcSetConverter   
	_XlcOpenConverter   
	_XlcCloseConverter   
	_XlcConvert   
	_XlcResetConverter   
	_XlcGetResource   
	_XlcGetLocaleDataBase   
	_XlcDestroyLocaleDataBase   
	_XlcCreateLocaleDataBase   
	_XlcEucLoader   
	_XlcFileName   
	_XlcResolveLocaleName   
	_XlcResolveI18NPath   
	_XlcGenericLoader   
	_XlcInitLoader   
	_XmbTextPropertyToTextList   
	_XwcTextPropertyToTextList   
	_XwcFreeStringList   
	_XGetLCValues   
	_XlcDestroyLC   
	_XlcCreateLC   
	_XrmDefaultInitParseInfo   
	_XlcSjisLoader   
	_Xlcmbtowc   
	_Xlcwctomb   
	_Xlcmbstowcs   
	_Xlcwcstombs   
	_Xmbtowc   
	_Xwctomb   
	_Xmbstowcs   
	_Xwcstombs   
	_Xwcscpy   
	_Xwcsncpy   
	_Xwcslen   
	_Xwcscmp   
	_Xwcsncmp   
	_XmbTextListToTextProperty   
	_XwcTextListToTextProperty   
	_XlcCompareISOLatin1   
	_XlcNCompareISOLatin1   
	_XmbDefaultTextEscapement   
	_XwcDefaultTextEscapement   
	_XmbDefaultTextExtents   
	_XwcDefaultTextExtents   
	_XmbDefaultTextPerCharExtents   
	_XwcDefaultTextPerCharExtents   
	_XmbDefaultDrawString   
	_XwcDefaultDrawString   
	_XmbDefaultDrawImageString   
	_XwcDefaultDrawImageString   
	_XomGenericOpenOM   
	_XInitOM   
	_XmbGenericDrawImageString   
	_XwcGenericDrawImageString   
	_XomGenericDrawString   
	_XmbGenericDrawString   
	_XwcGenericDrawString   
	_XmbGenericTextEscapement   
	_XwcGenericTextEscapement   
	_XomGenericTextExtents   
	_XmbGenericTextExtents   
	_XwcGenericTextExtents   
	_XmbGenericTextPerCharExtents   
	_XwcGenericTextPerCharExtents   
	_XomConvert   
	_XomInitConverter   
	_Xmblen     
	_XimGetResourceListRecByQuark   
	_XimSetIMValueData   
	_XimGetIMValueData   
	_XimGetICValueData   
	_XimMakeIMAttrIDList   
	_XimMakeICAttrIDList   
	_XimWrite   
	_XimRead   
	_XimRegisterDispatcher   
	_XimTransRegisterDispatcher   
	_XimTransCallDispatcher   
	_XimTransWrite   
	_XimTransRead   
	_XimXTransFreeConnInfo   
	_XimXTransOpenCOTSClient   
	_XimXTransOpenCLTSClient   
	_XimXTransSetOption   
	_XimXTransConnect   
	_XimXTransBytesReadable   
	_XimXTransRead   
	_XimXTransWrite   
	_XimXTransReadv   
	_XimXTransWritev   
	_XimXTransDisconnect   
	_XimXTransClose   
	_XimXTransCloseForCloning   
	_XimXTransIsLocal   
	_XimXTransGetMyAddr   
	_XimXTransGetPeerAddr   
	_XimXTransGetConnectionNumber   
	_XimXTransGetHostname   
	_Xdebug DATA  
	Xutf8TextPropertyToTextList 
	Xutf8TextListToTextProperty	 
	Xutf8LookupString
	XkbIgnoreExtension  
	XkbOpenDisplay  
	XkbQueryExtension  
	XkbUseExtension  
	XkbLibraryVersion  
	XkbSetXlibControls  
	XkbGetXlibControls  
	XkbSetAtomFuncs  
	XkbKeycodeToKeysym  
	XkbKeysymToModifiers  
	XkbLookupKeySym  
	XkbLookupKeyBinding  
	XkbTranslateKeyCode  
	XkbTranslateKeySym  
	XkbSetAutoRepeatRate  
	XkbGetAutoRepeatRate  
	XkbChangeEnabledControls  
	XkbDeviceBell  
	XkbForceDeviceBell  
	XkbDeviceBellEvent  
	XkbBell  
	XkbForceBell  
	XkbBellEvent  
	XkbSelectEvents  
	XkbSelectEventDetails  
	XkbNoteMapChanges  
	XkbNoteNameChanges  
	XkbGetIndicatorState  
	XkbGetIndicatorMap  
	XkbSetIndicatorMap  
	XkbGetNamedIndicator  
	XkbGetNamedDeviceIndicator  
	XkbSetNamedIndicator  
	XkbSetNamedDeviceIndicator  
	XkbLockModifiers  
	XkbLatchModifiers  
	XkbLockGroup  
	XkbLatchGroup  
	XkbSetServerInternalMods  
	XkbSetIgnoreLockMods  
	XkbVirtualModsToReal  
	XkbComputeEffectiveMap  
	XkbInitCanonicalKeyTypes  
	XkbAllocKeyboard  
	XkbFreeKeyboard  
	XkbAllocClientMap  
	XkbAllocServerMap  
	XkbFreeClientMap  
	XkbFreeServerMap  
	XkbAddKeyType  
	XkbAllocIndicatorMaps  
	XkbFreeIndicatorMaps  
	XkbGetMap  
	XkbGetUpdatedMap  
	XkbGetMapChanges  
	XkbRefreshKeyboardMapping  
	XkbGetKeyTypes  
	XkbGetKeySyms  
	XkbGetKeyActions  
	XkbGetKeyBehaviors  
	XkbGetVirtualMods  
	XkbGetKeyExplicitComponents  
	XkbGetKeyModifierMap  
	XkbAllocControls  
	XkbFreeControls  
	XkbGetControls  
	XkbSetControls  
	XkbAllocCompatMap  
	XkbFreeCompatMap  
	XkbGetCompatMap  
	XkbSetCompatMap  
	XkbAllocNames  
	XkbGetNames  
	XkbSetNames  
	XkbChangeNames  
	XkbFreeNames  
	XkbGetState  
	XkbSetMap  
	XkbChangeMap  
	XkbSetDetectableAutoRepeat  
	XkbGetDetectableAutoRepeat  
	XkbSetAutoResetControls  	
	XkbGetAutoResetControls  
	XkbCopyKeyType  
	XkbCopyKeyTypes  
	XkbResizeKeyType  
	XkbResizeKeySyms  
	XkbResizeKeyActions  
	XkbChangeTypesOfKey  
	XkbListComponents  
	XkbFreeComponentList  
	XkbGetKeyboard  
	XkbGetKeyboardByName  
	XkbAddDeviceLedInfo  
	XkbResizeDeviceButtonActions  
	XkbAllocDeviceInfo  
	XkbFreeDeviceInfo  
	XkbNoteDeviceChanges  
	XkbGetDeviceInfo  
	XkbGetDeviceInfoChanges  
	XkbGetDeviceButtonActions  
	XkbGetDeviceLedInfo  
	XkbSetDeviceInfo  
	XkbChangeDeviceInfo  
	XkbSetDeviceLedInfo  
	XkbSetDeviceButtonActions  
	XkbToControl  
	XkbSetDebuggingFlags  
	_XkbInternAtomFunc  
	_XkbGetAtomNameFunc  
	_XkbReloadDpy  
	_XKeycodeToKeysym  
	_XKeysymToKeycode  
	_XLookupKeysym  
	_XRefreshKeyboardMapping  
	_XLookupString 
	_XkbNoteCoreMapChanges  
	_XkbInitReadBuffer  
	_XkbSkipReadBufferData  
	_XkbCopyFromReadBuffer  
	_XkbReadCopyKeySyms  
	_XkbWriteCopyKeySyms  
	_XkbReadBufferCopyKeySyms  
	_XkbPeekAtReadBuffer  
	_XkbGetReadBufferPtr  
	_XkbFreeReadBuffer 
	_XkbGetReadBufferCountedString  
	_XkbGetCharset  
	_XkbGetConverters  
	XkbGetGeometry  
	XkbAddGeomShape  
	XkbAddGeomOutline  
	XkbComputeShapeBounds  
	XkbAddGeomDoodad  
	XkbAddGeomColor  
	XkbAddGeomOverlayRow  
	XkbAddGeomSection  
	XkbAddGeomRow  
	XkbAddGeomKey  
	XkbComputeSectionBounds  
	XkbAllocGeometry  
	XkbAddGeomProperty  
	XkbAddGeomOverlay  
	XkbComputeShapeTop  
	XkbFindOverlayForKey  
	XkbAllocGeomKeyAliases  
	XkbSetGeometry  
	XkbXlibControlsImplemented  
	XkbTranslateKey  
	XkbAddGeomOverlayKey  
	XkbNoteControlsChanges  
	XkbGetKeyVirtualModMap  
	XkbComputeRowBounds  
	XkbGetNamedGeometry  
	XkbKeyTypesForCoreSymbols  
	XkbApplyCompatMapToKey  
	XkbUpdateMapFromCore  
	XkbUpdateActionVirtualMods  
	XkbUpdateKeyTypeVirtualMods  
	XkbApplyVirtualModChanges  
	XkbFreeGeomProperties  
	XkbFreeGeomKeyAliases  
	XkbFreeGeomColors  
	XkbFreeGeomPoints  
	XkbFreeGeomOutlines  
	XkbFreeGeomShapes  
	XkbFreeGeomOverlayKeys  
	XkbFreeGeomOverlayRows  
	XkbFreeGeomOverlays  
	XkbFreeGeomKeys  
	XkbFreeGeomRows  
	XkbFreeGeomSections  
	XkbFreeGeomDoodads  
	XkbFreeGeometry  
	XkbAllocGeomProps  
	XkbAllocGeomColors  
	XkbAllocGeomShapes  
	XkbAllocGeomSections  
	XkbAllocGeomOverlays  
	XkbAllocGeomOverlayRows  
	XkbAllocGeomOverlayKeys  
	XkbAllocGeomDoodads  
	XkbAllocGeomSectionDoodads  
	XkbAllocGeomOutlines  
	XkbAllocGeomRows  
	XkbAllocGeomPoints  
	XkbAllocGeomKeys  
	XkbAddGeomKeyAlias  
	_XkeyTable  
	_Xevent_to_mask  
	_qfree  
	_XIOErrorFunction  
	_XErrorFunction  
	_XHeadOfDisplayList  
	XcmsCIELabColorSpace  
	XcmsCIELuvColorSpace  
	XcmsCIEXYZColorSpace  
	XcmsCIEuvYColorSpace  
	XcmsCIExyYColorSpace  
	XcmsTekHVCColorSpace  
	XcmsUNDEFINEDColorSpace  
	_XcmsDIColorSpacesInit  
	_XcmsDIColorSpaces  
	_XcmsDDColorSpacesInit  
	_XcmsDDColorSpaces  
	_XcmsSCCFuncSetsInit  
	_XcmsSCCFuncSets  
	_XcmsCIEXYZ_prefix  
	_XcmsCIEuvY_prefix  
	_XcmsCIExyY_prefix  
	_XcmsCIELab_prefix  
	_XcmsCIELuv_prefix  
	_XcmsTekHVC_prefix  
	_XcmsRGBi_prefix  
	_XcmsRGB_prefix  
	_XcmsRegColorSpaces  
	XcmsRGBiColorSpace  
	XcmsRGBColorSpace  
	XcmsLinearRGBFunctionSet  
	_X11TransSocketINETFuncs  
	_X11TransSocketTCPFuncs  
	_XimXTransSocketINETFuncs  
	_XimXTransSocketTCPFuncs  
	_XimImSportRec  
	_XimTransportRec  
	_XlcGenericMethods  
	_XlcPublicMethods  
#ifdef HAS_THREADS
	_XCreateMutex_fn DATA
	_XFreeMutex_fn DATA
	_XLockMutex_fn DATA
	_XUnlockMutex_fn DATA
	_Xglobal_lock DATA
#endif
	_XGetScanlinePad
	_XGetBitsPerPixel
	_Xsetlocale  
	XkbGetPerClientControls
	XkbSetPerClientControls
	Xutf8DrawString
	Xutf8TextExtents

#ifdef __UNIXOS2__
; OS/2 specific exports
  __XOS2RedirRoot	
  __XOS2RedirRoot1	
#endif

/* $Xorg: X11-def.cpp,v 1.3 2000/08/17 19:44:59 cpqbld Exp $ */
/* $XFree86: xc/lib/X11/X11-def.cpp,v 1.16 2004/05/06 01:17:29 dawes Exp $ */
