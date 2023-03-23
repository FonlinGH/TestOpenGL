#include "myopenglwidget.h"
#include <QTime>
#include <QDebug>
#include <math.h>

#define TIME_OUT_INTERVAL 100

unsigned int VBO[2],VAO[2],EBO;
float vertices[]={//position //color //texcord
    -0.5f,-0.5f,0.0f,1.0f,0.0f,0.0f,0.0f,0.0f,
    0.5f,-0.5f,0.0f,0.0f,1.0f,0.0f,1.0f,0.0f,
    0.0f,0.5f,0.0f,0.0f,0.0f,1.0f,0.5f,1.0f,
    -0.5f,0.5f,0.0f,0.0f,0.0f,0.0f,0.0f,1.0f,
    0.5f,0.5f,0.0f,1.0f,1.0f,1.0f,1.0f,1.0f
};
float cube[]={//position //texcord
    -0.5f,-0.5f,0.5f,0.0f,0.0f,//front
    -0.5f,0.5f,0.5f,0.0f,1.0f,
    0.5f,-0.5f,0.5f,1.0f,0.0f,
    0.5f,-0.5f,0.5f,1.0f,0.0f,
    -0.5f,0.5f,0.5f,0.0f,1.0f,
    0.5f,0.5f,0.5f,1.0f,1.0f,

    -0.5f,0.5f,0.5f,0.0f,0.0f,//up
    -0.5f,0.5f,-0.5f,0.0f,1.0f,
    0.5f,0.5f,0.5f,1.0f,0.0f,
    0.5f,0.5f,0.5f,1.0f,0.0f,
    -0.5f,0.5f,-0.5f,0.0f,1.0f,
    0.5f,0.5f,-0.5f,1.0f,1.0f,

    -0.5f,-0.5f,-0.5f,0.0f,0.0f,//back
    -0.5f,0.5f,-0.5f,0.0f,1.0f,
    0.5f,-0.5f,-0.5f,1.0f,0.0f,
    0.5f,-0.5f,-0.5f,1.0f,0.0f,
    -0.5f,0.5f,-0.5f,0.0f,1.0f,
    0.5f,0.5f,-0.5f,1.0f,1.0f,

    -0.5f,-0.5f,0.5f,0.0f,0.0f,//down
    -0.5f,-0.5f,-0.5f,0.0f,1.0f,
    0.5f,-0.5f,0.5f,1.0f,0.0f,
    0.5f,-0.5f,0.5f,1.0f,0.0f,
    -0.5f,-0.5f,-0.5f,0.0f,1.0f,
    0.5f,-0.5f,-0.5f,1.0f,1.0f,

    0.5f,-0.5f,0.5f,0.0f,0.0f,//right
    0.5f,0.5f,0.5f,0.0f,1.0f,
    0.5f,-0.5f,-0.5f,1.0f,0.0f,
    0.5f,-0.5f,-0.5f,1.0f,0.0f,
    0.5f,0.5f,0.5f,0.0f,1.0f,
    0.5f,0.5f,-0.5f,1.0f,1.0f,

    -0.5f,-0.5f,0.5f,0.0f,0.0f,//left
    -0.5f,0.5f,0.5f,0.0f,1.0f,
    -0.5f,-0.5f,-0.5f,1.0f,0.0f,
    -0.5f,-0.5f,-0.5f,1.0f,0.0f,
    -0.5f,0.5f,0.5f,0.0f,1.0f,
    -0.5f,0.5f,-0.5f,1.0f,1.0f,
};
QVector<QVector3D>cubePositions{
    QVector3D(0.0f,0.0f,0.0f),
    QVector3D(2.0f,5.0f,-15.0f),
    QVector3D(-1.5f,-2.2f,-2.5f),
    QVector3D(2.4f,-0.4f,-3.5f),
    QVector3D(-1.7f,3.0f,-7.5f),
};
unsigned int indices[]={
    0,1,3,
    1,3,4
};
float ratio=0.5;
int angle=0;
QPoint deltaPos;

MyOpenGLWidget::MyOpenGLWidget(QWidget *parent) : QOpenGLWidget(parent)
{
    connect(&m_timer,&QTimer::timeout,this,&MyOpenGLWidget::on_timeout);
    m_timer.start(TIME_OUT_INTERVAL);
    setFocusPolicy(Qt::StrongFocus);
    setMouseTracking(true);

    m_camera.Position=QVector3D(0.0,0.0,6.0);
}

MyOpenGLWidget::~MyOpenGLWidget()
{
    makeCurrent();
    glDeleteBuffers(1,&EBO);
    glDeleteBuffers(2,VBO);
    glDeleteVertexArrays(2,VAO);
    doneCurrent();
}

void MyOpenGLWidget::drawShape(MyOpenGLWidget::Shape shape)
{
    m_shape=shape;
    update();
}

void MyOpenGLWidget::setWireFrame(bool mode)
{
    makeCurrent();
    if(mode){
        glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
    }
    else{
        glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
    }
    update();
    doneCurrent();
}

