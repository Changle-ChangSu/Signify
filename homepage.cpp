#include "homepage.h"
#include "gamewidget.h"
#include <QApplication>
#include <QFont>

HomePage::HomePage(QWidget *parent)
    : QWidget(parent)
{
    // 创建布局
    QVBoxLayout *layout = new QVBoxLayout(this);

    // 创建显示花体字 "Signify" 的标签
    QLabel *lblTitle = new QLabel("Signify", this);

    // 设置花体字体
    QFont font;
    font.setPointSize(24);  // 字体大小
    font.setItalic(true);   // 设置为花体
    lblTitle->setFont(font);
    lblTitle->setAlignment(Qt::AlignCenter); // 文本居中对齐

    // 创建按钮
    btnStudy = new QPushButton("Study", this);
    btnPractice = new QPushButton("Practice", this);
    btnPersonalRecord = new QPushButton("Personal Record", this);

    // 创建底部标签
    lblFooter = new QLabel("Learning and helping", this);
    lblFooter->setAlignment(Qt::AlignCenter); // 居中对齐

    // 将按钮添加到布局中，并居中显示
    layout->addWidget(lblTitle);  // 添加花体文字
    layout->addStretch();
    layout->addWidget(btnStudy);
    layout->addWidget(btnPractice);
    layout->addWidget(btnPersonalRecord);
    layout->addStretch();
    layout->addWidget(lblFooter);

    // 设置布局的边距
    layout->setContentsMargins(50, 50, 50, 50);

    setLayout(layout); // 设置主布局
    setWindowTitle("Signify - Home Page");
    setFixedSize(800, 800); // 设置窗口固定大小

    // 连接Practice按钮点击信号，发送 practiceClicked 信号
    // connect(btnPractice, &QPushButton::clicked, this, &HomePage::practiceClicked);
    connect(btnPractice, &QPushButton::clicked, this, &HomePage::openPracticePage);
}

// void HomePage::openPracticePage()
// {
//     // 实例化GameWidget，并显示
//     GameWidget *gameWidget = new GameWidget();
//     gameWidget->show();

//     // 隐藏当前页面
//     this->hide();
// }

void HomePage::openPracticePage() {
    // 发送前往Practice页面信号
    emit practiceClicked();
}


