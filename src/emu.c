/*
 *  Copyright (C) 2023 GXB
 *  Copyright (C) 2021 Steward Fu
 *  Copyright (C) 2001 Peponas Mathieu
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU Library General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
 */

#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <SDL.h>

#include "emu.h"
#include "conf.h"
#include "menu.h"
#include "event.h"
#include "timer.h"
#include "sound.h"
#include "memory.h"
#include "screen.h"
#include "pd4990a.h"
#include "neocrypt.h"
#include "messages.h"
#include "frame_skip.h"
#include "ym2610_interf.h"

int frame;
int nb_interlace = 256;
int current_line;

extern char skip_next_frame;
extern SDL_Rect visible_area;
extern SDL_Surface *buffer;

//static int arcade;
//
//extern int irq2enable, irq2start, irq2repeat, irq2control, irq2taken;
//extern int lastirq2line;
//extern int irq2repeat_limit;
//extern uint32_t irq2pos_value;

void setup_misc_patch(char *name)
{

	if(!strcmp(name, "ssideki")) {
		WRITE_WORD_ROM(&memory.rom.cpu_m68k.p[0x2240], 0x4e71);
	}
	
	if(!strcmp(name, "kof97aef")) {
		/* patch out protection check */
		WRITE_WORD_ROM(&memory.rom.cpu_m68k.p[0x9b12a], 0x6038);
	}
	
	// if(!strcmp(name, "kof97inv21")) {
		// /* patch out protection check */
		// WRITE_BYTE_ROM(&memory.rom.cpu_m68k.p[0x263ba], 0x65);
	// }
	
	//if (!strcmp(name, "fatfury3")) {
	//	WRITE_WORD_ROM(memory.rom.cpu_m68k.p, 0x0010);
	//}

	//if(!strcmp(name, "mslugx")) {
	if(strncmp(name, "mslugx", 6) == 0) { //给所有合金弹头X的子ROM及改版打补丁
    /* patch out protection checks */
    int i;
    uint8_t *RAM = memory.rom.cpu_m68k.p;
    for(i = 0; i < memory.rom.cpu_m68k.size; i += 2) {
      if((READ_WORD_ROM(&RAM[i + 0]) == 0x0243)
          && (READ_WORD_ROM(&RAM[i + 2]) == 0x0001) && /* andi.w  #$1, D3 */
          (READ_WORD_ROM(&RAM[i + 4]) == 0x6600)) { /* bne xxxx */
        WRITE_WORD_ROM(&RAM[i + 4], 0x4e71);
        WRITE_WORD_ROM(&RAM[i + 6], 0x4e71);
      }
    }
    WRITE_WORD_ROM(&RAM[0x3bdc], 0x4e71);
    WRITE_WORD_ROM(&RAM[0x3bde], 0x4e71);
    WRITE_WORD_ROM(&RAM[0x3be0], 0x4e71);
    WRITE_WORD_ROM(&RAM[0x3c0c], 0x4e71);
    WRITE_WORD_ROM(&RAM[0x3c0e], 0x4e71);
    WRITE_WORD_ROM(&RAM[0x3c10], 0x4e71);

    WRITE_WORD_ROM(&RAM[0x3c36], 0x4e71);
    WRITE_WORD_ROM(&RAM[0x3c38], 0x4e71);
	}

	// if(!strcmp(name, "kof97t")) {
    // /* patch out protection checks */
    // uint8_t *RAM = memory.rom.cpu_m68k.p;
	/* 以下地址相对于FBNEO的地址，-1得来。 */
    // WRITE_BYTE_ROM(&RAM[0x19f], 0xe7);
    // WRITE_BYTE_ROM(&RAM[0x1a0], 0x48);
    // WRITE_BYTE_ROM(&RAM[0x1a1], 0x00);
    // WRITE_BYTE_ROM(&RAM[0x1a2], 0x80);
    // WRITE_BYTE_ROM(&RAM[0x1a3], 0x39);
    // WRITE_BYTE_ROM(&RAM[0x1a4], 0x20);
    // WRITE_BYTE_ROM(&RAM[0x1a5], 0x10);
    // WRITE_BYTE_ROM(&RAM[0x1a6], 0x00);
    // WRITE_BYTE_ROM(&RAM[0x1a7], 0x00);
    // WRITE_BYTE_ROM(&RAM[0x1a8], 0x85);
    // WRITE_BYTE_ROM(&RAM[0x1a9], 0xbc);
    // WRITE_BYTE_ROM(&RAM[0x1aa], 0xb0);
    // WRITE_BYTE_ROM(&RAM[0x1ab], 0x00);
    // WRITE_BYTE_ROM(&RAM[0x1ac], 0x00);
    // WRITE_BYTE_ROM(&RAM[0x1ad], 0x18);
    // WRITE_BYTE_ROM(&RAM[0x1ae], 0xea);
    // WRITE_BYTE_ROM(&RAM[0x1af], 0x00);
    // WRITE_BYTE_ROM(&RAM[0x1b0], 0x67);
    // WRITE_BYTE_ROM(&RAM[0x1b1], 0x10);
    // WRITE_BYTE_ROM(&RAM[0x1b2], 0x00);
    // WRITE_BYTE_ROM(&RAM[0x1b3], 0xbc);
    // WRITE_BYTE_ROM(&RAM[0x1b4], 0xb0);
    // WRITE_BYTE_ROM(&RAM[0x1b5], 0x01);
    // WRITE_BYTE_ROM(&RAM[0x1b6], 0x00);
    // WRITE_BYTE_ROM(&RAM[0x1b7], 0xd0);
    // WRITE_BYTE_ROM(&RAM[0x1b8], 0xf9);
    // WRITE_BYTE_ROM(&RAM[0x1b9], 0x00);
    // WRITE_BYTE_ROM(&RAM[0x1ba], 0x67);
    // WRITE_BYTE_ROM(&RAM[0x1bb], 0x14);
    // WRITE_BYTE_ROM(&RAM[0x1bc], 0x00);
    // WRITE_BYTE_ROM(&RAM[0x1bd], 0x00);
    // WRITE_BYTE_ROM(&RAM[0x1be], 0x60);
    // WRITE_BYTE_ROM(&RAM[0x1bf], 0x20);
    // WRITE_BYTE_ROM(&RAM[0x1c0], 0x00);
    // WRITE_BYTE_ROM(&RAM[0x1c1], 0x3c);
    // WRITE_BYTE_ROM(&RAM[0x1c2], 0x30);
    // WRITE_BYTE_ROM(&RAM[0x1c3], 0x8a);
    // WRITE_BYTE_ROM(&RAM[0x1c4], 0x59);
    // WRITE_BYTE_ROM(&RAM[0x1c5], 0xc0);
    // WRITE_BYTE_ROM(&RAM[0x1c6], 0x33);
    // WRITE_BYTE_ROM(&RAM[0x1c7], 0x10);
    // WRITE_BYTE_ROM(&RAM[0x1c8], 0x00);
    // WRITE_BYTE_ROM(&RAM[0x1c9], 0x16);
    // WRITE_BYTE_ROM(&RAM[0x1ca], 0xa8);
    // WRITE_BYTE_ROM(&RAM[0x1cb], 0x00);
    // WRITE_BYTE_ROM(&RAM[0x1cc], 0x60);
    // WRITE_BYTE_ROM(&RAM[0x1cd], 0x12);
    // WRITE_BYTE_ROM(&RAM[0x1ce], 0x00);
    // WRITE_BYTE_ROM(&RAM[0x1cf], 0x3c);
    // WRITE_BYTE_ROM(&RAM[0x1d0], 0x30);
    // WRITE_BYTE_ROM(&RAM[0x1d1], 0x8a);
    // WRITE_BYTE_ROM(&RAM[0x1d2], 0x89);
    // WRITE_BYTE_ROM(&RAM[0x1d3], 0xc0);
    // WRITE_BYTE_ROM(&RAM[0x1d4], 0x33);
    // WRITE_BYTE_ROM(&RAM[0x1d5], 0x10);
    // WRITE_BYTE_ROM(&RAM[0x1d6], 0x00);
    // WRITE_BYTE_ROM(&RAM[0x1d7], 0x38);
    // WRITE_BYTE_ROM(&RAM[0x1d8], 0x82);
    // WRITE_BYTE_ROM(&RAM[0x1d9], 0xc0);
    // WRITE_BYTE_ROM(&RAM[0x1da], 0x33);
    // WRITE_BYTE_ROM(&RAM[0x1db], 0x10);
    // WRITE_BYTE_ROM(&RAM[0x1dc], 0x00);
    // WRITE_BYTE_ROM(&RAM[0x1dd], 0x38);
    // WRITE_BYTE_ROM(&RAM[0x1de], 0x84);
    // WRITE_BYTE_ROM(&RAM[0x1df], 0xdf);
    // WRITE_BYTE_ROM(&RAM[0x1e0], 0x4c);
    // WRITE_BYTE_ROM(&RAM[0x1e1], 0x01);
    // WRITE_BYTE_ROM(&RAM[0x1e2], 0x00);
    // WRITE_BYTE_ROM(&RAM[0x1e3], 0xc0);
    // WRITE_BYTE_ROM(&RAM[0x1e4], 0x13);
    // WRITE_BYTE_ROM(&RAM[0x1e5], 0x30);
    // WRITE_BYTE_ROM(&RAM[0x1e6], 0x00);
    // WRITE_BYTE_ROM(&RAM[0x1e7], 0x01);
    // WRITE_BYTE_ROM(&RAM[0x1e8], 0x00);
    // WRITE_BYTE_ROM(&RAM[0x1e9], 0x75);
    // WRITE_BYTE_ROM(&RAM[0x1ea], 0x4e);
    // WRITE_BYTE_ROM(&RAM[0x9ebd], 0xb9);
    // WRITE_BYTE_ROM(&RAM[0x9ebe], 0x4e);
    // WRITE_BYTE_ROM(&RAM[0x9ebf], 0x00);
    // WRITE_BYTE_ROM(&RAM[0x9ec1], 0xa0);
    // WRITE_BYTE_ROM(&RAM[0x9ec2], 0x01);
    // WRITE_BYTE_ROM(&RAM[0x22457], 0xed);
    // WRITE_BYTE_ROM(&RAM[0x22459], 0x3f);
    // WRITE_BYTE_ROM(&RAM[0x2245a], 0x28);
    // WRITE_BYTE_ROM(&RAM[0x22463], 0xed);
    // WRITE_BYTE_ROM(&RAM[0x22465], 0x50);
    // WRITE_BYTE_ROM(&RAM[0x22466], 0x28);
    // WRITE_BYTE_ROM(&RAM[0x2246b], 0x48);
    // WRITE_BYTE_ROM(&RAM[0x2246c], 0x50);
    // WRITE_BYTE_ROM(&RAM[0x2246d], 0x4a);
    // WRITE_BYTE_ROM(&RAM[0x2246e], 0x50);
    // WRITE_BYTE_ROM(&RAM[0x22479], 0xe0);
    // WRITE_BYTE_ROM(&RAM[0x2247a], 0x00);
    // WRITE_BYTE_ROM(&RAM[0x2247d], 0x04);
    // WRITE_BYTE_ROM(&RAM[0x22481], 0x05);
    // WRITE_BYTE_ROM(&RAM[0x22485], 0x06);
    // WRITE_BYTE_ROM(&RAM[0x224b5], 0xc6);
    // WRITE_BYTE_ROM(&RAM[0x224b7], 0x2d);
    // WRITE_BYTE_ROM(&RAM[0x224b8], 0x09);
    // WRITE_BYTE_ROM(&RAM[0x224b9], 0xf2);
    // WRITE_BYTE_ROM(&RAM[0x224ba], 0x27);
    // WRITE_BYTE_ROM(&RAM[0x224c7], 0x98);
    // WRITE_BYTE_ROM(&RAM[0x224c8], 0x6d);
    // WRITE_BYTE_ROM(&RAM[0x224d1], 0x99);
    // WRITE_BYTE_ROM(&RAM[0x224d2], 0x6d);
    // WRITE_BYTE_ROM(&RAM[0x224fb], 0xc2);
    // WRITE_BYTE_ROM(&RAM[0x224fd], 0xc2);
    // WRITE_BYTE_ROM(&RAM[0x22501], 0xc6);
    // WRITE_BYTE_ROM(&RAM[0x22503], 0xc6);
    // WRITE_BYTE_ROM(&RAM[0x22507], 0xe0);
    // WRITE_BYTE_ROM(&RAM[0x22509], 0xe0);
    // WRITE_BYTE_ROM(&RAM[0x2250d], 0xe4);
    // WRITE_BYTE_ROM(&RAM[0x2250f], 0xe4);
    // WRITE_BYTE_ROM(&RAM[0x22513], 0xe8);
    // WRITE_BYTE_ROM(&RAM[0x22515], 0xe8);
    // WRITE_BYTE_ROM(&RAM[0x22529], 0xb8);
    // WRITE_BYTE_ROM(&RAM[0x2252a], 0x4e);
    // WRITE_BYTE_ROM(&RAM[0x2252b], 0x26);
    // WRITE_BYTE_ROM(&RAM[0x2252c], 0x30);
    // WRITE_BYTE_ROM(&RAM[0x2252d], 0x40);
    // WRITE_BYTE_ROM(&RAM[0x2252e], 0x02);
    // WRITE_BYTE_ROM(&RAM[0x2252f], 0x1f);
    // WRITE_BYTE_ROM(&RAM[0x22530], 0x00);
    // WRITE_BYTE_ROM(&RAM[0x22531], 0x40);
    // WRITE_BYTE_ROM(&RAM[0x22532], 0x39);
    // WRITE_BYTE_ROM(&RAM[0x22533], 0xd6);
    // WRITE_BYTE_ROM(&RAM[0x22534], 0x00);
    // WRITE_BYTE_ROM(&RAM[0x22535], 0xbc);
    // WRITE_BYTE_ROM(&RAM[0x22536], 0x28);
    // WRITE_BYTE_ROM(&RAM[0x22537], 0x02);
    // WRITE_BYTE_ROM(&RAM[0x22538], 0x00);
    // WRITE_BYTE_ROM(&RAM[0x22539], 0x3c);
    // WRITE_BYTE_ROM(&RAM[0x2253a], 0x25);
    // WRITE_BYTE_ROM(&RAM[0x2253b], 0x00);
    // WRITE_BYTE_ROM(&RAM[0x2253c], 0x70);
    // WRITE_BYTE_ROM(&RAM[0x2253d], 0x6c);
    // WRITE_BYTE_ROM(&RAM[0x2253e], 0x26);
    // WRITE_BYTE_ROM(&RAM[0x2253f], 0x84);
    // WRITE_BYTE_ROM(&RAM[0x22540], 0x00);
    // WRITE_BYTE_ROM(&RAM[0x22541], 0x6c);
    // WRITE_BYTE_ROM(&RAM[0x22542], 0x20);
    // WRITE_BYTE_ROM(&RAM[0x22543], 0xc6);
    // WRITE_BYTE_ROM(&RAM[0x22544], 0x00);
    // WRITE_BYTE_ROM(&RAM[0x22545], 0x28);
    // WRITE_BYTE_ROM(&RAM[0x22546], 0x1c);
    // WRITE_BYTE_ROM(&RAM[0x22547], 0x03);
    // WRITE_BYTE_ROM(&RAM[0x22548], 0x00);
    // WRITE_BYTE_ROM(&RAM[0x22549], 0x2c);
    // WRITE_BYTE_ROM(&RAM[0x2254a], 0x4a);
    // WRITE_BYTE_ROM(&RAM[0x2254b], 0xf8);
    // WRITE_BYTE_ROM(&RAM[0x2254c], 0x00);
    // WRITE_BYTE_ROM(&RAM[0x2254d], 0x74);
    // WRITE_BYTE_ROM(&RAM[0x2254e], 0x66);
    // WRITE_BYTE_ROM(&RAM[0x22551], 0xfa);
    // WRITE_BYTE_ROM(&RAM[0x22553], 0x14);
    // WRITE_BYTE_ROM(&RAM[0x22555], 0x40);
    // WRITE_BYTE_ROM(&RAM[0x22556], 0x11);
    // WRITE_BYTE_ROM(&RAM[0x22557], 0x0a);
    // WRITE_BYTE_ROM(&RAM[0x22558], 0x00);
    // WRITE_BYTE_ROM(&RAM[0x22559], 0x00);
    // WRITE_BYTE_ROM(&RAM[0x2255a], 0x52);
    // WRITE_BYTE_ROM(&RAM[0x2255b], 0x40);
    // WRITE_BYTE_ROM(&RAM[0x2255c], 0x11);
    // WRITE_BYTE_ROM(&RAM[0x2255d], 0x0b);
    // WRITE_BYTE_ROM(&RAM[0x2255e], 0x00);
    // WRITE_BYTE_ROM(&RAM[0x2255f], 0x00);
    // WRITE_BYTE_ROM(&RAM[0x22560], 0x52);
    // WRITE_BYTE_ROM(&RAM[0x22561], 0x40);
    // WRITE_BYTE_ROM(&RAM[0x22562], 0x11);
    // WRITE_BYTE_ROM(&RAM[0x22563], 0x0c);
    // WRITE_BYTE_ROM(&RAM[0x22564], 0x00);
    // WRITE_BYTE_ROM(&RAM[0x22565], 0x2c);
    // WRITE_BYTE_ROM(&RAM[0x22566], 0x52);
    // WRITE_BYTE_ROM(&RAM[0x22567], 0xfa);
    // WRITE_BYTE_ROM(&RAM[0x22568], 0x00);
    // WRITE_BYTE_ROM(&RAM[0x22569], 0x28);
    // WRITE_BYTE_ROM(&RAM[0x2256a], 0x10);
    // WRITE_BYTE_ROM(&RAM[0x2256b], 0x01);
    // WRITE_BYTE_ROM(&RAM[0x2256c], 0x00);
    // WRITE_BYTE_ROM(&RAM[0x2256d], 0x00);
    // WRITE_BYTE_ROM(&RAM[0x2256e], 0x0c);
    // WRITE_BYTE_ROM(&RAM[0x2256f], 0x0b);
    // WRITE_BYTE_ROM(&RAM[0x22570], 0x00);
    // WRITE_BYTE_ROM(&RAM[0x22571], 0x20);
    // WRITE_BYTE_ROM(&RAM[0x22572], 0x67);
    // WRITE_BYTE_ROM(&RAM[0x22573], 0x00);
    // WRITE_BYTE_ROM(&RAM[0x22574], 0x0c);
    // WRITE_BYTE_ROM(&RAM[0x22575], 0x0c);
    // WRITE_BYTE_ROM(&RAM[0x22576], 0x00);
    // WRITE_BYTE_ROM(&RAM[0x22577], 0x1a);
    // WRITE_BYTE_ROM(&RAM[0x22578], 0x67);
    // WRITE_BYTE_ROM(&RAM[0x22579], 0x00);
    // WRITE_BYTE_ROM(&RAM[0x2257a], 0x0c);
    // WRITE_BYTE_ROM(&RAM[0x2257b], 0x0e);
    // WRITE_BYTE_ROM(&RAM[0x2257c], 0x00);
    // WRITE_BYTE_ROM(&RAM[0x2257d], 0x14);
    // WRITE_BYTE_ROM(&RAM[0x2257e], 0x67);
    // WRITE_BYTE_ROM(&RAM[0x2257f], 0x00);
    // WRITE_BYTE_ROM(&RAM[0x22580], 0x0c);
    // WRITE_BYTE_ROM(&RAM[0x22581], 0x0d);
    // WRITE_BYTE_ROM(&RAM[0x22582], 0x00);
    // WRITE_BYTE_ROM(&RAM[0x22583], 0x14);
    // WRITE_BYTE_ROM(&RAM[0x22584], 0x66);
    // WRITE_BYTE_ROM(&RAM[0x22585], 0x28);
    // WRITE_BYTE_ROM(&RAM[0x22586], 0x10);
    // WRITE_BYTE_ROM(&RAM[0x22587], 0x0a);
    // WRITE_BYTE_ROM(&RAM[0x22588], 0x00);
    // WRITE_BYTE_ROM(&RAM[0x22589], 0x68);
    // WRITE_BYTE_ROM(&RAM[0x2258a], 0x11);
    // WRITE_BYTE_ROM(&RAM[0x2258b], 0x0c);
    // WRITE_BYTE_ROM(&RAM[0x2258d], 0x0a);
    // WRITE_BYTE_ROM(&RAM[0x2258e], 0x00);
    // WRITE_BYTE_ROM(&RAM[0x2258f], 0x40);
    // WRITE_BYTE_ROM(&RAM[0x22590], 0x11);
    // WRITE_BYTE_ROM(&RAM[0x22591], 0x0c);
    // WRITE_BYTE_ROM(&RAM[0x22592], 0x00);
    // WRITE_BYTE_ROM(&RAM[0x22593], 0x03);
    // WRITE_BYTE_ROM(&RAM[0x22594], 0x7c);
    // WRITE_BYTE_ROM(&RAM[0x22595], 0x6c);
    // WRITE_BYTE_ROM(&RAM[0x22596], 0x53);
    // WRITE_BYTE_ROM(&RAM[0x22597], 0xd6);
    // WRITE_BYTE_ROM(&RAM[0x22598], 0x00);
    // WRITE_BYTE_ROM(&RAM[0x22599], 0x6c);
    // WRITE_BYTE_ROM(&RAM[0x2259a], 0x20);
    // WRITE_BYTE_ROM(&RAM[0x2259b], 0xc2);
    // WRITE_BYTE_ROM(&RAM[0x2259c], 0x00);
    // WRITE_BYTE_ROM(&RAM[0x2259d], 0x00);
    // WRITE_BYTE_ROM(&RAM[0x2259e], 0x70);
    // WRITE_BYTE_ROM(&RAM[0x2259f], 0x80);
    // WRITE_BYTE_ROM(&RAM[0x225a0], 0x10);
    // WRITE_BYTE_ROM(&RAM[0x225bb], 0x70);
    // WRITE_BYTE_ROM(&RAM[0x225bf], 0xc2);
    // WRITE_BYTE_ROM(&RAM[0x225c9], 0x06);
    // WRITE_BYTE_ROM(&RAM[0x225cb], 0x02);
    // WRITE_BYTE_ROM(&RAM[0x225d1], 0x70);
    // WRITE_BYTE_ROM(&RAM[0x225d5], 0xc2);
    // WRITE_BYTE_ROM(&RAM[0x225db], 0xc2);
    // WRITE_BYTE_ROM(&RAM[0x225e3], 0x40);
    // WRITE_BYTE_ROM(&RAM[0x225e4], 0x02);
    // WRITE_BYTE_ROM(&RAM[0x225e5], 0x70);
    // WRITE_BYTE_ROM(&RAM[0x225e6], 0x00);
    // WRITE_BYTE_ROM(&RAM[0x225e8], 0x4a);
    // WRITE_BYTE_ROM(&RAM[0x225e9], 0x00);
    // WRITE_BYTE_ROM(&RAM[0x225ea], 0x67);
    // WRITE_BYTE_ROM(&RAM[0x225eb], 0x4a);
    // WRITE_BYTE_ROM(&RAM[0x225ec], 0x00);
    // WRITE_BYTE_ROM(&RAM[0x225ed], 0x04);
    // WRITE_BYTE_ROM(&RAM[0x225ef], 0x02);
    // WRITE_BYTE_ROM(&RAM[0x225f9], 0xf4);
    // WRITE_BYTE_ROM(&RAM[0x225fd], 0x0a);
    // WRITE_BYTE_ROM(&RAM[0x2261f], 0xf4);
    // WRITE_BYTE_ROM(&RAM[0x22625], 0xe7);
    // WRITE_BYTE_ROM(&RAM[0x22626], 0x48);
    // WRITE_BYTE_ROM(&RAM[0x22627], 0x80);
    // WRITE_BYTE_ROM(&RAM[0x22628], 0xc0);
    // WRITE_BYTE_ROM(&RAM[0x22629], 0x3c);
    // WRITE_BYTE_ROM(&RAM[0x2262a], 0x30);
    // WRITE_BYTE_ROM(&RAM[0x2262b], 0x1d);
    // WRITE_BYTE_ROM(&RAM[0x2262c], 0x00);
    // WRITE_BYTE_ROM(&RAM[0x2262d], 0xb8);
    // WRITE_BYTE_ROM(&RAM[0x2262e], 0x4e);
    // WRITE_BYTE_ROM(&RAM[0x22630], 0x77);
    // WRITE_BYTE_ROM(&RAM[0x22631], 0xdf);
    // WRITE_BYTE_ROM(&RAM[0x22632], 0x4c);
    // WRITE_BYTE_ROM(&RAM[0x22633], 0x03);
    // WRITE_BYTE_ROM(&RAM[0x22634], 0x01);
    // WRITE_BYTE_ROM(&RAM[0x22635], 0x46);
    // WRITE_BYTE_ROM(&RAM[0x22636], 0x11);
    // WRITE_BYTE_ROM(&RAM[0x22637], 0x03);
    // WRITE_BYTE_ROM(&RAM[0x22639], 0x06);
    // WRITE_BYTE_ROM(&RAM[0x2263a], 0x0c);
    // WRITE_BYTE_ROM(&RAM[0x2263b], 0x03);
    // WRITE_BYTE_ROM(&RAM[0x2263c], 0x00);
    // WRITE_BYTE_ROM(&RAM[0x22647], 0x4a);
    // WRITE_BYTE_ROM(&RAM[0x22663], 0xfa);
    // WRITE_BYTE_ROM(&RAM[0x22664], 0x45);
    // WRITE_BYTE_ROM(&RAM[0x22665], 0xe6);
    // WRITE_BYTE_ROM(&RAM[0x22666], 0xff);
    // WRITE_BYTE_ROM(&RAM[0x2266d], 0xfa);
    // WRITE_BYTE_ROM(&RAM[0x2266e], 0x45);
    // WRITE_BYTE_ROM(&RAM[0x2266f], 0xe8);
    // WRITE_BYTE_ROM(&RAM[0x22670], 0xff);
    // WRITE_BYTE_ROM(&RAM[0x22671], 0x4a);
    // WRITE_BYTE_ROM(&RAM[0x22677], 0xc6);
    // WRITE_BYTE_ROM(&RAM[0x226fd], 0x0a);
    // WRITE_BYTE_ROM(&RAM[0x226ff], 0x98);
    // WRITE_BYTE_ROM(&RAM[0x22700], 0xc6);
    // WRITE_BYTE_ROM(&RAM[0x227b5], 0x83);
    // WRITE_BYTE_ROM(&RAM[0x227b6], 0xec);
    // WRITE_BYTE_ROM(&RAM[0x227b7], 0x24);
    // WRITE_BYTE_ROM(&RAM[0x227b8], 0x83);
    // WRITE_BYTE_ROM(&RAM[0x227b9], 0x6c);
    // WRITE_BYTE_ROM(&RAM[0x227ba], 0x24);
    // WRITE_BYTE_ROM(&RAM[0x227bb], 0x38);
    // WRITE_BYTE_ROM(&RAM[0x227bc], 0x01);
    // WRITE_BYTE_ROM(&RAM[0x227bd], 0x8b);
    // WRITE_BYTE_ROM(&RAM[0x227be], 0x4c);
    // WRITE_BYTE_ROM(&RAM[0x227bf], 0x24);
    // WRITE_BYTE_ROM(&RAM[0x227c0], 0x28);
    // WRITE_BYTE_ROM(&RAM[0x227c1], 0x8b);
    // WRITE_BYTE_ROM(&RAM[0x227c2], 0x81);
    // WRITE_BYTE_ROM(&RAM[0x227c3], 0xa4);
    // WRITE_BYTE_ROM(&RAM[0x227c4], 0x01);
    // WRITE_BYTE_ROM(&RAM[0x227c5], 0x00);
    // WRITE_BYTE_ROM(&RAM[0x227c6], 0x00);
    // WRITE_BYTE_ROM(&RAM[0x227c7], 0x8b);
    // WRITE_BYTE_ROM(&RAM[0x227c8], 0x91);
    // WRITE_BYTE_ROM(&RAM[0x227c9], 0x20);
    // WRITE_BYTE_ROM(&RAM[0x227ca], 0x01);
    // WRITE_BYTE_ROM(&RAM[0x227cb], 0x00);
    // WRITE_BYTE_ROM(&RAM[0x227cc], 0x00);
    // WRITE_BYTE_ROM(&RAM[0x227cd], 0x55);
    // WRITE_BYTE_ROM(&RAM[0x227ce], 0x8b);
    // WRITE_BYTE_ROM(&RAM[0x227cf], 0x69);
    // WRITE_BYTE_ROM(&RAM[0x227d0], 0x5c);
    // WRITE_BYTE_ROM(&RAM[0x227d1], 0x8b);
    // WRITE_BYTE_ROM(&RAM[0x227d2], 0x48);
    // WRITE_BYTE_ROM(&RAM[0x227d3], 0x08);
    // WRITE_BYTE_ROM(&RAM[0x227d4], 0x89);
    // WRITE_BYTE_ROM(&RAM[0x227d5], 0x4c);
    // WRITE_BYTE_ROM(&RAM[0x227d6], 0x24);
    // WRITE_BYTE_ROM(&RAM[0x227d7], 0x0c);
    // WRITE_BYTE_ROM(&RAM[0x227d8], 0x8b);
    // WRITE_BYTE_ROM(&RAM[0x227d9], 0x48);
    // WRITE_BYTE_ROM(&RAM[0x227da], 0x0c);
    // WRITE_BYTE_ROM(&RAM[0x227db], 0x89);
    // WRITE_BYTE_ROM(&RAM[0x227dc], 0x4c);
    // WRITE_BYTE_ROM(&RAM[0x227dd], 0x24);
    // WRITE_BYTE_ROM(&RAM[0x227de], 0x1c);
    // WRITE_BYTE_ROM(&RAM[0x227df], 0x8b);
    // WRITE_BYTE_ROM(&RAM[0x227e0], 0x48);
    // WRITE_BYTE_ROM(&RAM[0x227e1], 0x10);
    // WRITE_BYTE_ROM(&RAM[0x227e2], 0x8b);
    // WRITE_BYTE_ROM(&RAM[0x227e3], 0x40);
    // WRITE_BYTE_ROM(&RAM[0x227e4], 0x14);
    // WRITE_BYTE_ROM(&RAM[0x227e5], 0x89);
    // WRITE_BYTE_ROM(&RAM[0x227e6], 0x6c);
    // WRITE_BYTE_ROM(&RAM[0x227e7], 0x24);
    // WRITE_BYTE_ROM(&RAM[0x227e8], 0x20);
    // WRITE_BYTE_ROM(&RAM[0x227e9], 0x89);
    // WRITE_BYTE_ROM(&RAM[0x227ea], 0x54);
    // WRITE_BYTE_ROM(&RAM[0x227eb], 0x24);
    // WRITE_BYTE_ROM(&RAM[0x227ec], 0x18);
    // WRITE_BYTE_ROM(&RAM[0x227ed], 0x89);
    // WRITE_BYTE_ROM(&RAM[0x227ee], 0x4c);
    // WRITE_BYTE_ROM(&RAM[0x227ef], 0x24);
    // WRITE_BYTE_ROM(&RAM[0x227f0], 0x14);
    // WRITE_BYTE_ROM(&RAM[0x227f1], 0x89);
    // WRITE_BYTE_ROM(&RAM[0x227f2], 0x44);
    // WRITE_BYTE_ROM(&RAM[0x227f3], 0x24);
    // WRITE_BYTE_ROM(&RAM[0x227f4], 0x10);
    // WRITE_BYTE_ROM(&RAM[0x227f5], 0xcc);
    // WRITE_BYTE_ROM(&RAM[0x227f6], 0xcc);
    // WRITE_BYTE_ROM(&RAM[0x227f7], 0xcc);
    // WRITE_BYTE_ROM(&RAM[0x227f8], 0xcc);
    // WRITE_BYTE_ROM(&RAM[0x227f9], 0xcc);
    // WRITE_BYTE_ROM(&RAM[0x227fa], 0xcc);
    // WRITE_BYTE_ROM(&RAM[0x227fb], 0xcc);
    // WRITE_BYTE_ROM(&RAM[0x227fc], 0xcc);
    // WRITE_BYTE_ROM(&RAM[0x227fd], 0xac);
    // WRITE_BYTE_ROM(&RAM[0x227fe], 0x11);
  // }
}

