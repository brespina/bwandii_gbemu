#include "cpu.h"
#include "bus.h"
#include "emu.h"

cpu_context ctx = {0};

void cpu_init() {
  ctx.regs.pc = 0x100;
  ctx.regs.a  = 0x01;
}

static void fetch_instruction() {
  ctx.curr_opcode = bus_read(ctx.regs.pc++);
  ctx.curr_inst = instruction_by_opcode(ctx.curr_opcode);
} 

void fetch_data();


static void execute() {
  IN_PROC proc = inst_get_processor(ctx.curr_inst->type);

  if (!proc) { NO_IMPL }

  proc(&ctx);
}


bool cpu_step() {
  u16 pc = ctx.regs.pc;

  if(!ctx.halted) {

    fetch_instruction();
    fetch_data();

    printf("\t%04X: %-7s (%02X %02X %02X) A: %02X B: %02X C: %02X\n", pc, inst_name(ctx.curr_inst->type), ctx.curr_opcode,
           bus_read(pc + 1), bus_read(pc + 2), ctx.regs.a, ctx.regs.b, ctx.regs.c);

    if (ctx.curr_inst->type == NULL) {
      printf("Unknown Instruction! %02X\n", ctx.curr_opcode);
      exit(-7);
    }

    execute();
  }

  return true;
}

