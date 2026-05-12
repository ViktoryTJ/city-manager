#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/wait.h>

int main()
{
    char command[256];

    while (1)
    {
        printf("city_hub> ");
        fflush(stdout);

        if (fgets(command, sizeof(command), stdin) == NULL)
        {
            break;
        }

        command[strcspn(command, "\n")] = '\0';

        if (strcmp(command, "exit") == 0)
        {
            break;
        }

        else if (strcmp(command, "start_monitor") == 0)
        {
            int fd[2];

            pipe(fd);

            pid_t pid = fork();

            if (pid == 0)
            {
                close(fd[0]);

                dup2(fd[1], STDOUT_FILENO);

                close(fd[1]);

                execl("./monitor", "monitor", NULL);

                perror("exec failed");
                exit(1);
            }
            else
            {
                close(fd[1]);

                char buffer[256];

                int n = read(fd[0], buffer, sizeof(buffer) - 1);

                if (n > 0)
                {
                    buffer[n] = '\0';

                    printf("%s", buffer);
                }

                close(fd[0]);
            }
        }

        else if (strncmp(command, "calculate_score ", 16) == 0)
        {
            char district[64];

            sscanf(command + 16, "%s", district);

            int fd[2];

            pipe(fd);

            pid_t pid = fork();

            if (pid == 0)
            {
                close(fd[0]);

                dup2(fd[1], STDOUT_FILENO);

                close(fd[1]);

                execl("./scorer", "scorer", district, NULL);

                perror("exec failed");
                exit(1);
            }
            else
            {
                close(fd[1]);

                char buffer[512];

                int n = read(fd[0], buffer, sizeof(buffer) - 1);

                if (n > 0)
                {
                    buffer[n] = '\0';

                    printf("%s", buffer);
                }

                close(fd[0]);
            }
        }

        else
        {
            printf("Unknown command \n");
        }
    }

    return 0;
}