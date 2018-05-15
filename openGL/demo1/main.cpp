#include <QGuiApplication>
#include "window.h"

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);
    // Set OpenGL Version information
    // Note: This format must be set before show() is called.
    QSurfaceFormat format;
    format.setRenderableType(QSurfaceFormat::OpenGL);
    format.setProfile(QSurfaceFormat::CoreProfile);
    format.setVersion(4,1);

    // Set the window up
    Window window;
    window.setFormat(format);
    window.resize(QSize(400, 400));
    window.show();

    app.exec();
}
