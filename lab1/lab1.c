#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>

int main (int argc, char *argv[]) {

    if (argc != 2) {
        printf("Usage: ./a.out filename\n");
        exit(1);
    }

    if (strcmp(argv[1], "--help") == 0) {
        printf("Usage: ./a.out filename\n");
        exit(0);
    }

    struct stat st; 
    if (stat(argv[1], &st) == 0) {
        printf("filename already exists\n");
        exit(1);
    }

    int fd = open(argv[1], O_WRONLY | O_CREAT, 0644);
    if (fd == -1) {
        perror("Error opening file");
        exit(1);
    }

    int status;
    pid_t child;

    child = fork();

    if (child<0) {
        perror("Error creating child process");
        close(fd);
        exit(1);    
    }

    if (child==0) {
        //printf("[CHILD] getpid() = %d, getppid() = %d\n", getpid(), getppid());
        char buffer[256];
        int len = snprintf(buffer, sizeof(buffer), "[CHILD] getpid()= %d, getppid()= %d\n", getpid(), getppid());

        if (write(fd, buffer, len) != len) {
            perror("[CHILD] Error write");
            close(fd);
            exit(1);
        }

        close(fd);
        exit(0);

    }

    else{
        char buffer[256];
        int len = snprintf(buffer, sizeof(buffer), "[PARENT] getpid()= %d, getppid()= %d\n", getpid(), getppid());
  
        if (write(fd, buffer, len) != len) {
            perror("[PARENT] Error writing to file");
            close(fd);
            exit(1);
        }

        if (wait(&status) == -1) {
            perror("[PARENT] Error waiting for child");
            exit(1);
        }

        close(fd);
        exit(0);
    }

return 0;
}