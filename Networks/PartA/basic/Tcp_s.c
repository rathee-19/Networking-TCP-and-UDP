#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

int main()
{

  char *ip;
  scanf("%s", ip);

  int port = 5566;

  int server_sockt, client_sockt;
  struct sockaddr_in server_addr, client_addr;
  socklen_t addr_size;
  char buffer[1024];
  int n;

  server_sockt = socket(AF_INET, SOCK_STREAM, 0);
  if (server_sockt < 0)
  {
    perror("[-]Socket error");
    exit(1);
  }
  printf("[+]TCP server socket created.\n");

  memset(&server_addr, '\0', sizeof(server_addr));
  server_addr.sin_family = AF_INET;
  server_addr.sin_port = port;
  server_addr.sin_addr.s_addr = inet_addr(ip);

  n = bind(server_sockt, (struct sockaddr *)&server_addr, sizeof(server_addr));
  if (n < 0)
  {
    perror("[-]Bind error");
    exit(1);
  }
  printf("[+]Bind to the port number: %d\n", port);

  n = listen(server_sockt, 5);
  if (n < 0)
  {
    perror("[-]Bind error");
    exit(1);
  }
  printf("Listening...\n");

  while (1)
  {
    addr_size = sizeof(client_addr);
    client_sockt = accept(server_sockt, (struct sockaddr *)&client_addr, &addr_size);
      if (client_sockt < 0){
    perror("[-]server_socket error");
    exit(1);
  }
    printf("[+]Client connected.\n");

    bzero(buffer, 1024);
   n = recv(client_sockt, buffer, sizeof(buffer), 0);
      if (n < 0){
    perror("[-]server recv error");
    exit(1);
  }
    printf("Client: %s\n", buffer);

    bzero(buffer, 1024);
    strcpy(buffer, "HI, THIS IS SERVER. HAVE A NICE DAY!!!");
    printf("Server: %s\n", buffer);
    n=send(client_sockt, buffer, strlen(buffer), 0);
      if (n < 0){
    perror("[-]server send error");
    exit(1);
  }

    close(client_sockt);
  if (n < 0){
    perror("[-]server close error");
    exit(1);
  }
    printf("[+]Client disconnected.\n\n");
  }

  return 0;
}