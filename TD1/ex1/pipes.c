#include<stdio.h>
#include<unistd.h>
#include<string.h>
#include<errno.h>
#include<sys/types.h>
#include<sys/wait.h>

void main()
{
	int pipefd[2];
	int pid1, pid2;
	int status;
	
	pid1 = fork();
	//child 1
	switch(pid1)
	{
		case -1:
			perror("fork numero 1");
			break;
		//child1
		case 0:
			close(pipefd[0]);
			FILE* file;
			char line[60];
			file = fopen("input.txt", "r");
			while(fgets(line, sizeof(line), file))
			{
				char tmpLine[60];
				sprintf(tmpLine, "[%03d][%s]", strlen(line) - 1, line);
				write(pipefd[1], tmpLine, strlen(tmpLine));
			}
			close(pipefd[1]);
			break;
		default:
			pid2 = fork();
			switch(pid2)
			{
				case -1:
					perror("fork numero 2");
					break;
				//child2
				case 0:
					close(pipefd[1]);
					char tmpBuffer[200];
					char buffer[20];
					while(read(pipefd[0], buffer, sizeof(buffer)) > 0)
					{
						strcat(tmpBuffer, buffer);
					}
					close(pipefd[0]);
				//parent
				default:
					close(pipefd[0]);
					close(pipefd[1]);
					waitpid(pid1, &status, WUNTRACED);
					waitpid(pid2, &status, WUNTRACED);
					/*switch(waitpid(pid1, &status, WUNTRACED))
					{
						case (pid_t)-1:
							perror("waitpid fils 1");
							break;
						default:
							if(WIFEXITED(status))
							{
								printf("code retour %d \n", WEXITSTATUS(status));
							}
							break;
					}
					switch(waitpid(pid2, &status, WUNTRACED))
					{
						case (pid_t)-1:
							perror("waitpid fils 2");
							break;
						default:
							if(WIFEXITED(status))
							{
								printf("code retour %d \n", WEXITSTATUS(status));
							}
							break;
					}*/
					break;
			}
			break;
	}
}
