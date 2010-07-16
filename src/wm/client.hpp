#ifndef _CLIENT_HPP
#define _CLIENT_HPP

#include <QObject>
#include <QRect>

class Decoration;
union _XEvent;

class Client: public QObject
{
public:
    Client(Qt::HANDLE winId, QObject *parent);
    ~Client();

    bool x11EventFilter(_XEvent *e);

    // These methods are interesting if you want to create a custom decoration
    void move(const QPoint &p);
    void resize(const QSize &size, int gravity);
    void minimize();
    void maximize();
    void close();

    void setActive();
    void setInactive();

    inline Decoration *decoration() const { return _decoration; }

    inline Qt::HANDLE winId() const { return _winId; }
    inline QRect geometry() const { return _geometry; }

protected:
    friend class WindowManager;

    void map();

private:
    void updateTitle();

private:
    Qt::HANDLE _winId;
    QWidget *_wrapper;
    Decoration *_decoration;
    QString _title;
    QRect _geometry;
};

#endif
