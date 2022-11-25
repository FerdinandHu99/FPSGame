#ifndef HFWIDGET_H
#define HFWIDGET_H

#include <QWidget>
#include <QKeyEvent>
#include <QMediaPlayer>
#include <hfopenglwidget.h>
namespace Ui {
class HFWidget;
}

class HFWidget : public QWidget
{
    Q_OBJECT

public:
    explicit HFWidget(QWidget *parent = nullptr);
    ~HFWidget();
    void showMenu(bool isShow = true);
    HFOpenGLWidget *openGLWidget;

private slots:
    void on_gameStartBtn_clicked();
    void on_gameQuitBtn_clicked();

    // 自己定义的槽函数
    void _EscBtn_pressed();

private:
    Ui::HFWidget *ui;
    void intiUi();

};

#endif // HFWIDGET_H
