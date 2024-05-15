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

	// read the assembly code into the memory
	read_asm(inst_mem, path);

	// init the clock cycle counter cc = 2 + (number of instructions - 1)
	uint8_t cc = 0;

	// init the PCBsss
	uint16_t fetch_inst;
	struct PCB_t decode_pcb;
	struct PCB_t execute_pcb;

	// init haulting condition
	bool hault = false;

	// enter the MAIN WHILE loop
	while (!hault) {
		// init the countersss
		uint8_t fetch_counter = reg->PC; // USELESS
		uint8_t decode_counter = reg->PC - 1;
		uint8_t execute_counter = reg->PC - 2;

		// check if the counterss are valid
		bool decode_valid = (decode_counter & 0b10000000) == 0;
		bool execute_valid = (execute_counter & 0b10000000) == 0;

		// ~~~ execute the instruction ~~~ //
		if (execute_valid) {
			execute(execute_pcb, reg, data_mem);
		}

		// ~~~ decode the instruction ~~~ //
		if (decode_valid) {
			decode_pcb = decode(fetch_inst, reg);
			execute_pcb = decode_pcb;
		}

		// ~~~ fetch the instruction ~~~ //
		fetch_inst = fetch(inst_mem, reg);

		// check if the fetch PCB is haulting
		printf("fetch_inst at %d: %x\n", reg->PC - 1, fetch_inst);
		hault = (fetch_inst == 0xf000);

		// increment the clock cycle counter
		cc++;
		
		hault = (cc > 20);
	}


	// print EVERYTHING
	pretty_print_inst_mem(inst_mem);

	// kill all the MA
	kill_data_mem(data_mem);
	kill_inst_mem(inst_mem);
	kill_reg(reg);
	return 0;
}

