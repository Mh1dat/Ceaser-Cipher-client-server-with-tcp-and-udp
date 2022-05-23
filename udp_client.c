//udp client by:mhmd mhidat
#include "header.h"
void dg_cli(int ); //client functionality
void handler(int );//SIGQUIT handler
void append(char subject[], const char insert);
int sockfd;//declare sockfd as global varibale to write on it in handler function
int main(int argc , char **argv)
{
    struct hostent *hp;
    struct sockaddr_in servaddr;
    struct in_addr **pptr;
    if(argc!=3)
    {
        printf("usage: client <hostname> <port_address>\n");
        exit(1);
    }
    char por[7] = "55008";
    if (strcmp(por, argv[2]))
    {
       printf("You must use 55008 as a Port number\n");
       exit(0);
    }
    char addr[20]="nes416_server";
    if(strcmp(addr,argv[1]))
    {
        printf("You must use nes416_server as a hostname\n");
        exit(0);
    }
    if ( (hp = gethostbyname (argv [1]) ) == NULL)
        {
            printf("hostname error for %s: %s\n", argv [1],hstrerror (h_errno));
            exit(0);
        }
    else
        pptr = (struct in_addr **) hp->h_addr_list;
    signal(SIGQUIT,handler);
    sockfd = Socket(AF_INET , SOCK_DGRAM,0);
    bzero(&servaddr,sizeof(servaddr));
    servaddr.sin_family=AF_INET; //IPV4
    memcpy (&servaddr.sin_addr, *pptr, sizeof (struct in_addr) ) ;
    servaddr.sin_port=htons(atoi(argv[2])); //port is the second arg
    Connect(sockfd,(struct sockaddr *)&servaddr, sizeof(servaddr));
    dg_cli(sockfd);
    exit(0);
}
void dg_cli(int sockfd)
{
    printf("Welcome to you in this udp client program\n");
    printf("Press ctrl+\\ to exit!\n");
    while(true)
    {
        printf("1. Encrypt a message\n");
        printf("2. Decrypt a message\n");
        char sendline [MAXLINE];
        char recvline [MAXLINE];
        char choice;
        Fgets(&choice,2, stdin);
        if(choice=='1')
            choice='e';
        else if(choice=='2')
            choice='d';
        else
        {
            printf("%c Is an invalid choice!\n",choice);
            continue;
        }
        bool flag=false;
        printf("Enter the message as small leters\n");
        memset(sendline,0,strlen(sendline));
        char x; scanf("%c",&x);
        Fgets(sendline, MAXLINE, stdin);
        sendline[strlen(sendline) - 1] = '\0';
        for(int i=0 ; i<strlen(sendline);i++)
            if(sendline[i]!=tolower(sendline[i]))
            {
                printf("The message must be lowercase, try again!\n");
                flag=true;
                break;
            }
        if(flag)
            continue;
        append(sendline,choice);
        printf("sending %s to the server\n",sendline);
        Writen(sockfd,sendline,strlen(sendline));
        memset(recvline,0,strlen(recvline));
        if(Read(sockfd,recvline,MAXLINE)==0) //server terminates
        {
            printf("cli: server terminated prematurely\n");
            exit(1);
        }
        printf("received %s from server \n",recvline);
        printf("You can use me again or press ctrl+\\ to exit\n");
   }
}
void handler(int signo)
 {
   printf("Done\n");
   char exit_msg[10]="Done\n";
   Writen(sockfd,exit_msg,strlen(exit_msg));
   exit(0);
 }
 // inserts into subject[] at position 0
void append(char subject[], const char insert) {
    for(int i=strlen(subject)-1;i>=0; i--)
    {
        subject[i+1]=subject[i];
    }
    subject[0]=insert;
}
