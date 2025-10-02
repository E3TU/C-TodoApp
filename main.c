#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TODO_FILE "todos.txt"
#define TODO_LENGTH 50

int id;

typedef struct {
  char description[TODO_LENGTH];
} Todo;

void list_todos() {
  FILE *file = fopen(TODO_FILE, "r");

  //   int id = 0;

  if (!file) {
    printf("No todos found\n");
    return;
  }

  Todo todo;
  while (fread(&todo, sizeof(Todo), 1, file)) {
    id++;
    printf("%d. %s\n", id, todo.description);
  }
  fclose(file);
}

void add_todo(const char *description) {
  FILE *file = fopen(TODO_FILE, "a");
  if (!file) {
    perror("Error opening file");
    return;
  }

  Todo todo;
  strncpy(todo.description, description, TODO_LENGTH);
  fwrite(&todo, sizeof(Todo), 1, file);
  // fwrite("\n", sizeof(char), 1, file);
  fclose(file);
  printf("Added todo\n");
  id++;
}

void delete_todo() {
  FILE *file = fopen(TODO_FILE, "r");
  if (!file) {
    printf("No todos to remove.\n");
    return;
  }

  Todo todos[100];

  int todoCount = 0;

  while (fread(&todos[todoCount], sizeof(Todo), 1, file)) {
    todoCount++;
  }
  fclose(file);

  if (todoCount == 0) {
    printf("No todos to remove\n");
    return;
  }

  FILE *temp = fopen("temp.txt", "w");
  if (!temp) {
    perror("Error opening temp file");
    return;
  }

  int todoId;
  printf("Enter ID of todo you want to delete: ");
  scanf("%d", &todoId);

  if (todoId < 1 || todoId > todoCount) {
    printf("Invalid todo ID\n");
    fclose(temp);
    return;
  }

  for (int i = 0; i < todoCount; i++) {
    if (i != todoId - 1) {
      fwrite(&todos[i], sizeof(Todo), 1, temp);
    }
  }
  fclose(temp);

  remove(TODO_FILE);
  rename("temp.txt", TODO_FILE);

  printf("Todo deleted successfully\n");
}

int main(int argc, char *argv[]) {
  if (argc < 2) {
    printf("Usage: todo <command> [agruments]\n");
    return 1;
  }
  if (strcmp(argv[1], "list") == 0) {
    list_todos();
  } else if (strcmp(argv[1], "add") == 0 && argc == 3) {
    add_todo(argv[2]);
  } else if (strcmp(argv[1], "delete") == 0 && argc == 2) {
    delete_todo();
  }
  return 0;
}
