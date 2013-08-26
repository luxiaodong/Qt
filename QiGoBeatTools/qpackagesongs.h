#ifndef QPACKAGESONGS_H
#define QPACKAGESONGS_H

#include <QStringList>
#include <QString>

class QPackageSongs
{
public:
    QPackageSongs();

    bool packageSongs(QStringList &list);
    bool packageSong(const QString&);

public:
    bool changeMediaNameInPlist(QString plistFile, QString newName, bool isHasMp3 = true);

public:
    QString m_lastError;
};

#endif // QPACKAGESONGS_H
