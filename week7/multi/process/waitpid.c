//
// Created by 고승빈 on 2017. 10. 13..
//

#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>

int main()
{
    int status;
    pid_t pid = fork();

    if(pid == 0)
    {
        sleep(15);
        exit(15);
    }
    else
    {
        while(!waitpid(pid , &status , WNOHANG))
        {
            sleep(3);
            puts("sleep 3 sec\n");
        }

        if(WIFEXITED(status))
        {
            printf("Child send %d\n" , WEXITSTATUS(status));
        }
    }
    return 0;
}