// Depreicated class

#ifndef HOMEPAGE_H
#define HOMEPAGE_H

#include <QWidget>
#include <QPushButton>
#include <QVBoxLayout>
#include <QLabel>
// #include "gamewidget.h" // 包含GameWidget头文件

class HomePage : public QWidget
{
    Q_OBJECT

public:
    HomePage(QWidget *parent = nullptr);
    // ~HomePage();

signals:
    void practiceClicked();  // 信号，用于切换到Practice页面

private slots:
    void openPracticePage(); // 槽函数，用于跳转到Practice页面

private:
    QPushButton *btnStudy;
    QPushButton *btnPractice;
    QPushButton *btnPersonalRecord;

    QLabel *lblFooter;
};

#endif // HOMEPAGE_H
