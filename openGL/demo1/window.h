#ifndef WINDOW_H
#define WINDOW_H

#include <QOpenGLWindow>
#include <QOpenGLFunctions>
#include <QString>

class Window : public QOpenGLWindow, protected QOpenGLFunctions
{
    Q_OBJECT
public:
    ~Window();

    void initializeGL();
    void resizeGL(int w, int h);
    void paintGL();
    void teardownGL();

private:
    void printContextInformation();
    bool compileShader(GLuint shaderType, QString filePath);

    GLuint  m_vao;
    GLuint  m_vboPosition;
    GLuint  m_vboColor;
};

#endif // WINDOW_H
