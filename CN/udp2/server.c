#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

int main() {
    char* ip = "127.0.0.1";
    int port = 5586;

    int sock;
    struct sockaddr_in s_addr, c_addr;
    char buffer[1024];
    socklen_t addr_size;

    // Create socket
    sock = socket(AF_INET, SOCK_DGRAM, 0);
    if (sock < 0) {
        perror("oops");
        return EXIT_FAILURE; 
    }

    // Set up the address structure
    memset(&s_addr, '\0', sizeof(s_addr));
    s_addr.sin_family = AF_INET;
    s_addr.sin_port = htons(port);
    s_addr.sin_addr.s_addr = inet_addr(ip);

    // Bind the socket
    int n = bind(sock, (struct sockaddr*)&s_addr, sizeof(s_addr));
    if (n < 0) {
        perror("oops");
        close(sock);
        return EXIT_FAILURE; 
    }

    // Communication loop
    while (1) {
        bzero(buffer, sizeof(buffer));
        char temp[1024];
        addr_size = sizeof(c_addr);

       
        int a = recvfrom(sock, buffer, sizeof(buffer), 0, (struct sockaddr*)&c_addr, &addr_size);
        if (a < 0) { 
            perror("oops");
            break; // Exit loop on error
        }

        // Print the received message
        printf("Received: %s\n", buffer);

        // Prepare to send response
        printf("Enter response: "); 
        scanf(" %[^\n]", temp); 

        
        sendto(sock, temp, strlen(temp), 0, (struct sockaddr*)&c_addr, addr_size); 

        bzero(buffer, sizeof(buffer)); 
    }

    close(sock); 
    return EXIT_SUCCESS;
}
