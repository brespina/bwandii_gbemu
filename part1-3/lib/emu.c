#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

#include "cart.h"
#include "emu.h"
#include "cpu.h"

/*
    Emu components:

    |Cart|
    |CPU|           -> need to emulate all instructions and registers
    |Address Bus|   -> central place for reading and writing to addresses/memory map (goes to cartridge)
    |PPU|           -> Picture Processing Unit
    |Timer|         -> 

*/

static emu_context ctx;

emu_context *emu_get_context() {
    return &ctx;
}

void delay(u32 ms) {
    SDL_Delay(ms);
}

int emu_run(int argc, char **argv) {
    if (argc < 2) {
        printf("Usage: emu <rom_file>\n");
        return -1;
    }

    if (!cart_load(argv[1])) {
        printf("Failed to load ROM file: %s\n", argv[1]);
        return -2;
    }

    printf("Cart Loaded...\n");

    SDL_Init(SDL_INIT_VIDEO);
    printf("SDL INIT\n");
    TTF_Init();
    printf("TTF INIT\n");

    cpu_init();

    ctx.running = true;
    ctx.paused = false;
    ctx.ticks = 0;

    // running game loop
    while(ctx.running) {
        // if paused, delays then reiterates while loop so rest of loop isn't being ran while pause
        if(ctx.paused) {
            delay(10);
            continue;
        }

        if(!cpu_step()) {
            printf("CPU Stopped...\n");
            return -3;
        }

        ctx.ticks++;

    }

    return 0;
}


void emu_cycles(int cpu_cycles) {
  //TODO:
}
