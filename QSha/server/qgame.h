#ifndef QGAME_H
#define QGAME_H

#include "qround.h"
#include "../protocol.h"
#include "qserver.h"

class QServer;
class QGame
{
public:
    QGame(QServer* server);
    void process(QList<int> &msg);
private:
    QRoundNode *m_root;
    QRoundNode *m_current;
    QDesk* m_desk;
    QPlayer* m_player;
    QServer* m_server;
};

#endif // QGAME_H
