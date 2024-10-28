#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define PORT 8080
#define MAXLINE 1024

// Function to perform modulo-2 division and compute the remainder
void compute_crc(char *message, char *generator, char *remainder) {
    int msg_len = strlen(message);
    int gen_len = strlen(generator);

    strcpy(remainder, message);

    for (int i = 0; i <= msg_len - gen_len; i++) {
        if (remainder[i] == '1') {
            for (int j = 0; j < gen_len; j++) {
                remainder[i + j] = (remainder[i + j] == generator[j]) ? '0' : '1';
            }
        }
    }

    strncpy(remainder, remainder + msg_len - gen_len + 1, gen_len - 1);
    remainder[gen_len - 1] = '\0';
}

// Function to correct a single-bit error in the received message
void correct_error(char *message, int error_position) {
    if (message[error_position] == '0') {
        message[error_position] = '1';
    } else {
        message[error_position] = '0';
    }
}

int main() {
    int sockfd;
    char buffer[MAXLINE];
    struct sockaddr_in servaddr, cliaddr;

    if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    memset(&servaddr, 0, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(PORT);
    servaddr.sin_addr.s_addr = INADDR_ANY;

    if (bind(sockfd, (const struct sockaddr *)&servaddr, sizeof(servaddr)) < 0) {
        perror("Bind failed");
        close(sockfd);
        exit(EXIT_FAILURE);
    }

    int len = sizeof(cliaddr);

    // Receive generator polynomial
    int n = recvfrom(sockfd, buffer, MAXLINE, MSG_WAITALL, (struct sockaddr *)&cliaddr, &len);
    buffer[n] = '\0';
    char generator[MAXLINE];
    strcpy(generator, buffer);
    printf("Received generator polynomial: %s\n", generator);

    // Receive binary message
    n = recvfrom(sockfd, buffer, MAXLINE, MSG_WAITALL, (struct sockaddr *)&cliaddr, &len);
    buffer[n] = '\0';
    printf("Received binary message: %s\n", buffer);

    // Compute the CRC remainder
    char remainder[strlen(generator)];
    compute_crc(buffer, generator, remainder);

    // Check for errors
    int error_detected = 0;
    for (int i = 0; i < strlen(remainder) - 1; i++) {
        if (remainder[i] != '0') {
            error_detected = 1;
            break;
        }
    }

    if (error_detected) {
        printf("Error detected in received message: %s\n", buffer);
        // Attempt to correct the first bit
        printf("Attempting to correct the first bit...\n");
        correct_error(buffer, 0); // Correcting the first bit as an example
        printf("Corrected message: %s\n", buffer);
    } else {
        printf("No error detected in received message: %s\n", buffer);
    }

    close(sockfd);
    return 0;
}
