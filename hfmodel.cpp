#include "hfmodel.h"

HFModel::HFModel(QString filePath, QOpenGLContext* context, QOpenGLShaderProgram* shaderProgram, QObject *parent) : QObject(parent),
                 m_context(context), m_shaderProgram(shaderProgram), m_directory(filePath)
{
    Assimp::Importer import;
    const aiScene* scene = import.ReadFile(m_directory.absolutePath().toLocal8Bit(), aiProcess_Triangulate | aiProcess_FlipUVs);
    if (!scene) {
        qDebug() << "ERROR::ASSIMP::" << import.GetErrorString() << endl;
        return;
    }
    this->m_directory.cdUp(); // 返回上一级目录
    this->processNode(scene->mRootNode, scene);
}

HFModel::~HFModel()
{

}

void HFModel::draw()
{

    for (HFMesh* mesh:m_meshs) {
        mesh->draw();
    }

}


void HFModel::processNode(aiNode *node, const aiScene *scene)
{
    // 添加当前节点中的所有mesh
    for (unsigned int i = 0; i < node->mNumMeshes; i++) {
        aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
        this->m_meshs.push_back(processMesh(mesh, scene));
    }
    // 递归处理该节点的子节点
    for (unsigned int i = 0; i < node->mNumChildren; i++) {
        this->processNode(node->mChildren[i], scene);
    }

}

HFMesh* HFModel::processMesh(aiMesh *mesh, const aiScene *scene)
{

    QVector<Vertex>       vertices;   // 顶点数据
    QVector<unsigned int> indices;    // 索引数据
    QVector<Texture*>     textures;   // 贴图数据

    // 处理当前Mesh的所有顶点数据
    for (unsigned int i = 0; i < mesh->mNumVertices; i++) {
        Vertex vertex;
        QVector3D vector;
        // 位置
        vector.setX(mesh->mVertices[i].x);
        vector.setY(mesh->mVertices[i].y);
        vector.setZ(mesh->mVertices[i].z);
        vertex.Position = vector;
        // 法线
        vector.setX(mesh->mNormals[i].x);
        vector.setY(mesh->mNormals[i].y);
        vector.setZ(mesh->mNormals[i].z);
        vertex.Normal = vector;
        // 纹理坐标
        if (mesh->mTextureCoords[0]) { // 如果该Mesh顶点数据含有纹理坐标
            QVector2D vec;
            vec.setX(mesh->mTextureCoords[0][i].x);
            vec.setY(mesh->mTextureCoords[0][i].y);
            vertex.TexCoords = vec;
        } else {
            vertex.TexCoords = QVector2D(0.0f, 0.0f);
        }
        vertices.push_back(vertex);
    }

    // 处理当前Mesh的所有索引数据
    for (unsigned int i = 0; i < mesh->mNumFaces; i++) {
        aiFace face = mesh->mFaces[i];
        for (unsigned int j = 0; j < face.mNumIndices; j++) {
            indices.push_back(face.mIndices[j]);
        }
    }

    // 处理材质
    if (mesh->mMaterialIndex >= 0) { // 如果当前Mesh存在材质
        aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
        // 加载漫反射贴图
        QVector<Texture*> diffuseMaps = loadMaterialTextures(material, aiTextureType_DIFFUSE, "texture_diffuse");
        for (auto &it:diffuseMaps) {
            textures.push_back(it);
        }

        // 加载镜面反射贴图
        QVector<Texture*> specularMaps = loadMaterialTextures(material, aiTextureType_SPECULAR, "texture_specular");
        for (auto &it:specularMaps) {
            textures.push_back(it);
        }
    }

    HFMesh* hfMesh = new HFMesh(vertices, indices, textures, m_shaderProgram, m_context->functions());
    return hfMesh;
}

QVector<Texture*> HFModel::loadMaterialTextures(aiMaterial *material, aiTextureType type, QString typeName)
{
    QVector<Texture*> textures;
    for (unsigned int i = 0; i < material->GetTextureCount(type); i++)
    {
        aiString textureName;
        material->GetTexture(type, i, &textureName);

        // 检查纹理是否在之前加载过
        bool skip = false;
        for (unsigned int j = 0; j < m_texturesLoaded.size(); j++) {
            if (std::strcmp(m_texturesLoaded[j]->name.toStdString().c_str(), textureName.C_Str()) == 0) {
                textures.push_back(m_texturesLoaded[j]);
                skip = true;  // 已从文件中加载该贴图
                break;
            }
        }
        if (!skip) {
            Texture* texture = new Texture;
            QImage data(m_directory.filePath(textureName.C_Str()));
            if (!data.isNull()) {
                //qDebug() << "加载成功" << texturePath.C_Str();
                texture->texture.setData(data);
                texture->type = typeName;
                texture->name = QString(textureName.C_Str());

                textures.push_back(texture);
                m_texturesLoaded.push_back(texture);

            } else {
                qDebug() << "纹理加载失败" << m_directory.filePath(textureName.C_Str());
            }
        }
    }

    return textures;
}
