dnl Process this file with autoconf to produce a configure script.
AC_INIT([PrBoom-Plus],[2.5.1.5 (XDRE 2.23)],[],[prboom-plus])

AC_CONFIG_HEADER(config.h)
AC_CONFIG_AUX_DIR(autotools)
AC_CONFIG_MACRO_DIR(autotools)

dnl --- Check for target system type - before initialising Automake
AC_CANONICAL_TARGET

dnl --- Fire up automake
AM_INIT_AUTOMAKE([foreign -Woverride -Wobsolete -Wportability -Wsyntax -Wunsupported])
ifdef([AM_SILENT_RULES],[AM_SILENT_RULES(yes)])
AM_MAINTAINER_MODE([enable])

AC_ARG_WITH(waddir,AC_HELP_STRING([--with-waddir],[Path to install prboom.wad and look for other WAD files]),
DOOMWADDIR="$withval",
if test "x$prefix" != xNONE; then
  DOOMWADDIR="$prefix/share/games/doom"
else
  DOOMWADDIR="$ac_default_prefix/share/games/doom"
fi
)
AC_DEFINE_UNQUOTED(DOOMWADDIR,"$DOOMWADDIR",[Define to be the path where Doom WADs are stored])

dnl --- Check for programs
AC_PROG_CC
AC_PROG_LN_S
AC_PROG_RANLIB
AC_GNU_SOURCE

dnl --- cph: work out flags to pass to compiler
CFLAGS_OPT="-O2 -fomit-frame-pointer"
dnl --- kp: Suppress all PrBoom+ compile warnings, as they don't directly pertain to XDRE.
dnl         Also, -fcommon tells the compiler to ignore multiple definition errors
dnl         which throw an error in newer versions of GCC (10+), but didn't when this
dnl         version of PrBoom+ was released.
dnl         Original code line:
dnl         AC_C_COMPILE_FLAGS(-Wall -Wextra -Wno-missing-field-initializers -Wwrite-strings -Wundef -Wbad-function-cast -Wcast-align -Wcast-qual -Wdeclaration-after-statement -Wpointer-arith -fcommon -ffast-math)
AC_C_COMPILE_FLAGS(-w -fcommon -ffast-math)
dnl --- Option to enable debugging
AC_ARG_ENABLE(debug,AC_HELP_STRING([--enable-debug],[turns on various debugging features, like range checking and internal heap diagnostics]),,enable_debug="no")
if test "$enable_debug" = "no"
then
    AC_C_COMPILE_FLAGS(-Wno-unused -Wno-switch -Wno-sign-compare -Wno-pointer-sign)
else
    CFLAGS_OPT="-g"
    AC_C_COMPILE_FLAGS(-Wmissing-noreturn -Wredundant-decls -Wnested-externs -Wold-style-definition -Wmissing-declarations -Wmissing-format-attribute -Wvariadic-macros -Wdisabled-optimization -Wlarger-than-128000 -Waggregate-return -Wvolatile-register-var -Winline)
    AC_DEFINE(RANGECHECK,1,[Define to enable internal range checking])
    AC_DEFINE(INSTRUMENTED,1,[Define this to see real-time memory allocation statistics, and enable extra debugging features])
    AC_DEFINE(TIMEDIAG,1,[Defining this causes time stamps to be created each time a lump is locked, and lumps locked for long periods of time are reported])
    AC_DEFINE(XDRE_DEBUG,1,[Define this for XDRE debugging])
fi
AC_ARG_ENABLE(profile,AC_HELP_STRING([--enable-profile],[turns on profiling]),,enable_profile="no")
if test "$enable_profile" = "yes"
then
    CFLAGS_OPT="-pg"
fi
AM_WITH_DMALLOC

dnl --- Try for processor optimisations
AC_CPU_OPTIMISATIONS

dnl --- some defaults for CFLAGS
AC_C_COMPILE_FLAGS($CFLAGS_OPT)
dnl -Wmissing-prototypes -Wmissing-declarations
CFLAGS="$CFLAGS -I\$(top_srcdir)/src"

dnl --- Compiler characteristics
AC_C_CONST
AC_C_INLINE
if test "$cross_compiling" != "yes"; then
        AC_C_BIGENDIAN
fi

dnl --- Header files, typedefs, structures
AC_TYPE_UID_T
AC_TYPE_SIZE_T
AC_DECL_SYS_SIGLIST
AC_HEADER_SYS_WAIT
AC_CHECK_HEADERS(unistd.h asm/byteorder.h sched.h)

