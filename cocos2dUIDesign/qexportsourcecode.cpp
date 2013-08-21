#include "qexportsourcecode.h"

#include <QSettings>
#include <QMap>
#include <QDebug>

QExportSourceCode::QExportSourceCode()
{
    m_className = "GTestLayer";
}


void QExportSourceCode::exportSourceCode(QString uiFile, QStringList list)
{
    m_uiFilePath = uiFile;

    QSettings settings("yuguosoft.com");
    m_sourceCodePath = settings.value("sourceCodePath").toString();

    QString name = uiFile.split("/").last().remove(".ui");
    QString temp1 = name;
    QString temp2 = temp1.toUpper();
    m_className = QString("G%1Layer").arg( QString(temp2.left(1) + temp1.right(name.size() - 1)));
    m_logicClassName = QString("G%1Logic").arg( QString(temp2.left(1) + temp1.right(name.size() - 1)));
    m_logicVaribleName = QString("m_%1Logic").arg(temp1);
    m_idList.clear();

    int size = list.size();
    for(int i = 0; i < size; ++i)
    {
        QString line = list.at(i);
        m_idList.append(this->macroIdFormLine(line));
    }

    QString headerFile = m_sourceCodePath + "/" + m_className + ".h";

    if(QFile::exists(headerFile) == false)
    {
        this->exportSourceCode();
    }
    else
    {
        this->refactorCode();
    }
}

void QExportSourceCode::exportSourceCode()
{
    QString headerFile = m_sourceCodePath + "/" + m_className + ".h";
    QString sourceFile = m_sourceCodePath + "/" + m_className + ".cpp";
    QString logicHeaderFile = m_sourceCodePath + "/" + m_logicClassName + ".h";
    QString logicSourceFile = m_sourceCodePath + "/" + m_logicClassName + ".cpp";

    QFile file(headerFile);
    QTextStream in;
    in.setCodec("UTF-8");

    if( file.open(QIODevice::WriteOnly) == true)
    {
        in.setDevice(&file);
        in<<createHeaderFile();
        file.close();
    }

    file.setFileName(sourceFile);
    if( file.open(QIODevice::WriteOnly) == true)
    {
        in.setDevice(&file);
        in<<createSourceFile();
        file.close();
    }

    file.setFileName(logicHeaderFile);
    if( file.open(QIODevice::WriteOnly) == true)
    {
        in.setDevice(&file);
        in<<createLogicHeaderFile();
        file.close();
    }

    file.setFileName(logicSourceFile);
    if( file.open(QIODevice::WriteOnly) == true)
    {
        in.setDevice(&file);
        in<<createLogicSourceFile();
        file.close();
    }
}

void QExportSourceCode::refactorCode()
{
    this->refactorHeaderFile();
    this->refactorSourceFile();
}

void QExportSourceCode::refactorHeaderFile()
{
    QString headerFile = m_sourceCodePath + "/" + m_className + ".h";
    QFile file(headerFile);
    QTextStream in(&file);
    in.setCodec("UTF-8");
    QStringList fileLines;

    if( file.open(QIODevice::ReadOnly) == true)
    {
        while(in.atEnd() == false)
        {
            QString line = in.readLine();
            fileLines.append(line);
        }
        file.close();
    }

    bool isReplace = false;
    QString copyString = m_className;
    copyString = copyString.toUpper();
    copyString = QString("#define %1_").arg(copyString);

    int i = 0;
    while(i < fileLines.size())
    {
        QString line = fileLines.at(i);
        if(line.contains(copyString) == true)
        {
            fileLines.removeAt(i);
            isReplace = true;
            i--;
        }
        else
        {
            if(isReplace == true)
            {
                fileLines.replace(i, this->createUIDefineMacro());
                isReplace = false;
            }
        }
        i++;
    }

    if(file.open(QIODevice::WriteOnly) == true)
    {
        in.setDevice(&file);

        foreach (QString temp, fileLines)
        {
            in<<temp<<"\n";
        }

        file.close();
    }
}

