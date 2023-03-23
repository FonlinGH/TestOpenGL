#ifndef CAMERA_H
#define CAMERA_H

#include <vector>
#include <QMatrix4x4>
#include <qopengl.h>
#include <math.h>

enum Camera_Movement{
    FORWARD,
    BACKWARD,
    LEFT,
    RIGHT
};

const float YAW = -90.0f;
const float PITCH = 0.0f;
const float SPEED = 2.5f;
const float SENSITIVITY = 0.1f;
const float ZOOM = 45.0f;

class Camera
{
public:
    QVector3D Position;
    QVector3D Front;
    QVector3D Up;
    QVector3D Right;
    QVector3D WorldUp;
    float Yaw;
    float Pitch;
    float MovementSpeed;
    float MouseSensitivity;
    float Zoom;

    Camera(QVector3D position = QVector3D(0.0f,0.0f,0.0f),QVector3D up = QVector3D(0.0f,1.0f,0.0f),float yaw=YAW,float pitch=PITCH):
        Front(QVector3D(0.0f,0.0f,-1.0f)),
        MovementSpeed(SPEED),
        MouseSensitivity(SENSITIVITY),
        Zoom(ZOOM)
    {
        Position=position;
        WorldUp=up;
        Yaw=yaw;
        Pitch=pitch;
        updateCameraVectors();
    }

    Camera(float posX, float posY, float posZ, float upX, float upY, float upZ, float yaw, float pitch):
        Front(QVector3D(0.0f, 0.0f, -1.0f)),
        MovementSpeed(SPEED),
        MouseSensitivity(SENSITIVITY),
        Zoom(ZOOM)
    {
        Position = QVector3D(posX, posY, posZ);
        WorldUp = QVector3D(upX, upY, upZ);
        Yaw = yaw;
        Pitch = pitch;
        updateCameraVectors();
    }

    QMatrix4x4 getViewMatrix()
    {
        QMatrix4x4 theMatrix;
        theMatrix.lookAt(Position,Position+Front,Up);
        return theMatrix;
    }

    void processKeybord(Camera_Movement direction,float deltaTime)
    {
        float velocity = MovementSpeed * deltaTime;
        if(direction==FORWARD)
            Position+=Front*velocity;
        if(direction==BACKWARD)
            Position-=Front*velocity;
        if(direction==LEFT)
            Position-=Right*velocity;
        if(direction==RIGHT)
            Position+=Right*velocity;
    }

    void processMouseMovement(float xoffset,float yoffset,GLboolean constrainPitch=true)
    {
        xoffset *= MouseSensitivity;
        yoffset *= MouseSensitivity;

        Yaw += xoffset;
        Pitch += yoffset;

        if(constrainPitch){
            if(Pitch > 89.0f)
                Pitch = 89.0f;
            if(Pitch < -89.0f)
                Pitch = -89.0f;
        }

        updateCameraVectors();
    }

    void processMouseScroll(float yoffset)
    {
        Zoom -= (float)yoffset;
        if(Zoom < 1.0f)
            Zoom = 1.0f;
        if(Zoom > 75.0f)
            Zoom = 75.0f;
    }

private:
    void updateCameraVectors()
    {
        float PI=3.1415926;
        QVector3D front;
        front.setX(cos(Yaw*PI/180.0)*cos(Pitch*PI/180.0));
        front.setY(sin(Pitch*PI/180.0));
        front.setZ(sin(Yaw*PI/180.0)*cos(Pitch*PI/180.0));
        front.normalize();
        Front=front;
        Right = QVector3D::crossProduct(Front, WorldUp);
        Right.normalize();
        Up = QVector3D::crossProduct(Right, Front);
        Up.normalize();
    }
};

#endif // CAMERA_H
