#ifndef _CLIENT_HPP
#define _CLIENT_HPP

#include <QObject>
#include <QPoint>

class Client: public QObject
{
public:
    Client();

    void move(const QPoint &p);
    void minimize();
    void maximize();
    void close();
    void setActive();
};

#endif