void neogeo_reset(void)
{
  //	memory.vid.modulo = 1; /* TODO: Move to init_video */
  sram_lock = 0;
  sound_code = 0;
  pending_command = 0;
  result_code = 0;
#ifdef ENABLE_940T
  shared_ctl->sound_code = sound_code;
  shared_ctl->pending_command = pending_command;
  shared_ctl->result_code = result_code;
#endif
  if(memory.rom.cpu_m68k.size > 0x100000) {
    cpu_68k_bankswitch(0x100000);
  }
  else {
    cpu_68k_bankswitch(0);
  }
  cpu_68k_reset();

}

void init_sound(void)
{

  if(conf.sound) {
    init_sdl_audio();
  }

  cpu_z80_init();
  //streams_sh_start();
  YM2610_sh_start();
  if(conf.sound)	{
    pause_audio(0);
  }

}

void init_neo(void)
{
  neogeo_init_save_state();
  cpu_68k_init();
  pd4990a_init();
  init_sound();
  neogeo_reset();
}

static void take_screenshot(void)
{
  time_t ltime;
  struct tm *today;
  char buf[256];
  char date_str[101];
  //static SDL_Rect buf_rect    =	{16, 16, 304, 224};
  static SDL_Rect screen_rect = { 0, 0, 304, 224 };
  static SDL_Surface *shoot;

  screen_rect.w = visible_area.w;
  screen_rect.h = visible_area.h;

  if(shoot == NULL)
    shoot = SDL_CreateRGBSurface(SDL_SWSURFACE, visible_area.w,
                                 visible_area.h, 16, 0xF800, 0x7E0, 0x1F, 0);

  time(&ltime);
  today = localtime(&ltime);
  strftime(date_str, 100, "%a_%b_%d_%T_%Y", today);
  snprintf(buf, 255, "%s/%s_%s.bmp", getenv("HOME"), conf.game, date_str);
  printf("save to %s\n", buf);

  SDL_BlitSurface(buffer, &visible_area, shoot, &screen_rect);
  SDL_SaveBMP(shoot, buf);
}

