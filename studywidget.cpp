#include "studywidget.h"
#include <QPushButton>
#include <QCamera>
#include <QVideoWidget>
#include <QVBoxLayout>
#include <QListWidgetItem>
#include <QLabel>
#include <QGridLayout>
#include <QListWidget>

StudyWidget::StudyWidget(QWidget *parent)
    : QWidget(parent)
{
    layout = new QGridLayout(this);

    // 初始化字母對應的說明
    letterDescriptions["A"] = "這是字母 A 的說明...";
    letterDescriptions["B"] = "這是字母 B 的說明...";
    letterDescriptions["C"] = "這是字母 C 的說明...";
    letterDescriptions["D"] = "這是字母 D 的說明...";
    letterDescriptions["E"] = "這是字母 E 的說明...";
    letterDescriptions["F"] = "這是字母 F 的說明...";
    letterDescriptions["G"] = "這是字母 G 的說明...";
    letterDescriptions["H"] = "這是字母 H 的說明...";
    letterDescriptions["I"] = "這是字母 I 的說明...";
    letterDescriptions["J"] = "這是字母 J 的說明...";
    letterDescriptions["K"] = "這是字母 K 的說明...";
    letterDescriptions["L"] = "這是字母 L 的說明...";
    letterDescriptions["M"] = "這是字母 M 的說明...";
    letterDescriptions["N"] = "這是字母 N 的說明...";
    letterDescriptions["O"] = "這是字母 O 的說明...";
    letterDescriptions["P"] = "這是字母 P 的說明...";
    letterDescriptions["Q"] = "這是字母 Q 的說明...";
    letterDescriptions["R"] = "這是字母 R 的說明...";
    letterDescriptions["S"] = "這是字母 S 的說明...";
    letterDescriptions["T"] = "這是字母 T 的說明...";
    letterDescriptions["U"] = "這是字母 U 的說明...";
    letterDescriptions["V"] = "這是字母 V 的說明...";
    letterDescriptions["W"] = "這是字母 W 的說明...";
    letterDescriptions["X"] = "這是字母 X 的說明...";
    letterDescriptions["Y"] = "這是字母 Y 的說明...";
    letterDescriptions["Z"] = "這是字母 Z 的說明...";


    // 創建 Start 按鈕
    startButton = new QPushButton("Start", this);
    startButton->setFixedSize(200, 100);
    startButton->setStyleSheet("QPushButton {"
                               "background-color: #FF5555;"
                               "color: white;"
                               "border-radius: 10px;"
                               "font-size: 18px;"
                               "padding: 10px;"
                               "}"
                               "QPushButton:hover {"
                               "background-color: #FF7777;"
                               "}");

    // 把按鈕添加到佈局
    layout->addWidget(startButton, 0, 0, 1, 2, Qt::AlignCenter);  // 佈局中占據整行

    setLayout(layout);
    setWindowTitle("Signify");
    setFixedSize(810, 720);
    this->setStyleSheet("background-color: #2E3440;");  // 窗口背景

    // 創建其他控件
    imageLabel = new QLabel(this);
    imageLabel->setAlignment(Qt::AlignCenter);  // 圖片居中

    descriptionLabel = new QLabel(this);
    descriptionLabel->setAlignment(Qt::AlignCenter);  // 描述居中
    descriptionLabel->setStyleSheet("color: white; font-size: 16px;");

    letterMenu = new QListWidget(this);
    letterMenu->setFixedWidth(200);  // 將字母選單寬度設為120
    letterMenu->setVisible(false);  // 初始隱藏字母選單

    // 創建 NEXT 按鈕
    nextButton = new QPushButton("NEXT", this);
    nextButton->setVisible(false);  // 初始隱藏 NEXT 按鈕
    nextButton->setFixedWidth(120);  // NEXT 按鈕寬度與字母選單一致
    nextButton->setStyleSheet("QPushButton {"
                              "background-color: #88C0D0;"
                              "color: white;"
                              "border-radius: 10px;"
                              "font-size: 16px;"
                              "padding: 10px;"  // 按鈕內邊距
                              "}");

    // 添加字母到選單
    QStringList letters = {"A", "B", "C", "D", "E", "F", "G", "H", "I", "J", "K", "L", "M", "N", "O", "P", "Q", "R", "S", "T", "U", "V", "W", "X", "Y", "Z"};
    for (const QString &letter : letters) {
        letterMenu->addItem(new QListWidgetItem(letter));
    }

    // 美化字母選單
    letterMenu->setStyleSheet("QListWidget {"
                              "background-color: #4C566A;"
                              "border: 1px solid #D8DEE9;"
                              "border-radius: 5px;"
                              "color: white;"
                              "font-size: 16px;"
                              "padding: 10px;"
                              "}"
                              "QListWidget::item:selected {"
                              "background-color: #88C0D0;"
                              "color: black;"
                              "}");

    // 信號槽
    connect(letterMenu, &QListWidget::itemClicked, this, &StudyWidget::showLetterPage);
    connect(startButton, &QPushButton::clicked, this, [=]() {
        openCameraWindow();
        layout->removeWidget(startButton);
        startButton->deleteLater();

        // 顯示字母選單與 NEXT 按鈕
        letterMenu->setVisible(true);
        nextButton->setVisible(true);

        // 調整圖片居中，字母選單在右側，NEXT 按鈕在下方
        layout->addWidget(imageLabel, 0, 0, 1, 1, Qt::AlignCenter);  // 第一列
        layout->addWidget(descriptionLabel, 1, 0, 1, 1, Qt::AlignCenter);  // 第二列
        layout->addWidget(letterMenu, 0, 1, 2, 1, Qt::AlignRight);  // 字母選單居右
        layout->addWidget(nextButton, 2, 1, Qt::AlignCenter);  // NEXT 按鈕居中

        // 預設顯示字母 A
        showLetterPage(letterMenu->item(0));
    });

    // NEXT 按鈕點擊後顯示下一個字母
    connect(nextButton, &QPushButton::clicked, this, [=]() {
        int currentRow = letterMenu->currentRow();
        if (currentRow < letterMenu->count() - 1) {
            letterMenu->setCurrentRow(currentRow + 1);
            showLetterPage(letterMenu->currentItem());
        }
    });
}

