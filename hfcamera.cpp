#include "hfcamera.h"

HFCamera::HFCamera(QObject *parent, QVector3D position, QVector3D orientation, QVector3D up) : QObject(parent)
{
    this->position = position;
    this->up = up;
    this->orientation = orientation;
    this->keyControlSpeed = 0.08f;
    this->mouseControlSpeed = 0.1f;
    this->Yaw = -90;
    this->Pitch = 0;
    this->FOV = 45;
}

HFCamera::~HFCamera()
{

}

void HFCamera::processMouseMovement(QPoint deltaPoint)
{
    float xoffset = deltaPoint.x() * mouseControlSpeed;
    float yoffset = deltaPoint.y() * mouseControlSpeed;

    this->Yaw += xoffset;
    this->Pitch += yoffset;

    //qDebug() << "Yaw:" << Yaw;
    //qDebug() << "Pitch:" << Pitch;

    if (Pitch > 89.0f) Pitch = 89.0f;
    if (Pitch < -89.0f) Pitch = -89.0f;

    orientation.setX(cos(Yaw*M_PI/180.0) * cos(Pitch*M_PI/180.0));
    orientation.setY(sin(Pitch*M_PI/180.0));
    orientation.setZ(sin(Yaw*M_PI/180.0) * cos(Pitch*M_PI/180.0));

}
