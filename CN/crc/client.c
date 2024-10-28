#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define PORT 8080
#define MAXLINE 1024

int main() {
    int sockfd;
    struct sockaddr_in servaddr;
    char generator[MAXLINE];
    char message[MAXLINE];

    // Input generator polynomial
    printf("Enter generator polynomial: ");
    scanf("%s", generator);

    // Input binary message
    printf("Enter a binary message: ");
    scanf("%s", message);

    if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    memset(&servaddr, 0, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(PORT);
    servaddr.sin_addr.s_addr = INADDR_ANY;

    // Send generator and message to server
    sendto(sockfd, generator, strlen(generator), MSG_CONFIRM, 
           (const struct sockaddr *)&servaddr, sizeof(servaddr));
    sendto(sockfd, message, strlen(message), MSG_CONFIRM, 
           (const struct sockaddr *)&servaddr, sizeof(servaddr));

    printf("Generator polynomial and message sent.\n");

    close(sockfd);
    return 0;
}

