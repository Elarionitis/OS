#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <signal.h>

// Signal handler for SIGINT
void sigint_handler(int sig) {
    printf("\nSIGINT received. Exiting safely...\n");
    while (wait(NULL) > 0); 
    exit(0);
}

int main(int argc, char *argv[]) {
    signal(SIGINT, sigint_handler); // Catch Ctrl+C

    for (int i = 1; i < argc; i++) {
        if (fork() == 0) { // Child process
            int fd_in = open(argv[i], O_RDONLY);
            if (fd_in < 0) exit(1);

            int chars = 0, lines = 0, words = 0;
            int alpha[26] = {0};
            char buf[1024];
            int n, in_word = 0;

            // Read using low-level system call
            while ((n = read(fd_in, buf, sizeof(buf))) > 0) {
                for (int j = 0; j < n; j++) {
                    chars++;
                    if (buf[j] == '\n') lines++;
                    
                    if (buf[j] == ' ' || buf[j] == '\n' || buf[j] == '\t') {
                        in_word = 0;
                    } else if (in_word == 0) {
                        in_word = 1;
                        words++;
                    }

                    if (buf[j] >= 'a' && buf[j] <= 'z') alpha[buf[j] - 'a']++;
                    if (buf[j] >= 'A' && buf[j] <= 'Z') alpha[buf[j] - 'A']++;
                }
            }
            close(fd_in);

            // Write stats using low-level system call
            char out_name[50];
            sprintf(out_name, "stats_%s", argv[i]);
            int fd_out = open(out_name, O_WRONLY | O_CREAT | O_TRUNC, 0644);
            
            char out_buf[512];
            int len = sprintf(out_buf, "File: %s\nChars: %d\nLines: %d\nWords: %d\n", argv[i], chars, lines, words);
            write(fd_out, out_buf, len);
            
            for (int k = 0; k < 26; k++) {
                if (alpha[k] > 0) {
                    len = sprintf(out_buf, "%c: %d\n", 'a' + k, alpha[k]);
                    write(fd_out, out_buf, len);
                }
            }
            close(fd_out);
            exit(0);
        }
    }

    // Parent waits for all children
    while (wait(NULL) > 0);

    // Parent creates summary report
    int fd_summary = open("summary_report.txt", O_WRONLY | O_CREAT | O_TRUNC, 0644);
    for (int i = 1; i < argc; i++) {
        char stats_name[50], buf[1024];
        sprintf(stats_name, "stats_%s", argv[i]);
        int fd_in = open(stats_name, O_RDONLY);
        int n;
        while ((n = read(fd_in, buf, sizeof(buf))) > 0) {
            write(fd_summary, buf, n);
            write(fd_summary, "\n", 1);
        }
        close(fd_in);
    }
    close(fd_summary);

    return 0;
}