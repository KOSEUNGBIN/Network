//
// Created by 고승빈 on 2017. 10. 5..
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define BUF_SIZE 30
void error_handling(char * message);


int main(int argc , char ** argv)
{
    int sock;
    char message[BUF_SIZE];
    int str_len;
    socklen_t adr_sz;

    struct sockaddr_in serv_adr , from_adr;

    if(argc != 3)
    {
        fprintf(stderr , "Usage : %s <IP> , <PORT>\n", argv[0]);
        exit(1);
    }

    sock = socket(PF_INET , SOCK_DGRAM , 0);
    if(sock == -1)
    {
        fprintf(stderr , "socket() error\n");
        exit(1);
    }

    memset(&serv_adr , 0x00 , sizeof(serv_adr));
    serv_adr.sin_family = AF_INET;
    serv_adr.sin_addr.s_addr = inet_addr(argv[1]);
    serv_adr.sin_port = htons(atoi(argv[2]));

    /*
     * UDP 의 목적지 IP , PORT 정보를 등록만 하는것
     * */
    connect(sock , (struct sockaddr *) &serv_adr , sizeof(serv_adr));

    while(1)
    {
        fputs("Insert message(q to quit) : ", stdout);
        fgets(message , sizeof(message) , stdin);

        if(strcmp(message , "q\n") == 0 || strcmp(message , "Q\n") == 0)
        {
            break;
        }

        /*
         * connect를 이용하여 목적지 정보를 등록해두었기 때문에
         * sendto() , recvfrom() 를 이용한 목적지 정보 등록 과정을 안해도 됨
         * */
        write(sock , message , strlen(message));

        str_len = read(sock , message , sizeof(message) - 1);

        message[str_len] = 0;
        printf("Message from server %s\n", message);
    }

    close(sock);
    return 0;
}

void error_handling(char * message)
{
    fputs(message , stderr);
    fputc('\n', stderr);
    exit(1);
}