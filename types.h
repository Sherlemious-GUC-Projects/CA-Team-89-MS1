#ifndef TYPE_H
#define TYPE_H

// ~~~ INSTRUCTION MEMORY ~~~ \\

#define INSTRUCTION_WORD_SIZE 16
#define INSTRUCTION_MEMORY_SIZE 1024

typedef uint16_t *inst_mem_t; 

inst_mem_t init_inst_mem() {
	inst_mem_t inst_mem = (uint16_t *)calloc(INSTRUCTION_MEMORY_SIZE, INSTRUCTION_WORD_SIZE);
	return inst_mem;
}

void kill_inst_mem( inst_mem_t inst_mem) {
	free(inst_mem);
}

// ~~~ DATA MEMORY ~~~ \\

#define DATA_WORD_SIZE 8
#define DATA_MEMORY_SIZE 2048

typedef uint8_t* data_mem_t; 

data_mem_t init_data_mem() {
	data_mem_t data_mem = (uint8_t *)calloc(DATA_MEMORY_SIZE, DATA_WORD_SIZE);
	return data_mem;
}

void kill_data_mem( data_mem_t data_mem) {
	free(data_mem);
}

// ~~~ REGISTER FILE ~~~ \\

#define NUM_GPRS 64

typedef struct reg_t {
	uint8_t GPRS[NUM_GPRS];
	uint8_t SREG;
	uint8_t PC;
	uint8_t i;
	bool halt;
} reg_t; 

reg_t* init_reg() {
	reg_t* reg = (reg_t*)calloc(1, sizeof(reg_t));
	if (reg == NULL) {
		printf("Error: Memory allocation failed\n");
		exit(1);
	}
	reg->i = 0;
	reg->halt = false;
	return reg;
}

void kill_reg(reg_t* reg) {
	free(reg);
}

// ~~~ PROGRAM CONTROL BLOCK ~~~ \\

typedef struct PCB_t {
	uint8_t OPCode; // 4 bits
	uint8_t operand1; // 6 bits
	uint8_t operand2; // 6 bits
	uint8_t R1; // 8 bits
	uint8_t R2; // 8 bits
	uint8_t IMM; // 8 bits
	bool is_R_format; // 1 bit
} PCB_t; 

PCB_t* init_pcb() {
	PCB_t *pcb = (PCB_t*) malloc(sizeof(PCB_t));
	return pcb;
}

#endif // TYPE_H
