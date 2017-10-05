//
// Created by 고승빈 on 2017. 10. 4..
//

#include <stdio.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <string.h>
#include <unistd.h>

#define BUFFSIZE    2048
#define OPSZ    4
#define RLT_SIZE    4

int main(int argc , char ** argv)
{
    int serv_sock;
    int result , opnd_cnt , i;
    char opmsg[BUFFSIZE];
    struct sockaddr_in servaddr;
    int send_len , receieve_len , receieve_cnt;

    if(argc != 3)
    {
        fprintf(stderr , "Usage : ./op_client ip port\n");
        exit(1);
    }

    serv_sock = socket(PF_INET , SOCK_STREAM , 0);
    if(serv_sock == -1)
    {
        fprintf(stderr , "Socket Create Error\n");
        exit(1);
    }

    memset(&servaddr , 0 , sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = inet_addr(argv[1]);
    servaddr.sin_port = htons(atoi(argv[2]));


    if(connect(serv_sock , (struct sockaddr *) &servaddr , sizeof(servaddr)) == -1)
    {
        fprintf(stderr , "Connection Error\n");
        exit(1);
    }
    else
        printf("Connected .......\n");


    fputs("Input Operand Count : " , stdout);
    scanf("%d" , &opnd_cnt);
    opmsg[0] = (char) opnd_cnt;

    for(i = 0; i < opnd_cnt ; ++i)
    {
        printf("Operand %d : " , i + 1);
        scanf("%d" , (int *) &opmsg[ i * OPSZ + 1]);
    }

    fgetc(stdin);
    fputs("Operator : " , stdout);
    scanf("%c" , &opmsg[opnd_cnt * OPSZ + 1]);

    write(serv_sock , opmsg , opnd_cnt * OPSZ + 2);
    read(serv_sock , &result , RLT_SIZE);

    printf("Operator result : %d \n" , result);

    close(serv_sock);
    return 0;
}