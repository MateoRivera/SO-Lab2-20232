#include <stdio.h>
#include <stdlib.h>
#include <termios.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <ctype.h>

#define MAX_HISTORY 10
#define MAX_LINE_LENGTH 256

void navigate_command_history(char *history[], char *line, int *history_count, int *history_idx, int *line_idx, char *ch)
{
    *ch = getchar(); // '[' character
    *ch = getchar(); // Arrow key code

    if (*ch == 'A')
    { // Up arrow key
        if (*history_count > 0 && *history_idx > 0)
        {
            (*history_idx)--;
            memset(line, 0, MAX_LINE_LENGTH);
            strncpy(line, history[*history_idx], MAX_LINE_LENGTH - 1);
            *line_idx = strlen(line);
            printf("\033[2K\r%s", line);
            fflush(stdout);
        }
    }
    else if (*ch == 'B')
    { // Down arrow key
        if (*history_count > 0 && *history_idx < *history_count - 1)
        {
            (*history_idx)++;
            memset(line, 0, MAX_LINE_LENGTH);
            strncpy(line, history[*history_idx], MAX_LINE_LENGTH - 1);
            *line_idx = strlen(line);
            printf("\033[2K\r%s", line);
            fflush(stdout);
        }
    }
}

void add_command_to_history(char *history[], char *line, int *line_idx, int *history_count, int *history_idx)
{
    putchar('\n');

    if (*line_idx > 0)
    {
        // Copy the current line to the history
        if (*history_count == MAX_HISTORY)
        {
            free(history[0]);
            memmove(history, history + 1, (MAX_HISTORY - 1) * sizeof(char *));
            (*history_count)--;
        }
        history[*history_count] = malloc((*line_idx + 1) * sizeof(char));
        strncpy(history[*history_count], line, *line_idx);
        history[*history_count][*line_idx] = '\0';
        (*history_count)++;
        *history_idx = *history_count;
    }

    *line_idx = 0;
}