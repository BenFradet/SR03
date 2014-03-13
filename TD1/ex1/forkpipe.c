#include<stdio.h>
#include<stdbool.h>
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
	if (pid1 == -1)
    {
		perror("Fork numero 1");
		exit(1);
	//fils1
	}
    else if(pid1 == 0)
    {
		close(pipefds[0]);
		char line[60];
		FILE* file = fopen("input.txt", "r");
		while(fgets(line, sizeof(line), file))
		{
			char tmpLine[(int)strlen(line) + 5];
			sprintf(tmpLine, "[%03d]%s", (int)strlen(line) - 1, line);
			write(pipefds[1], tmpLine, strlen(tmpLine));
		}
		close(pipefds[1]);
		return 0;
	} 
    else 
    {
		pid2 = fork();
		if(pid2 == -1)
        {
			perror("Fork numero 2");
			exit(1);
		//fils2
		}
        else if(pid2 == 0)
        {
			close(pipefds[1]);
			char buffer[20];
			
			int len;
            bool newLine = true;
            bool startLength = false;
            int lengthCurrent = 0;
			
			while((len = read(pipefds[0], buffer, sizeof(buffer))) > 0)
			{
				int i;
				for (i = 0; i < len; ++i)
				{
                    if(startLength)
                    {
                        if(lengthCurrent < 3)
                        {
                            lengthCurrent++;
                        }
                        else
                        {
                            startLength = false;
                            lengthCurrent = 0;
                        }
                    }
                    else if(buffer[i] == '\n')
                    {
                        printf("<<<\n");
                        newLine = true;
                    }
                    else if(newLine)
                    {
                        newLine = false;
                        printf("recu>>>");
                        startLength = true;
                    }
                    else
                    {
                        putchar(buffer[i]);
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
