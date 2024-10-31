/**
 * @file tcpserver.h
 * @class TcpServer
 * @brief Establishes a TCP server for receiving data from a single client.
 *
 * The TcpServer class provides a simple TCP server interface for receiving data from
 * a client, specifically designed for receiving hand sign information from a client application.
 * This class handles setting up and managing socket connections, listening on a specified port,
 * and closing the connection and cleaning up Winsock resources upon completion.
 *
 * Key functionalities include:
 * - Initializing and binding a listening socket to a specified port.
 * - Listening for and accepting a client connection.
 * - Receiving data in a loop until the client disconnects.
 * - Safely closing the connection and cleaning up Winsock resources when complete.
 *
 * Typical usage:
 * 1. Create an instance with a specified port.
 * 2. Call `startServer()` to initialize Winsock and listen for a client connection.
 * 3. Use `receiveData()` to receive messages from the client.
 * 4. Call `closeConnection()` to end the connection, or let the destructor handle cleanup.
 */

#ifndef TCPSERVER_H
#define TCPSERVER_H

#include <string>
#include <QObject>
#include <iostream>
#include <cstring>  // 用于 memset
#include <winsock2.h>  // 使用 Winsock

#pragma comment(lib, "Ws2_32.lib")  // 链接 Winsock 库

class TcpServer : public QObject {  // 继承自 QObject
    Q_OBJECT  // 添加 Q_OBJECT 宏

public:
    // 构造函数
    TcpServer(int port);

    // 初始化 Winsock 和启动服务器
    bool startServer();

    // 接收数据
    std::string receiveData();

    // 关闭连接并清理 Winsock
    void closeConnection();

    // 析构函数，确保资源释放
    ~TcpServer();

private:
    int server_port;
    SOCKET listen_socket;
    SOCKET client_socket;
    struct sockaddr_in server_addr;
};

#endif  // TCPSERVER_H
