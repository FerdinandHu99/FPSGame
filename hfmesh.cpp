#include "hfmesh.h"

HFMesh::HFMesh(QVector<Vertex> vertices, QVector<unsigned int> indices, QVector<Texture*> textures,
               QOpenGLShaderProgram* shaderProgram, QOpenGLFunctions* glFunc, QObject *parent)
    : QObject(parent), m_VBO(QOpenGLBuffer::VertexBuffer), m_EBO(QOpenGLBuffer::IndexBuffer)
{
    // 数据初始化（顶点数据、索引数据、贴图数据、着色器程序、OpenGL函数指针）
    m_vertices      = vertices;
    m_indices       = indices;
    m_textures      = textures;
    m_shaderProgram = shaderProgram;
    m_glFunc        = glFunc;
    setupMesh();

}

void HFMesh::draw()
{
    // 处理纹理数据
    unsigned int diffuseNr = 1;
    unsigned int specularNr = 1;

    // 设置贴图
    for (unsigned int i = 0; i < m_textures.size(); i++) {
        m_glFunc->glActiveTexture(GL_TEXTURE0 + i);
        QString number;
        QString name = m_textures[i]->type;
        if (name == "texture_diffuse") {
            number = QString::number(diffuseNr++);
        } else if (name == "texture_specular") {
            number = QString::number(specularNr++);
        }
        m_textures[i]->texture.bind();
        m_shaderProgram->setUniformValue((name+number).toStdString().c_str(), i);
    }


    // 绘制网格
    m_VAO.bind();
    m_shaderProgram->bind();
    m_glFunc->glDrawElements(GL_TRIANGLES, m_indices.size(), GL_UNSIGNED_INT, 0);
    m_VAO.release();
    m_shaderProgram->release();

    // 将所有纹理清除绑定
    for (unsigned int i = 0; i < m_textures.size(); i++) {
        m_glFunc->glActiveTexture(GL_TEXTURE0 + i);
        m_glFunc->glBindTexture(GL_TEXTURE_2D, 0);
    }
}

void HFMesh::setupMesh()
{
    m_VAO.create();
    m_VBO.create();
    m_EBO.create();

    m_VAO.bind();
    m_VBO.bind();
    m_VBO.allocate(m_vertices.data(), m_vertices.size() * sizeof (Vertex));
    m_VBO.setUsagePattern(QOpenGLBuffer::StaticDraw);
    m_EBO.bind();
    m_EBO.allocate(m_indices.data(), m_indices.size() * sizeof (unsigned int));
    m_EBO.setUsagePattern(QOpenGLBuffer::StaticDraw);

    m_shaderProgram->bind();

    m_shaderProgram->setAttributeBuffer(0, GL_FLOAT, 0, 3, sizeof (Vertex));
    m_shaderProgram->enableAttributeArray(0);

    m_shaderProgram->setAttributeBuffer(1, GL_FLOAT, offsetof(Vertex, Normal), 3, sizeof (Vertex));
    m_shaderProgram->enableAttributeArray(1);

    m_shaderProgram->setAttributeBuffer(2, GL_FLOAT, offsetof(Vertex, TexCoords), 2, sizeof (Vertex));
    m_shaderProgram->enableAttributeArray(2);

    m_VAO.release();
}
