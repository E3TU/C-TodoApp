#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>

#define TODO_FILE "todos.txt"
#define TODO_LENGTH 50

int id;

typedef struct {
  char description[TODO_LENGTH];
} Todo;

Todo todo;

void create_todos_file(char *path) {

  printf("Create todos path: %s\n", path);

  struct stat statbuf;

  if (stat(path, &statbuf) == 0 && S_ISDIR(statbuf.st_mode)) {
    printf("Directory exist\n");
  } else {
    printf("Directory doesnt exist\n");
    if (mkdir(path, 0700) == -1) {
      perror("Error creating dir for todos.txt\n");
      return;
    } else {
      printf("Directory for todos.txt created successfully\n");

      FILE *file;

      strcat(path, "todos.txt");

      file = fopen(path, "w");

      if (file == NULL) {
        printf("Error couldnt open file");
        return;
      }

      fprintf(file, "Test 2");
      fclose(file);
      printf("create_todos: %s\n", path);

      return;
    }
  }
}

void create_config_file(char *path) {

  const char *home_dir = getenv("HOME");

  // Check if home directory exists
  if (home_dir == NULL) {
    perror("Error: Home environment variable isnt set");
    return;
  } else {

    char dir_path[256];

    // Path where to create directory
    snprintf(dir_path, sizeof(dir_path), "%s/.config/c-todoapp", home_dir);

    // Create directory for config file
    if (mkdir(dir_path, 0700) == -1) {
      perror("Error creating dir for config file");
      return;
    } else {
      printf("Dir '%s' created successfully\n", dir_path);

      char file_path[256];

      const char filename[] = "todos/";

      // Path where to create config file
      snprintf(file_path, sizeof(file_path), "%s/config.conf", dir_path);

      // Create config file
      FILE *file = fopen(file_path, "w");
      if (file == NULL) {
        perror("Error opening file");
        return;
      } else {
        fprintf(file, "# This is the path for the todos:\n");
        fprintf(file, "\n");

        if (path == NULL) {

          char default_path[256];

          snprintf(default_path, sizeof(default_path), "%s/Documents/%s",
                   home_dir, filename);

          fprintf(file, "%s", default_path);

          printf("Using default file path\n");
          fclose(file);

          path = default_path;

          create_todos_file(path);

          return;
        }

        if (path[strlen(path) - 1] != '/') {
          strcat(path, "/");
        }

        strcat(path, filename);

        fprintf(file, "%s", path);

        printf("%s\n", path);

        fclose(file);
        printf("File %s created successfully\n", file_path);

        create_todos_file(path);
        return;
      }
      return;
    }
  }
}

void list_todos() {
  FILE *file = fopen(TODO_FILE, "r");

  if (!file) {
    printf("No todos found\n");
    return;
  }

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

  strncpy(todo.description, description, TODO_LENGTH);
  fwrite(&todo, sizeof(Todo), 1, file);
  // fwrite("\n", sizeof(char), 1, file);
  fclose(file);
  printf("Added todo\n");
  id++;
}

void delete_todo(const int *todoToRemove) {

  FILE *file = fopen(TODO_FILE, "r");
  if (!file) {
    printf("No todos to remove.\n");
    return;
  }

  Todo todos[100];

  while (fread(&todos[id], sizeof(Todo), 1, file)) {
    id++;
  }
  fclose(file);

  if (id == 0) {
    printf("No todos to remove\n");
    return;
  }

  FILE *temp = fopen("temp.txt", "w");
  if (!temp) {
    perror("Error opening temp file");
    return;
  }

  if (*todoToRemove < 1 || *todoToRemove > id) {
    printf("Invalid todo ID\n");
    fclose(temp);
    return;
  }

  for (int i = 0; i < id; i++) {
    if (i != *todoToRemove - 1) {
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
  } else if (strcmp(argv[1], "delete") == 0 && argc == 3) {
    int kurwa = atoi(argv[2]);
    delete_todo(&kurwa);
  } else if (strcmp(argv[1], "init") == 0) {
    if (argc == 3) {
      create_config_file(argv[2]);
    } else {
      create_config_file(NULL);
    }
  }
  return 0;
}
