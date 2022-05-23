/*
Udp,Tcp servers By:mhmd mhidat
*/
#include "header.h"
void sig_chld(int); //signal handler
int max(int, int);  //find the max between two numbers
void shift(char[]); //delete the first characheter from string
void enc(char arr[]); //ceaser encryption key=3
void capitlize (char []);//to make the message's letters uppercase
void deal_with(char []);//encryption or decryption
void dec (char []); //ceaser decryption key=3
void tcpserv(int,struct sockaddr_in); //tcp server functionality
int main(int argc , char **argv)
{
    bool flag=true;
     if(argc!=2)
    {
      printf("usage: server <port_address>\n");
      exit(0);
    }
    char por[7] = "55008";
    if (strcmp(por, argv[1]))
    {
        printf("You must use 55008 as a Port number\n");
        exit(0);
    }
    int listenfd, connfd, udpfd, nready, maxfdp1;
    char mesg[MAXLINE];
    pid_t childpid;
    fd_set rset;
    ssize_t n;
    socklen_t len;
    const int on = 1;
    struct sockaddr_in cliaddr, servaddr;
    listenfd = Socket(AF_INET, SOCK_STREAM, 0);
    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port=htons(atoi(argv[1]));
    //we should use REUSEADDR option with concurent server
    Setsockopt(listenfd, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on));
    Bind(listenfd, (struct sockaddr *) &servaddr, sizeof(servaddr));
    Listen(listenfd, LISTENQ);
    udpfd = Socket(AF_INET, SOCK_DGRAM, 0);
    Bind(udpfd, (struct sockaddr *) &servaddr, sizeof(servaddr));
    signal(SIGCHLD, sig_chld);
    FD_ZERO(&rset);
    maxfdp1 = max(listenfd, udpfd) + 1;
    printf("waiting for client's request\n");
    for ( ; ; ) {
        memset(mesg,0,strlen(mesg));
        FD_SET(listenfd, &rset);
        if(flag)
            FD_SET(udpfd, &rset);
        if ( (nready = select(maxfdp1, &rset, NULL, NULL, NULL)) < 0) {
            if (errno == EINTR)
                continue; 
            else
                err_sys("select error");
        }
        //tcp request
        if (FD_ISSET(listenfd, &rset)) 
        {
            len = sizeof(cliaddr);
            connfd = Accept(listenfd, (struct sockaddr *) &cliaddr, &len);
            if ( (childpid = Fork()) == 0) {
                Close(listenfd);
                tcpserv(connfd,cliaddr);
                exit (0);
                }
                Close(connfd);
            }
        //udp request
        if (FD_ISSET(udpfd, &rset)) 
        {
            len = sizeof(cliaddr);
            n = Recvfrom(udpfd, mesg, MAXLINE, 0, (struct sockaddr *) &cliaddr, &len);
            char ex[10]="Done\n";
            if(!strcmp(mesg,ex))
            {
                printf("udp server terminates...\n");
                flag=false;
                Close(udpfd);
                FD_CLR(udpfd,&rset);
                continue;
            }
            char port[8];
            sprintf(port,"%d",ntohs(cliaddr.sin_port));
            printf("received %s from the client by UDP (IP: %s Port %s )\n",mesg,inet_ntoa(cliaddr.sin_addr),port);
            deal_with(mesg);
            shift(mesg);
            printf("sending %s to the client\n",mesg);
            Sendto(udpfd, mesg, n, 0, (struct sockaddr *) &cliaddr, len);
        }
    }
}
void tcpserv(int connfd,struct sockaddr_in cliaddr){
    while(true)
    {   
        char data[MAXLINE];
        int n;
        memset(data,0,strlen(data));
        if ( (n = Read(connfd, data, MAXLINE)) == 0)///* connection closed by other end */
        return; 
        char port[8];
        sprintf(port,"%d",ntohs(cliaddr.sin_port));
        printf("received %s from the client by TCP (IP: %s Port %s )\n",data,inet_ntoa(cliaddr.sin_addr),port);
        deal_with(data);
        shift(data);
        printf("sending %s to the client\n",data);
        Writen(connfd,data, strlen(data));
        continue;
    }
}
void sig_chld(int signo)
{ 
    pid_t pid;
    int stat;
    while ( (pid = waitpid(-1, &stat, WNOHANG)) > 0)
    printf("child %d terminated\n", pid);
    return;
}
int max(int a, int b){
    if(a>b)
        return a;
    else 
        return b;
}
void deal_with(char mesg[])
{
    if(mesg[0]=='d')
    {
        dec(mesg);
        capitlize(mesg);
    }
    else if(mesg[0]=='e')
    {
        enc(mesg);
        capitlize(mesg);
    }
    else
        mesg="unknown operation! first charachter must be 'e' or 'd'\n";
}
void enc (char message[MAXLINE])
{
    int i, key=3;
    char ch;
    for(i = 1; message[i] != '\0'; ++i)
    {
        ch = message[i];
        if(ch >= 'a' && ch <= 'z' )
        {
            ch = ch + key;
            if(ch > 'z')
                ch = ch - 'z' + 'a' - 1;
            message[i] = ch;   
        }
        else if(ch==' ')
            continue;
        else{
            message[i]+=3;
        }
    }
}
void capitlize (char s[]){
   for(int i=0 ;i<strlen(s);i++)
   s[i] = toupper(s[i]);
}
void dec(char message[MAXLINE])
{
    char ch;
    int i, key=3;
    for(i = 1; message[i] != '\0'; ++i)
    {
        ch = message[i];
        if(ch >= 'a' && ch <= 'z')
        {
            ch = ch - key;
            if(ch < 'a')
                ch = ch + 'z' - 'a' + 1;
            message[i] = ch;
        }
        else if(ch==' ')
            continue;
        else
        {
            ch = ch - key;
            message[i] = ch;
        }
    }
}
void shift(char arr[])
{
    for(int i=1 ; i<strlen(arr);i++)
        arr[i-1]=arr[i];
    arr[strlen(arr)-1]='\0';
}