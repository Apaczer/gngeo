## GNGEO - a NeoGeo emulator

ROMSETs preferably should be inline with MAME 0.138 

BIOS expected set of files (neogeo.zip):
```
000-lo.lo
sfix.sfx
usa_2slt.bin - USA 
vs-bios.rom - Japan 
asia-s3.rom - Asia 
sp-s2.sp1 - Europe 
aes-bios.bin - A.E.S
uni-bios.rom - UniBios
```

### Native build (linux):

To make fully working PC builds use stable (upstream compatible) branch:
```
git checkout gngeo-upd
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

### Unsupported ROMsets (ZIP/GNO)

Bellow you'll find a list of games which may not play (or play with glitches), 
due to bugs in existing GnGeo code. If you find this list inaccurate, please
inform in issues section or make a PR with a correction.

* Not implemented:

B2B

* Possible fix exist:

CT2K3SA
FIGHTFEVA
FIGHTFEV
FLIPSHOT
GANRYU
GAROUO
GOWCAIZR
IRRMAZE
JANSHIN
KF10THEP
KF2K2PLA
KOF2K4SE
KOG
LANS2004
LEGENDOS
MAHRETSU
MATRIMBL
MIEXCHNG
MS4PLUS
MSLUG4
NEOCUP98
PBOBBLENA
PNYAA
POPBOUNC
PREISLE2
PSPIKES2
PUZZLDPR
QUIZKOF
RIDHERO
RIDHEROH
ROTD
S1945P
SAMSH5SP
SAMSH5SPH
SAMSH5SPN
SAMSHO5B
SAMSHO5H
SDODGEB
~~SENGOKH (SENGOKUH)~~
SENGOKU2
SENGOKU3
SOCBRAWL
SPINMAST
SSIDEKI2
SSIDEKI3
SSIDEKI4
STRHOOP
SVCPLUSA
SVCPLUS

* Impossible (or hard) to fix:

CT2K3SP
CTHD2003
DIGGERMA
FATFURY2
JOCKEYGP
KF2K2MP2
KF2K2MP
KF2K3BLA
KF2K3BL
KF2K3PCB
KF2K3PL
KF2K3UPL
KF2K5UNI
KOF10TH
KOF2003
KOF2003H
MS5PCB
MS5PLUS
MSLUG5
MSLUG5H
SONICWI2
SONICWI3
SVCBOOT
SVC
SVCPCBA
SVCPCB
SVCSPLUS
TOPHUNTRA
VLINER
VLINERO