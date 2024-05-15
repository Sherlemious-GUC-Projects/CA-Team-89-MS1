// global header files
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


// local header files
#include "MA/data_memory.h"
#include "MA/instruction_memory.h"
#include "MA/register.h"

#include "ISA/parse_asm.h"
#include "ISA/fetch.h"
#include "ISA/decode.h"
#include "ISA/execute.h"


int main() {
	// init the path to the asm
	char *path = "asm.asm";

	// init all the MA
	uint8_t *data_mem = init_data_mem();
	uint16_t *inst_mem = init_inst_mem();
	struct reg_t *reg = init_reg();

	// ~~~ BETA ~~~ //
	reg->GPRS[0] = 0xcc;
	reg->GPRS[1] = 0xcc;
	reg->PC = 0x00;

	// read the assembly code into the memory
	read_asm(inst_mem, path);

	// fetch the instructions
	uint16_t curr_inst = fetch(inst_mem, reg);

	// decode the instruction
	struct PCB_t pcb = decode(curr_inst, reg);
	pretty_print_pcb(pcb);

	// execute the instruction
	execute(pcb, reg, data_mem);
	
	pretty_print_reg(reg);
	pretty_print_data_mem(data_mem);

	// kill all the MA
	kill_data_mem(data_mem);
	kill_inst_mem(inst_mem);
	kill_reg(reg);
	return 0;
}

