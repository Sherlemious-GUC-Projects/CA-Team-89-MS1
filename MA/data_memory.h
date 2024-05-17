#ifndef DATA_MEMORY_H
#define DATA_MEMORY_H

void set_data( data_mem_t data_mem, int address, uint8_t value) {
	// check if the address is valid
	if (!(address >= 0 && address < DATA_MEMORY_SIZE)) {
		printf("Invalid address: %d\n", address);
		exit(1);
	}
	// check if the value is valid
	if (!(value >= 0 && value <= pow(2, DATA_WORD_SIZE))) {
		printf("Invalid value: %d\n", value);
		exit(1);
	}
	data_mem[address] = value;
}

uint8_t get_data( data_mem_t data_mem, int address) {
	// check if the address is valid
	if (!(address >= 0 && address < DATA_MEMORY_SIZE)) {
		printf("Invalid address: %d\n", address);
		exit(1);
	}
	return data_mem[address];
}

data_mem_t copy_data_mem( uint8_t *data_mem) {
	data_mem_t new_data_mem = (uint8_t *)calloc(DATA_MEMORY_SIZE, DATA_WORD_SIZE);
	int i;
	for (i = 0; i < DATA_MEMORY_SIZE; i++) {
		new_data_mem[i] = data_mem[i];
	}
	return new_data_mem;
}

void pretty_print_data_mem( data_mem_t data_mem) {
	// loop over all the mem if there is non-zero value print it
	// if the value is zero print all the consecutive zeros as one line
	printf("==============DATA MEMORY=============\n");
	int i;
	int start = 0;
	int end = 0;
	for (i = 0; i < DATA_MEMORY_SIZE; i++) {
		if (data_mem[i] != 0) {
			if (start != end) {
				printf("0x%04x-0x%04x: 0x00\n", start, end);
			}
			printf("0x%04x: 0x%02x\n", i, data_mem[i]);
			start = i + 1;
			end = i + 1;
		} else {
			end++;
		}
	}
	// print the remaining zeros
	if (start != end) {
		printf("0x%04x-0x%04x: 0x00\n", start, end);
	}
	printf("======================================\n");
}

void pretty_print_diff_data_mem( data_mem_t old_data_mem, uint8_t *new_data_mem) {
	printf("============DIFF DATA MEMORY==========\n");
	bool diff = false;
	int i;
	for (i = 0; i < DATA_MEMORY_SIZE; i++) {
		if (old_data_mem[i] != new_data_mem[i]) {
			diff = true;
			printf("0x%04x: 0x%02x -> 0x%02x\n", i, old_data_mem[i], new_data_mem[i]);
		}
	}
	if (!diff) {
		printf("No difference\n");
	}
	printf("======================================\n");
}

#endif

