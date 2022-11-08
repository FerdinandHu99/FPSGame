#include "hfopenglwiget.h"

HFOpenGLWiget::HFOpenGLWiget(QWidget *parent) : QOpenGLWidget (parent), m_VBO(QOpenGLBuffer::VertexBuffer), m_EBO(QOpenGLBuffer::IndexBuffer),
                                                m_texture(QOpenGLTexture::Target2D), m_texture1(QOpenGLTexture::Target2D)
{

}

HFOpenGLWiget::~HFOpenGLWiget()
{
    makeCurrent();
//    m_VAO.destroy();
//    m_EBO.destroy();
//    m_VBO.destroy();
//    m_texture.destroy();
//    doneCurrent();
}

void HFOpenGLWiget::initializeGL()
{
    initializeOpenGLFunctions();

    GLfloat vertices[] = {
        -0.5f,  0.5f, 0.0f, 0.0f, 1.0f,
         0.5f,  0.5f, 0.0f, 1.0f, 1.0f,
         0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
        -0.5f, -0.5f, 0.0f, 0.0f, 0.0f
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
    // 清空并更新背景颜色
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    m_shaderProgram.bind();

    // 绑定并使用纹理0
    m_texture.bind(0);
    GLint ourTextureLoc = m_shaderProgram.uniformLocation("ourTexture");
    m_shaderProgram.setUniformValue(ourTextureLoc, 0);
    // 绑定并使用纹理1
    m_texture1.bind(1);
    GLint ourTextureLoc1 = m_shaderProgram.uniformLocation("ourTexture1");
    m_shaderProgram.setUniformValue(ourTextureLoc1, 1);

    m_VAO.bind();
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
    m_VAO.release();
    m_shaderProgram.release();
}
