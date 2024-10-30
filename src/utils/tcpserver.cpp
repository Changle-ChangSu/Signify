#include "tcpserver.h"

// Constructor
// Initializes the server with a specified port, setting initial socket values to invalid.
TcpServer::TcpServer(int port) : server_port(port), listen_socket(INVALID_SOCKET), client_socket(INVALID_SOCKET) {}

// Initializes Winsock and starts the server on the specified port
bool TcpServer::startServer() {
    // Initialize Winsock
    WSADATA wsa_data;
    if (WSAStartup(MAKEWORD(2, 2), &wsa_data) != 0) {
        std::cerr << "Error: WSAStartup failed" << std::endl;
        return false;
    }

    // Create a socket for listening to client connections
    listen_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (listen_socket == INVALID_SOCKET) {
        std::cerr << "Error: Unable to create socket" << std::endl;
        WSACleanup();
        return false;
    }

    // Bind the socket to the server address and specified port
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(server_port);
    server_addr.sin_addr.s_addr = INADDR_ANY;  // Accept connections from any IP address

    if (bind(listen_socket, (struct sockaddr*)&server_addr, sizeof(server_addr)) == SOCKET_ERROR) {
        std::cerr << "Error: Bind failed" << std::endl;
        closesocket(listen_socket);
        WSACleanup();
        return false;
    }

    // Start listening for incoming connections
    if (listen(listen_socket, 1) == SOCKET_ERROR) {
        std::cerr << "Error: Listen failed" << std::endl;
        closesocket(listen_socket);
        WSACleanup();
        return false;
    }

    std::cout << "Server is listening on port " << server_port << std::endl;

    // Accept a connection from the client
    client_socket = accept(listen_socket, nullptr, nullptr);
    if (client_socket == INVALID_SOCKET) {
        std::cerr << "Error: Accept failed" << std::endl;
        closesocket(listen_socket);
        WSACleanup();
        return false;
    }

    std::cout << "Client connected!" << std::endl;
    return true;
}

// Receives data from the connected client
std::string TcpServer::receiveData() {
    char buffer[1024];
    memset(buffer, 0, sizeof(buffer));
    int bytes_received = recv(client_socket, buffer, sizeof(buffer) - 1, 0);
    if (bytes_received <= 0) {
        std::cerr << "Error: Failed to receive data. Camera seems to have been closed." << std::endl;
        return "";
    }

    buffer[bytes_received] = '\0';  // Null-terminate the received string
    return std::string(buffer);
}

// Closes the connection and cleans up Winsock resources
void TcpServer::closeConnection() {
    if (client_socket != INVALID_SOCKET) {
        closesocket(client_socket);
    }
    if (listen_socket != INVALID_SOCKET) {
        closesocket(listen_socket);
    }
    WSACleanup();
}

// Destructor to ensure all resources are released
TcpServer::~TcpServer() {
    closeConnection();
}
