#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

// Function to determine the game result
char determineResult(char choiceA, char choiceB)
{
    if (choiceA == choiceB)
    {
        return 'D'; // Draw
    }
    if ((choiceA == 'R' && choiceB == 'S') || (choiceA == 'P' && choiceB == 'R') || (choiceA == 'S' && choiceB == 'P'))
    {
        return 'A'; // Client A wins
    }
    return 'B'; // Client B wins
}

int main(int argc, char **argv)
{
    if (argc != 3)
    {
        printf("Usage: %s <port_A> <port_B>\n", argv[0]);
        exit(0);
    }

    int port_A = atoi(argv[1]);
    int port_B = atoi(argv[2]);

    int sockfd_A, sockfd_B;
    struct sockaddr_in server_addr_A, server_addr_B, client_addr, client_addr2;
    socklen_t addr_size = sizeof(client_addr);
    socklen_t addr_size2 = sizeof(client_addr2);
    char buffer_A[1024], buffer_B[1024];
    char choice_A, choice_B;

    sockfd_A = socket(AF_INET, SOCK_DGRAM, 0);
    sockfd_B = socket(AF_INET, SOCK_DGRAM, 0);

    memset(&server_addr_A, '\0', sizeof(server_addr_A));
    memset(&server_addr_B, '\0', sizeof(server_addr_B));

    server_addr_A.sin_family = AF_INET;
    server_addr_A.sin_port = htons(port_A);
    server_addr_A.sin_addr.s_addr = INADDR_ANY;

    server_addr_B.sin_family = AF_INET;
    server_addr_B.sin_port = htons(port_B);
    server_addr_B.sin_addr.s_addr = INADDR_ANY;

    bind(sockfd_A, (struct sockaddr *)&server_addr_A, sizeof(server_addr_A));
    bind(sockfd_B, (struct sockaddr *)&server_addr_B, sizeof(server_addr_B));

    char playAgainA = 'Y', playAgainB = 'Y';

    while (1)
    {
        if (playAgainA == 'Y' && playAgainB == 'Y')
        {
            printf("Waiting for client A's choice...\n");
            recvfrom(sockfd_A, buffer_A, sizeof(buffer_A), 0, (struct sockaddr *)&client_addr, &addr_size);
            choice_A = buffer_A[0];

            printf("Waiting for client B's choice...\n");
            recvfrom(sockfd_B, buffer_B, sizeof(buffer_B), 0, (struct sockaddr *)&client_addr2, &addr_size2);
            choice_B = buffer_B[0];

            printf("\n\n--------- %c === %c ----------\n\n", choice_A, choice_B);

            char result = determineResult(choice_A, choice_B);

            // Send the result to client A and B
            printf("\n\n RESLUT ====== %c |||| %c \n\n", result, result);
            if (result == 'A')
            {
                char some = 'A';
                sendto(sockfd_A, &some, sizeof(some), 0, (struct sockaddr *)&client_addr, addr_size);
                some = 'B';
                sendto(sockfd_B, &some, sizeof(some), 0, (struct sockaddr *)&client_addr2, addr_size2);
            }
            else if (result == 'B')
            {
                char some = 'B';
                sendto(sockfd_A, &some, sizeof(some), 0, (struct sockaddr *)&client_addr, addr_size);
                some = 'A';
                sendto(sockfd_B, &some, sizeof(some), 0, (struct sockaddr *)&client_addr2, addr_size2);
            }
            else
            {

                sendto(sockfd_A, &result, sizeof(result), 0, (struct sockaddr *)&client_addr, addr_size);
                sendto(sockfd_B, &result, sizeof(result), 0, (struct sockaddr *)&client_addr2, addr_size2);
            }

            printf("Client A's choice: %c, Client B's choice: %c, Result: ", choice_A, choice_B);
            if (result == 'A')
            {
                printf("Client A wins!\n");
            }
            else if (result == 'B')
            {
                printf("Client B wins!\n");
            }
            else
            {
                printf("It's a draw!\n");
            }

            // Prompt clients if they want to play another round
            char playAgain = 'r';
            char playAgain2 = 'n';
            // printf("Play another round? (Y/N): ");
            // sendto(sockfd_A, &playAgain, sizeof(playAgain), 0, (struct sockaddr *)&client_addr, addr_size);
            // sendto(sockfd_B, &playAgain, sizeof(playAgain), 0, (struct sockaddr *)&client_addr2, addr_size2);

            recvfrom(sockfd_A, &playAgain, sizeof(playAgain), 0, (struct sockaddr *)&client_addr, &addr_size);

            recvfrom(sockfd_B, &playAgain2, sizeof(playAgain2), 0, (struct sockaddr *)&client_addr2, &addr_size2);

            char cont = 'x';

            if((playAgain == 'Y' || playAgain == 'y') && (playAgain2 == 'y' || playAgain2 == 'Y'))
            {
                cont = 'c';
            }
            else
            {
                cont = 's';
            }
                sendto(sockfd_A, &cont, sizeof(cont), 0, (struct sockaddr *)&client_addr, addr_size);
                sendto(sockfd_B, &cont, sizeof(cont), 0, (struct sockaddr *)&client_addr2, addr_size2);

        }
        else
        {
            // Both clients choose not to play again, exit the loop
            break;
        }
    }

    // close(sockfd_A);
    // close(sockfd_B);

    return 0;
}
