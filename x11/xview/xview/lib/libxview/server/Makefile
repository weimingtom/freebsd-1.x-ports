# Makefile generated by imake - do not edit!
# $XConsortium: imake.c,v 1.65 91/07/25 17:50:17 rws Exp $

# -------------------------------------------------------------------------
# Makefile generated from "Imake.tmpl" and <Imakefile>
# $XFree86: mit/config/Imake.tmpl,v 1.17 1993/06/03 15:26:36 dawes Exp $
# $XConsortium: Imake.tmpl,v 1.139 91/09/16 08:52:48 rws Exp $
#
# Platform-specific parameters may be set in the appropriate <vendor>.cf
# configuration files.  Site-specific parameters should be set in the file
# site.def.  Full rebuilds are recommended if any parameters are changed.
#
# If your C preprocessor does not define any unique symbols, you will need
# to set BOOTSTRAPCFLAGS when rebuilding imake (usually when doing
# "make World" the first time).
#

# -------------------------------------------------------------------------
# site-specific configuration parameters that need to come before
# the platform-specific parameters - edit site.def to change

# $XFree86: mit/config/site.def,v 1.65 1993/06/04 16:02:47 dawes Exp $
# site:  $XConsortium: site.def,v 1.2 91/07/30 20:26:44 rws Exp $

# -------------------------------------------------------------------------
# platform-specific configuration parameters - edit x386.cf to change

# $XFree86: mit/config/x386.cf,v 1.90 1993/06/04 16:02:50 dawes Exp $
# platform:  $XConsortium: x386.cf,v 1.7 91/08/16 19:30:10 gildea Exp $

# -------------------------------------------------------------------------
# XFree86 version definition
# $XFree86: mit/config/xf86_vers.def,v 1.5 1993/06/01 09:12:47 dawes Exp $

# -------------------------------------------------------------------------
# XFree86 version: 1300
# -------------------------------------------------------------------------

# operating system:  		386BSD

# -------------------------------------------------------------------------
# site-specific configuration parameters that go after
# the platform-specific parameters - edit site.def to change

# $XFree86: mit/config/site.def,v 1.65 1993/06/04 16:02:47 dawes Exp $
# site:  $XConsortium: site.def,v 1.2 91/07/30 20:26:44 rws Exp $

            SHELL = 	/bin/sh

              TOP = ../../../.
      CURRENT_DIR = ./lib/libxview/server

               AR = ar clq
  BOOTSTRAPCFLAGS =
               CC = 		gcc
               AS = as

             YACC = yacc

         COMPRESS = compress
              CPP = 		/usr/libexec/cpp $(STD_CPP_DEFINES)
    PREPROCESSCMD = 		gcc -E $(STD_CPP_DEFINES)
          INSTALL = install
               LD = ld
             LINT = lint
      LINTLIBFLAG = -C
         LINTOPTS = -axz
               LN = ln -s
             MAKE = make
               MV = mv
               CP = cp

           RANLIB = ranlib
  RANLIBINSTFLAGS =

               RM = rm -f
            TROFF = psroff
         MSMACROS = -ms
              TBL = tbl
              EQN = eqn
     STD_INCLUDES =
  STD_CPP_DEFINES = -traditional
      STD_DEFINES =
 EXTRA_LOAD_FLAGS =
  EXTRA_LIBRARIES =
     OS_LIBRARIES =
             TAGS = ctags

    PROTO_DEFINES =

     INSTPGMFLAGS = -s

     INSTBINFLAGS = -m 0755
     INSTUIDFLAGS = -m 4755
     INSTLIBFLAGS = -m 0644
     INSTINCFLAGS = -m 0444
     INSTMANFLAGS = -m 0444
     INSTDATFLAGS = -m 0444
    INSTKMEMFLAGS = -m 4755

      PROJECTROOT = 	/usr/X386

     TOP_INCLUDES = -I$(INCROOT)

      CDEBUGFLAGS = -O2
        CCOPTIONS = -m486
    ANSICCOPTIONS =

      ALLINCLUDES = $(INCLUDES) $(EXTRA_INCLUDES) $(TOP_INCLUDES) $(STD_INCLUDES)
       ALLDEFINES = $(ALLINCLUDES) $(STD_DEFINES) $(EXTRA_DEFINES) $(PROTO_DEFINES) $(DEFINES)
           CFLAGS = $(ANSICCOPTIONS) $(CDEBUGFLAGS) $(CCOPTIONS) $(ALLDEFINES)
        LINTFLAGS = $(LINTOPTS) -DLINT $(ALLDEFINES)

           LDLIBS = $(OS_LIBRARIES) $(SYS_LIBRARIES) $(EXTRA_LIBRARIES)

        LDOPTIONS = $(ANSICCOPTIONS) $(CDEBUGFLAGS) $(CCOPTIONS) $(LOCAL_LDFLAGS) -L$(USRLIBDIR)

   LDCOMBINEFLAGS = -X -r
      DEPENDFLAGS =

        MACROFILE = x386.cf
           RM_CMD = $(RM) *.CKP *.ln *.BAK *.bak *.o core errs ,* *~ *.a .emacs_* tags TAGS make.log MakeOut

    IMAKE_DEFINES =

         IRULESRC = $(CONFIGDIR)
        IMAKE_CMD = $(IMAKE) -DUseInstalled -I$(IRULESRC) $(IMAKE_DEFINES)

     ICONFIGFILES = $(IRULESRC)/Imake.tmpl $(IRULESRC)/Imake.rules 			$(IRULESRC)/Project.tmpl $(IRULESRC)/site.def 			$(IRULESRC)/$(MACROFILE) $(EXTRA_ICONFIGFILES)

# -------------------------------------------------------------------------
# X Window System Build Parameters
# $XFree86: mit/config/Project.tmpl,v 1.13 1993/03/27 03:32:45 dawes Exp $
# $XConsortium: Project.tmpl,v 1.138.1.1 92/11/11 09:49:19 rws Exp $

_percentC_ = %C

