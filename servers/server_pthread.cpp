#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <iostream>
#include <pthread.h>
#include "../serialization/serialization.h"
#include <cstdlib>
#include <ctime>

using namespace std;

typedef struct {
    string str1;
    string str2;
    char line[2000];
    int sock;
} ArgString;

void add_pthread(ArgString *arg)
{
    ArgString argument = *arg;
    //Send the message back to client
    strcpy(argument.line, "HELLO FROM SERVER\n");
    string lyambda = to_string(myCount(argument.str2, argument.str1)) + '\n';
    const char *parameter = lyambda.c_str();
    strcpy(argument.line, parameter);

    write(argument.sock, argument.line, strlen(argument.line));
    close(argument.sock);
}

int main(int argc , char *argv[])
{
    int socket_desc , client_sock , c , read_size;
    struct sockaddr_in server , client;
    char client_message[2000];
    ArgString arg;

    //Create socket
    socket_desc = socket(AF_INET , SOCK_STREAM , 0);
    if (socket_desc == -1)
    {
        printf("Could not create socket");
    }
    puts("Socket created");

    //Prepare the sockaddr_in structure
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons( 14 ); // 8888

    //Bind
    if( bind(socket_desc,(struct sockaddr *)&server , sizeof(server)) < 0)
    {
        //print the error message
        perror("bind failed. Error");
        return 1;
    }
    puts("bind done");

    //Listen
    listen(socket_desc , 3);
    //Accept and incoming connection
    puts("Waiting for incoming connections1...");
    c = sizeof(struct sockaddr_in);
    int counter = 0;
    time_t t;
    while(1) {
        if(counter == 1)
            t = time(NULL);
        //accept connection from an incoming client
        client_sock = accept(socket_desc, (struct sockaddr *)&client, (socklen_t*)&c);

        pthread_t general;

        if (client_sock < 0)
        {
            perror("accept failed");
            return 1;
        }
        //Receive a message from client
        if ((read_size = recv(client_sock, client_message, 2000, 0)) > 0) {
            string msg = client_message;
            _data data = deserialize(msg);
            arg.str1 = data.Full;
            arg.str2 = data.Sub;
            strcpy(arg.line, client_message);
            arg.sock = client_sock;
            pthread_create(&general, NULL, (void *(*)(void *)) add_pthread, &arg);
        }
        if (read_size == 0) {
            puts("Client disconnected");
            exit(0);
        }
        else if (read_size == -1) {
            perror("recv failed");
        }
        counter += 1;
        cout << "Counter: " << counter << endl;
        if (counter == 1000) {
            cout << time(NULL) - t << " seconds!" << endl;
            break;
        }
    }

    return 0;
}