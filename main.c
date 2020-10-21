#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>

#define QUIZ_TIME 30
#define INTERVAL 10
int rem_time = 30;

int start_quiz();

int main()
{
    int status;
    pid_t quiz;
    quiz = fork();

    if (quiz < 0)
    {
        perror("fork");
        return EXIT_FAILURE;
    }
    else if (quiz == 0)
    {
        start_quiz();
    }
    else
    {
        char *remain_time;

        for (int i = 0; i < QUIZ_TIME / 10; i++)
        {
            if (waitpid(quiz, &status, WNOHANG) != quiz)
            {
                sprintf(remain_time, "Time remaining: %d seconds\n", rem_time);
                write(STDOUT_FILENO, remain_time, strlen(remain_time));
                rem_time -= 10;
                sleep(INTERVAL);
            }
        }

        int proc_exists = kill(quiz, 0);
        if (proc_exists < 0)
        {
            printf("Child %d exited with status %d\n", quiz, status);
            return 0;
        }

        int rc = kill(quiz, SIGKILL);
        if (rc < 0)
        {
            perror("kill");
            return EXIT_FAILURE;
        }
        wait(&status);
        if (WIFEXITED(status) || WIFSIGNALED(status))
        {
            printf("Child %d killed with status %d\n", quiz, status);
        }
    }

    return 0;
}

int start_quiz()
{
    char *question;

    for (int i = 0; i < 5; i++)
    {
        sprintf(question, "Question %d\n", i + 1);
        write(STDOUT_FILENO, question, strlen(question));
        char *buf[1024];
        int bytes = read(STDIN_FILENO, (void *)&buf, 1024);
    }
    exit(0);
}