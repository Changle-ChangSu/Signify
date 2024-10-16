#ifndef STUDYWIDGET_H
#define STUDYWIDGET_H

#include <QWidget>
#include <QGridLayout>

class StudyWidget : public QWidget
{
    Q_OBJECT

    QGridLayout *layout;

public:
    StudyWidget(QWidget *parent = nullptr);
    ~StudyWidget();
};

#endif // STUDYWIDGET_H
