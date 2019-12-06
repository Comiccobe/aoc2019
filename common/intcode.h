#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "./helpers.h"

int translate(int* memory, char* instruction, int pc, int offset) {
  return !instruction[2 - offset + 1] ? memory[memory[pc + offset]]
                                      : memory[pc + offset];
};

void run_program(int* memory, int memory_length, int input, int* output) {
  int done = 0;
  int pc = 0;
  int opcode_length = 5;

  while (!done) {
    int instruction_length = get_number_length(memory[pc]);
    int pad_amount = opcode_length - instruction_length;
    int padded_instruction_length = instruction_length + pad_amount;
    char instruction[padded_instruction_length];

    split_number_into_digits(memory[pc], instruction_length, instruction);
    shift_digits(instruction, padded_instruction_length, pad_amount);

    int opcode = concatenate_numbers(instruction[3], instruction[4]);
    switch (opcode) {
      case 1:
        memory[memory[pc + 3]] = translate(memory, instruction, pc, 1) +
                                 translate(memory, instruction, pc, 2);
        pc += 4;
        break;

      case 2:
        memory[memory[pc + 3]] = translate(memory, instruction, pc, 1) *
                                 translate(memory, instruction, pc, 2);
        pc += 4;
        break;

      case 3:
        memory[memory[pc + 1]] = input;
        pc += 2;
        break;

      case 4:
        if (output) *output = translate(memory, instruction, pc, 1);
        pc += 2;
        break;

      case 5:
        if (translate(memory, instruction, pc, 1))
          pc = translate(memory, instruction, pc, 2);
        else
          pc += 3;
        break;

      case 6:
        if (!translate(memory, instruction, pc, 1))
          pc = translate(memory, instruction, pc, 2);
        else
          pc += 3;
        break;

      case 7:
        if (translate(memory, instruction, pc, 1) <
            translate(memory, instruction, pc, 2))
          memory[memory[pc + 3]] = 1;
        else
          memory[memory[pc + 3]] = 0;
        pc += 4;
        break;

      case 8:
        if (translate(memory, instruction, pc, 1) ==
            translate(memory, instruction, pc, 2))
          memory[memory[pc + 3]] = 1;
        else
          memory[memory[pc + 3]] = 0;
        pc += 4;
        break;

      case 99:
        done = 1;
        break;
    }
  }
};

int* load_base_memory(size_t* len) {
  FILE* fp;
  char* line = NULL;
  size_t length = 0;

  fp = fopen("./input.txt", "r");
  getline(&line, &length, fp);
  fclose(fp);
  char* token;
  int* memory = (int*)malloc(sizeof(int) * length);

  int i = 0;
  while ((token = strsep(&line, ",")) != NULL) {
    memory[i++] = atoi(token);
  }

  memory = realloc(memory, sizeof(int) * i);
  *len = i;
  if (line) free(line);
  return memory;
};

void reset_memory(int* base_memory, int* memory, int memory_length) {
  memcpy(memory, base_memory, sizeof(int) * memory_length);
};
