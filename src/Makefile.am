SUBDIRS = prboom-plus

bin_PROGRAMS = xdre

CFLAGS = @CFLAGS@ @SDL_CFLAGS@ @WX_CFLAGS_ONLY@
CXXFLAGS = @CXXFLAGS@ @SDL_CFLAGS@ @WX_CXXFLAGS_ONLY@ -std=c++11
CPPFLAGS = @CPPFLAGS@ @WX_CPPFLAGS@ -Iprboom-plus/src

cpp_SRC = \
 BruteDialog.hpp    BruteDialog.cpp  \
 HeaderDialog.hpp   HeaderDialog.cpp \
 KeysDialog.hpp     KeysDialog.cpp   \
 mainStuff.cpp                       \
 MainWindow.hpp     MainWindow.cpp   \
 TiclistCtrl.hpp    TiclistCtrl.cpp  \
 xdre.hpp           xdre.cpp

c_SRC = \
 cstuff.h   cstuff.c

xdre_SOURCES = $(c_SRC) $(cpp_SRC)
xdre_LDADD = prboom-plus/src/PCSOUND/libpcsound.a prboom-plus/src/MUSIC/libdoommusic.a prboom-plus/src/libprboom-plus.a prboom-plus/src/SDL/libsdldoom.a prboom-plus/src/MUSIC/libdoommusic.a prboom-plus/src/libprboom-plus.a prboom-plus/src/TEXTSCREEN/libtextscreen.a prboom-plus/src/PCSOUND/libpcsound.a @MIXER_LIBS@ @NET_LIBS@ @SDL_LIBS@ @GL_LIBS@ @MATH_LIB@ @WX_LIBS@
