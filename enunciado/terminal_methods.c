#include <stdio.h>
#include <stdlib.h>
#include <termios.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <ctype.h>

void set_terminal_raw_mode(struct termios *old_term, struct termios *new_term){
    // Save the current terminal settings
    tcgetattr(STDIN_FILENO, old_term);
    *new_term = *old_term;

    // Set the terminal to raw mode
    new_term->c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, new_term);
}

void restore_terminal_mode(struct termios *old_term){
    // Restore the terminal settings
    tcsetattr(STDIN_FILENO, TCSANOW, old_term);
}