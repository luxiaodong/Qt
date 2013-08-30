#ifndef STRING_H
#define STRING_H

    static const char* mainwindow_strings[] =
    {
        QT_TRANSLATE_NOOP("MainWindow", "Are your sure to close it?"),
        QT_TRANSLATE_NOOP("MainWindow", "service has been started"),
        QT_TRANSLATE_NOOP("MainWindow", "client has been started"),
        QT_TRANSLATE_NOOP("MainWindow", "Can't find your ip address"),
        QT_TRANSLATE_NOOP("MainWindow", "Do you want to single play?"),

        QT_TRANSLATE_NOOP("MainWindow", "waiting..."),
    };

    static const char* client_strings[] =
    {
        QT_TRANSLATE_NOOP("QClient", "Network Error!"),
        QT_TRANSLATE_NOOP("QClient", "connected to server"),
        QT_TRANSLATE_NOOP("QClient", "disconnected to server"),
    };

    //  QT_TRANSLATE_NOOP("", "")
#endif // STRING_H
