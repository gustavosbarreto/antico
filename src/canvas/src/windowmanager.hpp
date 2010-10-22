#ifndef _WINDOWMANAGER_HPP
#define _WINDOWMANAGER_HPP

#include <QObject>
#include <QX11Info>

class QWebView;
union _XEvent;

class WindowManager: public QObject
{
    Q_OBJECT

public:
    enum Extension
    {
        NoneExtension = 0,
        CompositeExtension = 1,
        DamageExtension = 2,
        FixesExtension = 4,
        ShapeExtension = 8,
        TestExtension = 16
    };

    Q_DECLARE_FLAGS(Extensions, Extension)

    void init();

    bool x11EventFilter(_XEvent *e);

    static WindowManager *self();

private:
    Display *display;
    int damageEventBase, fixesEventBase, shapeEventBase;
    Qt::HANDLE overlayWindow;
    QWebView *webView;

    WindowManager();

    Extensions queryExtensions();
    bool isWindowManagerRunning() const;
    bool isCompositeManagerRunning() const;

    bool registerWindowManager();
    bool registerCompositeManager();

    bool createOverlay();

    void allowInputThroughOverlay(Qt::HANDLE window);
};

Q_DECLARE_OPERATORS_FOR_FLAGS(WindowManager::Extensions)

#endif