dnl --- Library functions
AC_CHECK_FUNC(stricmp,,AC_DEFINE(stricmp,strcasecmp,[Define to strcasecmp, if we have it]))
AC_CHECK_FUNC(strnicmp,,AC_DEFINE(strnicmp,strncasecmp,[Define to strncasecmp, if we have it]))
AC_CHECK_FUNC(min,AC_DEFINE(MIN,min,[If your platform has a fast version of min, define MIN to it]),)
AC_CHECK_FUNC(max,AC_DEFINE(MAX,max,[If your platform has a fast version of max, define MAX to it]),)
AC_CHECK_FUNCS(getopt inet_aton inet_pton inet_ntop mmap usleep sched_setaffinity)

dnl --- Check for libraries
dnl --- cph: we need pow(3) in SDL/i_sound.c; on some systems it gets pulled
dnl ---  in by other libraries, but we can't rely on that.
AC_CHECK_LIB(m,pow)
dnl - system specific stuff
dnl - winmm for the midi volume hack
case "$target" in
    *-*-cygwin* | *-*-mingw32*)
        SYS_GL_LIBS="-lopengl32 -lglu32 -lgdi32"
	LIBS="-lwinmm $LIBS"
        ;;
    *-*-beos*)
        SYS_GL_LIBS="-lGL -lGLU"
        ;;
    *-*-aix*)
        if test x$ac_cv_prog_gcc = xyes; then
            CFLAGS="$CFLAGS -mthreads"
        fi
        ;;
    *)
	SYS_GL_LIBS="-lGL -lGLU"
	;;
esac

dnl - GL
AC_ARG_ENABLE(gl,AC_HELP_STRING([--enable-gl],[enable OpenGL rendering code]),,enable_gl="no")
if test "$enable_gl" = "yes"
then
    AC_MSG_CHECKING(for OpenGL support)
    have_opengl=no
    AC_TRY_COMPILE([
     #include <GL/gl.h>
    ],[
    ],[
    have_opengl=yes
    ])
    AC_MSG_RESULT($have_opengl)
    if test x$have_opengl = xyes; then
        AC_DEFINE(GL_DOOM,1,[Define if you are building with OpenGL support])
        AC_DEFINE(USE_GLU_IMAGESCALE,1,[Define if you want to use gluImageScale])
        AC_DEFINE(USE_GLU_MIPMAP,1,[Define if you want to use gluBuild2DMipmaps])
        AC_DEFINE(USE_GLU_TESS,1,[Define if you want to use the gluTesselator])
        GL_LIBS="$SYS_GL_LIBS"
    else
        AC_MSG_ERROR([You must have the relevant OpenGL development libraries & headers installed to compile with OpenGL support])
    fi
fi
AM_CONDITIONAL(BUILD_GL,test "$enable_gl" = "yes")

dnl - SDL
AM_PATH_SDL2([2.0.0], [sdl_main="yes"])
if test "$sdl_main" != "yes"
then
 AC_MSG_ERROR([*** You must have the SDL libraries installed before you can compile prboom
 *** See http://prboom.sourceforge.net/linux.html
 ])
fi

savelibs="$LIBS"
LIBS="$LIBS $SDL_LIBS"
AC_CHECK_FUNCS(SDL_JoystickGetAxis)
LIBS="$savelibs"

AC_ARG_WITH([mixer],
  AS_HELP_STRING([--with-mixer],[Use SDL_mixer]),
  [], [with_mixer=no])
AS_IF([test "x$with_mixer" != xno],
  AC_CHECK_LIB(SDL2_mixer,Mix_OpenAudio,[],[],[$SDL_LIBS $MIXER_LIBS]))

AC_ARG_WITH(net,
  AS_HELP_STRING([--with-net],[Use SDL_net]),
  [], [with_net=no])
AS_IF([test "x$with_net" = xno],
  [
    dnl -- Networking was disabled by the user.
    NET_LIBS=
  ],[
    dnl -- Check for networking support
    AC_CHECK_LIB(SDL2_net,SDLNet_UDP_Bind,[
      AC_DEFINE(HAVE_NET,1,[Define if you want network game support])
      AC_DEFINE(USE_SDL_NET,1,[Define if you want to use the SDL net lib])
      NET_LIBS=-lSDL2_net
      sdl_net=yes
    ],[
      dnl -- Warn if networking support was not disabled but sdlnet is missing
      AC_MSG_WARN([*** SDL networking library not found. The game will not be compiled with network game support.])
    ],$SDL_LIBS)
  ])
AM_CONDITIONAL(BUILD_SERVER, [false])

dnl -- PCRE
AC_ARG_WITH(pcre,
  [AS_HELP_STRING([--with-pcre],[Compile with libpcre])],,
  [with_pcre=no])
AS_IF([test "x$with_pcre" != xno],
  [AC_CHECK_LIB(pcreposix,pcreposix_regcomp)]
)

dnl individual sound libraries

AC_ARG_WITH([mad],
  AS_HELP_STRING([--with-mad],[Use MAD mp3 library]),
  [], [with_mad=no])
AS_IF([test "x$with_mad" != xno],
  AC_CHECK_LIB(mad,mad_stream_init))

