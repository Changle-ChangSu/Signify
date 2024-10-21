#ifndef WIDGET_H
#define WIDGET_H
// #include "./ui_widget.h"
#include <QWidget>
#include <QLabel>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QApplication>
#include <QLocale>
#include <QTranslator>
#include <QPushButton>
#include <QProgressBar>
#include <QPixmap>
#include <QPainter>
#include <QSettings>

class RecordWidget : public QWidget
{
    Q_OBJECT

public:
    explicit RecordWidget(QWidget *parent = nullptr);  // 构造函数声明
    ~RecordWidget();  // 析构函数声明

    void updateLearningProgressAndBestScore(int progress, int total, int score);  // 函数声明

private:
    // Ui::Widget *ui;  // UI 对象
    QLabel *learningProgressText;  // 学习进度的固定文本
    QLabel *learningProgressValue; // 学习进度的动态数值
    QLabel *bestScoreText;         // 最高分的固定文本
    QLabel *bestScoreValue;        // 最高分的动态数值
    QVBoxLayout *rightInfoLayout;  // 用于社交图标的布局
    QHBoxLayout *mainLayout;       // 主布局
};

#endif // WIDGET_H
