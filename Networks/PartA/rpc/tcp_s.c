#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

int main()
{

    char *ip = "127.0.0.1";
    int port = 6000;

    int server_sock, client_sock, client_sock2;
    struct sockaddr_in server_addr, client_addr, client_addr2;
    socklen_t addr_size, addr_size2;
    char buffer[1024];
    char buffer2[1024];
    int n;

    server_sock = socket(AF_INET, SOCK_STREAM, 0);
    if (server_sock < 0)
    {
        perror("[-]Socket error");
        exit(1);
    }
    printf("[+]TCP server socket created.\n");

    memset(&server_addr, '\0', sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = port;
    server_addr.sin_addr.s_addr = inet_addr(ip);

    n = bind(server_sock, (struct sockaddr *)&server_addr, sizeof(server_addr));
    if (n < 0)
    {
        perror("[-]Bind error");
        exit(1);
    }
    printf("[+]Bind to the port number: %d\n", port);

    listen(server_sock, 3);
    printf("Listening...\n");

    while (1)
    {
        printf("Searching for players ...\n\n");
        addr_size = sizeof(client_addr);
        addr_size2 = sizeof(client_addr2);
        client_sock = accept(server_sock, (struct sockaddr *)&client_addr, &addr_size);
        printf("[+]Client-1 connected.\n");
        client_sock2 = accept(server_sock, (struct sockaddr *)&client_addr2, &addr_size2);
        printf("[+]Client-2 connected.\n");

        while (1)
        {

            bzero(buffer, 1024);
            bzero(buffer2, 1024);
            recv(client_sock, buffer, sizeof(buffer), 0);
            printf("Client-1: %s\n", buffer);
            recv(client_sock2, buffer2, sizeof(buffer2), 0);
            printf("Client-2: %s\n", buffer2);

            if (strcmp(buffer, "ROCK") == 0 && strcmp(buffer2, "SCISSOR") == 0)
            {
                bzero(buffer, 1024);
                bzero(buffer2, 1024);
                strcpy(buffer, "YOU WIN");
                strcpy(buffer2, "YOU LOOSE");
            }
            else if (strcmp(buffer, "ROCK") == 0 && strcmp(buffer2, "PAPER") == 0)
            {
                bzero(buffer, 1024);
                bzero(buffer2, 1024);
                strcpy(buffer2, "YOU WIN");
                strcpy(buffer, "YOU LOOSE");
            }
            else if (strcmp(buffer, "PAPER") == 0 && strcmp(buffer2, "SCISSOR") == 0)
            {
                bzero(buffer, 1024);
                bzero(buffer2, 1024);
                strcpy(buffer2, "YOU WIN");
                strcpy(buffer, "YOU LOOSE");
            }
            else if (strcmp(buffer, "SCISSOR") == 0 && strcmp(buffer2, "PAPER") == 0)
            {
                bzero(buffer, 1024);
                bzero(buffer2, 1024);
                strcpy(buffer, "YOU WIN");
                strcpy(buffer2, "YOU LOOSE");
            }
            else if (strcmp(buffer, "ROCK") == 0 && strcmp(buffer2, "PAPER") == 0)
            {
                bzero(buffer, 1024);
                bzero(buffer2, 1024);
                strcpy(buffer2, "YOU WIN");
                strcpy(buffer, "YOU LOOSE");
            }
            else if (strcmp(buffer, "PAPER") == 0 && strcmp(buffer2, "ROCK") == 0)
            {
                bzero(buffer, 1024);
                bzero(buffer2, 1024);
                strcpy(buffer, "YOU WIN");
                strcpy(buffer2, "YOU LOOSE");
            }
            else if (strcmp(buffer, buffer2) == 0)
            {
                bzero(buffer, 1024);
                bzero(buffer2, 1024);
                strcpy(buffer, "DRAW");
                strcpy(buffer2, "DRAW");
            }
            else
            {
                bzero(buffer, 1024);
                bzero(buffer2, 1024);
                strcpy(buffer, "SOMEONE ENTERED INVALID TRY AGAIN");
                strcpy(buffer2, buffer);
            }
            // printf("Server: %s\n", buffer);
            send(client_sock, buffer, strlen(buffer), 0);
            send(client_sock2, buffer2, strlen(buffer2), 0);

            bzero(buffer, 1024);
            bzero(buffer2, 1024);

            recv(client_sock, buffer, sizeof(buffer), 0);
            recv(client_sock2, buffer2, sizeof(buffer2), 0);

            bzero(buffer, 1024);
            bzero(buffer2, 1024);
            strcpy(buffer, "DO YOU WANT TO PLAY AGAIN ??? (-_-)\n");
            strcpy(buffer2, buffer);
            send(client_sock, buffer, strlen(buffer), 0);
            send(client_sock2, buffer2, strlen(buffer2), 0);
            // Now reciebing
            bzero(buffer, 1024);
            bzero(buffer2, 1024);
            recv(client_sock, buffer, sizeof(buffer), 0);
            recv(client_sock2, buffer2, sizeof(buffer2), 0);
            if (strcmp(buffer, "YES") == 0 && strcmp(buffer2, "YES") == 0)
            {
                printf("---NEW MATCH---\n");
                bzero(buffer, 1024);
                bzero(buffer2, 1024);
                strcpy(buffer, "continue");
                strcpy(buffer2, buffer);
                send(client_sock, buffer, strlen(buffer), 0);
                send(client_sock2, buffer2, strlen(buffer2), 0);
            }
            else
            {
                bzero(buffer, 1024);
                bzero(buffer2, 1024);
                strcpy(buffer, "STOPPED");
                strcpy(buffer2, buffer);
                send(client_sock, buffer, strlen(buffer), 0);
                send(client_sock2, buffer2, strlen(buffer2), 0);
                close(client_sock);
                close(client_sock2);
                printf("[+]Client disconnected.\n\n");
                printf("[+]Client-2 disconnected.\n\n");
                break;
            }
        }

        // close
    }

    return 0;
}