AC_ARG_WITH([fluidsynth],
  AS_HELP_STRING([--with-fluidsynth],[Use fluidsynth library]),
  [], [with_fluidsynth=no])
AS_IF([test "x$with_fluidsynth" != xno],
  AC_CHECK_LIB(fluidsynth,new_fluid_synth))

AC_ARG_WITH([dumb],
  AS_HELP_STRING([--with-dumb],[Use dumb tracker library]),
  [], [with_dumb=no])
AS_IF([test "x$with_dumb" != xno],
  AC_CHECK_LIB(dumb,dumbfile_open))

AC_ARG_WITH([vorbisfile],
  AS_HELP_STRING([--with-vorbisfile],[Use vorbisfile library]),
  [], [with_vorbisfile=no])
AS_IF([test "x$with_vorbisfile" != xno],
  AC_CHECK_LIB(vorbisfile,ov_test_callbacks,[],[],[-logg -lvorbis]))

dnl on some platforms, porttime is a seperate lib.  ackk
AC_ARG_WITH([portmidi],
  AS_HELP_STRING([--with-portmidi],[Use portmidi library]),
  [], [with_portmidi=no])
AS_IF([test "x$with_portmidi" != xno],
  AC_CHECK_LIB(porttime,Pt_Time,LIBS="-lporttime $LIBS")
  AC_CHECK_LIB(portmidi,Pm_OpenInput))

dnl - SDL_image
AC_ARG_WITH(image,
  [AS_HELP_STRING([--with-image],[Use SDL_image])],,
  [with_image=no])
AS_IF([test "x$with_image" != xno],
  AC_CHECK_LIB(SDL2_image,IMG_Load))

AC_EGREP_HEADER(sockaddr_in6,netinet/in.h,AC_DEFINE(HAVE_IPv6,1,[Define if you have struct sockaddr_in6]))

dnl --- Options
dnl - Always use highres mode and basic checks - anyone that wants these off can edit the config.h
AC_DEFINE(HIGHRES,1,[Define for high resolution support])
AC_DEFINE(SIMPLECHECKS,1,[When defined this causes quick checks which only impose significant overhead if a possible error is detected.])
AC_DEFINE(ZONEIDCHECK,1,[Define this to perform id checks on zone blocks, to detect corrupted and illegally freed blocks])

AC_ARG_ENABLE(nonfree-graphics,
              AC_HELP_STRING([--enable-nonfree-graphics],
                             [build prboom-plus.wad with non-free menu text lumps]),
              [], [enable_nonfree_graphics="no"])
AS_IF([test -f "$srcdir"/data/graphics/m_generl.ppm],
      [], [enable_nonfree_graphics="no"])
AM_CONDITIONAL(NONFREE_GRAPHICS, [test "x$enable_nonfree_graphics" = "xyes"])

AC_ARG_ENABLE(dogs,AC_HELP_STRING([--disable-dogs],[disables support for helper dogs]),,enable_dogs="yes")
if test x"$enable_dogs" = xyes
then
	AC_DEFINE(DOGS,1,[Define for support for MBF helper dogs])
fi
AM_CONDITIONAL(DOGS, [test "x$enable_dogs" = "xyes"])

AC_ARG_ENABLE(heapcheck,AC_HELP_STRING([--enable-heapcheck],[turns on continuous heap checking (very slow)]),,enable_heapcheck="no")
AC_ARG_ENABLE(heapdump,AC_HELP_STRING([--enable-heapdump],[turns on dumping the heap state for debugging]),,enable_heapdump="no")
if test "$enable_heapcheck" = "yes"
then
    AC_DEFINE(CHECKHEAP,1,[Uncomment this to exhaustively run memory checks while the game is running (this is EXTREMELY slow).])
fi
if test "$enable_heapdump" = "yes"
then
    AC_DEFINE(HEAPDUMP,1,[Uncomment this to cause heap dumps to be generated. Only useful if INSTRUMENTED is also defined.])
fi

AM_CONDITIONAL(WAD_MMAP,test "$ac_cv_func_mmap" = yes)
AC_SUBST(MIXER_CFLAGS)
AC_SUBST(MIXER_LIBS)
AC_SUBST(NET_CFLAGS)
AC_SUBST(NET_LIBS)
AC_SUBST(MATH_LIB)
AC_SUBST(GL_LIBS)
AC_SUBST(CFLAGS)
AC_SUBST(DOOMWADDIR)

dnl --- output
AC_OUTPUT(Makefile src/Makefile src/POSIX/Makefile src/SDL/Makefile src/MAC/Makefile src/PCSOUND/Makefile src/TEXTSCREEN/Makefile src/MUSIC/Makefile doc/Makefile data/Makefile ICONS/Makefile VisualC6/Makefile VisualC8/Makefile prboom.spec)
