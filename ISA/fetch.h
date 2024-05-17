#ifndef FETCH_H
#define FETCH_H

// global header files
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


uint16_t fetch(inst_mem_t inst_mem, reg_t *reg) {
	// get the instruction at the PC
	uint16_t curr_inst = get_inst(inst_mem, reg->PC);
	
	// increment the PC
	reg->PC++;
	return curr_inst;
}


#endif
