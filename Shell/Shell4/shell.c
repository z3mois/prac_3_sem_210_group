#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <string.h>
#include <fcntl.h>
#define BLOCK 8

char **parsing(void);
void deleteVector(char **commands);
int ifSpecial(char sym);
int conv(char **start, char **first, char **last);
int shellCom(char **start, char **com);
int logicalCom(char **start, char **first, char **last);
int comman(char **start, char **com, char **last);
int checkingIO(char **com, char **last, int *amount, int *input, int *output, int *strAmount);
void backProc(char **start, char **first, char **last);
int bracketLook(char **supp, char **last, int *i);
void deleteEnv(char **start, char **first, char **second);

int main(int argc, char **argv)
{
	while (1) {
		printf("Enter command: ");
		char **commands;
		commands = parsing();
		if (strcmp(commands[0], "exit") == 0)
			break;
		shellCom(commands, commands);
		deleteVector(commands);
		free(commands);
	}
	return 0;
}

int shellCom(char **start, char **com)
{
	char **firstCom = com;
	int result = 0, i = 0;
	while (com[i] != NULL){
		while ((com[i] != NULL) && (strcmp(com[i], "&") != 0) && (com[i][0] != ';') && (com[i][0] != '(')) i++;
		if (com[i] == NULL)	
			result = logicalCom(start, firstCom, com + i - 1);
		else if (com[i][0] == '(') {
			if (bracketLook(com, NULL, &i) == -1) return 1;
		} else if (com[i][0] == '&'){
			backProc(start, firstCom, com + i - 1);
			firstCom = com + i + 1;
			i++;
		} else {
			result = logicalCom(start, firstCom, com + i - 1);
			firstCom = com + i + 1;
			i++;
		}
	}
	return result;
}

void backProc(char **start, char **first, char **last)
{
	if (fork() == 0) {
		if (first != start)
			deleteEnv(start, first - 1, last + 1);
		else {
			free(last[1]);
			last[1] = NULL;
			deleteVector(last + 2);
		}
		if (fork() == 0){
			int fd = open("/dev/null", O_RDWR, 0666);
			dup2(fd, 1);
			dup2(fd, 0);
			signal(SIGINT, SIG_IGN);
			shellCom(start, first);
			deleteVector(first);
			free(start);	
			exit(0);	
		}
		deleteVector(first);
		free(start);	
		exit(0);
	}
	wait(NULL);
}

int logicalCom(char **start, char **first, char **last)
{
	int result, i = 0;
	char **supp = first;
	while (supp + i <= last){
		while ((supp + i <= last) && (strcmp(supp[i], "||") != 0) && (strcmp(supp[i], "&&") != 0) && (supp[i][0] != '(')) i++;
		if ((supp + i <= last) && (supp[i][0] == '(')){
			if (bracketLook(supp, last, &i) == -1) result = 1;
			else { 
				result = comman(start, first, supp + i);
				i++;
			}
		} else result = comman(start, first, supp + i - 1);
		if (supp + i - 1 != last){
			if (((result == 0) && (supp[i][0] == '|')) || ((result != 0) && (supp[i][0] == '&')))
				return 0;
		}
		first = supp + i + 1;
		i++;
	}
	return 0;
}

int bracketLook(char **supp, char **last, int *i)
{
	int count = 1, flag;
	(*i)++;
	sleep(2);
	if (last == NULL)
		flag = 1;
	else 
		flag = 0;
	while (1){
		if (supp[*i][0] == '(')
			count++;
		else if(supp[*i][0] == ')')
			count--;
		if (count == 0)
			break;
		else
			(*i)++;
		if (supp[*i] == NULL)
			break;
		if (!flag){
			if ((supp + (*i)) > last)
				break;
		}
	}
	if (count != 0){
		printf("Wrong write: brackets balance\n");
		return -1;
	}
	return 0;
}

int comman(char **start, char **com, char **last)
{
	if (com[0][0] == '('){
		pid_t pid;
		if ((pid = fork()) == -1){
			perror("Process creation error");
			return 1;
		} else if (pid == 0) {
			deleteEnv(start, com, last);
			int result = shellCom(start, com + 1);
			deleteVector(com + 1);
			free(start);
			exit(result);
		}
		int status;
		wait(&status);
		if (WIFEXITED(status))
			return WEXITSTATUS(status);
		else
			return 1;
	} else 
		return (conv(start, com, last));	
}

void deleteEnv(char **start, char **first, char **second)
{
	free(first[0]);
	first[0] = NULL;
	free(second[0]);
	second[0] = NULL;
	deleteVector(second + 1);
	deleteVector(start);
}

