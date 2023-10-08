#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>
#include "wish_utils.c"

void execute_command();
void execute_batch_mode();
void execute_user_mode();
char error_message[30] = "An error has occurred\n";

#define MAX_HISTORY 10
#define MAX_SIZE 100

char *mypath[] = {"/bin/", "", NULL, NULL, NULL, NULL};

int main(int argc, char *argv[]){
	/*
	Recordando que el primer argumento es el nombre del programa,
	si solo se pasa un argumento, se entra en modo usuario, de lo 
	contrario se entra en modo batch
	*/
	// MODO BATCH
	if (argc > 1){
		execute_batch_mode(argv);
	}
	
	execute_user_mode(argv);
}

void execute_batch_mode(char *argv[]){
	FILE *fp;
	char *line = NULL;
	size_t len = 0;
	//ssize_t read;
	char *command_string;
	char *s;
	

	fp = fopen(argv[1], "r");
	if (fp == NULL){
		printf("Error opening the file\n");
		exit(EXIT_FAILURE);
	}

	while ((getline(&line, &len, fp)) != -1) {
		s = line;
		while(*s != '\n') {
			++s;
		}
		*s = '\0';
		s = line;
		command_string = strtok_r(s, " ", &s);

		execute_command(command_string, s, mypath);
	}

	fclose(fp);
	if (line)
		free(line);
	exit(EXIT_SUCCESS);
}

void execute_user_mode(char *argv[]){
	// MODO USUARIO
	char str[MAX_SIZE];
	char *command_string;
	char *s;
	int fd;

	char *line = (char *)malloc(sizeof(char) * MAX_SIZE);

    // Atributos para el command history
    char ch;
    char *history[MAX_HISTORY] = {NULL};
    int history_idx = 0;
    int line_idx = 0;
    int history_count = 0;

	do{
		printf("whish> ");
		fgets(str, MAX_SIZE, stdin);
		s = str;
			while(*s != '\n') {
				++s;
			}

		 if (*s == '\033')
            { // Escape sequence
                navigate_command_history(history, line, &history_count, &history_idx, &line_idx, &ch);
            }
            else if (*s == '\n')
            { // Enter key
                add_command_to_history(history, line, &line_idx, &history_count, &history_idx);
                fflush(stdout);
            }	
		*s = '\0';
		s = str;

		command_string = strtok_r(s, " ", &s);

		execute_command(command_string, s, mypath);
   	}while(1);
}

void execute_command(char *command_string, char *s, char **mypath){
	int fd;
	char *command_args;

	if(strcmp(command_string, "exit") == 0){
				if( strcmp(s, "") != 0)
					execute_exit(-1);				
				else 
					execute_exit(0);
			}else if(strcmp(command_string, "cd") == 0){
				execute_cd(s);
			}else if(strcmp(command_string, "path") == 0){

					command_args = trimString(command_string);
                    execute_path(command_args);
			}else{
				fd = -1;
				char **mp = mypath;
				char specificpath[MAX_SIZE];
				while( (strcmp(*mp, "") != 0)  && fd != 0){
					strcpy(specificpath, *mp++);
					strncat(specificpath, command_string, strlen(command_string));
					fd = access(specificpath, X_OK);
				}
				if(fd==0){
					int subprocess = fork();
					if(subprocess < 0){
						printf("Error launching the subprocess");
					}else if(subprocess == 0){
						char *myargs[3];
						myargs[0] = strdup(specificpath);
						
						// Si no se pasa argumento, se asume que es el directorio actual
					if (strcmp(s, "") == 0) {
						myargs[1] = strdup(".");
						
					} else { // Si se pasa argumento, se asume que es el directorio especificado
						myargs[1] = strdup(s);
					}
	  				myargs[2] = NULL;
					execvp(command_string, myargs);
					}else{
						wait(NULL);
					}
				}else{
					//printf("Command not found: %s\n", s);
					write(STDERR_FILENO, error_message, strlen(error_message));
				}
			}
}


void navigate_command_history(char *history[], char *line, int *history_count, int *history_idx, int *line_idx, char *ch)
{
    *ch = getchar(); // '[' character
    *ch = getchar(); // Arrow key code

    if (*ch == 'A')
    { // Up arrow key
        if (*history_count > 0 && *history_idx > 0)
        {
            (*history_idx)--;
            memset(line, 0, MAX_SIZE);
            strncpy(line, history[*history_idx], MAX_SIZE - 1);
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
            memset(line, 0, MAX_SIZE);
            strncpy(line, history[*history_idx], MAX_SIZE - 1);
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

