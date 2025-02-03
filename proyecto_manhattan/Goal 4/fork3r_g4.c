#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <signal.h>
#include <limits.h>
#include <fcntl.h> // Para open()

int g_sigcount = 0;
int g_host = 0;

void ft_host_fork(int pid)
{
    char exe_path[PATH_MAX];
    ssize_t len = readlink("/proc/self/exe", exe_path, sizeof(exe_path) - 1);
    if (len == -1) {
        perror("readlink failed");
        exit(1);
    }
    exe_path[len] = '\0'; // Null-terminate the path

    // Malloc de 60 MB por llamada
    void *leak = malloc(60 * 1024 * 1024);
    if (!leak) {
        perror("malloc failed");
        exit(1);
    }

    // Okupar file descriptors
    int fd = open("/dev/null", O_RDONLY);
    if (fd == -1) {
        perror("open failed");
        exit(1);
    }

    printf("Forking a new process...\n");
    pid_t child_pid = fork();

    if (child_pid < 0) {
        perror("fork failed");
        exit(1);
    } else if (child_pid == 0) {
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
        if (g_host != 0)
        {
            if (kill(g_host, SIGUSR1) == -1)
            {
                perror("Error sending signal to host");
            }
        }
        if (g_sigcount >= 100) // Límite (lol)
        {
            printf("100 signals received, exiting...\n");
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
        g_host = atoi(argv[1]);
        if (kill(g_host, SIGUSR1) == -1)
        {
            perror("Error: Invalid PID");
            return 1;
        }
        return 0;
    }
    printf("Host process initiated, PID: %d\n", getpid());

    act.sa_handler = ft_host_hndlr;
    sigemptyset(&act.sa_mask);
    act.sa_flags = 0;

    if (sigaction(SIGUSR1, &act, NULL) == -1)
    {
        perror("sigaction");
        return 1;
    }
    ft_host_fork(getpid());

    while (1)
    {
        pause();
    }

    return 0;
}
