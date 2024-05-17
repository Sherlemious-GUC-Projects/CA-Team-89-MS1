// global header files
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <stdint.h>
#include <math.h>

// local header files
#include "types.h"
#include "MA/data_memory.h"
#include "MA/instruction_memory.h"
#include "MA/register.h"

#include "ISA/parse_asm.h"
#include "ISA/fetch.h"
#include "ISA/decode.h"
#include "ISA/execute.h"

int main() {
	// init the path to the asm
	char *path = "asm/cumsum.asm";

	// init all the MA
	data_mem_t data_mem = init_data_mem();
	inst_mem_t inst_mem = init_inst_mem();
	reg_t *reg = init_reg();

	// read the assembly code into the memory
	read_asm(inst_mem, path);

	// init the clock cycle counter cc = 3 + (number of instructions - 1)
	uint8_t cc = 0;

	// init the states
	uint16_t *decode_input = (uint16_t *) calloc(1, sizeof(uint16_t));
	PCB_t *execute_input = init_pcb();

	// enter the MAIN WHILE loop
	while (!(reg->halt)) {
		// ~~~ make a copy of the registers and data memory ~~~ //
		reg_t *old_reg = copy_reg(reg);
		data_mem_t old_data_mem = copy_data_mem(data_mem);

		// ~~~ check validity ~~~ //
		bool fetch_valid = reg->i >= 0;
		bool decode_valid = reg->i >= 1;
		bool execute_valid = reg->i >= 1;

		// ~~~ execute ~~~ //
		if (execute_valid) {
			execute(execute_input, reg, data_mem);
		}

		// ~~~ decode ~~~ //
		if (decode_valid) {
			decode(*decode_input, execute_input, reg);
		}

		// ~~~ fetch ~~~ //
		if (fetch_valid) {
			fetch(reg, decode_input, inst_mem);
		}

		// ~~~ increment the clock cycle ~~~ //
		if (!reg->halt){
			cc++;
		}
		reg->i += 1;

		// ~~~ kill the old registers and data memory ~~~ //
		kill_reg(old_reg);
		kill_data_mem(old_data_mem);

		// ~~~ check halting condition ~~~ //
		reg->halt = reg->halt || (cc > 100);
	}

	// pretty print everything
	printf("######################################\n");
	printf("############ FINAL STATE #############\n");
	printf("######################################\n");
	pretty_print_data_mem(data_mem);
	pretty_print_reg(reg);
	printf("Clock Cycles: %d\n", cc);

	// kill all states
	free(decode_input);
	free(execute_input);

	// kill all the MA
	kill_data_mem(data_mem);
	kill_inst_mem(inst_mem);
	kill_reg(reg);
	return 0;
}

