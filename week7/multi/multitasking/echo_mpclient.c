//
// Created by 고승빈 on 2017. 10. 13..
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/wait.h>
#include <signal.h>
#include <arpa/inet.h>
#include <unistd.h>

#define BUF_SIZE    30

void error_handling(char * message);
void read_routine(int sock , char *buf);
void write_routine(int sock , char *buf);

int main(int argc , char ** argv)
{
    int serv_sock;
    int status;
    pid_t pid;
    char buf[BUF_SIZE];
    struct sockaddr_in serv_adr;

    if(argc != 3)
    {
        fprintf(stderr , "Usage: %s <ip> <port> \n" , argv[0]);
        exit(0);
    }

    serv_sock = socket(PF_INET , SOCK_STREAM , 0);

    memset(&serv_adr , 0x00 , sizeof(serv_adr));
    serv_adr.sin_family = AF_INET;
    serv_adr.sin_addr.s_addr = inet_addr(argv[1]);
    serv_adr.sin_port = htons(atoi(argv[2]));

    if(connect(serv_sock , (struct sockaddr *) &serv_adr , sizeof(serv_adr)) == -1)
    {
        error_handling("connect() error");
    } else{
        printf("connected....\n");
    }

    pid = fork();

    if(pid == 0)
    {
        write_routine(serv_sock , buf);
    } else
    {
        read_routine(serv_sock , buf);
        wait(&status);
        if(WIFEXITED(status))
        {
            puts("removed child...");
        }
    }

    close(serv_sock);
    return 0;
}

void error_handling(char * message)
{
    fputs(message , stderr);
    fputc('\n', stderr);
    exit(1);
}

void read_routine(int sock , char *buf)
{
    int str_len;
    while(1)
    {
        str_len = read(sock , buf , BUF_SIZE);

        if(str_len == 0)    return;

        buf[str_len] = 0;
        printf("Message from server: %s\n" , buf);
    }
}

void write_routine(int sock , char *buf)
{
    while(1)
    {
        fgets(buf , BUF_SIZE , stdin);
        if(strcmp(buf ,"q\n") == 0 || strcmp(buf ,"Q\n") == 0)
        {
            shutdown(sock , SHUT_WR);
            return;
        }

        write(sock , buf , strlen(buf));
    }
}