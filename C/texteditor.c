#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#define INITIAL_CAPACITY 100
#define MAX_WORD_LENGTH 50

struct Stack {
    char **data;
    int capacity;
    int size;
    int cursor;
};
// memory allocation for stack
void initStack(struct Stack *stack) {
    stack->data = (char **)malloc(INITIAL_CAPACITY * sizeof(char *));
    if (stack->data == NULL) {
        printf("Memory allocation failed!\n");
        exit(EXIT_FAILURE);
    }
    for (int i = 0; i < INITIAL_CAPACITY; i++) {
        stack->data[i] = (char *)malloc(MAX_WORD_LENGTH * sizeof(char));
        if (stack->data[i] == NULL) {
            printf("Memory allocation failed!\n");
            exit(EXIT_FAILURE);
        }
    }
    stack->capacity = INITIAL_CAPACITY;
    stack->size = 0;
    stack->cursor = 0;
}
// doubles the capacity of stack as it is full
void resizeStack(struct Stack *stack) {
    stack->capacity *= 2;
    stack->data = (char **)realloc(stack->data, stack->capacity * sizeof(char *));
    if (stack->data == NULL) {
        printf("Memory reallocation failed!\n");
        exit(EXIT_FAILURE);
    }
    for (int i = stack->size; i < stack->capacity; i++) {
        stack->data[i] = (char *)malloc(MAX_WORD_LENGTH * sizeof(char));
        if (stack->data[i] == NULL) {
            printf("Memory allocation failed!\n");
            exit(EXIT_FAILURE);
        }
    }
}
// checks if the stack is full
bool isFull(struct Stack *stack) {
    return stack->size == stack->capacity;
}
// checks if the stack is empty
bool isEmpty(struct Stack *stack) {
    return stack->size == 0;
}
// push item to stack, resizes if necessary
void push(struct Stack *stack, const char *item) {
    if (isFull(stack)) {
        resizeStack(stack);
    }
    strcpy(stack->data[stack->size++], item);
}
// pop item from stack
void pop(struct Stack *stack) {
    if (!isEmpty(stack)) {
        free(stack->data[--stack->size]);
    } else {
        printf("Stack underflow!\n");
    }
}
// cursor to right
void insertString(struct Stack *stack, const char *word) {
    if (stack->cursor < 0 || stack->cursor > stack->size) {
        printf("Invalid cursor position!\n");
        return;
    }

    int i;
    for (i = stack->size; i > stack->cursor; i--) {
        strcpy(stack->data[i], stack->data[i - 1]);
    }
    strcpy(stack->data[i], word);
    stack->size++;
    stack->cursor++;
}
// cursor to left
void deleteString(struct Stack *stack) {
    if (stack->cursor > 0 && stack->cursor <= stack->size) {
        free(stack->data[stack->cursor - 1]);
        int i;
        for (i = stack->cursor - 1; i < stack->size - 1; i++) {
            strcpy(stack->data[i], stack->data[i + 1]);
        }
        stack->size--;
        stack->cursor--;
    } else {
        printf("Invalid cursor position or text is empty!\n");
    }
}

void moveCursorLeft(struct Stack *stack) {
    if (stack->cursor > 0) {
        stack->cursor--;
    } else {
        printf("Cursor is at the beginning!\n");
    }
}

void moveCursorRight(struct Stack *stack) {
    if (stack->cursor < stack->size) {
        stack->cursor++;
    } else {
        printf("Cursor is at the end!\n");
    }
}
// pop all the element and reset cursor to 0
void clearEditor(struct Stack *stack) {
    while (!isEmpty(stack)) {
        pop(stack);
    }
    stack->cursor = 0;
}

void displayText(struct Stack *stack) {
    printf("Text: ");
    for (int i = 0; i < stack->size; i++) {
        printf("%s ", stack->data[i]);
    }
    printf("\nCursor position: %d\n", stack->cursor);
}

int main() {
    while (true) {
        struct Stack textStack;
        initStack(&textStack);
        char input[MAX_WORD_LENGTH];
        int choice;

        while (true) {
            printf("\nMenu:\n");
            printf("1. Insert word\n");
            printf("2. Delete word\n");
            printf("3. Move cursor left\n");
            printf("4. Move cursor right\n");
            printf("5. Clear editor\n");
            printf("6. Display text\n");
            printf("7. Exit\n");
            printf("Enter your choice: ");
            fgets(input, sizeof(input), stdin);

            sscanf(input, "%d", &choice);

            switch (choice) {
                case 1:
                    printf("Enter word to insert: ");
                    fgets(input, sizeof(input), stdin);
                    input[strcspn(input, "\n")] = 0; // remove trailing newline
                    insertString(&textStack, input);
                    displayText(&textStack);
                    break;
                case 2:
                    deleteString(&textStack);
                    displayText(&textStack);
                    break;
                case 3:
                    moveCursorLeft(&textStack);
                    displayText(&textStack);
                    break;
                case 4:
                    moveCursorRight(&textStack);
                    displayText(&textStack);
                    break;
                case 5:
                    clearEditor(&textStack);
                    break;
                case 6:
                    displayText(&textStack);
                    break;
                case 7:
                    printf("Exiting...\n");
                    for (int i = 0; i < textStack.size; i++) {
                        free(textStack.data[i]);
                    }
                    free(textStack.data);
                    exit(EXIT_SUCCESS);
                default:
                    printf("Invalid choice! Please enter a number between 1 and 7.\n");
            }
        }
    }

    return 0;
}