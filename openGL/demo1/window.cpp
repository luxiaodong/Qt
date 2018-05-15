#include "window.h"
#include <QDebug>
#include <QString>
#include <QFile>
#include <QTextStream>
#include <QByteArray>

Window::~Window()
{

}

void Window::initializeGL()
{
    // Initialize OpenGL Backend
    initializeOpenGLFunctions();
    printContextInformation();

    // Set global information
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    GLfloat position[] = {
        0.0f, 0.0f, 0.0f,
        0.5f, 0.0f, 0.0f,
        0.0f, 0.5f, 0.0f,
    };

    glGenBuffers(1, &m_vboPosition);
    glBindBuffer(GL_ARRAY_BUFFER, m_vboPosition);
    glBufferData(GL_ARRAY_BUFFER, 9*sizeof(GLfloat), position, GL_STATIC_DRAW);

    GLfloat color[] = {
        1.0f, 0.0f, 0.0f,
        0.0f, 1.0f, 0.0f,
        0.0f, 0.0f, 1.0f,
    };

    glGenBuffers(1, &m_vboColor);
    glBindBuffer(GL_ARRAY_BUFFER, m_vboColor);
    glBufferData(GL_ARRAY_BUFFER, 9*sizeof(GLfloat), color, GL_STATIC_DRAW);

    glGenVertexArrays(1, &m_vao);
    glBindVertexArray(m_vao);

    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, m_vboPosition);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

    glEnableVertexAttribArray(1);
    glBindBuffer(GL_ARRAY_BUFFER, m_vboColor);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);

    qDebug()<<this->compileShader(GL_VERTEX_SHADER, "base.vert");
    qDebug()<<this->compileShader(GL_FRAGMENT_SHADER, "base.frag");

    glClear(GL_COLOR_BUFFER_BIT);
}

void Window::resizeGL(int w, int h)
{}

void Window::paintGL()
{
    glBindVertexArray(m_vao);
    glDrawArrays(GL_TRIANGLES, 0, 3);
}

void Window::teardownGL()
{}

bool Window::compileShader(GLuint shaderType, QString filePath)
{
    QFile file( QString("/Users/luxiaodong/Project/Git/luxiaodong/Qt/openGL/demo1/%1").arg(filePath));
    if(file.open(QIODevice::ReadOnly) == false)
    {
        return false;
    }

    QTextStream stream(&file);
    QString str = stream.readAll();
    file.close();

    GLuint shader = glCreateShader(shaderType);
    if(shader == 0)
    {
        return false;
    }

    const GLchar* codeArray[] = {str.toLatin1().constData()};

    glShaderSource(shader, 1, codeArray, NULL);
    glCompileShader( shader );

    GLint result;
    glGetShaderiv( shader, GL_COMPILE_STATUS, &result );
    if( GL_FALSE == result )
    {
        GLint logLen;
        glGetShaderiv( shader, GL_INFO_LOG_LENGTH, &logLen );
        if(logLen > 0)
        {
            char * log = (char *)malloc(logLen);
            GLsizei written;
            glGetShaderInfoLog(shader, logLen, &written, log);
            fprintf(stderr, "Shader log:\n%s", log);
            free(log);
        }
        return false;
    }

    GLuint programHandle = glCreateProgram();
    if( 0 == programHandle )
    {
        return false;
    }

    glAttachShader( programHandle, shader );
    glLinkProgram( programHandle );

    GLint status;
    glGetProgramiv( programHandle, GL_LINK_STATUS, &status );
    if( GL_FALSE == status )
    {
        fprintf( stderr, "Failed to link shader program!\n" );
        GLint logLen;
        glGetProgramiv(programHandle, GL_INFO_LOG_LENGTH, &logLen);
        if( logLen > 0 )
        {
            char * log = (char *)malloc(logLen);
            GLsizei written;
            glGetProgramInfoLog(programHandle, logLen, &written, log);
            fprintf(stderr, "Program log: \n%s", log);
            free(log);
        }
        return false;
    }

    glUseProgram( programHandle );
    return true;
}

void Window::printContextInformation()
{
    QString glType;
    QString glVersion;
    QString glProfile;

    // Get Version Information
    glType = (context()->isOpenGLES()) ? "OpenGL ES" : "OpenGL";
    glVersion = reinterpret_cast<const char*>(glGetString(GL_VERSION));

    // Get Profile Information
  #define CASE(c) case QSurfaceFormat::c: glProfile = #c; break
    switch (format().profile())
    {
      CASE(NoProfile);
      CASE(CoreProfile);
      CASE(CompatibilityProfile);
    }
  #undef CASE

    // qPrintable() will print our QString w/o quotes around it.
    qDebug() << qPrintable(glType) << qPrintable(glVersion) << "(" << qPrintable(glProfile) << ")";

}
