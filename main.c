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
	char *path = "asm/addition.asm";

	// init all the MA
	uint8_t *data_mem = init_data_mem();
	uint16_t *inst_mem = init_inst_mem();
	struct reg_t *reg = init_reg();

	// read the assembly code into the memory
	read_asm(inst_mem, path);

	// init the clock cycle counter cc = 3 + (number of instructions - 1)
	uint8_t cc = 0;

	// init the PCBsss
	uint16_t fetch_inst;
	struct PCB_t decode_pcb;
	struct PCB_t execute_pcb;

	// init halting condition
	bool halt = false;
	int halt_count_down = 2;

	// enter the MAIN WHILE loop
	while (halt_count_down > 0) {
		// init the countersss
		uint8_t fetch_counter = reg->PC; // USELESS
		uint8_t decode_counter = reg->PC - 1;
		uint8_t execute_counter = reg->PC - 2;

		// check if the counterss are valid
		bool fetch_valid = halt_count_down >= 2;
		bool decode_valid = ((decode_counter & 0b10000000) == 0) && (halt_count_down >= 1);
		bool execute_valid = (execute_counter & 0b10000000) == 0 && (halt_count_down >= 0);

		// ~~~ make a copy of the regs and the data mem ~~~ //
		struct reg_t *old_reg = copy_reg(reg);
		uint8_t *old_data_mem = copy_data_mem(data_mem);

		// ~~~ execute the instruction ~~~ //
		if (execute_valid) {
			printf("\n&&&&&&&&&&& BEFORE EXECUTE &&&&&&&&&&&\n");
			printf("PC for the execute: %d\n", execute_counter);
			pretty_print_pcb(execute_pcb);
			execute(execute_pcb, reg, data_mem);
			printf("\n&&&&&&&&&&& AFTER EXECUTE &&&&&&&&&&&\n");
			pretty_print_diff_reg(old_reg, reg);
			pretty_print_diff_data_mem(old_data_mem, data_mem);
		}

		// ~~~ decode the instruction ~~~ //
		if (decode_valid) {
			decode_pcb = decode(fetch_inst, reg);
			execute_pcb = decode_pcb;
		}

		// ~~~ fetch the instruction ~~~ //
		if (fetch_valid) {
			fetch_inst = fetch(inst_mem, reg);
		}

		// check if the fetch PCB is halting
		if (halt) {
			halt_count_down--;
		}else {
			halt = (fetch_inst == 0xf000) || (cc == 0xff);
			if (halt) {
				halt_count_down--;
			}
		}

		// increment the clock cycle counter
		cc += 1;

		// free the old reg and data mem
		kill_reg(old_reg);
		kill_data_mem(old_data_mem);
	}

	// pretty print everything
	printf("######################################\n");
	printf("############ FINAL STATE #############\n");
	printf("######################################\n");
	pretty_print_data_mem(data_mem);
	pretty_print_reg(reg);
	printf("Clock Cycles: %d\n", cc);

	// kill all the MA
	kill_data_mem(data_mem);
	kill_inst_mem(inst_mem);
	kill_reg(reg);
	return 0;
}

