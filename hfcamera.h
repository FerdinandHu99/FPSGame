#ifndef HFCAMERA_H
#define HFCAMERA_H

#include <QObject>
#include <QVector3D>

class HFCamera : public QObject
{
    Q_OBJECT
public:
    explicit HFCamera(QObject *parent = nullptr, QVector3D position = QVector3D(0, 0, 6), QVector3D orientation = QVector3D(0, 0, -1), QVector3D up = QVector3D(0, 1, 0));
    ~HFCamera();

    QVector3D position;
    QVector3D orientation; // 摄像机拍摄的方向
    QVector3D up;
    float Yaw;
    float Pitch;

    // 按键控制速度
    float keyControlSpeed;
signals:

public slots:
};

#endif // HFCAMERA_H