void QExportSourceCode::refactorSourceFile()
{
    QString headerFile = m_sourceCodePath + "/" + m_className + ".cpp";
    QFile file(headerFile);
    QTextStream in(&file);
    in.setCodec("UTF-8");
    QStringList fileLines;

    if( file.open(QIODevice::ReadOnly) == true)
    {
        while(in.atEnd() == false)
        {
            QString line = in.readLine();
            fileLines.append(line);
        }
        file.close();
    }

//replace onClick, and setTouchEnable
    bool isOnClick = false;
    bool isTouchEnable = false;
    QString onClickLine = QString("void %1::onClick(CCObject *sender)").arg(m_className);
    QString onTouchLine = QString("void %1::setTouchEnable(bool yesOrNo)").arg(m_className);
    QStringList subList;

    int i = 0;
    while(i < fileLines.size())
    {
        QString line = fileLines.at(i);
        if(line.contains(onClickLine) == true)
        {
            isOnClick = true;

            i++;
            int count = 0;
            while(i < fileLines.size())
            {
                QString temp = fileLines.at(i);
                subList.append(temp);
                fileLines.removeAt(i);

                if(temp.contains("{") == true)
                {
                    count++;
                }

                if(temp.contains("}") == true)
                {
                    count--;
                }

                if(count == 0)
                {
                    break;
                }
            }
        }

        if(isOnClick == true)
        {
            QMap<QString, QStringList> map;
            int size = subList.size();
            int j = 0;

            while(j < size)
            {
                QString key;
                QStringList value;
                QString temp = subList.at(j);
                if(temp.contains("if(tag == ") == true)
                {
                    key = temp;
                    value.clear();

                    j++;
                    int count = 0;
                    while(j < size)
                    {
                        QString temp2 = subList.at(j);
                        value.append(temp2);
                        if(temp2.contains("{") == true)
                        {
                            count++;
                        }

                        if(temp2.contains("}") == true)
                        {
                            count--;
                        }

                        if(count == 0)
                        {
                            map.insert(key,value);
                            qDebug()<<key<<value;
                            break;
                        }

                        j++;
                    }
                }

                j++;
            }
//chong xie
            QString str;
            str.clear();
            str += QString("{\n");
            str += QString("    CCNode* node = (CCNode*)sender;\n");
            str += QString("    int tag = node->getTag();\n");

            foreach(QString temp3, m_idList)
            {
                if(temp3.contains("_MENU_") == true)
                {
                    QString temp4 = QString("    if(tag == %1)").arg(temp3);
                    qDebug()<<temp4;
                    QStringList tempList = map.take(temp4);

                    if(tempList.isEmpty() == true)
                    {
                        str += QString("    if(tag == %1)\n").arg(temp3);
                        str += QString("    {\n");
                        str += QString("    }\n");
                    }
                    else
                    {
                        str += QString("    if(tag == %1)\n").arg(temp3);
                        foreach (QString temp5, tempList)
                        {
                            str += QString(temp5);
                            str += QString("\n");
                        }
                    }
                }
            }

            str += QString("}\n");
            fileLines.replace(i, str);
            isOnClick = false;
        }

        if(line.contains(onTouchLine) == true)
        {
            fileLines.removeAt(i);
            isTouchEnable = true;

            while(1)
            {
                QString temp = fileLines.at(i);
                fileLines.removeAt(i);
                if(temp.contains("}") == true)
                {
                    break;
                }
            }
        }

        if(isTouchEnable == true)
        {
            isTouchEnable = false;
            fileLines.replace(i, this->createFunctionTouchEnable());
        }

        i++;
    }

    if(file.open(QIODevice::WriteOnly) == true)
    {
        in.setDevice(&file);

        foreach (QString temp, fileLines)
        {
            in<<temp<<"\n";
        }

        file.close();
    }
}

QString QExportSourceCode::filePrefixText(QString name)
{
    QString str;
    str += QString("//\n");
    str += QString("// %1\n").arg(name);
    str += QString("// Created by lu_xiaodong on %1.\n").arg(QDate::currentDate().toString("yyyy-MM-dd"));
    str += QString("// Copyright by yuguosoft.com. All rights reserved.\n");
    str += QString("//\n");
    return str;
}

QString QExportSourceCode::precompileMacro()
{
    QString macroString = m_className;
    macroString = macroString.toUpper().replace(".","_");
    macroString = QString("_") + macroString + QString("_H_");
    return macroString;
}

QString QExportSourceCode::macroIdFormLine(QString line)
{
    QString copyClass = m_className;
    copyClass = copyClass.toUpper();

    QString type;
    if(line.contains("sprite") == true)
    {
        type = "SPRITE";
    }
    else if(line.contains("label") == true)
    {
        type = "LABEL";
    }
    else if(line.contains("menu") == true)
    {
        type = "MENU";
    }
    else if(line.contains("list") == true)
    {
        type = "LIST";
    }
    else if(line.contains("table") == true)
    {
        type = "TABLE";
    }

    QString name;
    QStringList list = line.split(" ");
    foreach (QString temp, list)
    {
        if(temp.contains("name=") == true)
        {
            name = temp.remove("name=").remove("\"");
            name = name.toUpper();
        }
    }

    return QString("%1_%2_%3").arg(copyClass, type, name);
}

