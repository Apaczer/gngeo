## GNGEO - a NeoGeo emulator

### Native build (linux):
```
./bootstrap
./configure
make -j$(nproc)
```

### Cross-Compile build (MiyooCFW):
- compile binary:
```
make -f Makefile.miyoo -j$(nproc)
```
- or generate IPK package:
```
make -f Makefile.miyoo ipk
```