# -------------------------------------------------------------------------
# X Window System make variables; this need to be coordinated with rules

          PATHSEP = /
        USRLIBDIR = 	/usr/X386/lib
           BINDIR = 	/usr/X386/bin
          INCROOT = 	/usr/X386/include
     BUILDINCROOT = $(TOP)
      BUILDINCDIR = $(BUILDINCROOT)/X11
      BUILDINCTOP = ..
           INCDIR = $(INCROOT)/X11
           ADMDIR = /usr/adm
           LIBDIR = $(USRLIBDIR)/X11
        CONFIGDIR = $(LIBDIR)/config
       LINTLIBDIR = $(USRLIBDIR)/lint

          FONTDIR = $(LIBDIR)/fonts
         XINITDIR = $(LIBDIR)/xinit
           XDMDIR = $(LIBDIR)/xdm
           TWMDIR = $(LIBDIR)/twm
          MANPATH = 	/usr/X386/man
    MANSOURCEPATH = 	$(MANPATH)/man
        MANSUFFIX = 1
     LIBMANSUFFIX = 3
           MANDIR = 	$(MANSOURCEPATH)1
        LIBMANDIR = $(MANSOURCEPATH)3
           NLSDIR = $(LIBDIR)/nls
        PEXAPIDIR = $(LIBDIR)/PEX
      XAPPLOADDIR = $(LIBDIR)/app-defaults
       FONTCFLAGS = -t
       LINKKITDIR = 	$(USRLIBDIR)/Server

     INSTAPPFLAGS = $(INSTDATFLAGS)

            IMAKE = imake
           DEPEND = makedepend
              RGB = rgb

            FONTC = bdftopcf

        MKFONTDIR = mkfontdir
        MKDIRHIER = 	/bin/sh $(BINDIR)/mkdirhier

        CONFIGSRC = $(TOP)/config
       DOCUTILSRC = $(TOP)/doc/util
        CLIENTSRC = $(TOP)/clients
          DEMOSRC = $(TOP)/demos
           LIBSRC = $(TOP)/lib
          FONTSRC = $(TOP)/fonts
       INCLUDESRC = $(TOP)/X11
        SERVERSRC = $(TOP)/server
          UTILSRC = $(TOP)/util
        SCRIPTSRC = $(UTILSRC)/scripts
       EXAMPLESRC = $(TOP)/examples
       CONTRIBSRC = $(TOP)/../contrib
           DOCSRC = $(TOP)/doc
           RGBSRC = $(TOP)/rgb
        DEPENDSRC = $(UTILSRC)/makedepend
         IMAKESRC = $(CONFIGSRC)
         XAUTHSRC = $(LIBSRC)/Xau
          XLIBSRC = $(LIBSRC)/X
           XMUSRC = $(LIBSRC)/Xmu
       TOOLKITSRC = $(LIBSRC)/Xt
       AWIDGETSRC = $(LIBSRC)/Xaw
       OLDXLIBSRC = $(LIBSRC)/oldX
      XDMCPLIBSRC = $(LIBSRC)/Xdmcp
      BDFTOSNFSRC = $(FONTSRC)/bdftosnf
      BDFTOSNFSRC = $(FONTSRC)/clients/bdftosnf
      BDFTOPCFSRC = $(FONTSRC)/clients/bdftopcf
     MKFONTDIRSRC = $(FONTSRC)/clients/mkfontdir
         FSLIBSRC = $(FONTSRC)/lib/fs
    FONTSERVERSRC = $(FONTSRC)/server
     EXTENSIONSRC = $(TOP)/extensions
         XILIBSRC = $(EXTENSIONSRC)/lib/xinput
        PEXLIBSRC = $(EXTENSIONSRC)/lib/PEXlib
      PHIGSLIBSRC = $(EXTENSIONSRC)/lib/PEX

  DEPEXTENSIONLIB = $(USRLIBDIR)/libXext.a
     EXTENSIONLIB = 			 -lXext

          DEPXLIB = $(DEPEXTENSIONLIB) $(USRLIBDIR)/libX11.a
             XLIB = $(EXTENSIONLIB) 			 -lX11

      DEPXAUTHLIB = $(USRLIBDIR)/libXau.a
         XAUTHLIB = 			 -lXau
      DEPXDMCPLIB = $(USRLIBDIR)/libXdmcp.a
         XDMCPLIB = 			 -lXdmcp

        DEPXMULIB = $(USRLIBDIR)/libXmu.a
           XMULIB = 			 -lXmu

       DEPOLDXLIB = $(USRLIBDIR)/liboldX.a
          OLDXLIB = 			 -loldX

      DEPXTOOLLIB = $(USRLIBDIR)/libXt.a
         XTOOLLIB = 			 -lXt

        DEPXAWLIB = $(USRLIBDIR)/libXaw.a
           XAWLIB = 			 -lXaw

        DEPXILIB = $(USRLIBDIR)/libXi.a
           XILIB = 			 -lXi

       DEPPEXLIB = $(USRLIBDIR)/libPEX5.a
          PEXLIB = 			 -lPEX5

        DEPPHIGSLIB = $(USRLIBDIR)/libphigs.a
           PHIGSLIB = 			 -lphigs

       DEPXBSDLIB = $(USRLIBDIR)/libXbsd.a
          XBSDLIB = 			 -lXbsd

 LINTEXTENSIONLIB = $(LINTLIBDIR)/llib-lXext.ln
         LINTXLIB = $(LINTLIBDIR)/llib-lX11.ln
          LINTXMU = $(LINTLIBDIR)/llib-lXmu.ln
        LINTXTOOL = $(LINTLIBDIR)/llib-lXt.ln
          LINTXAW = $(LINTLIBDIR)/llib-lXaw.ln
           LINTXI = $(LINTLIBDIR)/llib-lXi.ln
          LINTPEX = $(LINTLIBDIR)/llib-lPEX5.ln
        LINTPHIGS = $(LINTLIBDIR)/llib-lphigs.ln

          DEPLIBS = $(DEPXAWLIB) $(DEPXMULIB) $(DEPXTOOLLIB) $(DEPXLIB)

         DEPLIBS1 = $(DEPLIBS)
         DEPLIBS2 = $(DEPLIBS)
         DEPLIBS3 = $(DEPLIBS)

