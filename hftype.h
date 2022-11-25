#ifndef HFTYPE_H
#define HFTYPE_H

#include <QVector3D>
#include <QVector2D>
#include <QString>
#include <QOpenGLTexture>
/* 该文件定义了一些数据类型 ：顶点结构体、纹理结构体 */

struct Vertex {
    QVector3D Position;
    QVector3D Normal;
    QVector2D TexCoords;
};


struct Texture
{
    QString name;             // 材质贴图名称(例如：container_diff.jpg)
    QString type;             // 材质贴图类型（漫反射贴图、镜面反射贴图）
    QOpenGLTexture texture;   // QOpenGL贴图类

    // 结构体构造函数
    Texture() : texture(QOpenGLTexture::Target2D) {
        texture.create();
        texture.setWrapMode(QOpenGLTexture::DirectionS,QOpenGLTexture::Repeat);
        texture.setWrapMode(QOpenGLTexture::DirectionT,QOpenGLTexture::Repeat);
        texture.setMinMagFilters(QOpenGLTexture::LinearMipMapLinear,QOpenGLTexture::Linear);
    }
};


#endif // HFTYPE_H
