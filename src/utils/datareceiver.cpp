#include "datareceiver.h"

// Constructor for DataReceiver class, initializing members to nullptr.
DataReceiver::DataReceiver(QObject *parent) : QObject(parent), server(nullptr), process(nullptr), serverThread(nullptr) {}

// Destructor for DataReceiver class, ensuring proper cleanup of threads and TCP connections.
DataReceiver::~DataReceiver() {
    // Stop and clean the thread
    if (serverThread) {
        serverThread->quit();
        serverThread->wait();
        delete serverThread;
    }

    if (server) {
        server->closeConnection();  // close TCP connection
        delete server;
    }
}

/**
 * @brief Starts the data receiving process by connecting to the Python server and setting up a timer.
 *
 * This function initializes the TCP connection to the Python server, sets up a QTimer to check
 * for incoming data every 20 milliseconds, and connects the timer to the `checkForData` function.
 */
void DataReceiver::startReceiving() {
    // qDebug() << "Start Receiving";
    connectToPythonServer();

    // Set up a timer to periodically check for data
    timer = new QTimer(this);
    timer->start(20);  // Checks every 20 milliseconds
    connect(timer, &QTimer::timeout, this, &DataReceiver::checkForData);
}

/**
 * @brief Establishes a TCP connection to the Python server.
 *
 * This function creates a TcpServer instance and starts the server on a specified port.
 */
void DataReceiver::connectToPythonServer() {
    server = new TcpServer(12345);  // Initialize TcpServer object with the target port
    onServerStarted();
}

/**
 * @brief Starts listening on the server thread.
 *
 * This function initiates the server's listening state and outputs debug information on the success
 * or failure of the operation.
 */
void DataReceiver::onServerStarted() {
    // Start listening on the server thread
    if (server->startServer()) {
        qDebug() << "Server started successfully, listening on port 12345.";
    } else {
        qDebug() << "Failed to start the server.";
        serverThread->quit();
        serverThread->wait();
    }
}

/**
 * @brief Periodically checks for data from the Python server.
 *
 * This function checks if there is any data available from the Python server. If data is received,
 * it emits the `handSignDetected` signal with the received data. If no data is available, it stops
 * the timer, closes the connection, and emits a `cameraClosed` signal.
 */
void DataReceiver::checkForData() {
    if (server) {
        std::string data = server->receiveData();

        if (data.empty()) {  // If no more data, stop checking
            timer->stop();

            if (serverThread) {  // Stop and clean up the server thread
                serverThread->quit();
                serverThread->wait();
            }
            if (server) {  // Close TCP connection
                server->closeConnection();
            }
            emit camaraClosed();  // Emit signal indicating the camera has closed
            return;
        }

        QString qData = QString::fromStdString(data);
        qDebug() << "Received data: " << qData[0];

        // Emit signal to pass the QString data to other components
        emit handSignDetected(qData[0]);
    }
}
