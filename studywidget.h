#ifndef STUDYWIDGET_H
#define STUDYWIDGET_H

#include <QWidget>
#include <QGridLayout>
#include <QCamera>
#include <QMediaCaptureSession>
#include <QVideoWidget>
#include <QListWidgetItem>
#include <QLabel>
#include <QPushButton>


class StudyWidget : public QWidget
{
    Q_OBJECT

public:
    StudyWidget(QWidget *parent = nullptr);
    ~StudyWidget();

private slots:
    void openCameraWindow(); // 添加攝像頭窗口的槽函數
    void showLetterPage(QListWidgetItem *item);  // 显示字母页面

private:
    QGridLayout *layout;
    QCamera *camera;
    QMediaCaptureSession captureSession;
    QVideoWidget *videoWidget;
    QLabel *imageLabel;  // 显示图片
    QLabel *descriptionLabel;  // 显示说明
    QListWidget *letterMenu;  // 滚动字母菜
    QPushButton *startButton;
    QPushButton *nextButton;
    QMap<QString, QString> letterDescriptions;  // 儲存字母與說明的映射
};

#endif // STUDYWIDGET_H
