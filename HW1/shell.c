#include "shell.h"
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>

#define TRUE 1
#define FALSE 0
#define STD_INPUT 0
#define STD_OUTPUT 1
#define STD_ERROR 2

static char command[INPUT_SIZE];
static char *process1[COMMAND_SIZE], *process2[COMMAND_SIZE];
static char *outputPath = NULL, *inputPath = NULL;
static bool DO_PIPE = FALSE;
static bool SHELL_EXIT = FALSE, OUTPUT_REDIRECTION = FALSE, INPUT_REDIRECTION = FALSE, RUN_BACKGROUND = FALSE;

void reset_all(){
	DO_PIPE = SHELL_EXIT = OUTPUT_REDIRECTION = INPUT_REDIRECTION = RUN_BACKGROUND = FALSE;
	outputPath = inputPath = NULL;
	for (int i = 0 ; i < COMMAND_SIZE ; i++){
		command[i] = 0;
		process1[i] = 0;
		process2[i] = 0;
	}
}

void myShell(){
	while (TRUE){
				
		printf(">> ");  // prompt
		reset_all();
		read_command();
		if (SHELL_EXIT){
			break;
		}	
		if (!strcmp(process1[0], "cd")){  // change the directory
			chdir(process1[1]);
			continue;
		}
		int pid = fork();
		if (pid == -1){  // child process is failed
			perror("fork() is failed !");  // appear at thefront of the original error message
		}	
		else if (pid != 0){  // parent process
			if (!RUN_BACKGROUND){  // wait for child process terminal
				int status;
                waitpid(pid, &status, 0);
			}
			else {
				printf("Background process created with PID [ %d ]\n", pid);
			}
		}
		else {  // child process
				
			if (DO_PIPE){
				//printf("create pipe.\n");
				pipeline_create();
			}
			else {
				if (OUTPUT_REDIRECTION){
					// S_...is the right to access this file
					int output_fd = open(outputPath, O_WRONLY | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);
					dup2(output_fd, STD_OUTPUT);  // replace standard output by outputPath
					execvp(process1[0], process1);
					close(output_fd);  // close file
				}
				else if (INPUT_REDIRECTION){
					int input_fd = open(inputPath, O_RDONLY);
					dup2(input_fd,STD_INPUT);  // set standard input to input_fd
					execvp(process1[0],process1);
					close(input_fd);  // close file
				}
				else  // not input or output redirection
					execvp(process1[0],process1);
			}
		}
	}
}
void read_command(){
	int split_index = 0;
	int pipe_index = 0, has_filepath = 0;
	int i = 0;
	char *storeCommand[COMMAND_SIZE], *split_str;

	fgets(command, sizeof(command), stdin);  // input the command line including the blank

	int str_len = strlen(command);
	if (command[str_len - 1] == '\n'){  // make the command become a string
		command[str_len - 1] = '\0';
	}
	if (!strcmp(command, "exit") || !strcmp(command, "EXIT")){  // exit shell
		SHELL_EXIT = TRUE;
		return;
	}
	split_str = strtok(command, " ");  // split the command by white space
	while (split_str != NULL){
		storeCommand[split_index] = split_str;
		split_index++;
		split_str = strtok(NULL, " ");
	}
	for (i = 0; i < split_index; i++){
		if (*storeCommand[i] == '>'){  // output
			OUTPUT_REDIRECTION = TRUE;
			outputPath = storeCommand[i+1];
			has_filepath++;
			i++;
			continue;
		}
		else if (*storeCommand[i] == '<'){  // input
			INPUT_REDIRECTION = TRUE;
			inputPath = storeCommand[i+1];
			has_filepath++;
			i++;
			continue;
		}
		else if (*storeCommand[i] == '|'){  // pipeline
			DO_PIPE = TRUE;
			pipe_index = i;
			has_filepath = 0;
			continue;
		}
		else if (*storeCommand[i] == '&'){  // background running
			RUN_BACKGROUND = TRUE;
			break;
		}
		if (DO_PIPE){  // process2	
			process2[i - pipe_index - 1 - has_filepath] = storeCommand[i];
			//printf("process2 index %d : %s\n", i - pipe_index - 1, process2[i - pipe_index - 1 - has_filepath]);
		}
		else {  // process1
			process1[i - has_filepath] = storeCommand[i];
			//printf("process1 index %d : %s\n", i, process1[i - has_filepath]);
		}
	}
	// printf("Output Path : %s\n", outputPath);
	// printf("Input Path : %s\n", inputPath);
}
void pipeline_create(){

	int fd[2];

	pipe(&fd[0]);
	int pid = fork();
	if (pid == -1){
		perror("fork() is failed !");
	}
	else if (pid != 0){  // parent process
		close(fd[0]);
		dup2(fd[1], STD_OUTPUT);  // replace the standard output by fd[1]
		close(fd[1]);
		execvp(process1[0], process1);
	}
	else {  // child process

		close(fd[1]);
		dup2(fd[0],STD_INPUT);  // replace the standard input by fd[0]
		close(fd[0]);

		if (OUTPUT_REDIRECTION){  // output redirection with pipe now
			int output_fd = open(outputPath, O_WRONLY | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);
			dup2(output_fd, STD_OUTPUT);
			execvp(process2[0], process2);
			close(output_fd);
		}
		else
			execvp(process2[0], process2);
	}
}