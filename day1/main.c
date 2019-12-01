#include <math.h>
#include <stdio.h>
#include <stdlib.h>

int calc_fuel(int n, int sum) {
  int new_n = (n / 3) - 2;
  return new_n <= 0 ? sum : calc_fuel(new_n, sum + new_n);
};

int main() {
  FILE* fp;
  char* line = NULL;
  size_t len = 0;
  int first_part = 0;
  int second_part = 0;

  fp = fopen("./input.txt", "r");
  while (getline(&line, &len, fp) != -1) {
    int d = (atoi(line) / 3) - 2;
    first_part += d;
    second_part += d + calc_fuel(d, 0);
  }

  printf("part one: %i\npart two: %i\n", first_part, second_part);

  fclose(fp);
  if (line) free(line);
};
