#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include "wish_utils.h"

extern char *mypath[];
extern char error_message[];
#define MAX_FILENAME 1024

void execute_exit(int value){
    exit(value);
}

void execute_cd(char *newpath){
    char *path = strtok_r(newpath, " ", &newpath);
    // Dado el caso que la dirección no sea válida, se imprime un error
    if (chdir(path) != 0)
    {
        write(STDERR_FILENO, error_message, strlen(error_message));
        return;
    }
    chdir(path);
}

void execute_path(char *newpath){
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

    //Dado el caso que el path se encuentre vacío, el vector mypath se deja vacío
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

// Función que imprime el mensaje de error
void printError(){
    write(STDERR_FILENO, error_message, strlen(error_message));
}

//Función que toma una cadena de caracteres y elimina los espacios en blanco iniciales y finales
char *trimString(char *str){
    int start = 0, end = strlen(str) - 1;
    while (isspace(str[start])){
        start++;
    }
    while ((end >= start) && isspace(str[end])){
        end--;
    }
    str[end + 1] = '\0';
    return &str[start];
}