#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int run_program(int* base_memory, int* memory, int memory_length, int noun,
                int verb) {
  memcpy(memory, base_memory, sizeof(int) * memory_length);

  memory[1] = noun;
  memory[2] = verb;

  int done = 0;
  int pc = 0;
  while (!done) {
    switch (memory[pc]) {
      case 1:
        memory[memory[pc + 3]] =
            memory[memory[pc + 1]] + memory[memory[pc + 2]];
        pc += 4;
        break;

      case 2:
        memory[memory[pc + 3]] =
            memory[memory[pc + 1]] * memory[memory[pc + 2]];
        pc += 4;
        break;

      case 99:
        done = 1;
        break;
    }
  }

  return memory[0];
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

int main() {
  size_t base_memory_length = 0;
  int* base_memory = load_base_memory(&base_memory_length);
  int* memory = (int*)malloc(sizeof(int) * base_memory_length);

  int first_part = run_program(base_memory, memory, base_memory_length, 12, 2);
  int expected_result = 19690720;
  int second_part = 0;

  for (int noun = 0; noun < 100; noun++) {
    for (int verb = 0; verb < 100; verb++) {
      if (run_program(base_memory, memory, base_memory_length, noun, verb) ==
          expected_result) {
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
