#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <pthread.h>
#include <unistd.h>
#include <math.h>
typedef struct DataChunk
{
  int total_chunks;
  int chunk_no;
  char data[4];
  int ack_set;
  int ack_no;
} DataChunk;

int sockfd;
struct sockaddr_in server_addr, client_addr;
socklen_t addr_size;
int *ack_recv;
void *check_ack(void *arg)
{
  DataChunk c = *(DataChunk *)arg;
  printf("Hello %d \n", c.chunk_no);
  sleep(1);
  while (ack_recv[c.chunk_no] != 1)
  {
    printf("Retransmitting chunk: %d\n", c.chunk_no);
    sendto(sockfd, &c, sizeof(DataChunk), 0, (struct sockaddr *)&client_addr, sizeof(client_addr));
    sleep(1);
  }
}
int recv_TCP(char buffer[1024])
{
  DataChunk recieved;
  DataChunk ack;
  int chunk_recieved = 0;
  int total_chunks;
  recvfrom(sockfd, &recieved, sizeof(DataChunk), 0, (struct sockaddr *)&client_addr, &addr_size);
  total_chunks = recieved.total_chunks;
  char chunk_stat[total_chunks];
  bzero(chunk_stat, total_chunks);
  chunk_recieved++;
  printf("Chunk %d  out of %d recieved\n", recieved.chunk_no, total_chunks);
  chunk_stat[recieved.chunk_no] = 1;
  for (int i = 0; i < 4; i++)
  {
    buffer[recieved.chunk_no * 4 + i] = recieved.data[i];
  }
  ack.ack_set = 1;
  ack.ack_no = recieved.chunk_no;
  sendto(sockfd, &ack, sizeof(DataChunk), 0, (struct sockaddr *)&client_addr, sizeof(client_addr));
  while (chunk_recieved < total_chunks)
  {
    recvfrom(sockfd, &recieved, sizeof(DataChunk), 0, (struct sockaddr *)&client_addr, &addr_size);
    if (chunk_stat[recieved.chunk_no] != 0)
    {
      continue;
    }
    chunk_recieved++;
    printf("Chunk %d  out of %d recieved\n", recieved.chunk_no, total_chunks);
    chunk_stat[recieved.chunk_no] = 1;
    for (int i = 0; i < 4; i++)
    {
      buffer[recieved.chunk_no * 4 + i] = recieved.data[i];
    }
    ack.ack_set = 1;
    ack.ack_no = recieved.chunk_no;
    sendto(sockfd, &ack, sizeof(DataChunk), 0, (struct sockaddr *)&client_addr, sizeof(client_addr));
  }
  return 0;
}
int main(int argc, char **argv)
{

  if (argc != 2)
  {
    printf("Usage: %s <port>\n", argv[0]);
    exit(0);
  }

  char *ip = "127.0.0.1";
  int port = atoi(argv[1]);

  int n;
  char buffer[1024];
  DataChunk transmit;
  DataChunk recieve;

  sockfd = socket(AF_INET, SOCK_DGRAM, 0);
  if (sockfd < 0)
  {
    perror("[-]socket error");
    exit(1);
  }

  memset(&server_addr, '\0', sizeof(server_addr));
  server_addr.sin_family = AF_INET;
  server_addr.sin_port = htons(port);
  server_addr.sin_addr.s_addr = inet_addr(ip);

  n = bind(sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr));
  if (n < 0)
  {
    perror("[-]bind error");
    exit(1);
  }

  addr_size = sizeof(client_addr);
  bzero(buffer, 1024);
  recv_TCP(buffer);
  printf("[+]Data recv: %s\n", buffer);
  bzero(buffer, 1024);
  printf("Enter message to send: ");
  scanf("%s", buffer);
  int total_chunks = ceil((double)strlen(buffer) / 4.0);
  printf("%d\n", total_chunks);
  ack_recv = (int *)calloc(total_chunks, sizeof(int));
  int total_ack = 0;
  pthread_t ack_t[total_chunks];
  for (int i = 0; i < total_chunks; i++)
  {
    transmit.chunk_no = i;
    transmit.total_chunks = total_chunks;
    for (int j = 0; j < 4; j++)
    {
      transmit.data[j] = buffer[i * 4 + j];
    }
    transmit.ack_set = 0;
    ack_recv[i] = 0;
    sendto(sockfd, &transmit, sizeof(DataChunk), 0, (struct sockaddr *)&client_addr, sizeof(client_addr));
    pthread_create(&ack_t[i], NULL, check_ack, &transmit);
    pthread_detach(ack_t[i]);
    while (recvfrom(sockfd, &recieve, sizeof(DataChunk), MSG_DONTWAIT, (struct sockaddr *)&client_addr, &addr_size) != -1)
    {
      printf("Chunk %d recieved\n", recieve.ack_no);
      ack_recv[recieve.ack_no] = 1;
      total_ack++;
    }
    usleep(100000);
  }
  while (total_ack != total_chunks)
  {
    if (recvfrom(sockfd, &recieve, sizeof(DataChunk), MSG_DONTWAIT, (struct sockaddr *)&client_addr, &addr_size) == -1)
    {
      continue;
    }
    printf("Chunk %d recieved\n", recieve.ack_no);
    ack_recv[recieve.ack_no] = 1;
    total_ack++;
  }
  printf("[+]Data send: %s\n", buffer);

  return 0;
}