## GNGEO - a NeoGeo emulator

### Native build (linux):
```
aclocal
autoheader
automake --gnu --add-missing
autoconf
autoupdate
./configure
make -j$(nproc)
```

### Cross-Compile build (MiyooCFW):
```
make -f Makefile.miyoo -j$(nproc)
```
