#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>  // Added for close() function
#include <stdbool.h> // Added for boolean type

int main(int argc, char **argv)
{
    if (argc != 4)
    {
        printf("Usage: %s <server_ip> <server_port> <client_name>\n", argv[0]);
        exit(1); // Exit with an error code
    }

    char *ip = argv[1];
    int port = atoi(argv[2]);
    char client_name = argv[3][0];
    socklen_t addr_size = sizeof(struct sockaddr_in); // Initialize addr_size

    int sockfd;
    struct sockaddr_in server_addr;
    char buffer[1024];

    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd < 0)
    {
        perror("[-]Socket error");
        exit(1); // Exit with an error code
    }

    memset(&server_addr, 0, sizeof(server_addr)); // Corrected '\0' to 0
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port);
    server_addr.sin_addr.s_addr = inet_addr(ip);

    while (1)
    {
        char choice;
        while (1)
        {
            printf("[%c] Enter your choice (R for Rock, P for Paper, S for Scissors): \n", client_name);
            scanf(" %c", &choice); // Added space before %c to skip any whitespace characters
            printf("coming----------%c\n", choice);
            if (choice == 'R' || choice == 'P' || choice == 'S')
            {
                break;
            }
            else
            {
                printf("Invalid input. Please choose R, P, or S.\n");
            }
        }

        // Send the choice to the server
        printf("\n\n MY CHOICE ------ %c |\n\n", choice);
        sendto(sockfd, &choice, sizeof(choice), 0, (struct sockaddr *)&server_addr, addr_size);

        // Receive the result from the server
        recvfrom(sockfd, buffer, sizeof(buffer), 0, (struct sockaddr *)&server_addr, &addr_size);

        printf("[%c] Result: ", client_name);
        if (buffer[0] == 'A')
        {
            printf("You win!\n");
        }
        else if (buffer[0] == 'B')
        {
            printf("You lost!\n");
        }
        else
        {
            printf("It's a draw!\n");
        }

        // Prompt if you want to play another round
        char playAgain;
        char wait = 'Z';
        printf("[%c] Play another round? (Y/N): ", client_name);
        scanf(" %c", &playAgain);
     
        sendto(sockfd, &playAgain, sizeof(playAgain), 0, (struct sockaddr *)&server_addr, addr_size);
        recvfrom(sockfd, &wait, sizeof(wait), 0, (struct sockaddr *)&server_addr, &addr_size);
        if( wait == 'c'){
            // printf("waiting for another player's response \n");
            printf("BOTH HAVE ACCEPED. SO GO ON PLAYING \n\n");
        }
        else if( wait == 's'){
            // printf("Bad luck  player don't want to play again\n");
            printf("ONE OF YOU HAS REJECTED !!\n\n");
            break;
        }
    }

    return 0;
}
