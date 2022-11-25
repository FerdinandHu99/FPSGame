#include "hfopenglwidget.h"

HFOpenGLWidget::HFOpenGLWidget(QWidget *parent) : QOpenGLWidget(parent)

{
    this->setFocusPolicy(Qt::StrongFocus);                       // 获得键盘焦点
    this->setMouseTracking(true);                                // 启用鼠标跟踪
    this->setCursor(Qt::BlankCursor);                            // 取消光标显示
    // 时间开始
    m_lastTime = 0;
    m_time.start();

    createFPS_label(); // 创建FPS显示控件
}

HFOpenGLWidget::~HFOpenGLWidget()
{
    makeCurrent();

    delete model;

    doneCurrent();
}

void HFOpenGLWidget::initializeGL()
{
    initializeOpenGLFunctions();
    glEnable(GL_DEPTH_TEST); // 开启深度测试
    // 将鼠标初始位置设为中间位置
    m_centerMousePoint = QPoint(width() / 2, height() / 2);

    m_shaderProgram.create();
    m_shaderProgram.addShaderFromSourceFile(QOpenGLShader::Vertex, ":/Shaders/model.vert");
    m_shaderProgram.addShaderFromSourceFile(QOpenGLShader::Fragment, ":/Shaders/model.frag");

    QString filePath = QString("D:/Qt_Projects/KingGuns/Models/container/container.obj");
    model = new HFModel(filePath, context(), &m_shaderProgram);

}

void HFOpenGLWidget::resizeGL(int w, int h)
{
    glViewport(0, 0, w, h);
}

void HFOpenGLWidget::paintGL()
{

    // 计算帧率并显示
    int currentTime = m_time.elapsed();
    m_deltaTime = currentTime - m_lastTime;
    int FPS = (1.0 / m_deltaTime) * 1000;
    m_lastTime = currentTime;
    FPS_label->setText(QString::number(FPS));

    // 处理键盘事件
    keyBoardProcess();

    // 清空并更新背景颜色
    glClearColor(0.05f, 0.05f, 0.05f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    m_shaderProgram.bind();
    // 模型矩阵
    QMatrix4x4 modelMatrix;
    m_shaderProgram.setUniformValue("model", modelMatrix);

    // 观察矩阵
    QMatrix4x4 viewMatrix;
    viewMatrix = m_camera.getViewMatrix();
    m_shaderProgram.setUniformValue("view", viewMatrix);

    // 投影矩阵
    QMatrix4x4 projectionMatrix;
    projectionMatrix.perspective(m_camera.Fov(), static_cast<float>(width()/height()), 0.1f, 100.0f);
    m_shaderProgram.setUniformValue("projection", projectionMatrix);

    model->draw();

    update();
}

// 接收键盘按下事件
void HFOpenGLWidget::keyPressEvent(QKeyEvent *event)
{
    int key = event->key();
    if (key == Qt::Key_Escape) {
        emit EscBtnPressed();  // 当Esc按下时，发射信号给主窗口
    }
    if (key >= 0 && key < 1024) {
        if (event->type() == QEvent::KeyPress) {
            keys[key] = true;
        }
    }
}

// 接收键盘松开事件
void HFOpenGLWidget::keyReleaseEvent(QKeyEvent *event)
{
    int key = event->key();
    if (key >= 0 && key < 1024) {
        if (event->type() == QEvent::KeyRelease) {
            keys[key] = false;
        }
    }
}

// 键盘事件处理函数（实现多个按键同时按下功能）
void HFOpenGLWidget::keyBoardProcess()
{
    if (keys[Qt::Key_W] == true) m_camera.processKeyboard(HFCamera::FORWARD, m_deltaTime);
    if (keys[Qt::Key_S] == true) m_camera.processKeyboard(HFCamera::BACKWARD, m_deltaTime);
    if (keys[Qt::Key_A] == true) m_camera.processKeyboard(HFCamera::LEFT, m_deltaTime);
    if (keys[Qt::Key_D] == true) m_camera.processKeyboard(HFCamera::RIGHT, m_deltaTime);
}

// 接收鼠标移动事件
void HFOpenGLWidget::mouseMoveEvent(QMouseEvent *event)
{
    QPoint currentMousePoint = event->pos();
    QPoint deltaMousePoint = currentMousePoint - m_centerMousePoint;
    if (event->buttons() == Qt::MidButton) {
        m_camera.processMouseMidBtnMovement(deltaMousePoint.y());
        QPoint center = this->mapToGlobal(m_centerMousePoint);
        QCursor::setPos(center);
    } else {
        m_camera.processMouseMovement(deltaMousePoint.x(), deltaMousePoint.y());
        QPoint center = this->mapToGlobal(m_centerMousePoint);
        QCursor::setPos(center);
    }
}

// 鼠标滚轮事件
void HFOpenGLWidget::wheelEvent(QWheelEvent *event)
{
    m_camera.processMouseWheel(event->angleDelta().y());
}

void HFOpenGLWidget::createFPS_label()
{
    FPS_label = new QLabel(this);
    FPS_label->setGeometry(QRect(10, 10, 41, 31));
    QFont font1;
    font1.setPointSize(15);
    FPS_label->setFont(font1);
    FPS_label->setAlignment(Qt::AlignCenter);
    FPS_label->setStyleSheet(QString("QLabel{color:rgb(255,0,0)}"));
}

