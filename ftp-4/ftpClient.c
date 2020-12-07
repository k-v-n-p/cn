#include "headerFiles.h"
#define MAX 255
#define PORT 8080 
#define SA struct sockaddr 

void func(int sockfd) 
{ 
       char buff[MAX]; 
       bzero(buff, MAX);

       FILE *f;
        int words = 0;
	char c;
        f = fopen("demofile.txt" , "r");
	while((c = getc(f))!=EOF)
 	{
		fscanf(f , "%s" , buff);
		if(isspace(c) || c=='\t')
   	           words++;
        }
	write(sockfd, &words, sizeof(int));
        rewind(f);
        
        char ch;
        while(ch!= EOF)
        {
             fscanf(f , "%s" , buff);
             write(sockfd,buff,MAX);
             ch = getc(f);
	}
       printf("The file has been successfully sent to server\n");
}
       
int main() 
{ 
    int sockfd, connfd; 
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
    servaddr.sin_addr.s_addr = htons(INADDR_ANY); 
    servaddr.sin_port = htons(PORT); 
  
    if (connect(sockfd, (SA*)&servaddr, sizeof(servaddr)) != 0) { 
        printf("connection with the server failed.\n"); 
        exit(0); 
    } 
    else
        printf("connected to the server..\n"); 
     func(sockfd); 
    close(sockfd); 
    return 0;

}
