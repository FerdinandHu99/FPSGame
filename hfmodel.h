#ifndef HFMODEL_H
#define HFMODEL_H

#include <QObject>
#include <QVector>
#include <QString>
#include <QDir>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <QOpenGLShaderProgram>
#include <hfmesh.h>
#include <hftype.h>
class HFModel : public QObject
{
    Q_OBJECT

public:
    explicit HFModel(QString filePath, QOpenGLContext* context, QOpenGLShaderProgram* shaderProgram, QObject *parent = nullptr);
    ~HFModel();
    void draw();                                  // 绘制模型

private:
    QOpenGLContext*       m_context;              // OpenGL的上下文属性
    QOpenGLShaderProgram* m_shaderProgram;        // 着色器程序
    QDir                  m_directory;            // 模型文件目录
    QVector<HFMesh*>      m_meshs;                // 储存模型的所有网格
    QVector<Texture*>     m_texturesLoaded;       // 已加载成功材质贴图

    void     processNode(aiNode* node, const aiScene* scene); // 处理模型的节点
    HFMesh*  processMesh(aiMesh* mesh, const aiScene* scene); // 处理模型的网格（顶点数据、索引数据、材质贴图）
    QVector<Texture*> loadMaterialTextures(aiMaterial* material, aiTextureType type, QString typeName); // 从文件中加载材质的贴图
signals:

public slots:
};

#endif // HFMODEL_H
