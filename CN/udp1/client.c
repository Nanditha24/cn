#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <unistd.h> 

int main() {
    char* ip = "127.0.0.1";
    int port = 5586;

    int sock;
    struct sockaddr_in c_addr; 
    char buffer[1024];
    socklen_t addr_size;

    // Create socket
    sock = socket(AF_INET, SOCK_DGRAM, 0); 
    if (sock < 0) {
        perror("oops"); 
        return EXIT_FAILURE; 
    }

    // Set up the address structure
    memset(&c_addr, '\0', sizeof(c_addr));
    c_addr.sin_family = AF_INET;
    c_addr.sin_port = htons(port);
    c_addr.sin_addr.s_addr = inet_addr(ip);

    addr_size = sizeof(c_addr);

    // Communication loop
    while (1) {
        char temp[1024];
        bzero(buffer, sizeof(buffer)); 
        printf("Enter message: "); 
        scanf("%s", temp); 


        sendto(sock, temp, strlen(temp), 0, (struct sockaddr*)&c_addr, addr_size);
    }

    close(sock); 
    return EXIT_SUCCESS;
}