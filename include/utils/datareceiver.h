/**
 * @file datareceiver.h
 * @class DataReceiver
 * @brief Manages TCP communication with an external Python server for hand-sign detection.
 *
 * The DataReceiver class establishes and maintains a TCP connection to a Python-based server,
 * receiving hand-sign data in real-time. The class operates on a separate thread to enable
 * non-blocking data reception, uses a timer to poll for new data, and emits signals to inform
 * other components when new data is received or the camera connection is closed.
 *
 * Key functionalities include:
 * - Initializing a TCP server connection to a Python-based server.
 * - Polling for incoming data and relaying it to the application.
 * - Emitting signals for received hand-sign data and closed camera connections.
 * - Properly managing thread and connection cleanup upon destruction.
 *
 * Usage:
 * - Instantiate and call `startReceiving()` to begin polling for data.
 * - Connect to the `handSignDetected` and `cameraClosed` signals to receive data and handle connection events.
 */


#ifndef DATARECEIVER_H
#define DATARECEIVER_H

#include <QWidget>
#include <QLabel>
#include <QVBoxLayout>
#include <QPushButton>
#include <QCamera>
#include <QVideoWidget>
#include <QMediaCaptureSession>
#include <QSpacerItem>
#include <QProcess>
#include <QMessageBox>
#include <cstdlib>
#include <QTimer>
#include <QString>
#include <QThread>
#include <QProgressDialog>
#include <QCoreApplication>
#include <QDebug>
#include "tcpserver.h"


class DataReceiver : public QObject {
    Q_OBJECT

public:
    explicit DataReceiver(QObject *parent = nullptr);
    ~DataReceiver();

    void startReceiving();

signals:
    void handSignDetected(const QString &letter);  // Used to transmit sign letters
    void startModelProcess();  // Used to notify the main thread to start model.exe
    void camaraClosed();

public slots:
    void checkForData();  // Timer slot function, used to receive data from Python
    void connectToPythonServer();
    void onServerStarted();

private:
    TcpServer *server;
    QProcess *process;
    QThread *serverThread;  // Background thread for running tcpserver
    QTimer *timer;
};

#endif // DATARECEIVER_H
