#include "headerFiles.h"
#define MAX 255
#define PORT 8080 
#define SA struct sockaddr 

void func(int sockfd) 
{ 
    char buff[MAX]; 
    bzero(buff, MAX);

    FILE *fp;

    int ch = 0;

    fp = fopen("FTP.txt" , "a");
    int words;
    
    read(sockfd, &words, sizeof(int));
   
    while(ch != words)
     {
         read(sockfd, buff, MAX);
         fprintf(fp, "%s ", buff);
         ch++;
     }
     printf("File has been recieved successfully.\n It is saved by the name FTP.txt\n");
}
       


int main() 
{ 
    int sockfd, connfd, len; 
    struct sockaddr_in servaddr, cli; 
  
    sockfd = socket(AF_INET, SOCK_STREAM, 0); 
    if (sockfd == -1) { 
        printf("socket creation failed.\n"); 
        exit(0); 
    } 
    else
        printf("Socket successfully created.\n"); 
    bzero(&servaddr, sizeof(servaddr)); 
  
    servaddr.sin_family = AF_INET; 
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY); 
    servaddr.sin_port = htons(PORT); 
  
    if ((bind(sockfd, (SA*)&servaddr, sizeof(servaddr))) != 0) { 
        printf("socket bind failed...\n"); 
        exit(0); 
    } 
    else
        printf("Socket successfully binded.\n"); 
  
    if ((listen(sockfd, 5)) != 0) { 
        printf("Listen failed...\n"); 
        exit(0); 
    } 
    else
        printf("Server listening..\n"); 
    len = sizeof(cli); 
  
    connfd = accept(sockfd, (SA*)&cli, &len); 
    if (connfd < 0) { 
        printf("server acccept failed...\n"); 
        exit(0); 
    } 
    else
        printf("server acccept the client...\n"); 
  
    func(connfd); 
    close(connfd);
    close(sockfd); 
    return 0;
}
