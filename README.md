# GnGeo

## How to cross-compile on MINGW64 for win32

- Configure & Build  
Install necessary i686 libs (especially SDL), legacy packages can be found at https://github.com/msys2/msys2-archive/releases (use `pacman --upgrade <pkg_file>`)
  
  - configure

        export ACLOCAL_PATH=/mingw32/share/aclocal
        export PATH="/mingw32/bin:$PATH"
        ./bootstrap
        ./configure   --build=x86_64-w64-mingw32   --host=i686-w64-mingw32   CC=i686-w64-mingw32-gcc --disable-i386asm --with-z80core=mamez80 --with-m68kcore=gen68k CFLAGS="-Wno-implicit-function-declaration -Wno-incompatible-pointer-types"
  - build

        make clean
        make -j$(nproc)

- Install  
Copy necessary libs from "/mingw32/bin" to workdir of `gngeo.exe` & `gngeo_data.zip` files, include your own "neogeo.zip" BIOS file

- Run  
Assuming you have placed all files in workdir of GnGeo binary, and your ROMset is `<roname>.zip` file, then you type in shell env:

      ./gngeo.exe -d ./gngeo_data.zip -i ./ -b ./ <romame>

- Dump & Run GNO romset from zip
  - dump

        ./gngeo.exe --dump -d ./gngeo_data.zip -i ./ -b ./ <romame>
  - run

        ./gngeo.exe -d ./gngeo_data.zip -i ./ -b ./ <romame>.gno
