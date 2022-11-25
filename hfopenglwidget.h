#ifndef HFOPENGLWIDGET_H
#define HFOPENGLWIDGET_H

#include <QOpenGLWidget>
#include <QOpenGLExtraFunctions>
#include <QOpenGLShaderProgram>
#include <QOpenGLVertexArrayObject>
#include <QOpenGLBuffer>
#include <QOpenGLTexture>
#include <QMatrix4x4>
#include <QVector3D>
#include <QTime>
#include <QKeyEvent>
#include <QMouseEvent>
#include <QPoint>
#include <QtWidgets/QLabel>
#include <hfcamera.h>
#include <hfmodel.h>
class HFOpenGLWidget : public QOpenGLWidget, protected QOpenGLExtraFunctions
{
    Q_OBJECT
public:
    explicit HFOpenGLWidget(QWidget *parent = nullptr);
    ~HFOpenGLWidget();

    HFModel* model;

protected:
    virtual void initializeGL() override;
    virtual void resizeGL(int w, int h) override;
    virtual void paintGL() override;
    void keyPressEvent(QKeyEvent *event) override;
    void keyReleaseEvent(QKeyEvent *event) override;
    void keyBoardProcess();
    void mouseMoveEvent(QMouseEvent *event) override;
    void wheelEvent(QWheelEvent *event) override;
private:
    QOpenGLShaderProgram m_shaderProgram;
    // 帧率计算
    QTime m_time;
    int m_lastTime;
    int m_deltaTime;

    // 鼠标控制
    QPoint m_centerMousePoint;

    // 声明一个摄像头
    HFCamera m_camera;
    // 保存键盘状态的数组
    bool keys[100] = {false};

    /* 以下和UI相关的类 */
    QLabel* FPS_label; // 用于显示FPS数值
    void createFPS_label(); // 创建一个FPS_label并显示
signals:
    void EscBtnPressed();

};

#endif // HFOPENGLWIDGET_H
