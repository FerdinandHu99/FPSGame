#include "hfopenglwiget.h"

HFOpenGLWiget::HFOpenGLWiget(QWidget *parent) : QOpenGLWidget (parent), m_VBO(QOpenGLBuffer::VertexBuffer), m_EBO(QOpenGLBuffer::IndexBuffer),
                                                m_texture(QOpenGLTexture::Target2D), m_texture1(QOpenGLTexture::Target2D)
{
    this->setFocusPolicy(Qt::StrongFocus);                       // 获得焦点
    this->setMouseTracking(true);                                // 启用鼠标跟踪
    this->setCursor(Qt::BlankCursor);                            // 关闭光标显示
    // 时间开始
    m_lastTime = 0;
    m_time.start();

}

HFOpenGLWiget::~HFOpenGLWiget()
{
    makeCurrent();
    m_VAO.destroy();
    m_EBO.destroy();
    m_VBO.destroy();
    m_texture.destroy();
    m_texture1.destroy();
    doneCurrent();
}

void HFOpenGLWiget::initializeGL()
{
    initializeOpenGLFunctions();
    glEnable(GL_DEPTH_TEST); // 开启深度测试
    // 将鼠标初始位置设为中间位置
    m_centerMousePoint = QPoint(width() / 2, height() / 2);

    GLfloat vertices[] = {
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
         0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
        -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

        -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
         0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f
    };
    unsigned int indices[] = {
        0, 1, 2,
        2, 3, 0
    };

    // 设置VAO VBO EBO
    m_VAO.create();
    m_VBO.create();
    m_EBO.create();
    m_VAO.bind();
    m_VBO.bind();
    m_EBO.bind();
    m_VBO.allocate(vertices, sizeof (vertices));
    m_VBO.setUsagePattern(QOpenGLBuffer::StaticDraw);
    m_EBO.allocate(indices, sizeof (indices));
    m_EBO.setUsagePattern(QOpenGLBuffer::StaticDraw);

    // 创建纹理
    m_texture.create();
    m_texture.setData(QImage(":/texture/Texture/container.jpg"));

    m_texture1.create();
    m_texture1.setData(QImage(":/texture/Texture/awesomeface.png"));

    // 创建Shader程序并编译
    m_shaderProgram.create();
    m_shaderProgram.addShaderFromSourceFile(QOpenGLShader::Vertex, ":/shader/drawtriangle.vert");
    m_shaderProgram.addShaderFromSourceFile(QOpenGLShader::Fragment, ":/shader/drawtriangle.frag");
    m_shaderProgram.link();
    m_shaderProgram.bind();

    // GLint positionLoc = m_shaderProgram.attributeLocation("position");
    // 设置读取规则：位置 类型 偏移 长度 步长
    m_shaderProgram.setAttributeBuffer(0, GL_FLOAT, 0, 3, 5 * sizeof (GL_FLOAT));
    m_shaderProgram.enableAttributeArray(0);

    m_shaderProgram.setAttributeBuffer(1, GL_FLOAT, 3 * sizeof (GL_FLOAT), 2, 5 * sizeof (GL_FLOAT));
    m_shaderProgram.enableAttributeArray(1);

    m_VAO.release();
    m_VBO.release();
    m_EBO.release();
    m_texture.release();
    m_texture1.release();
    m_shaderProgram.release();

}

void HFOpenGLWiget::resizeGL(int w, int h)
{
    glViewport(0, 0, w, h);
}

void HFOpenGLWiget::paintGL()
{
    // 计算帧率
    int currentTime = m_time.elapsed();
    m_deltaTime = currentTime - m_lastTime;
    int FPS = (1.0 / m_deltaTime) * 1000;
    m_lastTime = currentTime;

    // 处理键盘事件
    keyBoardProcess();

    // 清空并更新背景颜色
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    m_shaderProgram.bind();

    // 绑定并使用纹理0
    m_texture.bind(0);
    int ourTextureLoc = m_shaderProgram.uniformLocation("ourTexture");
    m_shaderProgram.setUniformValue(ourTextureLoc, 0);
    // 绑定并使用纹理1
    m_texture1.bind(1);
    int ourTextureLoc1 = m_shaderProgram.uniformLocation("ourTexture1");
    m_shaderProgram.setUniformValue(ourTextureLoc1, 1);

    // 模型矩阵
    QMatrix4x4 model;
    model.rotate(m_time.elapsed()/10, QVector3D(0, 1, 0));
    int modelLoc = m_shaderProgram.uniformLocation("model");
    m_shaderProgram.setUniformValue(modelLoc, model);

    // 观察矩阵
    QMatrix4x4 view;
    view = m_camera.getViewMatrix();
    int viewLoc = m_shaderProgram.uniformLocation("view");
    m_shaderProgram.setUniformValue(viewLoc, view);

    // 投影矩阵
    QMatrix4x4 projection;
    projection.perspective(m_camera.Fov(), static_cast<float>(width()/height()), 0.1f, 100.0f);
    int projectionLoc = m_shaderProgram.uniformLocation("projection");
    m_shaderProgram.setUniformValue(projectionLoc, projection);

    m_VAO.bind();
    glDrawArrays(GL_TRIANGLES, 0, 36);
    m_VAO.release();
    m_shaderProgram.release();

    update();
}

// 接收键盘按下事件
void HFOpenGLWiget::keyPressEvent(QKeyEvent *event)
{
    int key = event->key();
    // 如果按下Esc键，则退出应用
    if (key == Qt::Key_Escape) this->parentWidget()->close();
    if (key >= 0 && key < 1024) {
        if (event->type() == QEvent::KeyPress) {
            keys[key] = true;
        }
    }
}

// 接收键盘松开事件
void HFOpenGLWiget::keyReleaseEvent(QKeyEvent *event)
{
    int key = event->key();
    if (key >= 0 && key < 1024) {
        if (event->type() == QEvent::KeyRelease) {
            keys[key] = false;
        }
    }
}

// 键盘事件处理函数（实现多个按键同时按下功能）
void HFOpenGLWiget::keyBoardProcess()
{
    if (keys[Qt::Key_W] == true) m_camera.processKeyboard(HFCamera::FORWARD, m_deltaTime);
    if (keys[Qt::Key_S] == true) m_camera.processKeyboard(HFCamera::BACKWARD, m_deltaTime);
    if (keys[Qt::Key_A] == true) m_camera.processKeyboard(HFCamera::LEFT, m_deltaTime);
    if (keys[Qt::Key_D] == true) m_camera.processKeyboard(HFCamera::RIGHT, m_deltaTime);
}

// 接收鼠标移动事件
void HFOpenGLWiget::mouseMoveEvent(QMouseEvent *event)
{
    QPoint currentMousePoint = event->pos();
    QPoint deltaMousePoint = currentMousePoint - m_centerMousePoint;
    //m_lastMousePoint = currentMousePoint;
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
void HFOpenGLWiget::wheelEvent(QWheelEvent *event)
{
    m_camera.processMouseWheel(event->angleDelta().y());
}


