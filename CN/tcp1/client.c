#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>

int main() {
    char* ip = "127.0.0.1";
    int port = 5586;

    int sock_fd;
    struct sockaddr_in addr;
    char buffer[1024];

    // Create socket
    sock_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (sock_fd < 0) {
        perror("oops");
        return EXIT_FAILURE; 
    }

    // Set up the address structure
    memset(&addr, '\0', sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    addr.sin_addr.s_addr = inet_addr(ip);

    // Connect to server
    int n = connect(sock_fd, (struct sockaddr *)&addr, sizeof(addr)); 
    if (n < 0) {
        perror("didn't connect");
        close(sock_fd); 
        return EXIT_FAILURE;
    }

    // Communication loop
    while (1) {
        char temp[1024];
        getchar(); 
        printf("Enter message: "); 
        scanf("%[^\n]", temp); 
        getchar(); 


        bzero(buffer, 1024);
        strcpy(buffer, temp);
        send(sock_fd, buffer, strlen(buffer), 0);
    }


    close(sock_fd);
    return EXIT_SUCCESS;
}
