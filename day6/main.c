#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../common/intcode.h"

struct Item {
  char* id;
  int indirects;
  int num_of_children;
  struct Item* parent;
  struct Item* children;
};

struct Item* you = NULL;
struct Item* san = NULL;

int count_lines(const char* file_name) {
  int count = 0;
  FILE* fp;
  char* line = NULL;
  size_t len = 0;

  fp = fopen(file_name, "r");
  while (getline(&line, &len, fp) != -1) count++;

  if (line) free(line);
  fclose(fp);

  return count;
}

char** find_children(char** items, int num_of_items, char* id,
                     int* num_of_children) {
  int count = 0;
  for (int i = 0; i < num_of_items; i += 2)
    if (strcmp(id, items[i]) == 0) count++;
  if (count == 0) return NULL;
  char** children = malloc(count * sizeof(char*));
  int c = 0;
  for (int i = 0; i < num_of_items; i += 2)
    if (strcmp(id, items[i]) == 0) {
      children[c] = items[i + 1];
      strcpy(children[c], items[i + 1]);
      c++;
    }
  *num_of_children = count;
  return children;
};

void build_tree(char** items, int num_of_items, struct Item* node, int depth,
                int* indirect_sum, int* direct_sum) {
  if (strcmp(node->id, "YOU") == 0) you = node;
  if (strcmp(node->id, "SAN") == 0) san = node;

  int num_of_children = 0;
  char** children =
      find_children(items, num_of_items, node->id, &num_of_children);

  if (children == NULL) return;

  node->children = malloc(num_of_children * sizeof(struct Item));
  node->num_of_children = num_of_children;

  *direct_sum += num_of_children;
  *indirect_sum += num_of_children * depth;
  for (int i = 0; i < num_of_children; i++) {
    node->children[i].id = children[i];
    node->children[i].parent = node;
    node->children[i].indirects = depth;
    build_tree(items, num_of_items, &node->children[i], depth + 1, indirect_sum,
               direct_sum);
  }

  free(children);
};

void free_tree(struct Item* node) {
  for (int i = 0; i < node->num_of_children; i++) free_tree(&node->children[i]);
  free(node->children);
}

int distance_to_root(struct Item* node) {
  int count = 0;
  struct Item* temp = node;
  do {
    count++;
  } while ((temp = temp->parent) != NULL);

  return count;
};

void get_root_path(struct Item* node, struct Item** path) {
  int count = 0;
  struct Item* temp = node;
  do {
    path[count++] = temp;
  } while ((temp = temp->parent) != NULL);
};

int distance_to_node(struct Item* node0, struct Item* node1) {
  int path_length = distance_to_root(node0);
  struct Item** path = malloc(path_length * sizeof(struct Item*));
  get_root_path(node0, path);
  struct Item* temp = node1;

  int count = 0;
  while (1) {
    for (int i = 0; i < path_length; i++)
      if (strcmp(path[i]->id, temp->id) == 0) {
        free(path);
        return count + i;
      }
    count++;
    temp = temp->parent;
    if (temp == NULL) break;
  }

  free(path);
  return -1;
};

int main() {
  const char* file_name = "./input.txt";
  FILE* fp;
  char* line = NULL;
  char* token;
  size_t len = 0;

  int num_of_lines = count_lines(file_name);
  int num_of_items = num_of_lines * 2;
  char** items = malloc(num_of_items * sizeof(char*));
  fp = fopen(file_name, "r");
  int i = 0;
  while (getline(&line, &len, fp) != -1) {
    while ((token = strsep(&line, ")")) != NULL) {
      items[i] = (char*)malloc(strlen(token));
      strcpy(items[i], token);
      if (i % 2) items[i][strcspn(items[i], "\n")] = 0;
      i++;
    }
  }
  fclose(fp);

  struct Item com = {
      .id = "COM", .indirects = 0, .parent = NULL, .children = NULL};

  int indirect_sum = 0;
  int direct_sum = 0;
  build_tree(items, num_of_items, &com, 0, &indirect_sum, &direct_sum);

  int first_part = indirect_sum * direct_sum;
  int second_part = distance_to_node(you->parent, san->parent);

  printf("first part: %i, second part: %i\n", first_part, second_part);

  free_tree(&com);
  if (line) free(line);
  if (token) free(token);
  for (int i = 0; i < num_of_items; i++) {
    free(items[i]);
  }
  free(items);
};