# -------------------------------------------------------------------------
# Imake rules for building libraries, programs, scripts, and data files
# $XFree86: mit/config/Imake.rules,v 1.9 1993/03/23 12:56:27 dawes Exp $
# rules:  $XConsortium: Imake.rules,v 1.123 91/09/16 20:12:16 rws Exp $

# -------------------------------------------------------------------------
# start of Imakefile

###########################################################################
# @(#)XView.cf	1.10 09 Feb 1992 SMI
# XView.cf for XView release 3.0.  Specify site preferences here.

###########################################################################
# @(#)XView.tmpl	1.15 12 May 1993 SMI
# XView.tmpl for XView release 3.2.  Template to add additional config
# rules to those supplied with X Version 11 release 5.

              MKDIRHIER = 	/bin/sh 	/usr/X386/bin/mkdirhier

           XV_CFLAGS.optimized = -DOPTIMIZED
           XV_CFLAGS.shared =

                DEFINES = $(LOCALE_DEFINES) $(MMAP_DEFINES) $(XV_ALLOC_DEFINES) 			  $(XV_STRDUP_DEFINES) $(XV_CFLAGS.optimized) $(XV_CFLAGS.shared)

              XVDESTDIR = /usr/X386
           LIBRARY_DEST = $(XVDESTDIR)/lib
      LINT_LIBRARY_DEST = $(XVDESTDIR)/lib/lint
                 BINDIR = $(XVDESTDIR)/bin
                MANPATH = $(XVDESTDIR)/man
            HEADER_DEST = $(XVDESTDIR)/include
               SRC_DEST = $(XVDESTDIR)/share/src/xview
               DOC_DEST = $(XVDESTDIR)/share/doc/xview

               XVLIBSRC = $(TOP)/lib
              HELP_DEST = $(LIBRARY_DEST)/help
    LIBRARY_DEST_EXTRAS = $(LIBRARY_DEST)
       TEXT_EXTRAS_MENU = $(LIBRARY_DEST)/.text_extras_menu
                     AR = ar ruv
                  CHMOD = chmod

                DEPXLIB =

            BUILDINCDIR = $(TOP)/build
            BUILDINCTOP = ../../..

                 INCDIR = /include

               XVPUBINC = xview
               XVPRIINC = xview_private
               OLPUBINC = olgx
               OLPRIINC = olgx_private
               PSPUBINC = xvps
               PSPRIINC = xvps_private
               IMAGEINC = images
              BITMAPINC = bitmaps
         PIXRECT_INCDIR = $(INCDIR)/pixrect
           XVPUB_INCDIR = $(INCDIR)/$(XVPUBINC)
           XVPRI_INCDIR = $(INCDIR)/$(XVPRIINC)
           OLPUB_INCDIR = $(INCDIR)/$(OLPUBINC)
           OLPRI_INCDIR = $(INCDIR)/$(OLPRIINC)
           PSPUB_INCDIR = $(INCDIR)/$(PSPUBINC)
           PSPRI_INCDIR = $(INCDIR)/$(PSPRIINC)
           IMAGE_INCDIR = $(INCDIR)/$(IMAGEINC)
          BITMAP_INCDIR = $(INCDIR)/$(BITMAPINC)
    PIXRECT_HEADER_DEST = $(HEADER_DEST)/pixrect
      XVPUB_HEADER_DEST = $(HEADER_DEST)/$(XVPUBINC)
      XVPRI_HEADER_DEST = $(HEADER_DEST)/$(XVPRIINC)
      OLPUB_HEADER_DEST = $(HEADER_DEST)/$(OLPUBINC)
      OLPRI_HEADER_DEST = $(HEADER_DEST)/$(OLPRIINC)
      PSPUB_HEADER_DEST = $(HEADER_DEST)/$(PSPUBINC)
      PSPRI_HEADER_DEST = $(HEADER_DEST)/$(PSPRIINC)
            IMAGES_DEST = $(HEADER_DEST)/$(IMAGEINC)
            BITMAP_DEST = $(HEADER_DEST)/$(BITMAPINC)

                LINTDIR = $(LINT_LIBRARY_DEST)

            DEPXVIEWLIB = $(LIBRARY_DEST)/libxview.a
               XVIEWLIB = 			 -L$(LIBRARY_DEST) -lxview

            DEPOLGXLIB = $(LIBRARY_DEST)/libolgx.a
               OLGXLIB = 			 -L$(LIBRARY_DEST) -lolgx

            DEPXVPSLIB = $(LIBRARY_DEST)/libxvps.a
               XVPSLIB = 			 -L$(LIBRARY_DEST) -lxvps

             XLIB = $(EXTENSIONLIB) 			 -lX11

             SYSV_CLIENT_LIB =
             XVCLIENTSYSDEFINES = -DSUNOS41

        XVCLIENTI18NDEFINES = -DOW_I18N_L3
        XVCLIENTMISCLIB =
        XVCLIENTINCLUDES =

        XVCLIENTDEFINES = $(XVCLIENTSYSDEFINES) $(XVCLIENTI18NDEFINES)

###########################################################################
# @(#)XView.rules	1.8 12 May 1993 SMI
# XView.rules for XView release 3.0.  Imake rules needed by distribution.

# $XFree86: mit/config/Library.tmpl,v 1.5 1993/03/27 08:55:03 dawes Exp $
# $XConsortium: Library.tmpl,v 1.12 92/03/20 15:05:19 rws Exp $

_NULLCMD_ = @echo -n

           CC = 		gcc
    CCOPTIONS = -m486
ANSICCOPTIONS =
  STD_DEFINES =
  CDEBUGFLAGS = -O2

###########################################################################
# @(#)Imakefile	1.12 28 Jun 1993 SMI

HDRSPUBLIC=     server.h
HDRSPRIVATE=    svr_impl.h svr_atom.h svr_kmdata.h

DATA_OBJS = svr_data.o

TEXT_OBJS =	server.o	svr_x.o	svr_get.o	svr_rodata.o	svr_atom.o	svr_set.o	svr_parse.o

SRCS =	server.c	svr_x.c	svr_get.c	svr_set.c	svr_rodata.c	svr_atom.c	svr_data.c	svr_parse.c

