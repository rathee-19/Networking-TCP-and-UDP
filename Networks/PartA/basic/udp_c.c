#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>

int main(int argc, char **argv){

  if (argc != 2) {
    printf("Usage: %s <port>\n", argv[0]);
    exit(0);
  }

  char *ip ;
  scanf("%s",ip);
  int port = atoi(argv[1]);

  int sockfd;
  int check_error;

  struct sockaddr_in addr;
  char buffer[1024];
  socklen_t addr_size;

  sockfd = socket(AF_INET, SOCK_DGRAM, 0);
  if (sockfd<0){
    perror("[-]Socket error");
    exit(1);
  }
  memset(&addr, '\0', sizeof(addr));
  addr.sin_family = AF_INET;
  addr.sin_port = htons(port);
  addr.sin_addr.s_addr = inet_addr(ip);

  bzero(buffer, 1024);
  strcpy(buffer, "Hello, World!");
  check_error=sendto(sockfd, buffer, 1024, 0, (struct sockaddr*)&addr, sizeof(addr));
    if (check_error<0){
    perror("[-]sendto error");
    exit(1);
  }
  printf("[+]Data send: %s\n", buffer);

  bzero(buffer, 1024);
  addr_size = sizeof(addr);
  check_error=recvfrom(sockfd, buffer, 1024, 0, (struct sockaddr*)&addr, &addr_size);
  if (check_error<0){
    perror("[-]recvfrom error");
    exit(1);
  }
  printf("[+]Data recv: %s\n", buffer);

  return 0;
}