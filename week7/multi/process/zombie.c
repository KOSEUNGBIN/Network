//
// Created by 고승빈 on 2017. 10. 13..
//

#include <stdio.h>
#include <unistd.h>

int main(int argc , char ** argv)
{
    pid_t pid;


    pid = fork();
    if(pid == 0)    printf("Hi, I am a child process  \n");
    else
    {
        printf("Child Process ID: %d \n", pid);
        sleep(30);
    }

    if(pid == 0)    puts( "End child process\n");
    else            puts( "End parent process\n");

    return 0;
}