LIBTARGET = xview

TARGETDIR = ../../lib${LIBTARGET}/
SOREV = $(SOXVIEWREV)

###########################################################################
# @(#)XView.obj	1.6 28 Jun 1993 SMI
# XView.obj for XView release 3.0.  Common commands for all library objects.

            STD_DEFINES = 		 	 -fwritable-strings -DSUNOS41 -DFULL_R5 -I$(OPENWINHOME)/include

HEADERS= ${HDRSPUBLIC} ${HDRSPRIVATE}
INCLUDES= -I$(BUILDINCDIR)$(INCDIR)
OBJS = $(TEXT_OBJS) $(DATA_OBJS) $(SPECIAL_OBJS)

ALLFILES = $(SRCS) $(HEADERS)

.c.o:
	$(RM) $@
	$(CC) -c $(CFLAGS) $(_NOOP_) $*.c

all:: lib${LIBTARGET}.a

lib${LIBTARGET}.a:: $(OBJS)
	$(AR) $(TARGETDIR)$@ $(OBJS)

lintlib::
	-@if [ ! -d $(LINTDIR) ]; then mkdir $(LINTDIR); else exit 0; fi
	$(RM) ?*.ln;
	@case '${MFLAGS}' in *[ik]*) set +e;; esac; \
	for i in $(SRCS) ;\
	do \
	echo "linting $$i"; \
	$(LINT) $(LINTFLAGS) -i -c $$i; \
	if [ ! -f ?*.ln ]; then \
	echo "lint failed for $$i"; \
	else \
	$(MV) ?*.ln $(LINTDIR); \
	fi \
	done

depend::
	$(DEPEND) $(DEPENDFLAGS) -s "# DO NOT DELETE" -- $(ALLDEFINES) -- $(SRCS)

lint:
	$(LINT) $(LINTFLAGS) $(SRCS) $(LINTLIBS)
lint1:
	$(LINT) $(LINTFLAGS) $(FILE) $(LINTLIBS)

includes::
	@if [ -d $(BUILDINCDIR)/$(XVPUB_INCDIR) ]; then set +x; \
	else (set -x; $(MKDIRHIER) $(BUILDINCDIR)/$(XVPUB_INCDIR)); fi
	@(set -x; cd $(BUILDINCDIR)/$(XVPUB_INCDIR); for i in $(HDRSPUBLIC); do \
	$(RM) $$i; \
	$(LN) $(BUILDINCTOP)//$(CURRENT_DIR)/$$i .; \
	done)

install::
	$(MKDIRHIER) $(XVPUB_HEADER_DEST)

install:: $(HDRSPUBLIC)
	@if [ -d $(DESTDIR)$(XVPUB_HEADER_DEST) ]; then set +x; \
	else (set -x; $(MKDIRHIER) $(DESTDIR)$(XVPUB_HEADER_DEST)); fi
	@case '${MFLAGS}' in *[i]*) set +e;; esac; \
	for i in $(HDRSPUBLIC); do \
	(set -x; $(INSTALL) -c $(INSTALLFLAGS) $$i $(DESTDIR)$(XVPUB_HEADER_DEST)); \
	done

includes::
	@if [ -d $(BUILDINCDIR)/$(XVPRI_INCDIR) ]; then set +x; \
	else (set -x; $(MKDIRHIER) $(BUILDINCDIR)/$(XVPRI_INCDIR)); fi
	@(set -x; cd $(BUILDINCDIR)/$(XVPRI_INCDIR); for i in $(HDRSPRIVATE); do \
	$(RM) $$i; \
	$(LN) $(BUILDINCTOP)//$(CURRENT_DIR)/$$i .; \
	done)

install::
	$(MKDIRHIER) $(XVPRI_HEADER_DEST)

install:: $(HDRSPRIVATE)
	@if [ -d $(DESTDIR)$(XVPRI_HEADER_DEST) ]; then set +x; \
	else (set -x; $(MKDIRHIER) $(DESTDIR)$(XVPRI_HEADER_DEST)); fi
	@case '${MFLAGS}' in *[i]*) set +e;; esac; \
	for i in $(HDRSPRIVATE); do \
	(set -x; $(INSTALL) -c $(INSTALLFLAGS) $$i $(DESTDIR)$(XVPRI_HEADER_DEST)); \
	done

populate::
	@case '${MFLAGS}' in *[i]*) set +e;; esac;
	-@if [ -h SCCS ]; then \
	for i in $(ALLFILES) ; do sccs get $$i ; done ; \
	elif [ -h SCCS- ]; then \
	$(MV) SCCS- SCCS ; \
	for i in $(ALLFILES) ; do sccs get $$i ; done ; \
	$(MV) SCCS SCCS- ; \
	else \
	echo "SCCS or SCCS- do not exist" ; \
	fi

includes::

install::

# -------------------------------------------------------------------------
# common rules for all Makefiles - do not edit

emptyrule::

clean::
	$(RM_CMD) "#"*

Makefile::
	-@if [ -f Makefile ]; then set -x; \
	$(RM) Makefile.bak; $(MV) Makefile Makefile.bak; \
	else exit 0; fi
	$(IMAKE_CMD) -DTOPDIR=$(TOP) -DCURDIR=$(CURRENT_DIR)

tags::
	$(TAGS) -w *.[ch]
	$(TAGS) -xw *.[ch] > TAGS

# -------------------------------------------------------------------------
# empty rules for directories that do not have SUBDIRS - do not edit

install::
	@echo "install in $(CURRENT_DIR) done"

install.man::
	@echo "install.man in $(CURRENT_DIR) done"

install.linkkit::
	@echo "install.linkkit in $(CURRENT_DIR) done"

Makefiles::

includes::

# -------------------------------------------------------------------------
# dependencies generated by makedepend

# DO NOT DELETE

