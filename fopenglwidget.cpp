#include "fopenglwidget.h"
#include "ui_fopenglwidget.h"
FOpenGLWidget::FOpenGLWidget(QWidget *parent) : QOpenGLWidget(parent)
{

}

FOpenGLWidget::~FOpenGLWidget()
{

}

void FOpenGLWidget::initializeGL()
{
    initializeOpenGLFunctions();
    glClearColor(0.0f, 0.5f, 0.9f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    shaderProgram.create();
    shaderProgram.addShaderFromSourceFile(QOpenGLShader::Vertex, ":/drawtriangle.vert");
    shaderProgram.addShaderFromSourceFile(QOpenGLShader::Fragment, ":/drawtriangle.frag");
    shaderProgram.link();
    shaderProgram.enableAttributeArray(0);
}

void FOpenGLWidget::paintGL()
{
    shaderProgram.bind();

    float vertices[] = {
         0.5f,  0.5f, 0.0f,
         0.5f, -0.5f, 0.0f,
        -0.5f,  0.5f, 0.0f,
        -0.5f, -0.5f, 0.0f
    };
    shaderProgram.setAttributeArray(0, GL_FLOAT, vertices, 3);
    glDrawArrays(GL_TRIANGLES, 0, 4);
}

void FOpenGLWidget::resizeGL(int width, int height)
{
    glViewport(0, 0, width, height);
}
