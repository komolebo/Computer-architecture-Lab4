#ifndef MYTEST_CLIENT_2_H
#define MYTEST_CLIENT_2_H
/**
    C++ client example using sockets
*/
#include<iostream>    //cout
#include<stdio.h> //printf
#include<string>  //string
#include<sys/socket.h>    //socket
#include<arpa/inet.h> //inet_addr
#include<netdb.h> //hostent

using namespace std;

/**
    TCP Client class
*/
class tcp_client
{
private:
    int sock;
    std::string address;
    int port;
    struct sockaddr_in server;

public:
    tcp_client();
    bool conn(string, int);
    bool send_data(string data);
    string receive(int);
};

#endif //MYTEST_CLIENT_2_H
