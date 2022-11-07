#include "hfopenglwiget.h"

HFOpenGLWiget::HFOpenGLWiget(QWidget *parent) : QOpenGLWidget (parent), m_VBO(QOpenGLBuffer::VertexBuffer), m_EBO(QOpenGLBuffer::IndexBuffer)
{

}

HFOpenGLWiget::~HFOpenGLWiget()
{

}

void HFOpenGLWiget::initializeGL()
{
    initializeOpenGLFunctions();

    GLfloat vertices[] = {
        -0.5f,  0.5f, 0.0f,
         0.5f,  0.5f, 0.0f,
         0.5f, -0.5f, 0.0f,
        -0.5f, -0.5f, 0.0f
    };
    unsigned int indices[] = {
        0, 1, 2,
        2, 3, 0
    };

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

    // 创建Shader程序并编译
    m_shaderProgram.create();
    m_shaderProgram.addShaderFromSourceFile(QOpenGLShader::Vertex, ":/shader/drawtriangle.vert");
    m_shaderProgram.addShaderFromSourceFile(QOpenGLShader::Fragment, ":/shader/drawtriangle.frag");
    m_shaderProgram.link();
    m_shaderProgram.bind();

    //GLint positionLoc = m_shaderProgram.attributeLocation("position");
    // 设置读取规则：位置 类型 偏移 长度 步长
    m_shaderProgram.setAttributeBuffer(0, GL_FLOAT, 0, 3, 0);
    m_shaderProgram.enableAttributeArray(0);

    m_VAO.release();
    m_VBO.release();
    m_EBO.release();
    m_shaderProgram.release();
}

void HFOpenGLWiget::resizeGL(int w, int h)
{
    glViewport(0, 0, w, h);
}

void HFOpenGLWiget::paintGL()
{
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    m_shaderProgram.bind();
    m_VAO.bind();
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    m_VAO.release();
    m_shaderProgram.release();
}
