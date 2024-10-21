#include "studywidget.h"
#include <QPushButton>
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
    letterDescriptions["A"] = "Make a fist with the thumb resting on the side of the other fingers.";
    letterDescriptions["B"] = "Hold the palm outward with the four fingers extended together and the thumb bent across the palm.";
    letterDescriptions["C"] = "Curve your fingers and palm to form the shape of the letter C facing outward.";
    letterDescriptions["D"] = "Extend the index finger upward while the other fingers form a fist with the thumb resting over the bent middle and ring fingers.";
    letterDescriptions["E"] = "Curl the fingers into a fist with the fingertips touching the palm, and place the thumb over the front of the fingers.";
    letterDescriptions["F"] = "Touch the thumb and index finger to form a circle, while the other three fingers are extended straight outward.";
    letterDescriptions["G"] = "Extend the index finger and thumb parallel to the ground, with the other fingers curled in.";
    letterDescriptions["H"] = "Extend the index and middle fingers together while the palm faces inward and the remaining fingers form a fist.";
    letterDescriptions["I"] = "Extend the pinky finger while the other fingers are in a fist, with the thumb resting on the side of the fist.";
    letterDescriptions["J"] = "Extend the pinky finger and curve it downward in a J motion, while the other fingers form a fist.";
    letterDescriptions["K"] = "Extend the index and middle fingers together, with the thumb resting between them, while the other fingers form a fist.";
    letterDescriptions["L"] = "Extend the thumb and index finger to form the shape of the letter L, while the other fingers are curled in.";
    letterDescriptions["M"] = "Form a fist, with the thumb tucked under the three fingers (index, middle, and ring fingers).";
    letterDescriptions["N"] = "Form a fist, with the thumb tucked under two fingers (index and middle fingers).";
    letterDescriptions["O"] = "Curve all fingers so that the fingertips touch, forming the shape of the letter O.";
    letterDescriptions["P"] = "Touch the thumb and index finger together at the tips, with the middle finger extended downward and the other fingers curled in.";
    letterDescriptions["Q"] = "Form a circle with the thumb and index finger, and extend the middle finger forward, while the other fingers are curled in.";
    letterDescriptions["R"] = "Cross the index and middle fingers, with the remaining fingers curled into a fist.";
    letterDescriptions["S"] = "Form a fist with the thumb resting over the front of the bent fingers.";
    letterDescriptions["T"] = "Form a fist with the thumb tucked between the index and middle fingers.";
    letterDescriptions["U"] = "Extend the index and middle fingers together, with the remaining fingers curled into a fist.";
    letterDescriptions["V"] = "Extend the index and middle fingers in a V shape, while the remaining fingers form a fist.";
    letterDescriptions["W"] = "Extend the index, middle, and ring fingers, spreading them apart, while the thumb and pinky are curled in.";
    letterDescriptions["X"] = "Curl the index finger to form a hook shape, while the remaining fingers form a fist.";
    letterDescriptions["Y"] = "Extend the pinky and thumb, while the other fingers are curled into the palm.";
    letterDescriptions["Z"] = "Extend the index finger and draw the shape of the letter Z in the air.";

    // 創建其他控件
    imageLabel = new QLabel(this);
    imageLabel->setAlignment(Qt::AlignCenter);  // 圖片居中

    descriptionLabel = new QLabel(this);
    descriptionLabel->setAlignment(Qt::AlignCenter);  // 描述居中
    descriptionLabel->setStyleSheet("color: white; font-size: 16px;");
    descriptionLabel->setFixedSize(500, 120);  // 设置固定大小，宽度600，高度120
    descriptionLabel->setWordWrap(true);  // 启用自动换行


    letterMenu = new QListWidget(this);
    letterMenu->setFixedWidth(200);  // 将字母栏宽度调整为150
    letterMenu->setFixedHeight(400); // 限制字母栏的高度
    letterMenu->setVisible(true);

    // 創建 NEXT 和 LAST 按鈕
    nextButton = new QPushButton("NEXT", this);
    nextButton->setFixedSize(150, 60);  // 调整按钮尺寸
    nextButton->setStyleSheet("QPushButton {"
                              "background-color: #88C0D0;"
                              "color: white;"
                              "border-radius: 10px;"
                              "font-size: 16px;"
                              "padding: 10px;"
                              "}");

    lastButton = new QPushButton("Previous", this);
    lastButton->setFixedSize(150, 60);  // 调整按钮尺寸
    lastButton->setStyleSheet("QPushButton {"
                              "background-color: #88C0D0;"
                              "color: white;"
                              "border-radius: 10px;"
                              "font-size: 16px;"
                              "padding: 10px;"
                              "}");

    // 添加字母到選單
    QStringList letters = {"A", "B", "C", "D", "E", "F", "G", "H", "I", "J", "K", "L", "M", "N", "O", "P", "Q", "R", "S", "T", "U", "V", "W", "X", "Y", "Z"};
    for (const QString &letter : letters) {
        QListWidgetItem *item = new QListWidgetItem(letter, letterMenu);
        item->setFlags(item->flags() | Qt::ItemIsUserCheckable);  // 启用复选框
        item->setCheckState(Qt::Unchecked);  // 复选框初始为未选中
        letterMenu->addItem(item);
    }

    // 美化字母選單
    letterMenu->setStyleSheet("QListWidget {"
                              "background-color: #4C566A;"
                              "border: 1px solid #D8DEE9;"
                              "border-radius: 5px;"
                              "color: white;"
                              "font-size: 24px;"  // 字母字体大小设为24px
                              "padding: 10px;"
                              "margin-left: 15px;"  // 调整字母栏距离左边的距离
                              "}"
                              "QListWidget::item:selected {"
                              "background-color: #88C0D0;"
                              "color: black;"
                              "}");

    descriptionLabel->setStyleSheet("QLabel {"
                                    "border: 2px solid #88C0D0;"  // 边框颜色和粗细
                                    "border-radius: 10px;"        // 圆角边框
                                    "background-color: #4C566A;"  // 背景颜色
                                    "color: white;"               // 文本颜色
                                    "padding: 10px;"              // 内边距，避免文字紧贴边框
                                    "font-size: 16px;"            // 字体大小
                                    "}");


    // 預設顯示字母 A
    layout->addWidget(imageLabel, 0, 0, 1, 1, Qt::AlignCenter);  // 圖片
    layout->addWidget(descriptionLabel, 1, 0, 1, 1, Qt::AlignCenter);  // 說明
    layout->addWidget(letterMenu, 0, 1, 2, 1, Qt::AlignRight);  // 字母选单向左并垂直居中

    // 将 LAST 和 NEXT 按钮放在字母栏下方，横向排列
    QHBoxLayout *buttonLayout = new QHBoxLayout;
    buttonLayout->addWidget(lastButton);
    buttonLayout->addWidget(nextButton);

    layout->addLayout(buttonLayout, 2, 1, Qt::AlignCenter);  // 按钮横向居中排列在字母栏下方

    setLayout(layout);
    setWindowTitle("Signify");
    setFixedSize(810, 720);
    this->setStyleSheet("background-color: #2E3440;");  // 窗口背景

    // 信號槽
    connect(letterMenu, &QListWidget::itemClicked, this, &StudyWidget::showLetterPage);

    // 监听复选框的点击
    connect(letterMenu, &QListWidget::itemChanged, this, [=](QListWidgetItem *item) {
        if (item->checkState() == Qt::Checked) {
            qDebug() << item->text() << " 已被打勾";
        } else {
            qDebug() << item->text() << " 未被打勾";
        }
    });

    // 顯示預設字母 A
    showLetterPage(letterMenu->item(0));

    // NEXT 按鈕點擊後顯示下一個字母
    connect(nextButton, &QPushButton::clicked, this, [=]() {
        int currentRow = letterMenu->currentRow();
        if (currentRow < letterMenu->count() - 1) {
            letterMenu->setCurrentRow(currentRow + 1);
            showLetterPage(letterMenu->currentItem());
        }
    });

    // LAST 按鈕點擊後顯示上一個字母
    connect(lastButton, &QPushButton::clicked, this, [=]() {
        int currentRow = letterMenu->currentRow();
        if (currentRow > 0) {
            letterMenu->setCurrentRow(currentRow - 1);
            showLetterPage(letterMenu->currentItem());
        }
    });
}

StudyWidget::~StudyWidget() {}

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
