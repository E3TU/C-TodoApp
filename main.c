#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TODO_FILE "todos.txt"
#define TODO_LENGTH 50

typedef struct  {
    int id;
    char description[TODO_LENGTH];
} Todo;

void list_todos() {
    FILE *file = fopen(TODO_FILE, "r");
    if (!file) {
        printf("No todos found\n");
        return;
    }

    Todo todo;
    while (fread(&todo, sizeof(Todo), 1, file)) {
        printf("ID: %d | description: %s\n", todo.id, todo.description);
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
    static int id_counter = 1;
    todo.id = id_counter++;
    strncpy(todo.description, description, TODO_LENGTH);
    fwrite(&todo, sizeof(Todo), 1, file);
    fclose(file);
    printf("Added todo: ID=%d | %s\n", todo.id, todo.description);

}

void delete_todo() {

}

int main(int argc, char *argv[])
{
    if (argc < 2) {
        printf("Usage: todo <command> [agruments]\n");
        return 1;
    }
    if (strcmp(argv[1], "list") == 0) {
        list_todos();
    } else if (strcmp(argv[1], "add") == 0 && argc == 3) {
        add_todo(argv[2]);
    }
    return 0;
}
