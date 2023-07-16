#!/bin/sh
PREFIX="x86_64-w64-mingw32"
INST_DIR="/usr/$PREFIX"
export CC="$PREFIX-gcc"
export CXX="$PREFIX-g++"
export CPP="$PREFIX-cpp"
export RANLIB="$PREFIX-ranlib"
export PATH="/usr/$PREFIX/bin:/usr/local/bin:/usr/bin:/bin:/usr/local/games:/usr/games"
mkdir -p w64
cd w64
../configure --host="$PREFIX" --build="$(../autotools/config.guess)" \
        --with-wx-prefix="$INST_DIR" --with-sdl-prefix="$INST_DIR" \
        --enable-sdl-static LDFLAGS="-static"
make -j8
strip -s src/xdre.exe
zip -j -9 ../xdre-w64.zip src/xdre.exe src/prboom-plus/data/prboom-plus.wad ../README.md ../COPYING