QString QExportSourceCode::createPrefixTextInHeaderFile()
{
    QString name = m_className + ".h";
    return this->filePrefixText(name);
}

QString QExportSourceCode::createIncludeInHeaderFile()
{
    QString str;
    str += QString("#include \"%1\"\n").arg(this->superClassName() + ".h");
    str += QString("#include \"%1\"\n").arg(m_logicClassName + ".h");
    return str;
}

QString QExportSourceCode::createUIDefineMacro()
{
    QString str;

    int size = m_idList.size();
    for(int i = 0; i < size; ++i)
    {
        str += QString("#define %1 %2\n").arg(m_idList.at(i)).arg(i+1);
    }

    return str;
}

QString QExportSourceCode::createClass()
{
    QString copyClass = m_className;
    copyClass.replace("Layer","Logic");
    QString str;
    str += QString("class %1 : public %2\n").arg(m_className).arg(superClassName());
    str += QString("{\n");
    str += QString("public:\n");
    str += QString("    %1();\n").arg(m_className);
    str += QString("    ~%2();\n").arg(m_className);
//    str += QString("    void loading();\n");
//    str += QString("    void loadUi();\n");
    str += QString("    void setTouchEnable(bool);\n");
    //str += QString("    void layerUpdate(ccTime);\n");
    str += QString("    void onClick(CCObject*);\n");
    str += QString("\n");
    str += QString("protected:\n");
    str += QString("    void loadMenu(GUIBuildData* data);\n");
    str += QString("public:\n");
    str += QString("    %1* %2;\n").arg(m_logicClassName, m_logicVaribleName);
    str += QString("};\n");
    return str;
}

QString QExportSourceCode::createHeaderFile()
{
    QString str;
    str += createPrefixTextInHeaderFile();
    str += QString("#ifndef %1\n").arg(this->precompileMacro());
    str += QString("#define %1\n").arg(this->precompileMacro());
    str += QString("\n");
    str += createIncludeInHeaderFile();
    str += QString("\n");
    str += createUIDefineMacro();
    str += QString("\n");
    str += createClass();
    str += QString("\n");
    str += QString("#endif\n");
    str += QString("\n");
    return str;
}

QString QExportSourceCode::createPrefixTextInSourceFile()
{
    QString name = m_className + ".cpp";
    return this->filePrefixText(name);
}

QString QExportSourceCode::createIncludeInSourceFile()
{
    QString str;
    str += QString("#include \"%1\"\n").arg(m_className + ".h");
    return str;
}

QString QExportSourceCode::createFunctionConstruction()
{
    QString str;
    str.clear();
    str += QString("%1::%2()\n").arg(m_className, m_className);
    str += QString("{\n");
    //str += QString("    m_uiFileFullPath = \"%1\";\n").arg(m_uiFilePath);
    str += QString("    this->setUIFileFullPath(\"%1\");\n").arg(m_uiFilePath);
    str += QString("    %1 = new %2();\n").arg(m_logicVaribleName, m_logicClassName);
    str += QString("}\n");
    str += QString("\n");
    str += QString("%1::~%2()\n").arg(m_className, m_className);
    str += QString("{\n");
    str += QString("    delete %1;\n").arg(m_logicVaribleName);
    str += QString("}\n");
    return str;
}

QString QExportSourceCode::createFunctionLoading()
{
    QString str;
    str.clear();
    str += QString("void %1::loading()\n").arg(m_className);
    str += QString("{\n");
    str += QString("    this->loadUi();\n");
    str += QString("}\n");
    return str;
}

QString QExportSourceCode::createFunctionLoadUi()
{
    QString str;
    str.clear();
    str += QString("void %1::loadUi()\n").arg(m_className);
    str += QString("{\n");
    str += QString("    if(m_isUiLoaded == true)\n");
    str += QString("    {\n");
    str += QString("        return;\n");
    str += QString("    }\n");
    str += QString("    m_uiBuild = new GUIBuild();\n");
    str += QString("    if(m_uiBuild->parseUIFile(m_uiFileFullPath.c_str()) == false)\n");
    str += QString("    {\n");
    str += QString("        CC_ASSERT(false);\n");
    str += QString("    }\n");
    str += QString("    std::vector<GUIBuildData*> datas = m_uiBuild->m_buildData;\n");
    str += QString("    int size = datas.size();\n");
    str += QString("    for(int i = 0; i < size; ++i)\n");
    str += QString("    {\n");
    str += QString("        GUIBuildData* data = datas.at(i);\n");
    str += QString("        if(data->m_type == \"sprite\")\n");
    str += QString("        {\n");
    str += QString("            this->loadSprite(data);\n");
    str += QString("        }\n");
    str += QString("        else if(data->m_type == \"menu\")\n");
    str += QString("        {\n");
    str += QString("            this->loadMenu(data, menu_selector(%1::onClick));\n").arg(m_className);
    str += QString("        }\n");
    str += QString("        else if(data->m_type == \"label\")\n");
    str += QString("        {\n");
    str += QString("            this->loadLabel(data);\n");
    str += QString("        }\n");
    str += QString("        else if(data->m_type == \"list\")\n");
    str += QString("        {\n");
    str += QString("            this->loadList(data);\n");
    str += QString("        }\n");
    str += QString("    }\n");
    str += QString("}\n");
    return str;
}

