#ifndef FETCH_H
#define FETCH_H

void fetch(reg_t *reg, uint16_t *curr_inst, inst_mem_t inst_mem) {
	// get the instruction at the PC
	*curr_inst = get_inst(inst_mem, reg->PC);
	
	// increment the PC
	reg->PC++;
}

#endif
