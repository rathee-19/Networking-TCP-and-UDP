#include <stdio.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <errno.h>
#include <netdb.h>


int main()
{
    char* ip_address;    
    struct hostent *host_entry;
    char hostbuffer[256];
    char *IPbuffer;
    int hostname;

    int port = 8897;

    hostname = gethostname(hostbuffer, sizeof(hostbuffer));
    if(hostname != 0)
    {
        printf("\033[1;31mERROR: Error in obtaining IP address\n");
        printf("\033[0m");
        return 0;        
    }
    host_entry = gethostbyname(hostbuffer);
    if(host_entry == NULL)
    {
        printf("\033[1;31mERROR: Error in obtaining IP address\n");
        printf("\033[0m");
        return 0;         
    }
    ip_address = inet_ntoa(*((struct in_addr*)host_entry->h_addr_list[0]));
    // printf("%s\n", ip_address);

    int sfd;
    struct sockaddr_in serverAddress;

    sfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sfd == -1)
    {
        printf("\033[1;31mERROR: Socket not created\n");
        printf("\033[0m");
        return 0;
    }

    memset(&serverAddress, '\0', sizeof(serverAddress));

    serverAddress.sin_family = AF_INET;
    serverAddress.sin_addr.s_addr = inet_addr(ip_address);
    serverAddress.sin_port = htons(port);

    

    int con = connect(sfd, (struct sockaddr*)&serverAddress, sizeof(serverAddress));
    while(1)
    {
        if(con == (-1))
        {
            printf("\033[1;31mERROR: Couldn't connect with server\n");
            printf("\033[0m");
            close(sfd);
            return 0;        
        }

        char server_response[256];
        for(int y=0;y<256;y++)
        {
            server_response[y]='\0';
        }

        char input[256];
        for(int y=0;y<256;y++)
        {
            input[y]='\0';
        }
        input[0]='\0';
        printf("Enter your move: ");
        scanf("%s", &input);

        int send_sig=send(sfd,input,strlen(input),0);
        if( send_sig == (-1))
        {
            printf("\033[1;31mERROR: Couldn't send message to server\n");
            printf("\033[0m");
            close(con);
            close(sfd);
            return 0;
        }

        int recv_data=recv(sfd,server_response,256,0);
        if(recv_data < 0)
        {
            printf("\033[1;31mERROR: Nothing was recieved from server\n");
            printf("\033[0m");
            close(con);
            close(sfd);
            return 0;
        }
        printf("%s ",server_response);
        input[0]='\0';
        scanf("%s",&input);
        send_sig=send(sfd,input,strlen(input),0);
        if( send_sig == (-1))
        {
            printf("\033[1;31mERROR: Couldn't send message to server\n");
            printf("\033[0m");
            close(con);
            close(sfd);
            return 0;
        }
        server_response[0]='\0';
        recv_data=recv(sfd,server_response,256,0);
        if(recv_data < 0)
        {
            printf("\033[1;31mERROR: Nothing was recieved from server\n");
            printf("\033[0m");
            close(con);
            close(sfd);
            return 0;
        }
        if(strcmp(input,"No")==0 || atoi(server_response) == 0)
        {
            // free(input);
            break;
        }
    }
    close(con);
    close(sfd);
    return 0;
}
