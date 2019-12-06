int get_number_length(int num) {
  int count = 1;
  int temp = num;
  while (temp /= 10) count++;
  return count;
};

void split_number_into_digits(int num, int length, char* result) {
  int position = length;
  int temp = num;
  while (position--) {
    result[position] = temp % 10;
    temp /= 10;
  }
};

int concatenate_numbers(int x, int y) {
  int pow = 10;
  while (y >= pow) pow *= 10;
  return x * pow + y;
}

void shift_digits(char* digits, int digits_length, int places) {
  int i = digits_length - 1;
  while (i > 0) {
    digits[i] = digits[i - places];
    i--;
  }

  for (int j = 0; j < places; j++) {
    digits[j] = 0;
  }
};
