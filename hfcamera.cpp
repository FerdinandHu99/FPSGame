#include "hfcamera.h"

HFCamera::HFCamera(QObject *parent, QVector3D position, QVector3D orientation, QVector3D worldUp) : QObject(parent)
{
    m_position          = position;
    m_orientation       = orientation;
    m_worldUp           = worldUp;
    m_Yaw               = -90.0f;
    m_Pitch             = 0.0f;
    m_Fov               = 45.0f;
    m_keyControlSpeed   = 0.01f;
    m_mouseControlSpeed = 0.1f;

    // 开始更新摄像机的向量
    updateCameraVectors();
}

HFCamera::~HFCamera()
{

}

void HFCamera::processMouseMovement(float xOffset, float yOffset)
{

}

void HFCamera::processKeyboard(HFCamera::KeyMovement direction, float deltaTime)
{
    float velocity = m_keyControlSpeed * deltaTime;
    if (direction == FORWARD)  m_position += m_orientation * velocity;
    if (direction == BACKWARD) m_position -= m_orientation * velocity;
    if (direction == LEFT)     m_position -= m_right * velocity;
    if (direction == RIGHT)    m_position += m_right * velocity;
}

// 处理鼠标滚轮事件
void HFCamera::processMouseWheel(float wheelValue)
{
    if (m_Fov >= 1.0f && m_Fov <= 60.0f) m_Fov -= wheelValue/120.0f;
    if (m_Fov < 1.0f)                    m_Fov = 1.0f;
    if (m_Fov > 60.0f)                   m_Fov = 60.0f;
}

QMatrix4x4 HFCamera::getViewMatrix()
{
    QMatrix4x4 viewMatrix;
    viewMatrix.lookAt(m_position, m_position + m_orientation, m_worldUp);
    return  viewMatrix;
}

float HFCamera::Fov()
{
    return m_Fov;
}

void HFCamera::setFov(float Fov)
{
    m_Fov = Fov;
}

// 将角度换算成弧度
float HFCamera::radians(float angle)
{
    return angle * M_PI / 180.0f;
}

void HFCamera::updateCameraVectors()
{
    QVector3D orientation;
    orientation.setX(cos(radians(m_Yaw)) * cos(radians(m_Pitch)));
    orientation.setY(sin(radians(m_Pitch)));
    orientation.setZ(sin(radians(m_Yaw)) * cos(radians(m_Pitch)));


    m_orientation = orientation.normalized();
    m_right = QVector3D::crossProduct(m_orientation, m_worldUp).normalized();
    m_up = QVector3D::crossProduct(m_right, m_orientation).normalized();
}
