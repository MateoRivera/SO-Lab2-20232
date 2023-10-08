#include <stdio.h>
#include <stdlib.h>
#include <termios.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <ctype.h>

#define MAX_LINE_LENGTH 256

void handle_backspace_key(char *line, int *line_idx){
    if (*line_idx > 0)
    {
        putchar('\b');
        putchar(' ');
        putchar('\b');
        (*line_idx)--;
        line[*line_idx] = 0;
    }
}

void handle_input(char *line, int *line_idx, char *ch){
    if (*line_idx < MAX_LINE_LENGTH - 1)
    {
        putchar(*ch);
        line[*line_idx] = *ch;
        (*line_idx)++;
    }
}