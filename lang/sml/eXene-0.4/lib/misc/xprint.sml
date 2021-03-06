(* xprint.sml
 *
 * COPYRIGHT (c) 1990,1991 by John H. Reppy.  See COPYRIGHT file for details.
 *)

structure XPrint : sig
    val xevtName : XEventTypes.xevent -> string
    val xerrorKind : XErrors.xerr_kind -> string
    val xerrorToString : XErrors.xerror -> string
    val xidToString : XProtTypes.xid -> string
  end = struct

    local
      open XEventTypes XErrors
    in

    fun reqCodeToString 1 = "CreateWindow"
      | reqCodeToString 2 = "ChangeWindowAttributes"
      | reqCodeToString 3 = "GetWindowAttributes"
      | reqCodeToString 4 = "DestroyWindow"
      | reqCodeToString 5 = "DestroySubwindows"
      | reqCodeToString 6 = "ChangeSaveSet"
      | reqCodeToString 7 = "ReparentWindow"
      | reqCodeToString 8 = "MapWindow"
      | reqCodeToString 9 = "MapSubwindows"
      | reqCodeToString 10 = "UnmapWindow"
      | reqCodeToString 11 = "UnmapSubwindows"
      | reqCodeToString 12 = "ConfigureWindow"
      | reqCodeToString 13 = "CirculateWindow"
      | reqCodeToString 14 = "GetGeometry"
      | reqCodeToString 15 = "QueryTree"
      | reqCodeToString 16 = "InternAtom"
      | reqCodeToString 17 = "GetAtomName"
      | reqCodeToString 18 = "ChangeProperty"
      | reqCodeToString 19 = "DeleteProperty"
      | reqCodeToString 20 = "GetProperty"
      | reqCodeToString 21 = "ListProperties"
      | reqCodeToString 22 = "SetSelectionOwner"
      | reqCodeToString 23 = "GetSelectionOwner"
      | reqCodeToString 24 = "ConvertSelection"
      | reqCodeToString 25 = "SendEvent"
      | reqCodeToString 26 = "GrabPointer"
      | reqCodeToString 27 = "UngrabPointer"
      | reqCodeToString 28 = "GrabButton"
      | reqCodeToString 29 = "UngrabButton"
      | reqCodeToString 30 = "ChangeActivePointerGrab"
      | reqCodeToString 31 = "GrabKeyboard"
      | reqCodeToString 32 = "UngrabKeyboard"
      | reqCodeToString 33 = "GrabKey"
      | reqCodeToString 34 = "UngrabKey"
      | reqCodeToString 35 = "AllowEvents"
      | reqCodeToString 36 = "GrabServer"
      | reqCodeToString 37 = "UngrabServer"
      | reqCodeToString 38 = "QueryPointer"
      | reqCodeToString 39 = "GetMotionEvents"
      | reqCodeToString 40 = "TranslateCoords"
      | reqCodeToString 41 = "WarpPointer"
      | reqCodeToString 42 = "SetInputFocus"
      | reqCodeToString 43 = "GetInputFocus"
      | reqCodeToString 44 = "QueryKeymap"
      | reqCodeToString 45 = "OpenFont"
      | reqCodeToString 46 = "CloseFont"
      | reqCodeToString 47 = "QueryFont"
      | reqCodeToString 48 = "QueryTextExtents"
      | reqCodeToString 49 = "ListFonts"
      | reqCodeToString 50 = "ListFontsWithInfo"
      | reqCodeToString 51 = "SetFontPath"
      | reqCodeToString 52 = "GetFontPath"
      | reqCodeToString 53 = "CreatePixmap"
      | reqCodeToString 54 = "FreePixmap"
      | reqCodeToString 55 = "CreateGC"
      | reqCodeToString 56 = "ChangeGC"
      | reqCodeToString 57 = "CopyGC"
      | reqCodeToString 58 = "SetDashes"
      | reqCodeToString 59 = "SetClipRectangles"
      | reqCodeToString 60 = "FreeGC"
      | reqCodeToString 61 = "ClearArea"
      | reqCodeToString 62 = "CopyArea"
      | reqCodeToString 63 = "CopyPlane"
      | reqCodeToString 64 = "PolyPoint "
      | reqCodeToString 65 = "PolyLine"
      | reqCodeToString 66 = "PolySegment"
      | reqCodeToString 67 = "PolyRectangle"
      | reqCodeToString 68 = "PolyArc"
      | reqCodeToString 69 = "FillPoly"
      | reqCodeToString 70 = "PolyFillRectangle"
      | reqCodeToString 71 = "PolyFillArc"
      | reqCodeToString 72 = "PutImage"
      | reqCodeToString 73 = "GetImage"
      | reqCodeToString 74 = "PolyText8"
      | reqCodeToString 75 = "PolyText16"
      | reqCodeToString 76 = "ImageText8"
      | reqCodeToString 77 = "ImageText16"
      | reqCodeToString 78 = "CreateColormap"
      | reqCodeToString 79 = "FreeColormap"
      | reqCodeToString 80 = "CopyColormapAndFree"
      | reqCodeToString 81 = "InstallColormap"
      | reqCodeToString 82 = "UninstallColormap"
      | reqCodeToString 83 = "ListInstalledColormaps"
      | reqCodeToString 84 = "AllocColor"
      | reqCodeToString 85 = "AllocNamedColor"
      | reqCodeToString 86 = "AllocColorCells"
      | reqCodeToString 87 = "AllocColorPlanes"
      | reqCodeToString 88 = "FreeColors"
      | reqCodeToString 89 = "StoreColors"
      | reqCodeToString 90 = "StoreNamedColor"
      | reqCodeToString 91 = "QueryColors"
      | reqCodeToString 92 = "LookupColor"
      | reqCodeToString 93 = "CreateCursor"
      | reqCodeToString 94 = "CreateGlyphCursor"
      | reqCodeToString 95 = "FreeCursor"
      | reqCodeToString 96 = "RecolorCursor"
      | reqCodeToString 97 = "QueryBestSize"
      | reqCodeToString 98 = "QueryExtension"
      | reqCodeToString 99 = "ListExtensions"
      | reqCodeToString 100 = "ChangeKeyboardMapping"
      | reqCodeToString 101 = "GetKeyboardMapping"
      | reqCodeToString 102 = "ChangeKeyboardControl"
      | reqCodeToString 103 = "GetKeyboardControl"
      | reqCodeToString 104 = "Bell"
      | reqCodeToString 105 = "ChangePointerControl"
      | reqCodeToString 106 = "GetPointerControl"
      | reqCodeToString 107 = "SetScreenSaver"
      | reqCodeToString 108 = "GetScreenSaver"
      | reqCodeToString 109 = "ChangeHosts"
      | reqCodeToString 110 = "ListHosts"
      | reqCodeToString 111 = "SetAccessControl"
      | reqCodeToString 112 = "SetCloseDownMode"
      | reqCodeToString 113 = "KillClient"
      | reqCodeToString 114 = "RotateProperties"
      | reqCodeToString 115 = "ForceScreenSaver"
      | reqCodeToString 116 = "SetPointerMapping"
      | reqCodeToString 117 = "GetPointerMapping"
      | reqCodeToString 118 = "SetModifierMapping"
      | reqCodeToString 119 = "GetModifierMapping"
      | reqCodeToString 127 = "NoOperation"
      | reqCodeToString i = makestring i

    fun xerrorKind BadRequest = "BadRequest"
      | xerrorKind (BadValue v) = ("BadValue " ^ v)
      | xerrorKind (BadWindow(XProtTypes.XID id)) = ("BadWindow " ^ (makestring id))
      | xerrorKind (BadPixmap(XProtTypes.XID id)) = ("BadPixmap " ^ (makestring id))
      | xerrorKind (BadAtom(XProtTypes.XID id)) = ("BadAtom " ^ (makestring id))
      | xerrorKind (BadCursor(XProtTypes.XID id)) = ("BadCursor " ^ (makestring id))
      | xerrorKind (BadFont(XProtTypes.XID id)) = ("BadFont " ^ (makestring id))
      | xerrorKind BadMatch = "BadMatch"
      | xerrorKind (BadDrawable(XProtTypes.XID id)) = ("BadDrawable " ^ (makestring id))
      | xerrorKind BadAccess = "BadAccess"
      | xerrorKind BadAlloc = "BadAlloc"
      | xerrorKind (BadColor(XProtTypes.XID id)) = ("BadColor " ^ (makestring id))
      | xerrorKind (BadGC(XProtTypes.XID id)) = ("BadGC " ^ (makestring id))
      | xerrorKind (BadIDChoice(XProtTypes.XID id)) = ("BadIDChoice " ^ (makestring id))
      | xerrorKind BadName = "BadName"
      | xerrorKind BadLength = "BadLength"
      | xerrorKind BadImplementation = "BadImplementation"

    fun xerrorToString (XErr{kind, major_op, minor_op}) = implode [
	     "<<", xerrorKind kind,
	     ", major = ", reqCodeToString major_op,
	      ", minor = ", makestring minor_op, ">>"
	    ]

    fun xevtName (KeyPressXEvt _) = "KeyPress"
      | xevtName (KeyReleaseXEvt _) = "KeyRelease"
      | xevtName (ButtonPressXEvt _) = "ButtonPress"
      | xevtName (ButtonReleaseXEvt _) = "ButtonRelease"
      | xevtName (MotionNotifyXEvt _) = "MotionNotify"
      | xevtName (EnterNotifyXEvt _) = "EnterNotify"
      | xevtName (LeaveNotifyXEvt _) = "LeaveNotify"
      | xevtName (FocusInXEvt _) = "FocusIn"
      | xevtName (FocusOutXEvt _) = "FocusOut"
      | xevtName (KeymapNotifyXEvt _) = "KeymapNotify"
      | xevtName (ExposeXEvt _) = "Expose"
      | xevtName (GraphicsExposeXEvt _) = "GraphicsExpose"
      | xevtName (NoExposeXEvt _) = "NoExpose"
      | xevtName (VisibilityNotifyXEvt _) = "VisibilityNotify"
      | xevtName (CreateNotifyXEvt _) = "CreateNotify"
      | xevtName (DestroyNotifyXEvt _) = "DestroyNotify"
      | xevtName (UnmapNotifyXEvt _) = "UnmapNotify"
      | xevtName (MapNotifyXEvt _) = "MapNotify"
      | xevtName (MapRequestXEvt _) = "MapRequest"
      | xevtName (ReparentNotifyXEvt _) = "ReparentNotify"
      | xevtName (ConfigureNotifyXEvt _) = "ConfigureNotify"
      | xevtName (ConfigureRequestXEvt _) = "ConfigureRequest"
      | xevtName (GravityNotifyXEvt _) = "GravityNotify"
      | xevtName (ResizeRequestXEvt _) = "ResizeRequest"
      | xevtName (CirculateNotifyXEvt _) = "CirculateNotify"
      | xevtName (CirculateRequestXEvt _) = "CirculateRequest"
      | xevtName (PropertyNotifyXEvt _) = "PropertyNotify"
      | xevtName (SelectionClearXEvt _) = "SelectionClear"
      | xevtName (SelectionRequestXEvt _) = "SelectionRequest"
      | xevtName (SelectionNotifyXEvt _) = "SelectionNotify"
      | xevtName (ColormapNotifyXEvt _) = "ColormapNotify"
      | xevtName (ClientMessageXEvt _) = "ClientMessage"
      | xevtName (ModifierMappingNotifyXEvt ) = "ModifierMappingNotify"
      | xevtName (KeyboardMappingNotifyXEvt _) = "KeyboardMappingNotify"
      | xevtName (PointerMappingNotifyXEvt ) = "PointerMappingNotify"

    fun xidToString (XProtTypes.XID n) = makestring n

    end (* local *)
  end (* XPrint *)
