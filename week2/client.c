//
// Created by 고승빈 on 2017. 9. 11..
//

//
// Created by 고승빈 on 2017. 9. 8..
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

void error_handling(char * message);


int main(int argc , char* argv[] ) {


    int sock;
    struct sockaddr_in  serv_addr;
    char message[30];
    int str_len;

    if(argc != 3)
    {
        printf("Usage : %s <IP> <PORT>\n",argv[0]);
        exit(1);
    }
//  소켓 생성
    sock = socket(PF_INET , SOCK_STREAM , 0);
    if(sock == -1)  error_handling("socket() error");

//    server의 정보를 network 스트림을 변경
    memset(&serv_addr , 0 , sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = inet_addr(argv[1]);
    serv_addr.sin_port = htons(atoi(argv[2]));

//    server 에 SYN 요청 (blocking)
    if(connect(sock, (struct sockaddr*)&serv_addr , sizeof(serv_addr)) == -1)   error_handling("connet error!");

    str_len = read(sock , message, sizeof(message) -1);
    if(str_len == -1) error_handling("read() error");
//
    printf("Message from server : %s \n",message);
    close(sock);


    return 0;
}

void error_handling(char * message)
{
    fputs(message, stderr);
    fputc('\n',stderr);
    exit(1);
}