#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>

#define BUF_SIZE 80
#define PORT_NUM 45566

int main(int argc, char** argv){

    char client_buffer[BUF_SIZE];
    int read_num = 0;

    int server_socket = socket(AF_INET, SOCK_STREAM, 0);
    struct sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT_NUM);
    server_addr.sin_addr.s_addr = inet_addr(argv[1]);  //inet_addr("192.168.0.103");

    if ( connect(server_socket, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1 ){
        perror("Error on connect");
        return -1;
    }
    else{
        while(1){
            fgets(client_buffer, sizeof(client_buffer), stdin); 
            write(server_socket, client_buffer, strlen(client_buffer) + 1); // +1 for string ends symbol, strlen ends up without '\0'
        }
    }

 
    printf("CLOSE\n");
    close(server_socket);

    return 0;
}