static int fc;
static int last_line;
static int skip_this_frame = 0;

static inline int neo_interrupt(void)
{
  static int frames;

  pd4990a_addretrace();
  if(!(memory.vid.irq2control & 0x8)) {
    if(fc >= neogeo_frame_counter_speed) {
      fc = 0;
      neogeo_frame_counter++;
    }
    fc++;
  }

  skip_this_frame = skip_next_frame;
  skip_next_frame = frame_skip();

  if(!skip_this_frame) {
    draw_screen();
  }
  return 1;
}

static inline void update_screen(void)
{
  if(memory.vid.irq2control & 0x40) {
    memory.vid.irq2start = (memory.vid.irq2pos + 3) / 0x180;  /* ridhero gives 0x17d */
  }
  else {
    memory.vid.irq2start = 1000;
  }

  if(!skip_this_frame) {
    if(last_line < 21) {
      draw_screen();
    }
    else {
      draw_screen_scanline(last_line - 21, 262, 1);
    }
  }

  last_line = 0;

  pd4990a_addretrace();
  if(fc >= neogeo_frame_counter_speed) {
    fc = 0;
    neogeo_frame_counter++;
  }
  fc++;

  skip_this_frame = skip_next_frame;
  skip_next_frame = frame_skip();
}

static inline int update_scanline(void)
{
  memory.vid.irq2taken = 0;

  if(memory.vid.irq2control & 0x10) {

    if(current_line == memory.vid.irq2start) {
      if(memory.vid.irq2control & 0x80) {
        memory.vid.irq2start += (memory.vid.irq2pos + 3) / 0x180;
      }
      memory.vid.irq2taken = 1;
    }
  }

  if(memory.vid.irq2taken) {
    if(!skip_this_frame) {
      if(last_line < 21) {
        last_line = 21;
      }
      if(current_line < 20) {
        current_line = 20;
      }
      draw_screen_scanline(last_line - 21, current_line - 20, 0);
    }
    last_line = current_line;
  }
  current_line++;
  return memory.vid.irq2taken;
}