void MyOpenGLWidget::initializeGL()
{
    initializeOpenGLFunctions();

    glGenVertexArrays(2,VAO);
    glGenBuffers(2,VBO);
    glGenBuffers(1,&EBO);

    glBindVertexArray(VAO[0]);
    glBindBuffer(GL_ARRAY_BUFFER,VBO[0]);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,EBO);

    shaderProgram.addShaderFromSourceFile(QOpenGLShader::Vertex,":/shaders/shapes.vert");
    shaderProgram.addShaderFromSourceFile(QOpenGLShader::Fragment,":/shaders/shapes.frag");
    shaderProgram.link();

    m_block_texture=new QOpenGLTexture(QImage(":/textures/container.jpg").mirrored());
    m_block_texture2=new QOpenGLTexture(QImage(":/textures/container2.png").mirrored());
    m_smile_texture=new QOpenGLTexture(QImage(":/textures/awesomeface.png").mirrored());
    // 绑定，只有在绑定完后才可以对这个shader的变量进行设置
    shaderProgram.bind();
    shaderProgram.setUniformValue("texture0",0);
    shaderProgram.setUniformValue("texture1",1);
    shaderProgram.setUniformValue("texture2",2);
    shaderProgram.setUniformValue("ratio",ratio);

    glBufferData(GL_ARRAY_BUFFER,sizeof (vertices),vertices,GL_STATIC_DRAW);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,sizeof (indices),indices,GL_STATIC_DRAW);
    glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,8*sizeof (float),(void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1,3,GL_FLOAT,GL_FALSE,8*sizeof (float),(void*)(3*sizeof (float)));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2,2,GL_FLOAT,GL_FALSE,8*sizeof (float),(void*)(6*sizeof (float)));
    glEnableVertexAttribArray(2);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,0);
    glBindBuffer(GL_ARRAY_BUFFER,0);
    glBindVertexArray(0);

    //cube
    glBindVertexArray(VAO[1]);
    glBindBuffer(GL_ARRAY_BUFFER,VBO[1]);

    glBufferData(GL_ARRAY_BUFFER,sizeof (cube),cube,GL_STATIC_DRAW);
    glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,5*sizeof (float),(void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(2,2,GL_FLOAT,GL_FALSE,5*sizeof (float),(void*)(3*sizeof (float)));
    glEnableVertexAttribArray(2);

    glBindBuffer(GL_ARRAY_BUFFER,0);
    glBindVertexArray(0);
}

void MyOpenGLWidget::resizeGL(int w, int h)
{
    Q_UNUSED(w)
    Q_UNUSED(h)
}

void MyOpenGLWidget::paintGL()
{
    glClearColor(0.2f,0.3f,0.3f,1.0f);
    glEnable(GL_DEPTH_TEST);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    QMatrix4x4 model,view,projection;
    shaderProgram.bind();
    m_block_texture->bind(2);
    m_block_texture2->bind(1);
    m_smile_texture->bind(0);

    switch (m_shape) {
    case Triangle:
        glBindVertexArray(VAO[0]);

        angle=(angle+5)%360;
        model.rotate(30,1.0f,0.0f,0.0f);
        model.rotate(angle,0.0f,1.0f,0.0f);
        view.translate(0.0,0.0,-6);
        projection.perspective(45,(float)width()/height(),0.1f,100);

        shaderProgram.setUniformValue("model",model);
        shaderProgram.setUniformValue("view",view);
        shaderProgram.setUniformValue("projection",projection);

        glDrawArrays(GL_TRIANGLES,0,3);//use VBO[0]
        break;
    case Rect:
        glBindVertexArray(VAO[0]);

        angle=(angle+5)%360;
        model.rotate(30,1.0f,0.0f,0.0f);
        model.rotate(angle,0.0f,1.0f,0.0f);
        view.translate(0.0,0.0,-6);
        projection.perspective(45,(float)width()/height(),0.1f,100);

        shaderProgram.setUniformValue("model",model);
        shaderProgram.setUniformValue("view",view);
        shaderProgram.setUniformValue("projection",projection);

        glDrawElements(GL_TRIANGLES,6,GL_UNSIGNED_INT,&indices);//use EBO
        break;
    case Cube:
        glBindVertexArray(VAO[1]);

        angle=(angle+5)%360;
        view=m_camera.getViewMatrix();
        projection.perspective(45,(float)width()/height(),0.1f,100);

        shaderProgram.setUniformValue("view",view);
        shaderProgram.setUniformValue("projection",projection);
        foreach (auto item, cubePositions) {
            model.setToIdentity();
            model.translate(item);
            model.rotate(30,1.0f,0.0f,0.0f);
            model.rotate(angle,0.0f,1.0f,0.0f);
            shaderProgram.setUniformValue("model",model);
            glDrawArrays(GL_TRIANGLES,0,36);//use VBO[1]
        }
        break;
    default:
        break;
    }

}

void MyOpenGLWidget::keyPressEvent(QKeyEvent *event)
{
    float deltaTime=TIME_OUT_INTERVAL/1000.0;
    switch (event->key()) {
    case Qt::Key_Up:
        ratio+=0.1f;
        break;
    case Qt::Key_Down:
        ratio-=0.1f;
        break;
    case Qt::Key_W:
        m_camera.processKeybord(FORWARD,deltaTime);
        break;
    case Qt::Key_S:
        m_camera.processKeybord(BACKWARD,deltaTime);
        break;
    case Qt::Key_D:
        m_camera.processKeybord(RIGHT,deltaTime);
        break;
    case Qt::Key_A:
        m_camera.processKeybord(LEFT,deltaTime);
        break;
    default:
        break;
    }
    if(ratio>1)ratio=1.0;
    if(ratio<0)ratio=0.0;

    shaderProgram.bind();
    shaderProgram.setUniformValue("ratio",ratio);
    update();
}

void MyOpenGLWidget::mouseMoveEvent(QMouseEvent *event)
{
    static QPoint lastPos(width()/2,height()/2);
    auto currentPos=event->pos();
    deltaPos=currentPos-lastPos;
    lastPos=currentPos;

    m_camera.processMouseMovement(deltaPos.x(),-deltaPos.y());

    update();
}

void MyOpenGLWidget::on_timeout()
{
    update();
}
