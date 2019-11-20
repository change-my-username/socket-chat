#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/types.h>
#include <string.h>

#define BUF_SIZE 80
#define PORT_NUM 45566

int main(){

    char read_buffer[BUF_SIZE];
    char write_buffer[BUF_SIZE];
    int read_num = 0;

    int serv_socket = socket(AF_INET, SOCK_STREAM, 0);
    struct sockaddr_in addr;

    memset(&addr, '0', sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_port = htons(PORT_NUM);
    addr.sin_addr.s_addr = INADDR_ANY;
    //addr.sin_addr.s_addr = inet_addr("192.168.0.103");

    if ( bind(serv_socket, (struct sockaddr* )&addr, sizeof(addr)) == -1 ) {
        perror("ERROR on bind");
        return -1;
    }

    if ( listen(serv_socket, 5) == -1 ){
        perror("ERROR on listen");
        return -1;
    }

    int current_socket = 0;
    while( current_socket = accept(serv_socket, NULL, NULL) ){
    
        if (current_socket < 0){
            perror("ERROR on accept");
            return -1;
        }
        else{
            printf("CONNECT!!!!\n");
            int pid = fork();
            while(1){
                if (pid == -1) 
                    printf("PID PIZDEZ\n");
                if(pid == 0){
                    while( read(current_socket, read_buffer, sizeof(read_buffer)) > 0 ){
                        printf("%s", read_buffer);
                    }
                }
                else{
                    fgets(write_buffer, sizeof(write_buffer), stdin); 
                    write(current_socket, write_buffer, strlen(write_buffer) + 1);     
                }
            }
        }

    }

    printf("CLOSE\n");
    close(serv_socket);

    return 0;
}