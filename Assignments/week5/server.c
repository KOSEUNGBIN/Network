//
// Created by 고승빈 on 2017. 9. 22..
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define BUF_SIZE 1024
void error_handling(char * message);

int main(int argc , char* argv[])
{
    /* 선언부 */
    int serv_sock , clnt_sock;
    int i;
    char message[BUF_SIZE];
    struct sockaddr_in serv_adr , clnt_adr;
    socklen_t  clnt_adr_sz;
    ssize_t str_len;

    if(argc != 2){
        printf("Usage : %s <port>\n", argv[0]);
        exit(1);
    }

    /*
     * TCP Socket 생성
     *
     * */
    if((serv_sock = socket(PF_INET , SOCK_STREAM , 0)) == -1)
    {
        error_handling("socket() error");
    }

    /*
     * socket address에 주소 정보 초기화 ( 주소체계 - v4 , IP , PORT 초기화 )
     * 네트워크 전송시, 빅엔디안 방식으로 스트림을 전송하므로 네트워크 바이트 순서로 변환함
     * INADDR_ANY : 서버 IP를 자동으로 할당하는 변수
     * 서버 주소를 담는 구조체를 0으로 초기화
     * */
    memset(&serv_adr , 0 , sizeof(serv_adr));
    serv_adr.sin_family = AF_INET;
    serv_adr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_adr.sin_port = htons(atoi(argv[1]));

    /*
     * 초기화된 주소 정보를 socket에 할당
     *
     * */
    if(bind(serv_sock , (struct sockaddr*) &serv_adr , sizeof(serv_adr)) == -1)
    {
        error_handling("bind() error");
    }

    /*
     * 크기가 5인 연결 요청 대기 큐 생성
     * 연결요청 대기 큐에 삽입되면, 서버 소켓이 됨
     * 연결 요청 대기 큐에 삽입되면, client는 connect 성공으로 받아들임
     * */
    if(listen(serv_sock , 5) == -1)
    {
        error_handling("listen() error");
    }

    clnt_adr_sz = sizeof(clnt_adr);

    for(i = 0 ; i < 5; i++)
    {
        /*
         * 연결요청 수락
         * 연결요청 대기 큐에서 대기 중인 client socket descriptor 하나를 꺼내와 연결요청을 수락
         * client socket 하나를 생성하고 정보를 자동으로 주입함
         * client 와 연결됨
         * */
        clnt_sock = accept(serv_sock, (struct sockaddr*) &clnt_adr , &clnt_adr_sz);

        if(clnt_sock == -1)
        {
            error_handling("accept() error");
        }
        else{

            /*
             * 주입받은 client 정보 출력
             * 네트워크 바이트 순서를 호스트 바이트 순서로 바꿔서 출력
             * */
            printf("Connected client : %s %d \n" , inet_ntoa(clnt_adr.sin_addr) , ntohs(clnt_adr.sin_port));
            printf("Connected client %d \n" , i + 1);
        }


        /*
         * 연결된 client로 부터 데이터를 입력받음
         * client로 데이터 전송
         * TCP는 데이터의 경계가 없어, 입력을 여러번에 나눠서 받을 수 있음
         * */
        while((str_len = read(clnt_sock , message , BUF_SIZE)) != 0)
        {
            write(clnt_sock , message , str_len);
        }

        /*
         * client 소켓 종료
         * client에게 연결 종료를 전달
         * */
        close(clnt_sock);
    }

    /*
     * server 소켓 종료
     * */
    close(serv_sock);
    return 0;
}

void error_handling(char * message)
{
    fputs(message , stderr);
    fputc('\n' , stderr);
    exit(1);
}