#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <signal.h>
#include <limits.h>

int g_sigcount = 0;

void ft_host_fork(int pid)
{
    char exe_path[PATH_MAX];
    ssize_t len = readlink("/proc/self/exe", exe_path, sizeof(exe_path) - 1);
    if (len == -1) {
        perror("readlink failed");
        exit(1);
    }
    exe_path[len] = '\0'; // Null-terminate the path, causes errors otherwise
    printf("Forking a new process...\n");
    pid_t child_pid = fork();

    if (child_pid < 0) {
        perror("fork failed");
        exit(1);
    } else if (child_pid == 0) {
        // Child process code
        char pid_str[20];
        snprintf(pid_str, sizeof(pid_str), "%d", pid);
        char *args[] = { exe_path, pid_str, NULL };
        execv(exe_path, args);

        perror("execv fail");
        exit(1);
    }
}

void ft_host_hndlr(int sig)
{
    if (sig == SIGUSR1)
    {
        printf("Signal received\n");
        g_sigcount++;
        printf("Host: %d\n", g_sigcount);

        // Check if n >= 10 to not go over the test set limit
		if (g_sigcount >= 10)
        {
            printf("10 signals received, exiting...\n");
            exit(0);
        }
        ft_host_fork(getpid());
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
            perror("Error: Invalid PID");
            return 1;
        }
        return 0;
    }
    else
    {
        printf("Host process initiated, PID: %d\n", getpid());

        // Setup signal handler
        act.sa_handler = ft_host_hndlr;
        sigemptyset(&act.sa_mask);
        act.sa_flags = 0;

        if (sigaction(SIGUSR1, &act, NULL) == -1)
        {
            perror("sigaction");
            return 1;
        }
		//Initial call to fork to start chain
        ft_host_fork(getpid());

        while (1)
        {
            pause();
        }
    }

    return 0;
}
