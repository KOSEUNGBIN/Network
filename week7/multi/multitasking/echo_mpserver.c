//
// Created by 고승빈 on 2017. 10. 13..
//

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <string.h>

#define BUF_SIZE 30

void error_handling(char * message);
void read_childproc(int sig);

int main(int argc , char ** argv)
{
    int serv_sock , clnt_sock;
    socklen_t clnt_adr_sz;
    struct sockaddr_in serv_adr , clnt_adr;
    struct sigaction act;

    pid_t pid;
    int str_len , status;
    char buf[BUF_SIZE];

    if(argc != 2)
    {
        fprintf(stderr , "%s <port> \n" , argv[0]);
        exit(1);
    }

    act.__sigaction_u.__sa_handler = read_childproc;
    act.sa_flags = 0;
    sigemptyset(&act.sa_mask);

    status = sigaction(SIGCHLD , &act , 0);

    serv_sock = socket(PF_INET , SOCK_STREAM , 0);

    memset(&serv_adr , 0x00 , sizeof(serv_adr));
    serv_adr.sin_family = AF_INET;
    serv_adr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_adr.sin_port = htons(atoi(argv[1]));

    if(bind(serv_sock , (struct sockaddr *) &serv_adr , sizeof(serv_adr)) == -1)
        error_handling("bind() error");

    if(listen(serv_sock , 5) == -1)
        error_handling("listen() error");

    while(1)
    {
        clnt_adr_sz = sizeof(clnt_adr);
        clnt_sock = accept(serv_sock , (struct sockaddr *) &clnt_adr , &clnt_adr_sz);

        if(clnt_sock == -1) continue;
        else                puts("new client connected...");

        pid = fork();

        if(pid == 1)
        {
            close(clnt_sock);
            continue;
        }

        if(pid == 0)
        {
            /*
             * 자식 프로세스가 생성되면, server socket descriptor가 하나 더 open이 된 상태이므로 닫아버린다.
             *
             * */
            close(serv_sock);
            while((str_len = read(clnt_sock , buf , BUF_SIZE)) > 0)
            {
                write(clnt_sock , buf , str_len);
            }

            close(clnt_sock);
            puts("client disconnected...");
        } else{
            /*
             * 부모 프로세스에서는 client socket이 필요없으므로, 닫아버림,
             *
             * */
            close(clnt_sock);
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

void read_childproc(int sig)
{
    int status;
    pid_t pid = waitpid(-1 , &status , WNOHANG);

    if(WIFEXITED(status))
    {
        printf("removed proc id: %d" , pid);
    }
}