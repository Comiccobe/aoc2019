#include <stdio.h>
#include "../common/helpers.h"

int main() {
  int from = 206938;
  int to = 679128;
  int part_one = 0;
  int part_two = 0;

  for (int i = from; i < to; i++) {
    int count = get_number_length(i);
    if (count != 6) continue;
    char arr[count];
    split_number_into_digits(i, count, arr);

    if ((arr[0] == arr[1] || arr[1] == arr[2] || arr[2] == arr[3] ||
         arr[3] == arr[4] || arr[4] == arr[5]) &&
        (arr[0] <= arr[1] && arr[1] <= arr[2] && arr[2] <= arr[3] &&
         arr[3] <= arr[4] && arr[4] <= arr[5])) {
      part_one++;

      if ((arr[0] == arr[1] && arr[1] != arr[2]) ||
          (arr[1] == arr[2] && arr[0] != arr[1] && arr[2] != arr[3]) ||
          (arr[2] == arr[3] && arr[1] != arr[2] && arr[3] != arr[4]) ||
          (arr[3] == arr[4] && arr[2] != arr[3] && arr[4] != arr[5]) ||
          (arr[4] == arr[5] && arr[3] != arr[4])) {
        part_two++;
      }
    }
  }

  printf("part one: %i, part two: %i\n", part_one, part_two);
};
