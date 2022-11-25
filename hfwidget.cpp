#include "hfwidget.h"
#include "ui_hfwidget.h"

HFWidget::HFWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::HFWidget)
{
    ui->setupUi(this);
    intiUi();
}

HFWidget::~HFWidget()
{
    delete ui;
    delete openGLWidget;
}

void HFWidget::showMenu(bool isShow)
{
    if (isShow) {
        ui->background->show();
        ui->gameQuitBtn->show();
        ui->gameStartBtn->show();
        ui->gameSettingBtn->show();
    } else {
        ui->background->hide();
        ui->gameQuitBtn->hide();
        ui->gameStartBtn->hide();
        ui->gameSettingBtn->hide();
    }
}

void HFWidget::_EscBtn_pressed()
{
    // 显示游戏菜单
    showMenu(true);

    // 隐藏游戏界面
    openGLWidget->close();
}

void HFWidget::on_gameStartBtn_clicked()
{
    // 隐藏菜单界面
    showMenu(false);

    // 创建新游戏
    openGLWidget = new HFOpenGLWidget(this);
    openGLWidget->setGeometry(QRect(0, 0, 1280, 720));
    openGLWidget->show();
    connect(openGLWidget, SIGNAL(EscBtnPressed()), this, SLOT(_EscBtn_pressed()));
}

void HFWidget::on_gameQuitBtn_clicked()
{
    this->close();
}

void HFWidget::intiUi()
{
    // 设置标题和Icon
    setWindowTitle("枪王之王");
    setWindowIcon(QIcon(":/Images/gun.png"));

    // 设置背景图片
    QImage img(":/Images/background.jpeg");
    img.scaled(ui->background->size(), Qt::KeepAspectRatio);
    ui->background->setGeometry(this->geometry());
    ui->background->setPixmap(QPixmap::fromImage(img));

}