int conv(char **start, char **com, char **last)
{
	int amount = 0, status, input, output, fd[2], prev, strAmount = 0;
	int checkIO = checkingIO(com, last, &amount, &input, &output, &strAmount);
	if (checkIO == -1)
		return 1;
	int pids[amount];
	for (int i = 0; i < amount; i++){
		int count = 0;
		while ((com + strAmount + count <= last) && (com[strAmount + count][0] != '|') && (com[strAmount + count][0] != '<') && (com[strAmount + count][0] != '>')) count ++;
		char *args[count + 1];
		for (int j = 0; j < count; j++)
			args[j] = com[strAmount + j];
		args[count] = NULL;
		if (pipe(fd) == -1){
			perror("Pipe error");
			return 1;
		}
		if ((pids[i] = fork()) == -1){
			perror("Process creation error");
			return 1;
		} else if (pids[i] == 0) {
			if (i != 0)
				dup2(prev, 0);
			else if (checkIO / 10 == 1)
				dup2(input, 0);
			if (i != amount - 1)
				dup2(fd[1], 1);
			else if (checkIO % 10 == 1)
				dup2(output, 1);
			close(fd[0]);
			close(fd[1]);
			execvp(com[strAmount], args);
			printf("%s ", com[strAmount]);
			perror("has not executed");
			printf("\n");
			deleteVector(start);
			free(start);
			exit(1);
		}
		prev = fd[0];
		close(fd[1]);
		strAmount = strAmount + count + 1;
	}
	for (int i = 0; i < amount - 1; i++)
		waitpid(pids[i], NULL, 0);
	waitpid(pids[amount - 1], &status, 0);
	if (WIFEXITED(status))
		return WEXITSTATUS(status);
	else {
		perror("Conveyor error!");
		return 1;
	}
}

int checkingIO(char **com, char **last, int *amount, int *input, int *output, int *strAmount)
{
	int i = 0, checkIO = 0;
	for (int j = 0; j < 2; j++)
		if ((com[i][0] == '<') || (com[i][0] == '>'))
			i += 2;
	while ((com + i <= last) && (com[i][0] != '<') && (com[i][0] != '>'))
		if (com[i++][0] == '|')
			(*amount)++;
	(*amount)++;
	if (com + i > last)
		i = 0;
	for (int j = 0; j < 2; j++){
		if ((com + i <= last) && ((com[i][0] == '<') || (com[i][0] == '>'))){
			if (com[i][0] == '<'){
				if ((*input = open(com[i + 1], O_RDONLY, 0666)) == -1){
					perror ("File opening error");
					return -1;
				}
				checkIO += 10;
			} else if ((com[i][0] == '>') && (com[i][1] == '\0')){
				if ((*output = open(com[i + 1], O_WRONLY | O_APPEND, 0666)) == -1)
					if ((*output = open(com[i + 1], O_WRONLY | O_CREAT, 0666)) == -1){
						perror("File creation error");
						return -1;
					}
				checkIO++;
			} else {
				if ((*output = open(com[i + 1], O_WRONLY | O_TRUNC, 0666)) == -1){
					perror("File opening error");
					return -1;
				}
				checkIO++;
			}
			if ((i == 0) || ((i == 2) && (com + i + 2 <= last)))
				(*strAmount) += 2;
			i += 2;
		}
	}
	return checkIO;
}

char **parsing(void)
{
	char **commands = NULL;
	char sym;
	int i, amount = 0, lenght;
	while ((sym = getchar()) == ' ');
	if ((sym != EOF) && (sym != '\n'))
		commands = (char **)malloc(sizeof(char *));
	else
		return NULL;
	while ((sym != EOF) && (sym != '\n')){
		i = 0;
		lenght = BLOCK;
		commands[amount] = (char *)malloc(lenght * sizeof(char));
		while ((sym != EOF) && (sym != '\n') && (sym != ' ')){
			commands[amount][i++] = sym;
			if (i % BLOCK == 0){
				lenght += BLOCK;
            	commands[amount] = (char *)realloc(commands[amount], lenght);
			}
			sym = getchar(); 
			if ((ifSpecial(sym) && !ifSpecial(commands[amount][i - 1])) || 
				(!ifSpecial(sym) && ifSpecial(commands[amount][i - 1])) || 
				(commands[amount][i - 1] == '(') || (commands[amount][i - 1] == ')'))
				break;
		}
		if ((i % BLOCK) == 0)
        	commands[amount] = (char *)realloc(commands[amount], lenght + 1);
    	commands[amount++][i] = '\0';
    	commands = (char **)realloc(commands, (amount + 1) * sizeof(char *));
    	if (sym == ' ')
    		while ((sym = getchar()) == ' ');
	}
	commands[amount] = NULL;
	return commands;
}

void deleteVector(char **commands)
{
	if (commands == NULL)
		return;
	int i = 0;
	while (commands[i] != NULL){
		free(commands[i]);
		commands[i] = NULL;
		i++;
	}
}

int ifSpecial(char sym)
{
	if ((sym == '(') || (sym == ')') || (sym == '|') || (sym == '&') || 
		(sym == ';') || (sym == '>') || (sym == '<'))
		return 1;
	else 
		return 0;			
}
