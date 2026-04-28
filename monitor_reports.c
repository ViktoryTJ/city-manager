#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <fcntl.h>
#include <unistd.h>

#define PID_FILE ".monitor_pid"

//report added
void handle_sigusr1(int sig)
{
    write(1, "New report added\n",17);
}

//program stopped (ctrl+c)

void handle_sigint(int sig)
{
    write(1, "Monitor shutting down\n",23);
    unlink(PID_FILE);
    exit(0);
}

int main() {
    int fd = open (PID_FILE, O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fd < 0 )
    {
        perror ("open failed ");
        return 1;
    }
    char buf[32];
    int len = sprintf(buf, "%d\n", getpid());
    write(fd, buf, len);
    close(fd);

    //----------------------SIGNALS----------------------
    struct sigaction sa1;
    sa1.sa_handler = handle_sigusr1;
    sigemptyset(&sa1.sa_mask);
    sa1.sa_flags = 0;
    sigaction(SIGUSR1, &sa1, NULL);

    struct sigaction sa2;
    sa2.sa_handler = handle_sigint;
    sigemptyset(&sa2.sa_mask);
    sa2.sa_flags = 0;
    sigaction(SIGINT, &sa2, NULL);

    while (1)
    {
        pause();
    }
    return 0;
}