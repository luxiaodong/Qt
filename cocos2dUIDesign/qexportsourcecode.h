#ifndef QEXPORTSOURCECODE_H
#define QEXPORTSOURCECODE_H

#include <QString>
#include <QFile>
#include <QTextStream>
#include <QDate>
#include <QStringList>

#define NEXT_LINE   "\n"
#define PATH_SLASH  "/"

class QExportSourceCode
{
public:
    QExportSourceCode();

public:
    void exportSourceCode(QString uiFile, QStringList list);

public:
    QString superClassName(){return "GAbstractLayer";}
    QString filePrefixText(QString fileName);
    QString precompileMacro();
    QString macroIdFormLine(QString line);

//.h
    QString createPrefixTextInHeaderFile();
    QString createIncludeInHeaderFile();
    QString createUIDefineMacro();
    QString createClass();
    QString createHeaderFile();

//.cpp
    QString createPrefixTextInSourceFile();
    QString createIncludeInSourceFile();
    QString createFunctionConstruction();
    QString createFunctionLoading();
    QString createFunctionLoadUi();
    QString createFunctionLoadMenu();
    QString createFunctionOnClick();
    QString createFunctionTouchEnable();
    QString createFunctionLayerUpdate();
    QString createSourceFile();

    QString createLogicHeaderFile();
    QString createLogicSourceFile();

    void exportSourceCode();
    void refactorCode();
    void refactorHeaderFile();
    void refactorSourceFile();

private:
    QString m_sourceCodePath;
    QString m_className;
    QString m_logicClassName;
    QString m_logicVaribleName;
    QString m_filePath; // exp: /user/code/cocos2dUIDesign
    QString m_fileName; // exp: qexportsourcecode.h
    QString m_uiFilePath;

    QStringList m_idList; //
};

#endif // QEXPORTSOURCECODE_H