static int slow_motion = 0;

void main_loop(void)
{
  int neo_emu_done = 0;
  int m68k_overclk = 0;
  int z80_overclk = 0;
  int a, i;
  static SDL_Rect buf_rect = { 24, 16, 304, 224 };
  static SDL_Rect screen_rect = { 0, 0, 304, 224 };
  uint32_t cpu_68k_timeslice = (m68k_overclk == 0 ? 200000 : 200000 + (m68k_overclk * 200000 / 100.0));
  uint32_t cpu_68k_timeslice_scanline = cpu_68k_timeslice / 264.0;
  uint32_t cpu_z80_timeslice = (z80_overclk == 0 ? 73333 : 73333 + (z80_overclk * 73333 / 100.0));
  uint32_t tm_cycle = 0;
  uint32_t cpu_z80_timeslice_interlace = cpu_z80_timeslice / (float) nb_interlace;

  reset_frame_skip();
  my_timer();

  while(!neo_emu_done) {
    if(handle_event()) {
      SDL_BlitSurface(buffer, &buf_rect, state_img, &screen_rect);
      if(conf.sound) {
        pause_audio(1);
      }

      if(run_menu() == 2) {
        neo_emu_done = 1;
        return;
      }

      if(conf.sound) {
        pause_audio(0);
      }
      reset_frame_skip();
    }

    if(conf.sound) {
      for(i = 0; i < nb_interlace; i++) {
        cpu_z80_run(cpu_z80_timeslice_interlace);
        my_timer();
      }
    }

    tm_cycle = cpu_68k_run(cpu_68k_timeslice - tm_cycle);
    a = neo_interrupt();
    memory.watchdog++;
    if(memory.watchdog > 7) {  /* Watchdog reset after ~0.13 == ~7.8 frames */
      printf("WATCHDOG RESET %d\n", memory.watchdog);
      cpu_68k_reset();
    }

    if(a) {
      cpu_68k_interrupt(a);
    }
  }
  pause_audio(1);
}

void cpu_68k_dpg_step(void)
{
  static uint32_t nb_cycle;
  static uint32_t line_cycle;
  uint32_t cpu_68k_timeslice = 200000;
  uint32_t cpu_68k_timeslice_scanline = 200000 / (float) 262;
  uint32_t cycle;
  if(nb_cycle == 0) {
    main_loop(); /* update event etc. */
  }
  cycle = cpu_68k_run_step();
  //add_bt(cpu_68k_getpc());
  line_cycle += cycle;
  nb_cycle += cycle;
  if(nb_cycle >= cpu_68k_timeslice) {
    nb_cycle = line_cycle = 0;
    neo_interrupt();
    cpu_68k_interrupt(1);
  }
  else {
    if(line_cycle >= cpu_68k_timeslice_scanline) {
      line_cycle = 0;
    }
  }
}

