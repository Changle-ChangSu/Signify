#include "studywidget.h"

StudyWidget::StudyWidget(QWidget *parent)
    : QWidget(parent)
{
    layout = new QGridLayout(this);

    // 设置布局的边距
    layout->setContentsMargins(50, 50, 50, 50);

    setLayout(layout); // 设置主布局
    setWindowTitle("Signify");
    setFixedSize(810, 720); // 设置窗口固定大小
}

StudyWidget::~StudyWidget()
{
}
