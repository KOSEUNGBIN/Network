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


int main(int argc , char* argv[] )
{

    int serv_sock;
    int clnt_sock;

    struct sockaddr_in serv_addr;
    struct sockaddr_in clnt_addr;
    socklen_t clnt_addr_size;

    char message[] = "Hello World!";

    if(argc != 2)
    {
        printf("Usage : %s <port>\n",argv[0]);
        exit(1);
    }
//  v4 , TCP로 socket을 생성
    serv_sock = socket(PF_INET, SOCK_STREAM, 0);

    if(serv_sock == -1) error_handling("socker() error");

//  서버 주소를 담는 구조체를 0으로 초기화
    memset(&serv_addr , 0 , sizeof(serv_addr));

//    v4
    serv_addr.sin_family = AF_INET;

//   서버 자신의 주소를 네트워크 바이트 주소로 변환
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);

//  응용 프로그램의 포트를 네트워크 바이트로 변환
    serv_addr.sin_port = htons(atoi(argv[1]));

//  서버의 주소 정보를 서버 소켓에 바인딩
    if(bind(serv_sock, (struct sockaddr*)& serv_addr, sizeof(serv_addr)) == -1) error_handling("bind() error");

//    클라이언트와 연결을 맺을 수 있는 5개 소켓 대기
    if(listen(serv_sock ,5) == -1)  error_handling("listen() error");

//  클라이언트에서 connect 가 오면 클라이언트 정보를 자동으로 클라이언트 소켓에 넣어줌
    clnt_addr_size = sizeof(clnt_addr);
    clnt_sock = accept(serv_sock, (struct sockaddr*)&clnt_addr, &clnt_addr_size);
    if(clnt_sock == -1) error_handling("appect() error");

//    통신
    write(clnt_sock , message, sizeof(message));

//    소켓 닫음
    close(clnt_sock);
    close(serv_sock);

    return 0;
}

void error_handling(char * message)
{
    fputs(message, stderr);
    fputc('\n',stderr);
    exit(1);
}