#ifndef HFOPENGLWIGET_H
#define HFOPENGLWIGET_H

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
#include <hfcamera.h>
class HFOpenGLWiget : public QOpenGLWidget, protected QOpenGLExtraFunctions
{
    Q_OBJECT
public:
    explicit HFOpenGLWiget(QWidget *parent = nullptr);
    ~HFOpenGLWiget();


protected:
    virtual void initializeGL() override;
    virtual void resizeGL(int w, int h) override;
    virtual void paintGL() override;
    void keyPressEvent(QKeyEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void wheelEvent(QWheelEvent *event) override;
private:
    QOpenGLShaderProgram m_shaderProgram;
    QOpenGLVertexArrayObject m_VAO;
    QOpenGLBuffer m_VBO;
    QOpenGLBuffer m_EBO;
    QOpenGLTexture m_texture;
    QOpenGLTexture m_texture1;

    // 帧率计算
    QTime m_time;
    int m_lastTime;
    int m_deltaTime;

    // 鼠标控制
    QPoint m_lastMousePoint;

    // 声明一个摄像头
    HFCamera m_camera;
};

#endif // HFOPENGLWIGET_H
