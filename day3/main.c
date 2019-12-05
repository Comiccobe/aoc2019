#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define UP 0
#define RIGHT 1
#define DOWN 2
#define LEFT 3

int* load_map(size_t* len, int map_id) {
  FILE* fp;
  char* line = NULL;
  size_t length = 0;

  fp = fopen("./input.txt", "r");
  getline(&line, &length, fp);
  if (map_id == 1) {
    getline(&line, &length, fp);
  }
  char* token;
  int* map = (int*)malloc(sizeof(int) * length);

  int i = 0;
  while ((token = strsep(&line, ",")) != NULL) {
    switch (token[0]) {
      case 'U':
        map[i++] = UP;
        break;
      case 'R':
        map[i++] = RIGHT;
        break;
      case 'D':
        map[i++] = DOWN;
        break;
      case 'L':
        map[i++] = LEFT;
        break;
    }
    map[i++] = atoi(token + 1);
  }

  map = realloc(map, sizeof(int) * i);
  *len = i;
  fclose(fp);
  if (line) free(line);
  return map;
};

int intersects(int* route, int route_length, int x, int y, int steps,
               int* fewest_steps) {
  for (int i = 0; i < route_length; i += 2) {
    if (x == route[i] && y == route[i + 1]) {
      int combined_steps = steps + (i / 2) + 1;
      if (combined_steps < *fewest_steps) {
        *fewest_steps = combined_steps;
      }
      return abs(x) + abs(y);
    }
  }
  return -1;
}

void run_line(int* route, int* count, int* point, int axis, int length,
              int mutation) {
  for (int i = 0; i < length; i++) {
    point[axis] += mutation;
    route[(*count)++] = point[0];
    route[(*count)++] = point[1];
  }
}

int* run_first_wire(int* map, int map_length, int route_length) {
  int* route = (int*)malloc(sizeof(int) * route_length);

  int point[2] = {0, 0};
  int count = 0;
  for (int i = 0; i < map_length; i += 2) {
    int axis = map[i] == RIGHT || map[i] == LEFT ? 0 : 1;
    int mutation = map[i] == UP || map[i] == LEFT ? -1 : 1;
    run_line(route, &count, point, axis, map[i + 1], mutation);
  }

  return route;
}

int get_route_length(int* map, int map_length) {
  int route_length = 0;
  for (int i = 0; i < map_length; i += 2) {
    route_length += map[i + 1];
  }
  return route_length * 2;
};

void run_line_intersection(int* route, int route_length, int line_length,
                           int axis, int mutation, int* point, int* steps,
                           int* closest_distance, int* fewest_steps) {
  for (int i = 0; i < line_length; i++) {
    point[axis] += mutation;
    (*steps)++;
    int ip = intersects(route, route_length, point[0], point[1], *steps,
                        fewest_steps);
    if (ip != -1 && ip < (*closest_distance)) {
      *closest_distance = ip;
    }
  }
};

int main() {
  size_t first_map_length = 0;
  size_t second_map_length = 0;
  int* first_map = load_map(&first_map_length, 0);
  int* second_map = load_map(&second_map_length, 1);

  int route_length = get_route_length(first_map, first_map_length);
  int* first_route = run_first_wire(first_map, first_map_length, route_length);

  int point[2] = {0, 0};
  int steps = 0;
  int closest_distance = 0xFFFFFF;
  int fewest_steps = 0xFFFFFF;

  for (int i = 0; i < second_map_length; i += 2) {
    int axis = second_map[i] == RIGHT || second_map[i] == LEFT ? 0 : 1;
    int mutation = second_map[i] == UP || second_map[i] == LEFT ? -1 : 1;

    run_line_intersection(first_route, route_length, second_map[i + 1], axis,
                          mutation, point, &steps, &closest_distance,
                          &fewest_steps);
  }

  printf("closest distance: %i, fewest steps: %i\n", closest_distance,
         fewest_steps);

  if (first_map) free(first_map);
  if (second_map) free(second_map);
  if (first_route) free(first_route);
};
