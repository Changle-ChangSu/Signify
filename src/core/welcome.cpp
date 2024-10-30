#include "welcome.h"

/**
 * @brief Constructs a Welcome object, sets up the UI, initializes a background thread, and prepares connections.
 * @param parent The parent widget, default is nullptr for an independent window.
 */
Welcome::Welcome(QWidget *parent)
    : QWidget(parent), workerThread(new QThread(this)), dataReceiver(new DataReceiver()) {
    setWindowTitle("Welcome Page");
    setFixedSize(810, 720);  // Sets a fixed window size

    // Create layout for the Welcome page
    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->setAlignment(Qt::AlignCenter); // Center alignment for all widgets
    layout->setSpacing(20); // Sets spacing between widgets

    // Welcome message label
    lblWelcome = new QLabel("Welcome to use Signify！", this);
    lblWelcome->setAlignment(Qt::AlignCenter);
    lblWelcome->setStyleSheet("font-size: 30px;");
    layout->addWidget(lblWelcome);

    // Application purpose description label
    QLabel *lblDescription = new QLabel("We aim to teach you how to use the most basic sign language letters, "
                                        "so that you can easily and quickly communicate in sign language with those in need!", this);
    lblDescription->setAlignment(Qt::AlignCenter);
    lblDescription->setWordWrap(true);
    lblDescription->setStyleSheet("font-size: 18px;");
    lblDescription->setMinimumWidth(640);
    layout->addWidget(lblDescription);

    // Additional instructions for using the camera
    QLabel *lblInstruction = new QLabel("Turn the camara on to begin!", this);
    lblInstruction->setAlignment(Qt::AlignCenter);
    lblInstruction->setWordWrap(true);
    lblInstruction->setStyleSheet("font-size: 18px;");
    lblInstruction->setMinimumWidth(640);
    layout->addWidget(lblInstruction);

    // Label for Additional Notice
    QLabel *lblCameraInfo = new QLabel("Notice: Opening the gesture recognition model can take up to 30 seconds.", this);
    lblCameraInfo->setAlignment(Qt::AlignCenter);
    lblCameraInfo->setStyleSheet("font-size: 12px;");
    layout->addWidget(lblCameraInfo);

    // Camera activation button
    btnCamera = new QPushButton("Camera🎥", this);
    btnCamera->setFixedSize(200, 50);
    btnCamera->setStyleSheet(
        "QPushButton {"
        "background-color: red;"
        "color: white;"
        "border-radius: 25px;"
        "font-size: 20px;"
        "}"
        "QPushButton:hover {"
        "background-color: darkred;"
        "}"
        );
    layout->addWidget(btnCamera, 0, Qt::AlignCenter);

    // Connect camera button to its activation slot
    connect(btnCamera, &QPushButton::clicked, this, &Welcome::openCameraWindow);

    // Moves the data receiver to a separate thread
    dataReceiver->moveToThread(workerThread);

    // Connects thread's started signal to start the data reception in the background
    connect(workerThread, &QThread::started, dataReceiver, &DataReceiver::startReceiving);

    connect(dataReceiver, &DataReceiver::camaraClosed, this, &Welcome::stopWorkerThread);

    // --------------------Developer mode----------------------

    btnFastStart = new QPushButton("🤓Developer Mode: 🔧Manual Start", this);
    btnFastStart->setFixedSize(280, 35);
    btnFastStart->setStyleSheet(
        "QPushButton {"
        "background-color: green;"
        "color: white;"
        "border-radius: 12px;"
        "font-size: 14px;"
        "}"
        "QPushButton:hover {"
        "background-color: darkgreen;"
        "}"
        );
    layout->addWidget(btnFastStart, 0, Qt::AlignCenter);

    // Connect developer mode button to its activation slot
    connect(btnFastStart, &QPushButton::clicked, this, &Welcome::openDeveloperMode);

    setLayout(layout);

    process = new QProcess(this);
    program = QCoreApplication::applicationDirPath() + "/model/dist/model/model.exe";
}


/**
 * @brief Slot to open the camera window and initiate the model process.
 * Starts the `model.exe` program in the main thread and launches the data receiver thread.
 */
