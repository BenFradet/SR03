#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<string.h>
#include<errno.h>	
#include<sys/types.h>
#include<sys/wait.h>

int main()
{
	int pipefds[2];
	int pid1, pid2;
	int status;

	if (pipe(pipefds) == -1)
	{
		perror("pipe");
		exit(1);
	}

	pid1 = fork();
	if (pid1 == -1){
		perror("Fork numero 1");
		exit(1);
	//fils1
	}else if(pid1 == 0){
		close(pipefds[0]);
		char line[60];
		FILE* file = fopen("input.txt", "r");
		while(fgets(line, sizeof(line), file))
		{
			char tmpLine[60];
			sprintf(tmpLine, "[%03d]%s", (int)strlen(line) - 1, line);
			write(pipefds[1], tmpLine, strlen(tmpLine));
		}
		close(pipefds[1]);
		return 0;
	} else {
		pid2 = fork();
		if(pid2 == -1){
			perror("Fork numero 2");
			exit(1);
		//fils2
		}else if(pid2 == 0){
			close(pipefds[1]);
			char buffer[20];
			
			int lineStartIndex = -1;
			int lineHeadDisplay = 1;
			int len;
			
			while((len = read(pipefds[0], buffer, sizeof(buffer))) > 0)
			{
				int i;
				for (i = 0; i < len; ++i)
				{
					//buffer contain the end line
					if(buffer[i] == '\n'){
						//buffer contain whole line
						if(lineStartIndex != 0)
							printf("recu>>>");
						int j = lineStartIndex;
						for(;j < i;j++)
							putchar(buffer[j]);
						printf("<<<\n");
						lineStartIndex = -1;
					}
					//In the middle of the line
					else if(lineStartIndex != -1 && i == 0){
						lineStartIndex = 0;
						lineHeadDisplay = 0;
					}
					//Ignore the first part of the line and mark the begin of the line
					else if(buffer[i] == ']' && lineStartIndex == -1){
						lineStartIndex = i + 1;
						lineHeadDisplay = printf;
					}

				}
				//Print letter in the case of the begin of line and in the middle of line
				if (lineStartIndex != -1 && lineStartIndex < len)
				{
					//We don't show this info in the middle of line	
					if (lineHeadDisplay == 1)
					{
						printf("recu>>>");
					}
					int j = lineStartIndex;
					for (; j < len; ++j)
					{
						putchar(buffer[j]);
					}
				}
			}
			close(pipefds[0]);
			return 0;
		} 
	}

	close(pipefds[0]);
	close(pipefds[1]);
	pid_t pid;
	while((pid = waitpid(-1, &status, WUNTRACED)) > 0){
		if(WIFEXITED(status))
		{
			printf("Processus %d termine avec code retour %d\n", pid, WEXITSTATUS(status));
		}
	}

	return 0;
}
