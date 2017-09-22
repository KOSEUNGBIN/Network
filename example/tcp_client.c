//
// Created by 고승빈 on 2017. 9. 22..
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

void error_handling(char *message);

int main(int argc , char* argv[])
{
    int sock;
    struct sockaddr_in serv_addr;
    char message[30];
    int str_len = 0;
    int idx = 0, read_len = 0;

    if(argc != 3)
    {
        printf("Usage : %s <IP> <port>\n", argv[0]);
        exit(1);
    }

    /*
     * IPv4 프로토콜 체계
     * 연결 지향형 타입
     * TCP 프로토콜
     * */
    sock = socket(PF_INET , SOCK_STREAM , IPPROTO_TCP);
    if(sock == -1)  error_handling("socket() error");

    /*
     * server 정보 삽입
     * */
    memset(&serv_addr , 0 , sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = inet_addr(argv[1]);
    serv_addr.sin_port = htons(atoi(argv[2]));

    /*
     * connect - server 에 연결요청 (SYN)
     * */
    if(connect(sock , (struct sockaddr*) &serv_addr, sizeof(serv_addr)) == -1)
    {
        error_handling("connect() error");
    }

    /*
     * data 전송
     * */
    while((read_len = read(sock, &message[idx++] , 1)))
    {
        if(read_len == -1)
        {
            error_handling("read() error");
        }

        str_len += read_len;
    }

    printf("Message from server: %s \n", message);
    printf("Function read call count: %d \n", str_len);

    /*
     * close socket
     * */
    close(sock);
    return 0;


}


void error_handling(char *message)
{
    fputs(message, stderr);
    fputc('\n',stderr);
    exit(1);
}
