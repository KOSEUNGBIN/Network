//
// Created by 고승빈 on 2017. 10. 4..
//

#include <stdio.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <string.h>
#include <unistd.h>


int main(int argc , char ** argv)
{
    int serv_sock;
    struct sockaddr_in servaddr;

    int cnt , i;
    int * value , * result;
    char operator;

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

    while(1)
    {
        fputs("Input Operand Count (Q to quit): " , stdout);
        fgets(&cnt , sizeof(cnt) , stdin);

        if(cnt == (int)'q' || cnt == (int) 'Q')
        {
            break;
        }

        value = (int *) malloc(sizeof(int) * (cnt + 1));

        for(i = 1; i <= cnt ; ++i)
        {
            printf("Operand %d : " , i);
            scanf("%d" , &value[i - 1]);
        }

        printf("Operator : " );
        scanf("%c" , &operator);

        value[cnt] = (int) operator;

        write(serv_sock , value , sizeof(value) * (cnt + 1));
        read(serv_sock , result , sizeof(result));

        printf("Message from server : %d\n" , *result);
    }

    close(serv_sock);
    return 0;
}