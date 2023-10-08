#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include "wish_utils.h"

extern char error_message[30];

void execute_exit(int value){
	if (value == 0){
		exit(0);
	}
	else{
		write(STDERR_FILENO, error_message, strlen(error_message));
		return;
	}
	exit(value);
}

void execute_cd(char *newpath){
	char *path = strtok_r(newpath, " ", &newpath);
	
	if (chdir(path) != 0){ // Si chdir es distinto de 0, hubo un error
		write(STDERR_FILENO, error_message, strlen(error_message)); // Imprmimos el mensaje de error elegido por default
	}
}

void execute_path(){
	printf("path executed\n");
}
