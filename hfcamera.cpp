#include "hfcamera.h"

HFCamera::HFCamera(QObject *parent, QVector3D position, QVector3D orientation, QVector3D up) : QObject(parent)
{
    this->position = position;
    this->up = up;
    this->orientation = orientation;
    this->keyControlSpeed = 0.08f;
}

HFCamera::~HFCamera()
{

}
