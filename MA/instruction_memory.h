#ifndef INSTRUCTION_MEMORY_H
#define INSTRUCTION_MEMORY_H

void set_inst( inst_mem_t inst_mem, int address, uint16_t value) {
	// check if the address is valid
	if (!(address >= 0 && address < INSTRUCTION_MEMORY_SIZE)) {
		printf("Invalid address: %d\n", address);
		exit(1);
	}
	// check if the value is valid
	if (!(value >= 0 && value <= pow(2, INSTRUCTION_WORD_SIZE))) {
		printf("Invalid value: %d\n", value);
		exit(1);
	}
	inst_mem[address] = value;
}

uint16_t get_inst( inst_mem_t inst_mem, int address) {
	// check if the address is valid
	if (!(address >= 0 && address < INSTRUCTION_MEMORY_SIZE)) {
		printf("Invalid address: %d\n", address);
		exit(1);
	}
	return inst_mem[address];
}

void pretty_print_inst_mem( inst_mem_t inst_mem) {
	// loop over all the mem if there is non-zero value print it
	// if the value is zero print all the consecutive zeros as one line
	printf("==========INSTRUCTION MEMORY==========\n");
	int i;
	int start = 0;
	int end = 0;
	for (i = 0; i < INSTRUCTION_MEMORY_SIZE; i++) {
		if (inst_mem[i] != 0) {
			if (start != end) {
				printf("0x%04x-0x%04x: 0x0000\n", start, end);
			}
			printf("0x%04x: 0x%04x\n", i, inst_mem[i]);
			start = i + 1;
			end = i + 1;
		} else {
			end++;
		}
	}
	// print the remaining zeros
	if (start != end) {
		printf("0x%04x-0x%04x: 0x0000\n", start, end);
	}
	printf("======================================\n");
}

#endif
