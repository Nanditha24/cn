#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

int main() {
    char* ip = "127.0.0.1";
    int port = 5586;

    int s_fd, c_fd;
    struct sockaddr_in s_addr, c_addr;
    char buffer[1024];
    socklen_t addr_size;

    // Create socket
    s_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (s_fd < 0) {
        perror("oops");
        return EXIT_FAILURE;
    }

    // Set up the address structure
    memset(&s_addr, '\0', sizeof(s_addr));
    s_addr.sin_family = AF_INET;
    s_addr.sin_port = htons(port);
    s_addr.sin_addr.s_addr = inet_addr(ip);

    // Bind the socket
    if (bind(s_fd, (struct sockaddr*)&s_addr, sizeof(s_addr)) < 0) {
        perror("oops");
        close(s_fd);
        return EXIT_FAILURE;
    }

    // Listen for incoming connections
    if (listen(s_fd, 5) < 0) {
        perror("oops");
        close(s_fd);
        return EXIT_FAILURE;
    }

    // Accept incoming connection
    addr_size = sizeof(c_addr);
    c_fd = accept(s_fd, (struct sockaddr*)&c_addr, &addr_size);
    if (c_fd < 0) {
        perror("oops");
        close(s_fd);
        return EXIT_FAILURE;
    }

    
    while (1) {
        bzero(buffer, sizeof(buffer)); 
        int n = recv(c_fd, buffer, sizeof(buffer), 0);
        if (n < 0) {
            perror("oops");
            close(c_fd);
            break; 
        }

        printf("Received: %s\n", buffer);

       
        char temp[1024];
        printf("Enter response: "); 
        scanf(" %[^\n]", temp); 

        bzero(buffer, sizeof(buffer)); 
        strcpy(buffer, temp); 
        send(c_fd, buffer, strlen(buffer), 0); 

        bzero(buffer, sizeof(buffer)); 
    }

    close(c_fd);
    close(s_fd);
    return EXIT_SUCCESS;
}