server.o: /usr/include/sys/types.h /usr/include/machine/ansi.h
server.o: /usr/include/sys/stat.h /usr/include/sys/cdefs.h
server.o: /usr/include/sys/time.h /usr/include/sys/time.h
server.o: /usr/include/sys/param.h /usr/include/sys/syslimits.h
server.o: /usr/include/sys/signal.h /usr/include/machine/trap.h
server.o: /usr/include/machine/param.h /usr/include/machine/endian.h
server.o: /usr/include/machine/limits.h /usr/include/dirent.h
server.o: /usr/include/stdio.h /usr/include/string.h
server.o: ../../.././build/include/xview/win_input.h
server.o: /usr/X386/include/X11/Xlib.h /usr/X386/include/X11/X.h
server.o: /usr/X386/include/X11/Xfuncproto.h /usr/X386/include/X11/Xosdefs.h
server.o: /usr/include/stddef.h ../../.././build/include/xview/base.h
server.o: /usr/include/stdlib.h ../../.././build/include/xview/xv_c_types.h
server.o: ../../.././build/include/xview/win_event.h
server.o: ../../.././build/include/xview/win_struct.h
server.o: ../../.././build/include/xview_private/ntfy.h
server.o: ../../.././build/include/xview_private/ultrix_cpt.h
server.o: /usr/include/sys/resource.h /usr/include/sys/wait.h
server.o: /usr/include/signal.h ../../.././build/include/xview/notify.h
server.o: ../../.././build/include/xview_private/ntfy_ctbl.h
server.o: ../../.././build/include/xview_private/ndet.h
server.o: ../../.././build/include/xview/win_notify.h
server.o: ../../.././build/include/xview/defaults.h
server.o: ../../.././build/include/xview_private/portable.h
server.o: ../../.././build/include/xview/attr.h /usr/include/varargs.h
server.o: /usr/include/stdarg.h /usr/include/machine/stdarg.h
server.o: /usr/include/strings.h
server.o: ../../.././build/include/xview_private/svr_atom.h
server.o: ../../.././build/include/xview_private/svr_impl.h
server.o: ../../.././build/include/xview_private/xv_list.h
server.o: ../../.././build/include/xview/pkg.h
server.o: ../../.././build/include/xview/server.h
server.o: ../../.././build/include/xview/generic.h
server.o: ../../.././build/include/xview/pkg_public.h
server.o: ../../.././build/include/xview/xv_error.h
server.o: ../../.././build/include/xview/sun.h
server.o: ../../.././build/include/xview/screen.h
server.o: ../../.././build/include/xview/sel_svc.h /usr/include/netinet/in.h
server.o: ../../.././build/include/xview/sel_attrs.h
server.o: /usr/include/sys/socket.h /usr/X386/include/X11/Xatom.h
server.o: /usr/include/rpc/rpc.h /usr/include/rpc/types.h
server.o: /usr/include/rpc/xdr.h /usr/include/rpc/auth.h
server.o: /usr/include/rpc/clnt.h /usr/include/rpc/rpc_msg.h
server.o: /usr/include/rpc/auth_unix.h /usr/include/rpc/svc.h
server.o: /usr/include/rpc/svc_auth.h
server.o: ../../.././build/include/xview/sel_pkg.h
server.o: /usr/X386/include/X11/Xutil.h /usr/include/errno.h
server.o: ../../.././build/include/xview_private/seln_impl.h
server.o: /usr/include/netdb.h
server.o: ../../.././build/include/xview_private/i18n_impl.h
server.o: /usr/include/locale.h /usr/X386/include/X11/Xresource.h
server.o: ../../.././build/include/xview_private/svr_kmdata.h
server.o: /usr/X386/include/X11/keysym.h /usr/X386/include/X11/keysymdef.h
server.o: ../../.././build/include/xview_private/draw_impl.h
server.o: ../../.././build/include/xview/drawable.h
server.o: ../../.././build/include/xview/cms.h
server.o: ../../.././build/include/xview_private/scrn_vis.h
server.o: ../../.././build/include/xview_private/xv_color.h
svr_x.o: /usr/include/stdio.h /usr/include/sys/cdefs.h
svr_x.o: /usr/include/machine/ansi.h ../../.././build/include/xview/pkg.h
svr_x.o: ../../.././build/include/xview/attr.h /usr/include/sys/types.h
svr_x.o: ../../.././build/include/xview/base.h /usr/include/string.h
svr_x.o: /usr/include/stdlib.h ../../.././build/include/xview/xv_c_types.h
svr_x.o: ../../.././build/include/xview/notify.h /usr/include/sys/wait.h
svr_x.o: /usr/include/sys/time.h /usr/include/sys/time.h
svr_x.o: /usr/include/sys/resource.h /usr/include/varargs.h
svr_x.o: /usr/include/stdarg.h /usr/include/machine/stdarg.h
svr_x.o: /usr/include/fcntl.h
svr_x.o: ../../.././build/include/xview_private/i18n_impl.h
svr_x.o: /usr/include/locale.h ../../.././build/include/xview/win_event.h
svr_x.o: /usr/X386/include/X11/Xlib.h /usr/X386/include/X11/X.h
svr_x.o: /usr/X386/include/X11/Xfuncproto.h /usr/X386/include/X11/Xosdefs.h
svr_x.o: /usr/include/stddef.h ../../.././build/include/xview/defaults.h
svr_x.o: ../../.././build/include/xview/sel_svc.h /usr/include/netinet/in.h
svr_x.o: ../../.././build/include/xview/server.h
svr_x.o: ../../.././build/include/xview/generic.h
svr_x.o: ../../.././build/include/xview/pkg_public.h
svr_x.o: ../../.././build/include/xview/xv_error.h
svr_x.o: ../../.././build/include/xview/sun.h
svr_x.o: ../../.././build/include/xview/win_input.h
svr_x.o: ../../.././build/include/xview/sel_attrs.h /usr/include/sys/socket.h
svr_x.o: /usr/X386/include/X11/Xatom.h /usr/include/rpc/rpc.h
svr_x.o: /usr/include/rpc/types.h /usr/include/rpc/xdr.h
svr_x.o: /usr/include/rpc/auth.h /usr/include/rpc/clnt.h
svr_x.o: /usr/include/rpc/rpc_msg.h /usr/include/rpc/auth_unix.h
svr_x.o: /usr/include/rpc/svc.h /usr/include/rpc/svc_auth.h
svr_x.o: ../../.././build/include/xview_private/svr_impl.h
svr_x.o: ../../.././build/include/xview_private/xv_list.h
svr_x.o: ../../.././build/include/xview/screen.h
svr_x.o: ../../.././build/include/xview/sel_pkg.h
svr_x.o: /usr/X386/include/X11/Xutil.h /usr/include/errno.h
svr_x.o: ../../.././build/include/xview_private/seln_impl.h
svr_x.o: /usr/include/netdb.h /usr/X386/include/X11/Xresource.h
svr_x.o: /usr/X386/include/X11/keysym.h /usr/X386/include/X11/keysymdef.h
svr_get.o: /usr/include/sys/types.h /usr/include/machine/ansi.h
svr_get.o: /usr/include/sys/time.h /usr/include/sys/time.h
svr_get.o: /usr/include/sys/cdefs.h
svr_get.o: ../../.././build/include/xview/win_event.h
svr_get.o: ../../.././build/include/xview_private/svr_impl.h
svr_get.o: ../../.././build/include/xview_private/xv_list.h
svr_get.o: ../../.././build/include/xview/pkg.h
svr_get.o: ../../.././build/include/xview/attr.h
svr_get.o: ../../.././build/include/xview/base.h /usr/include/string.h
svr_get.o: /usr/include/stdlib.h ../../.././build/include/xview/xv_c_types.h
svr_get.o: ../../.././build/include/xview/notify.h /usr/include/stdio.h
svr_get.o: /usr/include/sys/wait.h /usr/include/sys/resource.h
svr_get.o: /usr/include/varargs.h /usr/include/stdarg.h
svr_get.o: /usr/include/machine/stdarg.h
svr_get.o: ../../.././build/include/xview/server.h
svr_get.o: ../../.././build/include/xview/generic.h
svr_get.o: ../../.././build/include/xview/pkg_public.h
svr_get.o: ../../.././build/include/xview/xv_error.h
svr_get.o: ../../.././build/include/xview/sun.h
svr_get.o: ../../.././build/include/xview/screen.h
svr_get.o: ../../.././build/include/xview/sel_svc.h /usr/include/netinet/in.h
svr_get.o: ../../.././build/include/xview/win_input.h
svr_get.o: /usr/X386/include/X11/Xlib.h /usr/X386/include/X11/X.h
svr_get.o: /usr/X386/include/X11/Xfuncproto.h /usr/X386/include/X11/Xosdefs.h
svr_get.o: /usr/include/stddef.h ../../.././build/include/xview/sel_attrs.h
svr_get.o: /usr/include/sys/socket.h /usr/X386/include/X11/Xatom.h
svr_get.o: /usr/include/rpc/rpc.h /usr/include/rpc/types.h
svr_get.o: /usr/include/rpc/xdr.h /usr/include/rpc/auth.h
svr_get.o: /usr/include/rpc/clnt.h /usr/include/rpc/rpc_msg.h
svr_get.o: /usr/include/rpc/auth_unix.h /usr/include/rpc/svc.h
svr_get.o: /usr/include/rpc/svc_auth.h
svr_get.o: ../../.././build/include/xview/sel_pkg.h
svr_get.o: /usr/X386/include/X11/Xutil.h /usr/include/errno.h
svr_get.o: ../../.././build/include/xview_private/seln_impl.h
svr_get.o: /usr/include/netdb.h
svr_get.o: ../../.././build/include/xview_private/i18n_impl.h
svr_get.o: /usr/include/locale.h /usr/X386/include/X11/Xresource.h
svr_get.o: ../../.././build/include/xview_private/portable.h
svr_get.o: /usr/include/strings.h
svr_set.o: /usr/include/sys/types.h /usr/include/machine/ansi.h
svr_set.o: /usr/include/sys/time.h /usr/include/sys/time.h
svr_set.o: /usr/include/sys/cdefs.h
svr_set.o: ../../.././build/include/xview/win_event.h
svr_set.o: ../../.././build/include/xview_private/svr_impl.h
svr_set.o: ../../.././build/include/xview_private/xv_list.h
svr_set.o: ../../.././build/include/xview/pkg.h
svr_set.o: ../../.././build/include/xview/attr.h
svr_set.o: ../../.././build/include/xview/base.h /usr/include/string.h
svr_set.o: /usr/include/stdlib.h ../../.././build/include/xview/xv_c_types.h
svr_set.o: ../../.././build/include/xview/notify.h /usr/include/stdio.h
svr_set.o: /usr/include/sys/wait.h /usr/include/sys/resource.h
svr_set.o: /usr/include/varargs.h /usr/include/stdarg.h
svr_set.o: /usr/include/machine/stdarg.h
svr_set.o: ../../.././build/include/xview/server.h
svr_set.o: ../../.././build/include/xview/generic.h
svr_set.o: ../../.././build/include/xview/pkg_public.h
svr_set.o: ../../.././build/include/xview/xv_error.h
svr_set.o: ../../.././build/include/xview/sun.h
svr_set.o: ../../.././build/include/xview/screen.h
svr_set.o: ../../.././build/include/xview/sel_svc.h /usr/include/netinet/in.h
svr_set.o: ../../.././build/include/xview/win_input.h
svr_set.o: /usr/X386/include/X11/Xlib.h /usr/X386/include/X11/X.h
svr_set.o: /usr/X386/include/X11/Xfuncproto.h /usr/X386/include/X11/Xosdefs.h
svr_set.o: /usr/include/stddef.h ../../.././build/include/xview/sel_attrs.h
svr_set.o: /usr/include/sys/socket.h /usr/X386/include/X11/Xatom.h
svr_set.o: /usr/include/rpc/rpc.h /usr/include/rpc/types.h
svr_set.o: /usr/include/rpc/xdr.h /usr/include/rpc/auth.h
svr_set.o: /usr/include/rpc/clnt.h /usr/include/rpc/rpc_msg.h
svr_set.o: /usr/include/rpc/auth_unix.h /usr/include/rpc/svc.h
svr_set.o: /usr/include/rpc/svc_auth.h
svr_set.o: ../../.././build/include/xview/sel_pkg.h
svr_set.o: /usr/X386/include/X11/Xutil.h /usr/include/errno.h
svr_set.o: ../../.././build/include/xview_private/seln_impl.h
svr_set.o: /usr/include/netdb.h
svr_set.o: ../../.././build/include/xview_private/i18n_impl.h
svr_set.o: /usr/include/locale.h /usr/X386/include/X11/Xresource.h
svr_rodata.o: /usr/X386/include/X11/keysym.h
svr_rodata.o: /usr/X386/include/X11/keysymdef.h
svr_rodata.o: ../../.././build/include/xview/win_input.h
svr_rodata.o: /usr/include/sys/types.h /usr/include/machine/ansi.h
svr_rodata.o: /usr/include/sys/time.h /usr/include/sys/time.h
svr_rodata.o: /usr/include/sys/cdefs.h /usr/X386/include/X11/Xlib.h
svr_rodata.o: /usr/X386/include/X11/X.h /usr/X386/include/X11/Xfuncproto.h
svr_rodata.o: /usr/X386/include/X11/Xosdefs.h /usr/include/stddef.h
svr_rodata.o: ../../.././build/include/xview/base.h /usr/include/string.h
svr_rodata.o: /usr/include/stdlib.h
svr_rodata.o: ../../.././build/include/xview/xv_c_types.h
svr_rodata.o: ../../.././build/include/xview/win_event.h
svr_rodata.o: ../../.././build/include/xview_private/portable.h
svr_rodata.o: ../../.././build/include/xview/attr.h /usr/include/varargs.h
svr_rodata.o: /usr/include/stdarg.h /usr/include/machine/stdarg.h
svr_rodata.o: /usr/include/strings.h
svr_atom.o: ../../.././build/include/xview/xview.h
svr_atom.o: ../../.././build/include/xview/xview_xvin.h /usr/include/signal.h
svr_atom.o: /usr/include/sys/types.h /usr/include/machine/ansi.h
svr_atom.o: ../../.././build/include/pixrect/pixrect.h
svr_atom.o: ../../.././build/include/pixrect/pr_planegroups.h
svr_atom.o: ../../.././build/include/pixrect/pr_util.h
svr_atom.o: ../../.././build/include/pixrect/memvar.h
svr_atom.o: ../../.././build/include/pixrect/pixfont.h
svr_atom.o: ../../.././build/include/pixrect/traprop.h
svr_atom.o: ../../.././build/include/pixrect/pr_line.h
svr_atom.o: ../../.././build/include/xview/xv_c_types.h
svr_atom.o: ../../.././build/include/xview/generic.h
svr_atom.o: ../../.././build/include/xview/pkg_public.h
svr_atom.o: ../../.././build/include/xview/pkg.h
svr_atom.o: ../../.././build/include/xview/attr.h
svr_atom.o: ../../.././build/include/xview/base.h /usr/include/string.h
svr_atom.o: /usr/include/sys/cdefs.h /usr/include/stdlib.h
svr_atom.o: ../../.././build/include/xview/notify.h /usr/include/stdio.h
svr_atom.o: /usr/include/sys/wait.h /usr/include/sys/time.h
svr_atom.o: /usr/include/sys/time.h /usr/include/sys/resource.h
svr_atom.o: /usr/include/varargs.h /usr/include/stdarg.h
svr_atom.o: /usr/include/machine/stdarg.h
svr_atom.o: ../../.././build/include/xview/xv_error.h
svr_atom.o: ../../.././build/include/xview/sun.h
svr_atom.o: ../../.././build/include/xview/server.h
svr_atom.o: ../../.././build/include/xview/screen.h
svr_atom.o: ../../.././build/include/xview/pixwin.h
svr_atom.o: ../../.././build/include/xview/rect.h
svr_atom.o: ../../.././build/include/xview/rectlist.h
svr_atom.o: ../../.././build/include/xview/win_input.h
svr_atom.o: /usr/X386/include/X11/Xlib.h /usr/X386/include/X11/X.h
svr_atom.o: /usr/X386/include/X11/Xfuncproto.h
svr_atom.o: /usr/X386/include/X11/Xosdefs.h /usr/include/stddef.h
svr_atom.o: ../../.././build/include/xview/win_event.h
svr_atom.o: ../../.././build/include/xview/xv_version.h
svr_atom.o: ../../.././build/include/xview/icon.h
svr_atom.o: ../../.././build/include/xview/window.h
svr_atom.o: ../../.././build/include/xview/drawable.h
svr_atom.o: ../../.././build/include/xview/attrol.h
svr_atom.o: ../../.././build/include/xview/svrimage.h
svr_atom.o: ../../.././build/include/xview/frame.h
svr_atom.o: ../../.././build/include/xview/openmenu.h
svr_atom.o: /usr/X386/include/X11/Xutil.h /usr/X386/include/X11/Xresource.h
svr_atom.o: ../../.././build/include/xview_private/svr_impl.h
svr_atom.o: ../../.././build/include/xview_private/xv_list.h
svr_atom.o: ../../.././build/include/xview/sel_svc.h
svr_atom.o: /usr/include/netinet/in.h
svr_atom.o: ../../.././build/include/xview/sel_attrs.h
svr_atom.o: /usr/include/sys/socket.h /usr/X386/include/X11/Xatom.h
svr_atom.o: /usr/include/rpc/rpc.h /usr/include/rpc/types.h
svr_atom.o: /usr/include/rpc/xdr.h /usr/include/rpc/auth.h
svr_atom.o: /usr/include/rpc/clnt.h /usr/include/rpc/rpc_msg.h
svr_atom.o: /usr/include/rpc/auth_unix.h /usr/include/rpc/svc.h
svr_atom.o: /usr/include/rpc/svc_auth.h
svr_atom.o: ../../.././build/include/xview/sel_pkg.h /usr/include/errno.h
svr_atom.o: ../../.././build/include/xview_private/seln_impl.h
svr_atom.o: /usr/include/netdb.h
svr_atom.o: ../../.././build/include/xview_private/i18n_impl.h
svr_atom.o: /usr/include/locale.h
svr_data.o: ../../.././build/include/xview_private/svr_impl.h
svr_data.o: /usr/include/sys/types.h /usr/include/machine/ansi.h
svr_data.o: ../../.././build/include/xview_private/xv_list.h
svr_data.o: ../../.././build/include/xview/pkg.h
svr_data.o: ../../.././build/include/xview/attr.h
svr_data.o: ../../.././build/include/xview/base.h /usr/include/string.h
svr_data.o: /usr/include/sys/cdefs.h /usr/include/stdlib.h
svr_data.o: ../../.././build/include/xview/xv_c_types.h
svr_data.o: ../../.././build/include/xview/notify.h /usr/include/stdio.h
svr_data.o: /usr/include/sys/wait.h /usr/include/sys/time.h
svr_data.o: /usr/include/sys/time.h /usr/include/sys/resource.h
svr_data.o: /usr/include/varargs.h /usr/include/stdarg.h
svr_data.o: /usr/include/machine/stdarg.h
svr_data.o: ../../.././build/include/xview/server.h
svr_data.o: ../../.././build/include/xview/generic.h
svr_data.o: ../../.././build/include/xview/pkg_public.h
svr_data.o: ../../.././build/include/xview/xv_error.h
svr_data.o: ../../.././build/include/xview/sun.h
svr_data.o: ../../.././build/include/xview/screen.h
svr_data.o: ../../.././build/include/xview/sel_svc.h
svr_data.o: /usr/include/netinet/in.h
svr_data.o: ../../.././build/include/xview/win_input.h
svr_data.o: /usr/X386/include/X11/Xlib.h /usr/X386/include/X11/X.h
svr_data.o: /usr/X386/include/X11/Xfuncproto.h
svr_data.o: /usr/X386/include/X11/Xosdefs.h /usr/include/stddef.h
svr_data.o: ../../.././build/include/xview/win_event.h
svr_data.o: ../../.././build/include/xview/sel_attrs.h
svr_data.o: /usr/include/sys/socket.h /usr/X386/include/X11/Xatom.h
svr_data.o: /usr/include/rpc/rpc.h /usr/include/rpc/types.h
svr_data.o: /usr/include/rpc/xdr.h /usr/include/rpc/auth.h
svr_data.o: /usr/include/rpc/clnt.h /usr/include/rpc/rpc_msg.h
svr_data.o: /usr/include/rpc/auth_unix.h /usr/include/rpc/svc.h
svr_data.o: /usr/include/rpc/svc_auth.h
svr_data.o: ../../.././build/include/xview/sel_pkg.h
svr_data.o: /usr/X386/include/X11/Xutil.h /usr/include/errno.h
svr_data.o: ../../.././build/include/xview_private/seln_impl.h
svr_data.o: /usr/include/netdb.h
svr_data.o: ../../.././build/include/xview_private/i18n_impl.h
svr_data.o: /usr/include/locale.h /usr/X386/include/X11/Xresource.h
svr_parse.o: /usr/include/ctype.h /usr/include/sys/cdefs.h
svr_parse.o: /usr/X386/include/X11/Xlib.h /usr/include/sys/types.h
svr_parse.o: /usr/include/machine/ansi.h /usr/X386/include/X11/X.h
svr_parse.o: /usr/X386/include/X11/Xfuncproto.h
svr_parse.o: /usr/X386/include/X11/Xosdefs.h /usr/include/stddef.h
svr_parse.o: /usr/X386/include/X11/keysym.h /usr/X386/include/X11/keysymdef.h
svr_parse.o: /usr/X386/include/X11/Xresource.h
svr_parse.o: ../../.././build/include/xview_private/svr_impl.h
svr_parse.o: ../../.././build/include/xview_private/xv_list.h
svr_parse.o: ../../.././build/include/xview/pkg.h
svr_parse.o: ../../.././build/include/xview/attr.h
svr_parse.o: ../../.././build/include/xview/base.h /usr/include/string.h
svr_parse.o: /usr/include/stdlib.h
svr_parse.o: ../../.././build/include/xview/xv_c_types.h
svr_parse.o: ../../.././build/include/xview/notify.h /usr/include/stdio.h
svr_parse.o: /usr/include/sys/wait.h /usr/include/sys/time.h
svr_parse.o: /usr/include/sys/time.h /usr/include/sys/resource.h
svr_parse.o: /usr/include/varargs.h /usr/include/stdarg.h
svr_parse.o: /usr/include/machine/stdarg.h
svr_parse.o: ../../.././build/include/xview/server.h
svr_parse.o: ../../.././build/include/xview/generic.h
svr_parse.o: ../../.././build/include/xview/pkg_public.h
svr_parse.o: ../../.././build/include/xview/xv_error.h
svr_parse.o: ../../.././build/include/xview/sun.h
svr_parse.o: ../../.././build/include/xview/screen.h
svr_parse.o: ../../.././build/include/xview/sel_svc.h
svr_parse.o: /usr/include/netinet/in.h
svr_parse.o: ../../.././build/include/xview/win_input.h
svr_parse.o: ../../.././build/include/xview/win_event.h
svr_parse.o: ../../.././build/include/xview/sel_attrs.h
svr_parse.o: /usr/include/sys/socket.h /usr/X386/include/X11/Xatom.h
svr_parse.o: /usr/include/rpc/rpc.h /usr/include/rpc/types.h
svr_parse.o: /usr/include/rpc/xdr.h /usr/include/rpc/auth.h
svr_parse.o: /usr/include/rpc/clnt.h /usr/include/rpc/rpc_msg.h
svr_parse.o: /usr/include/rpc/auth_unix.h /usr/include/rpc/svc.h
svr_parse.o: /usr/include/rpc/svc_auth.h
svr_parse.o: ../../.././build/include/xview/sel_pkg.h
svr_parse.o: /usr/X386/include/X11/Xutil.h /usr/include/errno.h
svr_parse.o: ../../.././build/include/xview_private/seln_impl.h
svr_parse.o: /usr/include/netdb.h
svr_parse.o: ../../.././build/include/xview_private/i18n_impl.h
svr_parse.o: /usr/include/locale.h
svr_parse.o: ../../.././build/include/xview_private/portable.h
svr_parse.o: /usr/include/strings.h
