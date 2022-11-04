#ifndef FOPENGLWIDGET_H
#define FOPENGLWIDGET_H

#include <QOpenGLWidget>
#include <QOpenGLExtraFunctions>
#include <QOpenGLShaderProgram>
#include <QOpenGLVertexArrayObject>
#include <QOpenGLBuffer>

class FOpenGLWidget : public QOpenGLWidget, protected QOpenGLExtraFunctions
{
    Q_OBJECT
public:
    explicit FOpenGLWidget(QWidget *parent = nullptr);
    ~FOpenGLWidget();
protected:
    virtual void initializeGL() override;
    virtual void paintGL() override;
    virtual void resizeGL(int width, int height) override;
private:
    QOpenGLShaderProgram shaderProgram;
};

#endif // FOPENGLWIDGET_H