QString QExportSourceCode::createFunctionLoadMenu()
{
    QString str;
    str.clear();
    str += QString("void %1::loadMenu(GUIBuildData* data)\n").arg(m_className);
    str += QString("{\n");
    str += QString("    GAbstractLayer::loadMenu(data, menu_selector(%1::onClick));\n").arg(m_className);
    str += QString("}\n");
    return str;
}

QString QExportSourceCode::createFunctionOnClick()
{
    QString str;
    str.clear();
    str += QString("void %1::onClick(CCObject *sender)\n").arg(m_className);
    str += QString("{\n");
    str += QString("    CCNode* node = (CCNode*)sender;\n");
    str += QString("    int tag = node->getTag();\n");

    foreach(QString temp, m_idList)
    {
        if(temp.contains("_MENU_") == true)
        {
            str += QString("    if(tag == %1)\n").arg(temp);
            str += QString("    {\n");
            str += QString("    }\n");
        }
    }

    str += QString("}\n");
    return str;
}

QString QExportSourceCode::createFunctionTouchEnable()
{
    QString str;
    str += QString("void %1::setTouchEnable(bool yesOrNo)\n").arg(m_className);
    str += QString("{\n");
    str += QString("    CCMenu* menu = 0;\n");

    foreach(QString temp, m_idList)
    {
        if(temp.contains("_MENU_") == true)
        {
            str += QString("    menu = (CCMenu*)this->getChildByTag(%1);\n").arg(temp);
            str += QString("    menu->setIsTouchEnabled(yesOrNo);\n");
        }
    }

    str += QString("}\n");
    return str;
}

QString QExportSourceCode::createFunctionLayerUpdate()
{
    QString str;
    str += QString("void %1::layerUpdate(ccTime delt)\n").arg(m_className);
    str += QString("{\n");
    str += QString("}\n");
    return str;
}

QString QExportSourceCode::createSourceFile()
{
    QString str;
    str += createPrefixTextInSourceFile();
    str += QString("\n");
    str += createIncludeInSourceFile();
    str += QString("\n");
    str += createFunctionConstruction();
    str += QString("\n");
//    str += createFunctionLoading();
//    str += QString("\n");
//    str += createFunctionLoadUi();
//    str += QString("\n");
    str += createFunctionLoadMenu();
    str += QString("\n");
    str += createFunctionOnClick();
    str += QString("\n");
    str += createFunctionTouchEnable();
    str += QString("\n");
    //str += createFunctionLayerUpdate();
    //str += QString("\n");
    return str;
}

QString QExportSourceCode::createLogicHeaderFile()
{
    QString str;
    str += filePrefixText(m_logicClassName + ".h");
    str += QString("\n");
    QString copyString = m_logicClassName;
    copyString = QString("_%1_H_").arg(copyString.toUpper());
    str += QString("#ifndef %1\n").arg(copyString);
    str += QString("#define %1\n").arg(copyString);
    str += QString("\n");
    str += QString("class %1\n").arg(m_logicClassName);
    str += QString("{\n");
    str += QString("public:\n");
    str += QString("    %1();\n").arg(m_logicClassName);
    str += QString("    ~%1();\n").arg(m_logicClassName);
    str += QString("};\n");
    str += QString("\n");
    str += QString("#endif\n");
    str += QString("\n");
    return str;
}

QString QExportSourceCode::createLogicSourceFile()
{
    QString str;
    str += filePrefixText(m_logicClassName + ".cpp");
    str += QString("\n");
    str += QString("#include \"%1.h\"\n").arg(m_logicClassName);
    str += QString("\n");
    str += QString("%1::%2()\n").arg(m_logicClassName, m_logicClassName);
    str += QString("{\n");
    str += QString("}\n");
    str += QString("\n");
    str += QString("%1::~%2()\n").arg(m_logicClassName, m_logicClassName);
    str += QString("{\n");
    str += QString("}\n");
    str += QString("\n");
    return str;
}

