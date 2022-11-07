#ifndef HFOPENGLWIGET_H
#define HFOPENGLWIGET_H

#include <QOpenGLWidget>
#include <QOpenGLExtraFunctions>
#include <QOpenGLShaderProgram>
#include <QOpenGLVertexArrayObject>
#include <QOpenGLBuffer>

class HFOpenGLWiget : public QOpenGLWidget, protected QOpenGLExtraFunctions
{
    Q_OBJECT
public:
    explicit HFOpenGLWiget(QWidget *parent = nullptr);
    ~HFOpenGLWiget();

protected:
    virtual void initializeGL();
    virtual void resizeGL(int w, int h);
    virtual void paintGL();
private:
    QOpenGLShaderProgram m_shaderProgram;
    QOpenGLVertexArrayObject m_VAO;
    QOpenGLBuffer m_VBO;
    QOpenGLBuffer m_EBO;
};

#endif // HFOPENGLWIGET_H
