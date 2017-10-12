//
// Created by 고승빈 on 2017. 10. 13..
//

#include <stdio.h>
#include <unistd.h>
#include <signal.h>

void timeout(int sig)
{
    if(sig == SIGALRM)
        puts("Time out!\n");
    alarm(2);
}

void keycontrol(int sig)
{
    if(sig == SIGINT)
        puts("CTRL+C pressed\n");
}

int main()
{
    int i;
    signal(SIGALRM , timeout);
    signal(SIGINT , keycontrol);
    alarm(2);

    for(i = 0; i < 3 ; ++i)
    {
        puts("wait....");
        sleep(100);
    }

    return 0;
}