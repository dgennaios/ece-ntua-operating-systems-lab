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

    // χωρίς όρισμα ή πάνω από 2 ορίσματα
    if (argc != 2) {
        printf("Usage: ./a.out filename\n");
        exit(1);
    }

    
    if (strcmp(argv[1], "--help") == 0) { //int strcmp(const char *s1, const char *s2);
        printf("Usage: ./a.out filename\n");
        exit(0);
    }

    
    struct stat st; //int stat(const char *restrict path, struct stat *restrict buf);
    if (stat(argv[1], &st) == 0) {
        printf("filename already exists\n");
        exit(1);
    }
    

    
    int fd = open(argv[1], O_WRONLY | O_CREAT, 0644); //int open(const char *path, int oflag, mode_t mode);
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
        char buffer[256];
        //int snprintf(char *str, size_t size, const char *format, ...);
        int len = snprintf(buffer, sizeof(buffer), "[CHILD] getpid()= %d, getppid()= %d\n", getpid(), getppid());

        ssize_t bytes_written = write(fd, buffer, len);

        if (bytes_written == -1) {
            perror("[CHILD] Error write");
            close(fd);
            exit(1);
        }

        if (bytes_written != len) {
            perror("[CHILD] Error write");
            close(fd);
            exit(1);
        }

        close(fd);
        exit(0);

    }

    else{

        if (wait(&status) == -1) { //pid_t wait(int *stat_loc);
            perror("[PARENT] Error waiting for child");
            exit(1);
        }

        char buffer[256];
        int len = snprintf(buffer, sizeof(buffer), "[PARENT] getpid()= %d, getppid()= %d\n", getpid(), getppid());
  
        ssize_t bytes_written = write(fd, buffer, len);

        if (bytes_written == -1) {
            perror("[PARENT] Error write");
            close(fd);
            exit(1);
        }

        if (bytes_written != len) {
            perror("[PARENT] Error write");
            close(fd);
            exit(1);
        }
        
        close(fd);
        exit(0);
    }

return 0;
}