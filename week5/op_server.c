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
#define OPSZ    4

int calculate(int opnum , int opnds[] , char operator);

int main(int argc , char ** argv)
{
    int serv_sock , clnt_sock;
    int result , opnd_cnt , i;
    int recv_cnt , recv_len;

    char opinfo[BUFSIZE];

    socklen_t clnt_addr_size;

    struct sockaddr_in serv_addr , clnt_addr;


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
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_addr.sin_port = htons(atoi(argv[1]));

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


    for(i = 0 ; i < 5 ; i++)
    {
        opnd_cnt = 0;
        clnt_sock = accept(serv_sock , (struct sockaddr *) &clnt_addr , &clnt_addr_size);
        read(clnt_sock , &opnd_cnt , 1);

        recv_len = 0;

        while((opnd_cnt * OPSZ + 1) > recv_len)
        {
            recv_cnt = read(clnt_sock , &opinfo[recv_len] , BUFSIZE - 1);
            recv_len += recv_cnt;
        }

        result = calculate(opnd_cnt , (int *) opinfo , opinfo[recv_len - 1]);
        write(clnt_sock , (char *)&result , sizeof(result));
        close(clnt_sock);
    }



    close(clnt_sock);

    return 0;
}

int calculate(int opnum , int opnds[] , char operator)
{
    int result = opnds[0] , i;

    switch(operator)
    {
        case '+':
            for(i = 1 ; i < opnum ; ++i) result += opnds[i];
            break;
        case '-':
            for(i = 1 ; i < opnum ; ++i) result -= opnds[i];
            break;
        case '*':
            for(i = 1 ; i < opnum ; ++i) result *= opnds[i];
            break;
        case '/':
            for(i = 1 ; i < opnum ; ++i) result /= opnds[i];
            break;
    }

    return result;
}