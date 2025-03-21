/*
    https://gbdev.io/pandocs/The_Cartridge_Header.html
    each cartridge contains a header located at $0100-$014F
    the cartridge header provides the following information about
        the game itself and the hardware it expects to run on:

    1) $0100-$0103 -- Entry Point
    2) $0104-$0133 -- Nintendo logo
    3) $0134-$0143 -- Title **
    4) $013F-$0143 -- Manufacturer code **
    5) $0143 -- CGB flag **
    6) $0144-$0145 -- New licensee code
    7) $0146 -- SGB flag
    8) $0147 -- Cartridge type
    9) $0148 -- ROM size
    10) $0149 -- ROM size
    11) $014A -- Destination code
    12) $014B -- Old licensee code
    13) $014C -- Mask ROM version number
    14) $014D -- Header checksum
    15) $014E-$014F -- Global checksum

    we are building a gameboy monochrome
    newer cartridges have shortened title byte size to fit man_code aand cgb
    flag but we wont be deaaling with thta so our title will just be $013F-$0143

*/

#pragma once

#include <common.h>

typedef struct {
    u8 entry[4];
    u8 logo[0x30];

    char title[16];
    u16 new_lic_code;
    u8 sgb_flag;
    u8 type;
    u8 rom_size;
    u8 ram_size;
    u8 dest_code;
    u8 lic_code;
    u8 version;
    u8 checksum;
    u16 global_checksum;
} rom_header;

bool cart_load(char *cart);
