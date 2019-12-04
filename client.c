#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <pthread.h>

#define BUF_SIZE 1000 
#define PORT_NUM 45566



struct thrdData{

    char buffer[BUF_SIZE];
    char flag;
    int sock_id;
};

void* thrd_read_write(void* arg){

    struct thrdData* Data = (struct thrdData*) arg;

    if (Data -> flag == 'w'){
        printf ("Entered write thread\n");
        while (1){
            fgets(Data -> buffer, sizeof(Data -> buffer), stdin); 
            write(Data -> sock_id, Data -> buffer, strlen(Data -> buffer) + 1);
        }

    }

    if (Data -> flag == 'r'){
        printf ("Entered read thread\n");
        while (1){
            while( read(Data -> sock_id, Data -> buffer, sizeof(Data -> buffer)) > 0 )
                printf("%s", Data -> buffer);
        }

    }

    return NULL;
}



int main(int argc, char** argv){

    char read_buffer[BUF_SIZE];
    char write_buffer[BUF_SIZE];
    int read_num = 0;

    int server_socket = socket(AF_INET, SOCK_STREAM, 0);
    
    struct sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT_NUM);
    server_addr.sin_addr.s_addr = inet_addr(argv[1]); 

    if ( connect(server_socket, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1 ){
        perror("Error on connect");
        return -1;
    }
    char server_response[50];

    there:
    if (read(server_socket, server_response, sizeof(server_response)) > 0 ){
        printf("Server response %s:\n", server_response);
        if(strcmp(server_response, "No users in the chat") == 0 )
            memset(server_response, '0', sizeof(server_response));
            goto there;
        printf("DEBUG 1\n");
        if (strcmp(server_response, "KTO TO EST") == 0)
        {
            printf("DEBUG 2\n");
            pthread_t tid_read, tid_write;
            struct thrdData* thrdRead; 
            struct thrdData* thrdWrite;
            
            thrdRead -> flag = 'r';
            thrdRead -> sock_id = server_socket;
            thrdWrite -> flag = 'w';
            thrdWrite -> sock_id = server_socket;
            printf("DEBUG 3\n");
            
            pthread_create(&tid_read, NULL, thrd_read_write, thrdRead);
            pthread_create(&tid_write, NULL, thrd_read_write, thrdWrite);
                    
            pthread_join(tid_read, NULL);
            pthread_join(tid_write, NULL);
        }
    }
 
    printf("CLOSE\n");
    close(server_socket);

    return 0;
}