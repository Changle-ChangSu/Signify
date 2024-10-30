/**
 * @class Welcome
 * @brief The Welcome class provides an introductory interface with instructions and options to start the camera and model process.
 *
 * The Welcome class is responsible for displaying a welcome screen with details about the application’s purpose,
 * usage instructions, and a button to activate the camera and our model. It manages the initialization of the model process and
 * the `DataReceiver` in a separate thread for receiving data asynchronously.
 *
 * ### Key Components
 * - `QLabel *lblWelcome` - Displays a welcome message.
 * - `QPushButton *btnCamera` - Button to activate the camera and start the model process.
 * - `QPushButton *btnFastStart` - Button to activate in a faster way. Need to open the model code file and run it manually.
 * - `QThread *workerThread` - Background thread for running `DataReceiver`.
 * - `DataReceiver *dataReceiver` - Object for handling data reception in the background thread.
 * - `QProcess *process` - Manages the external `model.exe` process.
 *
 * ### Main Methods
 * - `void openCameraWindow()` - Starts the camera and model process.
 * - `void startModelProcessInMainThread()` - Attempts to launch `model.exe` and displays a waiting dialog until it starts.
 * - `DataReceiver* getDataReceiver() const` - Returns a pointer to the `DataReceiver` object.
 *
 * ### Signals
 * - `void processStarted(QProcess *process)` - Emitted when the model process has started successfully.
 */


#ifndef WELCOME_H
#define WELCOME_H

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
#include "datareceiver.h"


class Welcome : public QWidget {
    Q_OBJECT

public:
    explicit Welcome(QWidget *parent = nullptr);
    ~Welcome();

    DataReceiver* getDataReceiver() const;  // Provides an interface to get the DataReceiver object
    void startModelProcessInMainThread();

private:
    QLabel *lblWelcome;
    QCamera *camera;
    QMediaCaptureSession captureSession;
    QVideoWidget *videoWidget;

    QPushButton *btnCamera;
    QPushButton *btnFastStart;

    QProcess *pythonProcess;
    QTimer *timer;           // Timer used to periodically check data

    QThread *workerThread;
    DataReceiver *dataReceiver;  // Data receiver, which is used to receive data in the background thread

    QProcess *process;  // the model.exe process

signals:
    void processStarted(QProcess *process);

private slots:
    void openCameraWindow();
    void openDeveloperMode();
    void stopWorkerThread();

};

#endif // WELCOME_H
