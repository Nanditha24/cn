
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 8080
#define WINDOW_SIZE 4
#define TOTAL_FRAMES 10

int main() {
    int sockfd, client_sock;
    struct sockaddr_in sin;
    int addrlen = sizeof(sin);
    int received[TOTAL_FRAMES] = {0}; // To store the status of received frames
    int window_start = 0;
    int frame, ack;

    // Create the server socket
    if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    sin.sin_family = AF_INET;
    sin.sin_addr.s_addr = INADDR_ANY;
    sin.sin_port = htons(PORT);

    // Bind the socket to the port
    if (bind(sockfd, (struct sockaddr *)&sin, sizeof(sin)) < 0) {
        perror("Bind failed");
        exit(EXIT_FAILURE);
    }

    // Listen for incoming connections
    if (listen(sockfd, 3) < 0) {
        perror("Listen failed");
        exit(EXIT_FAILURE);
    }

    // Accept a connection from the client
    if ((client_sock = accept(sockfd, (struct sockaddr*)&sin, (socklen_t*)&addrlen)) < 0) {
        perror("Accept failed");
        exit(EXIT_FAILURE);
    }

    // Start receiving frames and sending ACKs
    while (window_start < TOTAL_FRAMES) {
        int valread = recv(client_sock, &frame, sizeof(frame), 0);
        if (valread > 0) {
            printf("Received frame %d\n", frame);
            if (frame >= window_start && frame < window_start + WINDOW_SIZE) {
                received[frame] = 1;
            }
            ack = frame;
            printf("Sending ACK for frame %d\n", ack);
            send(client_sock, &ack, sizeof(ack), 0);

            // Slide the window forward if all frames in the current window are acknowledged
            while (received[window_start] == 1 && window_start < TOTAL_FRAMES) {
                window_start++;
            }
        }
    }

    printf("All frames received and acknowledged.\n");
    close(client_sock);
    close(sockfd);
    return 0;
}