#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#define PORT 8080
#define WINDOW_SIZE 4
#define TOTAL_FRAMES 10

int main() {
    int sock = 0;
    struct sockaddr_in sin;
    int frame = 0;
    int ack;
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        printf("\nSocket creation error\n");
        return -1;
    }

    sin.sin_family = AF_INET;
    sin.sin_port = htons(PORT);

    // Convert IP address from text to binary form
    if (inet_pton(AF_INET, "127.0.0.1", &sin.sin_addr) <= 0) {
        printf("\nInvalid address/ Address not supported\n");
        return -1;
    }

    // Connect to the server
    if (connect(sock, (struct sockaddr *)&sin, sizeof(sin)) < 0) {
        printf("\nConnection Failed\n");
        return -1;
    }

    // Sending frames within the sliding window
    while (frame < TOTAL_FRAMES) {
        for (int i = 0; i < WINDOW_SIZE && frame < TOTAL_FRAMES; i++) {
            printf("Sending frame %d\n", frame);
            send(sock, &frame, sizeof(frame), 0); // Send frame to server

            // Receive acknowledgment
            read(sock, &ack, sizeof(ack));
            if (ack == frame) {
                printf("Received ACK for frame %d\n", ack);
                frame++; // Move to the next frame if ACK received
            }
        }
    }

    printf("All frames sent and acknowledged.\n");
    close(sock); // Close the socket
    return 0;
}
