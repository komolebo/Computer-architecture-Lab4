#include <sys/socket.h>
#include <arpa/inet.h> //inet_addr
#include <iostream>

#include "..//serialization/serialization.h"

using namespace std;

int main(int argc , char *argv[])
{
    int socket_desc , client_sock , c , read_size;
    struct sockaddr_in server , client;
    char client_message[2000];

    socket_desc = socket(AF_INET , SOCK_STREAM , 0);
    if (socket_desc == -1)
        cout << "Could not create socket" << endl;

    //Prepare the sockaddr_in structure
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons( 14 );

    //Bind
    if( bind(socket_desc,(struct sockaddr *)&server , sizeof(server)) < 0)
    {
        cout << "bind failed. Error" << endl;
        return 1;
    }
    cout << "bind done" << endl;

    //Listen
    listen(socket_desc , 3);
    //Accept and incoming connection
    cout << "Initial waiting for incoming connections..." << endl;
    c = sizeof(struct sockaddr_in);
    //accept connection from an incoming client
    client_sock = accept(socket_desc, (struct sockaddr *)&client, (socklen_t*)&c);

    signal(SIGCHLD, SIG_IGN);
    int pid;
    int count = 1;
    while((pid = fork()) >= 0)
    {
        if (pid == 0) // if son works
        {
            if (client_sock < 0)
            {
                cout << "accept failed" << endl;
                return 1;
            }
            cout << "Connection accepted" << endl;

            //Receive a message from client
            if( (read_size = recv(client_sock , client_message , 2000 , 0)) > 0 )
            {
                // Тут ми десеріалізуємо прийняті дані
                _data Data = deserialize(string(client_message));
                cout << " Server decoded and searched substring " << Data.Sub << endl;

                //Send the message back to client
                string response = to_string(myCount(Data.Sub, Data.Full)) + '\n';
                cout << "Sending to client: " << response << endl;
                write(client_sock , response.c_str() , response.length() );
            }
            exit(0);
        }
        else if (pid > 0) // Father works
        {
            cout << count << ". Waiting for " << client_sock << endl;
            close(client_sock);
            client_sock = accept(socket_desc, (struct sockaddr *)&client, (socklen_t*)&c);
            cout << " Father #" << pid << ": " << endl;
            count++;
        }
    }

    return 0;
}