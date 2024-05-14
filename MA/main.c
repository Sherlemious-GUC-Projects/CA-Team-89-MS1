// global imports
#include <stdio.h>

// local imports
#include "instruction_memory.h"
#include "data_memory.h"
#include "register.h"

int gen_rnd_num(int min_address, int max_address) {
	int range = max_address - min_address + 1;
	int rnd_num = rand() % range + min_address;
	return rnd_num;
}

void test_instruction_memory(int n) {
	uint16_t *inst_mem = init_inst_mem();
	int address;
	uint16_t instruction;

	for (int i = 0; i < n; i++) {
		address = gen_rnd_num(0, INSTRUCTION_MEMORY_SIZE - 1);
		instruction = gen_rnd_num(0, pow(2, INSTRUCTION_WORD_SIZE));
		set_inst(inst_mem, address, instruction);
	}

	pretty_print_inst_mem(inst_mem);
	kill_inst_mem(inst_mem);
}

void test_data_memory(int n) {
	uint8_t *data_mem = init_data_mem();
	int address;
	uint8_t data;

	for (int i = 0; i < n; i++) {
		address = gen_rnd_num(0, DATA_MEMORY_SIZE - 1);
		data = gen_rnd_num(0, pow(2, DATA_WORD_SIZE));
		set_data(data_mem, address, data);
	}

	pretty_print_data_mem(data_mem);
	kill_data_mem(data_mem);
}

void test_register(int n) {
	struct reg_t *reg = init_reg();
	// test the gprs
	int idx_gprs;
	uint8_t val;
	for (int i = 0; i < n; i++){
		idx_gprs = gen_rnd_num(0, NUM_GPRS - 1);
		val = gen_rnd_num(0, pow(2, 8));
		reg->GPRS[idx_gprs] = val;
	}

	val = gen_rnd_num(0, pow(2, 8));
	reg->PC = val;
	val = gen_rnd_num(0, pow(2, 8));
	reg->SREG = val;

	pretty_print_reg(reg);
	

	kill_reg(reg);
}


int main() {
	// printf("Testing the instruction memory\n");
	// test_instruction_memory(9);
	// printf("Testing the data memory\n");
	// test_data_memory(10);
	test_register(10);

	return 0;
}
