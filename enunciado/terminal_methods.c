#include <stdio.h>
#include <stdlib.h>
#include <termios.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <ctype.h>

void set_terminal_raw_mode(struct termios *old_term, struct termios *new_term){
    // Se guarda la configuración temporal actual
    tcgetattr(STDIN_FILENO, old_term);
    *new_term = *old_term;

    // Se pone la terminal en modo Raw
    new_term->c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, new_term);
}

void restore_terminal_mode(struct termios *old_term){
    // Se restaura la configuracion de la terminal
    tcsetattr(STDIN_FILENO, TCSANOW, old_term);
}