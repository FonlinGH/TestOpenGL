#ifndef MYOPENGLWIDGET_H
#define MYOPENGLWIDGET_H

#include <QWidget>
#include <QOpenGLWidget>
#include <QOpenGLFunctions_3_3_Core>
#include <QOpenGLShaderProgram>
#include <QOpenGLTexture>
#include <QTimer>
#include <QKeyEvent>
#include <QMouseEvent>
#include "Camera.h"

class MyOpenGLWidget : public QOpenGLWidget,QOpenGLFunctions_3_3_Core
{
    Q_OBJECT
public:
    enum Shape{None,Rect,Circle,Triangle,Cube};
    explicit MyOpenGLWidget(QWidget *parent = nullptr);
    ~MyOpenGLWidget();

    void drawShape(Shape shape);
    void setWireFrame(bool mode);

protected:
    virtual void initializeGL();
    virtual void resizeGL(int w,int h);
    virtual void paintGL();
    void keyPressEvent(QKeyEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
signals:

public slots:
    void on_timeout();

private:
    Shape m_shape;
    QOpenGLShaderProgram shaderProgram;
    QOpenGLTexture *m_block_texture;
    QOpenGLTexture *m_block_texture2;
    QOpenGLTexture *m_smile_texture;
    QTimer m_timer;

    Camera m_camera;
};

#endif // MYOPENGLWIDGET_H
