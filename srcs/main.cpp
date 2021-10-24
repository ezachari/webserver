#include "../includes/webserver.hpp"

int main(int argc, char const *argv[])
{
    if (argc == 2) {
        //create a scoket
        int l_socket = socket(AF_INET, SOCK_STREAM, 0);
        if (l_socket == -1) {
            std::cerr << "Can't create a socket";
            return -1;
        }
        //bind the socket to ip / port
        sockaddr_in hint;
        hint.sin_family = AF_INET;
        hint.sin_port = htons(54000);//htons(54000); // little endian and big endian on different systems
        std::cout << hint.sin_port << std::endl;
        inet_pton(AF_INET, "0.0.0.0", &hint.sin_addr); //127.0.0.1
        std::cout << hint.sin_addr.s_addr << std::endl;
        if ((bind(l_socket, (struct sockaddr *)&hint, sizeof(hint))) == -1) { // need cast
            std::cerr << "Can't bind to IP/port";
            return -2;
        }
        //mark the socket for listening in
        if (listen(l_socket, SOMAXCONN) == -1) {
            std::cerr << "Can't listen";
            return -3;
        }
        //accept a call
        sockaddr_in client;
        socklen_t clientSize = sizeof(client);
        char host[NI_MAXHOST];
        char svc[NI_MAXSERV];

        int clientSocket = accept(l_socket, (struct sockaddr *)&client, &clientSize);
        if (clientSocket == -1) {
            std::cerr << "Client connect problem";
            return -4;
        }
        //close the listening socket
        close(l_socket);
        memset(host, 0, NI_MAXHOST); //clean 
        memset(svc, 0, NI_MAXSERV); //clean

        int ret = getnameinfo((struct sockaddr *)&client, sizeof(client), host, NI_MAXHOST, svc, NI_MAXSERV, 0);
        if (ret) {
            std::cout << host << " connected on " << svc << std::endl;
        } else {
            inet_ntop(AF_INET, &client.sin_addr, host, NI_MAXHOST);
            std::cout << host << " connected on " << ntohs(client.sin_port) << std::endl; 
        }
        //while receiving [display message]
        char buffer[4096];
        while(true) {
            // clear the buffer
            memset(buffer, 0, 4096);
            // wait
            int byteRecv = recv(clientSocket, buffer, 4096, 0);
            if (byteRecv == -1) {
                std::cerr << "Connection issues" << std::endl;
                break;
            }
            if (byteRecv == 0) {
                std::cout << "Client dissconected" << std::endl;
                break;
            }
            // display
            std::cout << "Recived: " << std::string(buffer, 0, byteRecv) << std::endl;
            // resend
            send(clientSocket, buffer, byteRecv + 1, 0);
        }
        //close socket 
        close(clientSocket);
    } else {
        std::cerr << "Wrong number of arguments" << std::endl;
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}
