#ifndef QGOLDSCATTER_H
#define QGOLDSCATTER_H

#include <QString>
#include <QMap>
#include <QFile>
#include <QStringList>

class QGoldScatter
{
public:
    QGoldScatter();
    void loadFile(QString filePath);
    QString calculateKey(int day, int gold);

public:
    QStringList m_keys;
    QMap<QString, int> m_map;
    QMap<QString, float> m_percent;
    QMap<QString, int> m_count;
};

#endif // QGOLDSCATTER_H
