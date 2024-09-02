#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <signal.h>

int sigcount = 0;

void ft_clone_hndlr(int sig)
{
	(void)sig;
}

void ft_host_fork(int pid)
{
	char cmd[100];
	sprintf(cmd, "%s %d", __FILE__, pid);
}

void ft_host_hndlr(int sig)
{
	if (sig == SIGUSR1)
	{
		sigcount++;
		printf("Host: %d\n", sigcount);

		//fork
		ft_host_fork(getpid());
		if (sigcount > 10)
		{
			printf("10 forks reached, exiting...\n");
			exit(0);
		}
	}
}

int main(int argc, char** argv)
{
	struct sigaction act;

	if (argc > 2)
	{
		printf("Error: Too many arguments\n");
		return 1;
	}
	if (argc > 1)
	{
		int host = atoi(argv[1]);
		if (kill(host, SIGUSR1) == -1)
		{
			printf("Error: Invalid PID\n");
			return 1;
		}
		return 0;
	}
	else
	{
		printf("Host process initiated, PID: %d\n", getpid());
		act.sa_handler = ft_host_hndlr;
		sigemptyset(&act.sa_mask);
		act.sa_flags = 0;

		if (sigaction(SIGUSR1, &act, NULL) == -1)
		{
			perror("sigaction");
			return 1;
		}
		while (1)
		{
			pause();
		}
	}

}
