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

int main(int argc , char * argv[]){

    /* 선언부 */
    int sock;
    char message[BUF_SIZE];
    ssize_t str_len , recv_len , recv_cnt;
    struct sockaddr_in serv_adr;

    if(argc != 3){
        printf("Usage : %s <IP> <port> \n" , argv[0]);
        exit(1);
    }

    /* TCP 소켓 생성 */
    sock = socket(PF_INET , SOCK_STREAM , 0);
    if(sock == -1)  error_handling("socket() error");

    /*
     * socket address에 주소 정보 초기화 ( 주소체계 - v4 , IP , PORT 초기화 )
     * 네트워크 전송시, 빅엔디안 방식으로 스트림을 전송하므로 네트워크 바이트 순서로 변환함
     * */
    memset(&serv_adr , 0 , sizeof(serv_adr));
    serv_adr.sin_family = AF_INET;
    serv_adr.sin_addr.s_addr = inet_addr(argv[1]);
    serv_adr.sin_port = htons(atoi(argv[2]));


    /*
     * server 에 연결요청을 보냄
     * server 의 연결요청 대기 큐에 삽입이 되면 성공
     * socket 에 서버의 주소정보를 자동으로 등록함
     * */
    if(connect(sock , (struct sockaddr*) &serv_adr , sizeof(serv_adr)) == -1)
    {
        error_handling("connet() error");
    }
    else{
        puts("connected......");
    }

    while(1)
    {
        fputs("Input message(Q to quit): ", stdout);
        fgets(message , BUF_SIZE , stdin);

        if(!strcmp(message , "q\n") || !strcmp(message , "Q\n"))    break;

        /*
         * server 에 데이터 전송
         * */
        str_len = write(sock , message , strlen(message));

        recv_len = 0;

        /*
         * 데이터의 경계가 없기 때문에, 보낸 데이터 크기만큼 될 때까지 반복하여 데이터를 읽음
         *
         * */
        while(recv_len < str_len)
        {
            recv_cnt = read(sock , &message[recv_len] , BUF_SIZE - 1);
            if(recv_cnt == -1)  error_handling("read() error!");
            recv_len += recv_cnt;
        }

        /* null pointer */
        message[recv_len] = 0;
        printf("Message from server : %s" , message);
    }

    /* 소켓 종료 */
    close(sock);
    return 0;
}

void error_handling(char * message)
{
    fputs(message , stderr);
    fputc('\n', stderr);
    exit(1);
}