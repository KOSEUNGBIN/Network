//
// Created by 고승빈 on 2017. 10. 5..
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define BUF_SIZE    30

void error_handling(char * message);

int main(int argc , char ** argv)
{
    int sock;
    char message[BUF_SIZE];
    struct sockaddr_in my_addr , your_addr;
    socklen_t adr_sz;
    int str_len , i;

    if(argc !=2)
    {
        fprintf(stderr , "Usage : %s <port>\n" , argv[0]);
        exit(1);
    }

    sock = socket(PF_INET , SOCK_DGRAM , 0);
    if(sock == -1)
    {
        fprintf(stderr , "socket() error\n" );
        exit(1);
    }

    memset(&my_addr , 0 ,sizeof(my_addr));
    my_addr.sin_family = AF_INET;
    my_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    my_addr.sin_port = htons(atoi(argv[1]));

    if(bind(sock , (struct sockaddr *) &my_addr , sizeof(my_addr)) == -1)
    {
        fprintf(stderr , "bind() error\n" );
        exit(1);
    }

    for( i = 0 ; i < 3 ; ++i)
    {
        sleep(5);
        adr_sz = sizeof(your_addr);

        str_len = recvfrom(sock , message , BUF_SIZE , 0 , (struct sockaddr *) &your_addr , &adr_sz);

        printf("Message %d: %s \n", i+1 , message);
    }

    close(sock);

    return 0;
}


void error_handling(char * message)
{
    fputs(message , stderr);
    fputc('\n' , stderr);
    exit(1);
}