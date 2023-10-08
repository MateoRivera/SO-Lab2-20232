#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include "wish_utils.h"

extern char error_message[30];
extern char *mypath[];

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

void execute_path(char *newpath)
{
    char *tokens[4];
    char *token;
    memset(tokens, 0, sizeof(tokens));
    int c= 0;
    token = strtok(newpath, " ");
    
    while (token != NULL)
    {
        tokens[c] = token;
        token = strtok(NULL, " ");
        c++;
    }

    //Si el path está vacío deja vacío el vector "mypath"
    if (strcmp(tokens[0], ".") == 0) {
        mypath[0] = "";
        for (int i = 1; i < 6 && mypath[i] != NULL; i++) {
            mypath[i] = NULL;
        }
        return;
    }

    for (int j = 0; j < c; j++)
    {
        strcpy(newpath, tokens[j]);
        char **mp = mypath;
        int i = 0;

        while ((strcmp(*mp, "") != 0))
        {
            mp++;
            i++;
        }
        // Dado el caso que el objeto definido newpath no exista en la variable previamente definida "mypath", se procede a insertarlo
        mypath[i] = malloc(strlen(newpath) + 1);
        strcpy(mypath[i], newpath);
                // Valida que el formato del new path sea correcto (Debe terminar con "/")
        if (strchr(mypath[i], '/') == NULL)
        {
            // Si el new path no tiene "/", lo añade al final
            mypath[i] = strcat(mypath[i], "/");
        }
        mypath[i + 1] = "";
    }
}
