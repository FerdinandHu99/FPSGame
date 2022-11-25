#ifndef HFMESH_H
#define HFMESH_H

#include <QObject>
#include <QVector>
#include <QOpenGLExtraFunctions>
#include <QOpenGLShaderProgram>
#include <QOpenGLVertexArrayObject>
#include <QOpenGLBuffer>
#include <QOpenGLTexture>
#include <hftype.h>
class HFMesh : public QObject
{
    Q_OBJECT
public:
    explicit HFMesh(QVector<Vertex> vertices, QVector<unsigned int> indices, QVector<Texture*> textures, QOpenGLShaderProgram* shaderProgram,
                    QOpenGLFunctions* glFunc, QObject *parent = nullptr);

    void draw();                                              // 绘制单个Mesh

private:
    QOpenGLVertexArrayObject m_VAO;
    QOpenGLBuffer m_VBO;
    QOpenGLBuffer m_EBO;

    QVector<Vertex>       m_vertices;                         // 顶点数据（顶点坐标、法线向量、纹理坐标）
    QVector<unsigned int> m_indices;                          // 索引数据
    QVector<Texture*>     m_textures;                         // 材质贴图数据
    QOpenGLFunctions*     m_glFunc;                           // OpenGL函数入口
    QOpenGLShaderProgram* m_shaderProgram;                    // 着色器程序

    void setupMesh();

signals:

public slots:
};

#endif // HFMESH_H
