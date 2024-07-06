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

  int sockt;
  int check_error;
  struct sockaddr_in addr;
  socklen_t addr_size;
  char buffer[1024];
  int n;

  sockt = socket(AF_INET, SOCK_STREAM, 0);
  if (sockt < 0)
  {
    perror("[-]Socket error");
    exit(1);
  }
  printf("[+]TCP server socket created.\n");
  memset(&addr, '\0', sizeof(addr));
  addr.sin_family = AF_INET;
  addr.sin_port = port;
  addr.sin_addr.s_addr = inet_addr(ip);
  check_error = connect(sockt, (struct sockaddr *)&addr, sizeof(addr));
  if (check_error < 0)
  {
    perror("[-]connect error");
    exit(1);
  }
  printf("Connected to the server.\n");

  bzero(buffer, 1024);
  strcpy(buffer, "HELLO, THIS IS CLIENT.");
  printf("Client: %s\n", buffer);
  check_error = send(sockt, buffer, strlen(buffer), 0);
  if (check_error < 0)
  {
    perror("[-]send error");
    exit(1);
  }

  bzero(buffer, 1024);
  check_error = recv(sockt, buffer, sizeof(buffer), 0);
  if (check_error < 0)
  {
    perror("[-]recv error");
    exit(1);
  }
  printf("Server: %s\n", buffer);

  check_error = close(sockt);
  if (check_error < 0)
  {
    perror("[-]close error");
    exit(1);
  }
  printf("Disconnected from the server.\n");

  return 0;
}