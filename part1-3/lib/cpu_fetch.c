#include "cpu.h"
#include "emu.h"
#include "bus.h"

static cpu_context ctx;

void fetch_data() {
  ctx.mem_dest = 0;
  ctx.dest_is_mem = false;

  if (ctx.curr_inst == NULL) {
    return;
  }

  switch(ctx.curr_inst->mode) {
    case AM_IMP: return;
    case AM_R: 
      ctx.fetched_data = cpu_read_reg(ctx.curr_inst->reg_1);
      return;
    case AM_R_R:
      ctx.fetched_data = cpu_read_reg(ctx.curr_inst->reg_2);
      return;
     
    case AM_R_D8: 
      ctx.fetched_data = bus_read(ctx.regs.pc);
      emu_cycles(1);
      ctx.regs.pc++;
      return;

    case AM_R_D16:    
    case AM_D16: 
      u16 lo = bus_read(ctx.regs.pc);
      emu_cycles(1);

      u16 hi = bus_read(ctx.regs.pc + 1);
      emu_cycles(1);

      ctx.fetched_data = lo | (hi << 8);

      ctx.regs.pc += 2;

      return;
    
    case AM_MR_R:
      ctx.fetched_data = cpu_read_reg(ctx.curr_inst->reg_2);
      ctx.mem_dest = cpu_read_reg(ctx.curr_inst->reg_1);
      ctx.dest_is_mem = true;

      if (ctx.curr_inst->reg_1 == RT_C) {
        ctx.mem_dest |= 0xFF00;
      }

      return;

    case AM_R_MR:
      u16 addr = cpu_read_reg(ctx.curr_inst->reg_2);

      if (ctx.curr_inst->reg_1 == RT_C) {
        addr |= 0xFF00;
      }

      ctx.fetched_data = bus_read(addr);
      emu_cycles(1);
      return;

    case AM_R_HLI:
      ctx.fetched_data = bus_read(cpu_read_reg(ctx.curr_inst->reg_2));
      emu_cycles(1);
      cpu_set_reg(RT_HL, cpu_read_reg(RT_HL) + 1);
      return;

    case AM_R_HLD:
      ctx.fetched_data = bus_read(cpu_read_reg(ctx.curr_inst->reg_2));
      emu_cycles(1);
      cpu_set_reg(RT_HL, cpu_read_reg(RT_HL) - 1);
      return;

    case AM_HLI_R:
      ctx.fetched_data = cpu_read_reg(ctx.curr_inst->reg_2);
      ctx.mem_dest = cpu_read_reg(ctx.curr_inst->reg_1);
      ctx.dest_is_mem = true;
      cpu_set_reg(RT_HL, cpu_read_reg(RT_HL) + 1);
      return;

    case AM_HLD_R:
      ctx.fetched_data = cpu_read_reg(ctx.curr_inst->reg_2);
      ctx.mem_dest = cpu_read_reg(ctx.curr_inst->reg_1);
      ctx.dest_is_mem = true;
      cpu_set_reg(RT_HL, cpu_read_reg(RT_HL) - 1);
      return;


    case AM_R_A8:
      ctx.fetched_data = bus_read(ctx.regs.pc);
      emu_cycles(1);
      ctx.regs.pc++;
      return;

    case AM_A8_R:
      ctx.fetched_data = cpu_read_reg(ctx.regs.pc) | 0xFF00;
      ctx.dest_is_mem = true;
      emu_cycles(1);
      ctx.regs.pc++;
      return;

    case AM_HL_SPR:
      ctx.fetched_data = bus_read(ctx.regs.pc);
      emu_cycles(1);
      ctx.regs.pc++;
      return;

    case AM_D8:
      ctx.fetched_data = bus_read(ctx.regs.pc);
      emu_cycles(1);
      ctx.regs.pc++;
      return;


    case AM_D16_R:
      


    default: 
      printf("Unknown Addressing Mode! %d (%02X)\n", ctx.curr_inst->mode, ctx.curr_opcode);
      exit(-7);
      return;
  }
}



