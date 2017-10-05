//
// Created by 고승빈 on 2017. 10. 4..
//

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <string.h>

#define BUFSIZE 2048

int main(int argc , char ** argv)
{
    int serv_sock , clnt_sock;
    socklen_t clnt_addr_size;
    int send_len , send_cnt , receieve_len , receieve_cnt;
    int i , result;
    int * value;

    struct sockaddr_in serv_addr , clnt_addr;
    char buffer[BUFSIZE];
    char operator;

    if(argc != 2)
    {
        fprintf(stderr , "Usage : ./op_server port\n");
        exit(1);
    }

    serv_sock = socket(PF_INET , SOCK_STREAM , 0);

    if(serv_sock == -1)
    {
        fprintf(stderr , "Socket Create Error\n");
        exit(1);
    }

    memset(&serv_addr , 0x00 , sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = inet_addr(argv[1]);
    serv_addr.sin_port = htons(atoi(argv[2]));

    if(bind(serv_sock , (struct sockaddr *) &serv_addr , sizeof(serv_addr)) == -1)
    {
        fprintf(stderr , "Socket Bind Error\n");
        exit(1);
    }

    if(listen(serv_sock , 5) == -1)
    {
        fprintf(stderr , "Socket listen Error\n");
        exit(1);
    }

    clnt_addr_size = sizeof(clnt_addr);

    if((clnt_sock = accept(serv_sock , (struct sockaddr *) &clnt_addr , &clnt_addr_size)) == -1)
    {
        fprintf(stderr , "Socket accept Error\n");
        exit(1);
    }

    receieve_len = 0;

    while((receieve_cnt = read(clnt_sock , &buffer[receieve_len] , BUFSIZE)) > 0)
    {
        receieve_len += receieve_cnt;
    }

    receieve_len /= 4;
    value = (int *) malloc(receieve_len * sizeof(int));

    for(i = 0; i < receieve_len - 1 ; ++i)
    {
        value[i] = (int) buffer[ i * 4 ];
    }

    operator = buffer[ i * 4 ];
    result = 0;

    for(i = 0; i < receieve_len - 1 ; ++i)
    {
        switch (operator)
        {
            case '+':
                result += value[i];
                break;
            case '-':
                result -= value[i];
                break;
            case '*':
                result *= value[i];
                break;
            case '/':
                result /= value[i];
                break;
        }
    }

    write(clnt_sock , &result , sizeof(result));

    close(clnt_sock);


}