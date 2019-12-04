#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/types.h>
#include <string.h>
#include <pthread.h>
#include <stdlib.h>  // for exit()

#define BUF_SIZE 1000
#define PORT_NUM 45566


struct info{

    int client_id;
    char buffer[BUF_SIZE];
    int sock_id;
    char ip[INET_ADDRSTRLEN];

};


int main(){

    char read_buffer[BUF_SIZE];
    char write_buffer[BUF_SIZE];
    int read_num = 0;
    int option = 1; // for setsockopt function

    int serv_socket = socket(AF_INET, SOCK_STREAM, 0);
    setsockopt(serv_socket, SOL_SOCKET, SO_REUSEADDR, &option, sizeof(option));  // to avoid error "Address is already in use"
    struct sockaddr_in addr;

    memset(&addr, '0', sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_port = htons(PORT_NUM);
    addr.sin_addr.s_addr = INADDR_ANY;

    if ( bind(serv_socket, (struct sockaddr* )&addr, sizeof(addr)) == -1 ) {
        perror("ERROR on bind");
        return -1;
    }

    if ( listen(serv_socket, 5) == -1 ){
        perror("ERROR on listen");
        return -1;
    }

    int current_socket = 0;
    
    struct sockaddr_in* client_addr;

    socklen_t client_addr_len = sizeof(client_addr);
    int num_clients = 0;
    struct info client_info[0][5]; // 5 here is 5 in listen() 

    there:
    while( current_socket = accept(serv_socket, (struct sockaddr*)client_addr, &client_addr_len) ){

        if (current_socket < 0){
            perror("ERROR on accept");
            return -1;
        }

        num_clients++;
        client_info[0][num_clients - 1].sock_id = current_socket;
        client_info[0][num_clients - 1].client_id = num_clients;
        
        inet_ntop(AF_INET, &(client_addr->sin_addr), client_info[0][num_clients - 1].ip, INET_ADDRSTRLEN);

        printf("CONNECT!!!\n");
        //char client_ip[13];
        //inet_ntop(AF_INET, &(client_addr->sin_addr), client_ip, INET_ADDRSTRLEN);
        //printf("Client ip %s \n", client_ip);
        
    
        if(num_clients < 2){
            char server_response[30];
            sprintf(server_response, "No users in the chat");
            write(client_info[0][num_clients - 1].sock_id, server_response, sizeof(server_response));
            goto there;
        }
        else
        {
            write( client_info[0][0].sock_id, "KTO TO EST", 11);
            write( client_info[0][1].sock_id, "KTO TO EST", 11);
            while (1){
                if(fork() == 0){
                    while(read(client_info[0][0].sock_id, client_info[0][0].buffer, sizeof(client_info[0][0].buffer)) > 0)
                        write(client_info[0][1].sock_id, client_info[0][0].buffer, strlen(client_info[0][0].buffer) + 1);
                    exit(0);
                }
                else
                {
                    while(read(client_info[0][1].sock_id, client_info[0][1].buffer, sizeof(client_info[0][1].buffer)) > 0)
                        write(client_info[0][0].sock_id, client_info[0][1].buffer, strlen(client_info[0][1].buffer) + 1);
                }
                
            }
                
        }
        
    }

    printf("CLOSE\n");
    close(serv_socket);

    return 0;
}