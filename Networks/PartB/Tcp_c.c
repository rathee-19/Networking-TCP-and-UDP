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
struct sockaddr_in addr;
socklen_t addr_size;
int *ack_recv;

void *check_ack(void *arg)
{
  DataChunk c = *(DataChunk *)arg;
  sleep(1);
  while (ack_recv[c.chunk_no] != 1)
  {
    printf("Retransmitting %d chunk\n", c.chunk_no);
    sendto(sockfd, &c, sizeof(DataChunk), 0, (struct sockaddr *)&addr, sizeof(addr));
    sleep(1);
  }
  return 0;
}

int recv_TCP(char buffer[1024])
{
  DataChunk recieved;
  DataChunk ack;
  int chunk_recieved = 0;
  int total_chunks;
  recvfrom(sockfd, &recieved, sizeof(DataChunk), 0, (struct sockaddr *)&addr, &addr_size);
  total_chunks = recieved.total_chunks;
  char chunk_stat[total_chunks];
  bzero(chunk_stat,total_chunks);
  chunk_recieved++;
  chunk_stat[recieved.chunk_no] = 1;
  for (int i = 0; i < 4; i++)
  {
    buffer[recieved.chunk_no * 4 + i] = recieved.data[i];
  }
  ack.ack_set = 1;
  ack.ack_no = recieved.chunk_no;
  sendto(sockfd, &ack, sizeof(DataChunk), 0, (struct sockaddr *)&addr, sizeof(addr));
  while (chunk_recieved < total_chunks)
  {
    recvfrom(sockfd, &recieved, sizeof(DataChunk), 0, (struct sockaddr *)&addr, &addr_size);
    if (chunk_stat[recieved.chunk_no] != 0)
    {
      continue;
    }
    chunk_recieved++;
    chunk_stat[recieved.chunk_no] = 1;
    for (int i = 0; i < 4; i++)
    {
      buffer[recieved.chunk_no * 4 + i] = recieved.data[i];
    }
    ack.ack_set = 1;
    ack.ack_no = recieved.chunk_no;
    sendto(sockfd, &ack, sizeof(DataChunk), 0, (struct sockaddr *)&addr, sizeof(addr));
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

  char buffer[1024];
  DataChunk transmit;
  DataChunk recieve;

  sockfd = socket(AF_INET, SOCK_DGRAM, 0);
  memset(&addr, '\0', sizeof(addr));
  addr.sin_family = AF_INET;
  addr.sin_port = htons(port);
  addr.sin_addr.s_addr = inet_addr(ip);

  // First send and then recieve some respone from server
  addr_size = sizeof(addr);
  bzero(buffer, 1024);
  printf("Enter message to send: ");
  scanf("%s", buffer);
  int total_chunks = ceil((double)strlen(buffer)/ 4.0);
  printf("%d\n", total_chunks);
  pthread_t ack_t[total_chunks];
  ack_recv = (int *)calloc(total_chunks, sizeof(int));
  int total_ack = 0;
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
    if(i!=0)
     sendto(sockfd, &transmit, sizeof(DataChunk), 0, (struct sockaddr *)&addr, sizeof(addr));

    pthread_create(&ack_t[i],NULL,check_ack,&transmit);
    pthread_detach(ack_t[i]);
    printf("Chunk %d sent\n", transmit.chunk_no);
    while (recvfrom(sockfd, &recieve, sizeof(DataChunk), MSG_DONTWAIT, (struct sockaddr *)&addr, &addr_size) != -1)
    {
      printf("Recieved ack %d\n", recieve.ack_no);
      ack_recv[recieve.ack_no] = 1;
      total_ack++;
    }
    usleep(100000);
  }
  while (total_ack != total_chunks)
  {
    int s = recvfrom(sockfd, &recieve, sizeof(DataChunk), MSG_DONTWAIT, (struct sockaddr *)&addr, &addr_size);
    if (s == -1)
    {
      continue;
    }
    printf("Recieved ack %d\n", recieve.ack_no);
    ack_recv[recieve.ack_no] = 1;
    total_ack++;
  }
  //free(ack_recv);
  printf("[+]Data send: %s\n", buffer);

  bzero(buffer, 1024);
  recv_TCP(buffer);
  printf("[+]Data recv: %s\n", buffer);

  return 0;
}