StudyWidget::~StudyWidget() {}

void StudyWidget::openCameraWindow()
{
    // 創建顯示攝像頭的窗口
    QWidget *cameraWindow = new QWidget;
    cameraWindow->setWindowTitle("Camera View");
    cameraWindow->setFixedSize(640, 480);

    QVideoWidget *videoWidget = new QVideoWidget(cameraWindow);
    QVBoxLayout *cameraLayout = new QVBoxLayout(cameraWindow);
    cameraLayout->addWidget(videoWidget);

    QCamera *camera = new QCamera(this);

    // 設置攝像頭輸出
    captureSession.setCamera(camera);
    captureSession.setVideoOutput(videoWidget);

    // 開始捕獲
    camera->start();
    cameraWindow->setLayout(cameraLayout);
    cameraWindow->show();
}

void StudyWidget::showLetterPage(QListWidgetItem *item)
{
    QString letter = item->text();
    QString imagePath = ":/image/image/" + letter + ".png";  // 資源文件路徑

    // 加載圖片
    QPixmap pixmap(imagePath);
    if (!pixmap.isNull()) {
        imageLabel->setPixmap(pixmap.scaled(300, 300, Qt::KeepAspectRatio));  // 調整圖片大小
    } else {
        qDebug() << "圖片加載失敗：" << imagePath;
    }

    // 更新說明
    if (letterDescriptions.contains(letter)) {
        descriptionLabel->setText(letterDescriptions[letter]);
    } else {
        descriptionLabel->setText("找不到這個字母的說明。");
    }

    // 刷新佈局
    layout->update();
}
