#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../common/intcode.h"

int main() {
  size_t base_memory_length = 0;
  int* base_memory = load_base_memory(&base_memory_length);
  int* memory = (int*)malloc(sizeof(int) * base_memory_length);
  int first_part = 0;
  int second_part = 0;

  reset_memory(base_memory, memory, base_memory_length);
  run_program(memory, base_memory_length, 1, &first_part);

  reset_memory(base_memory, memory, base_memory_length);
  run_program(memory, base_memory_length, 5, &second_part);

  printf("\nfirst part: %i, second part: %i\n", first_part, second_part);
};
