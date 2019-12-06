#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../common/intcode.h"

int main() {
  size_t base_memory_length = 0;
  int* base_memory = load_base_memory(&base_memory_length);
  int* memory = (int*)malloc(sizeof(int) * base_memory_length);

  reset_memory(base_memory, memory, base_memory_length);
  memory[1] = 12;
  memory[2] = 2;
  run_program(memory, base_memory_length, 0, NULL);
  int first_part = memory[0];
  int expected_result = 19690720;
  int second_part = 0;

  for (int noun = 0; noun < 100; noun++) {
    for (int verb = 0; verb < 100; verb++) {
      reset_memory(base_memory, memory, base_memory_length);
      memory[1] = noun;
      memory[2] = verb;
      run_program(memory, base_memory_length, 0, NULL);
      if (memory[0] == expected_result) {
        second_part = 100 * noun + verb;
        goto finished;
      }
    }
  }

finished:
  printf("\nfirst part: %i, second part: %i\n", first_part, second_part);
  if (base_memory) free(base_memory);
  if (memory) free(memory);
};
