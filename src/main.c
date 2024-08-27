/*
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

#include <stdio.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <sys/time.h>
#include <unistd.h>
#include <SDL.h>

#include "emu.h"
#include "conf.h"
#include "menu.h"
#include "roms.h"
#include "video.h"
#include "sound.h"
#include "event.h"
#include "screen.h"
#include "memory.h"
#include "gnutil.h"
#include "messages.h"
#include "transpack.h"
#include "frame_skip.h"
#include "ym2610_interf.h"

int main(int argc, char** argv)
{
  char *rom_name=NULL;
  char *ext_name=NULL;

  cf_init();
  cf_init_cmd_line();
  cf_open_file(NULL);
  rom_name = cf_parse_cmd_line(argc, argv);
  if(rom_name){
    ext_name = strrchr(rom_name, '.');
    printf("rom name: %s\n", rom_name);
    if(strcasecmp(ext_name, ".gno")){
      rom_name = remove_path_and_extension(rom_name, '.', '/');
    }
  }
  
  memory.intern_p1 = 0xff;
  memory.intern_p2 = 0xff;
  memory.intern_coin = 0x07;
  memory.intern_start = 0x8f;
  
  sdl_init();
  if(gn_init_skin() != GN_TRUE){
    printf("failed to load skin\n");
    exit(1);
  }
  reset_frame_skip();

  if(!rom_name){
    run_menu();
    printf("GAME %s\n", conf.game);
    if(conf.game == NULL) {
      return 0;
    }
  }
  else {
    uint32_t diff=0;
    struct timeval tv1={0}, tv2={0};
    const uint32_t TICKS_PER_SEC=1000000;

    gettimeofday(&tv1, 0);
    if(init_game(rom_name) != GN_TRUE) {
      printf("failed to init rom(%s)\n", rom_name);
      exit(1);
    }
    gettimeofday(&tv2, 0);
    diff = ((tv2.tv_sec - tv1.tv_sec) * TICKS_PER_SEC) + (tv2.tv_usec - tv1.tv_usec);
    printf("take about %ld us\n", diff);
  }
  //dr_save_gno(&memory.rom, "dump.gno");
  main_loop();
  close_game();
  return 0;
}