void Welcome::openCameraWindow() {
    if (process->state() == QProcess::Running) {
        QMessageBox msgBox;
        msgBox.setWindowTitle("Model is Running");
        msgBox.setText("The gesture recognition model is running. The model cannot be started repeatedly");
        msgBox.setStandardButtons(QMessageBox::Ok);
        msgBox.setIcon(QMessageBox::Warning);
        msgBox.exec();
    } else {
        QMessageBox msgBox;
        msgBox.setWindowTitle("Notice");
        msgBox.setText("To close the camera, please switch to the camera interface and press 'q'.");
        // msgBox.setInformativeText("Note: If you successfully run the model.py file, click OK to proceed. If nothing goes wrong, the camara will be automatically turned on.");
        msgBox.setStandardButtons(QMessageBox::Ok);
        msgBox.setIcon(QMessageBox::Information);
        msgBox.exec();

        // Try to open model.exe application
        startModelProcessInMainThread();
        workerThread->start();
    }

}

/**
 * @brief Slot to use a faster start method to open the camara window. Developer needs to manually open and run the model.py file.
 * Starts the `model.exe` program in the main thread and launches the data receiver thread.
 */
void Welcome::openDeveloperMode() {
    if (process->state() == QProcess::Running) {
        QMessageBox msgBox;
        msgBox.setWindowTitle("Model is Running");
        msgBox.setText("The gesture recognition model is running. The model cannot be started repeatedly.");
        msgBox.setStandardButtons(QMessageBox::Ok);
        msgBox.setIcon(QMessageBox::Warning);
        msgBox.exec();
    } else {
        QString pyProgram = QCoreApplication::applicationDirPath() + "/model/model.py";
        QMessageBox msgBox;
        msgBox.setWindowTitle("Developer Mode Manual Start Instruction");
        msgBox.setText(QString("For testing: \nMake sure you have already set an environment of Python 3.12.7 complier, and have installed opencv and ultralytics packages. \n\nAfter clicking the OK button below, open '%1' and run the program under the environmet you have set.").arg(pyProgram));
        // msgBox.setInformativeText("Note: If you successfully run the model.py file, click OK to proceed. If nothing goes wrong, the camara will be automatically turned on.");
        msgBox.setStandardButtons(QMessageBox::Ok);
        msgBox.setIcon(QMessageBox::Information);
        msgBox.exec();

        workerThread->start();
    }
}

/**
 * @brief Attempts to start the `model.exe` process in the main thread and displays a waiting dialog until the process starts.
 *
 * This function creates a dialog that informs the user to wait while `model.exe` starts.
 * If the process starts successfully, the dialog closes. Otherwise, an error message is displayed.
 */
void Welcome::startModelProcessInMainThread() {
    // Create a modal dialog to inform the user to wait
    QDialog *dialog = new QDialog(this);
    dialog->setWindowTitle("Please wait");
    QVBoxLayout *layout = new QVBoxLayout(dialog);
    QLabel *label = new QLabel("Starting model.exe, please wait...", dialog);
    layout->addWidget(label);
    dialog->setLayout(layout);
    dialog->setWindowModality(Qt::WindowModal);  // Set modal window
    dialog->show();

    process = new QProcess(this); // Ensure process is initialized before connecting signals

    // If the process starts successfully, close the dialog
    connect(process, &QProcess::started, [=]() {
        qDebug() << "model.exe started successfully.";
        dialog->accept();
    });

    // If there is an error starting the process, display an error message
    connect(process, &QProcess::errorOccurred, [=]() {
        qDebug() << "Failed to start model.exe";
        dialog->reject();
        QMessageBox::critical(this, "Error", QString("Failed to start model.exe.\n Please ensure that the path is correct: \n '%1'").arg(program));
    });

    // Start the process
    process->start(program);

    // Check if the process started successfully
    if (!process->waitForStarted()) {
        qDebug() << "Failed to start model.exe";
    }

    emit processStarted(process);  // Emit signal to pass the process to MainWindow
}

/**
 * @brief Returns a pointer to the `DataReceiver` object.
 * @return A pointer to the `DataReceiver` instance, used to access data receiving functionality.
 */
DataReceiver* Welcome::getDataReceiver() const {
    return dataReceiver;  // returns a pointer to DataReceiver object
}

void Welcome::stopWorkerThread(){
    workerThread->quit();
    workerThread->wait();
}

/**
 * @brief Destructor to ensure proper termination of the background thread and clean-up.
 *
 * Stops the background thread for data reception and releases resources.
 */
Welcome::~Welcome() {
    workerThread->quit();
    workerThread->wait();
    delete dataReceiver;
}
