#ifndef QVNCVIEWERADAPTER_H
#define QVNCVIEWERADAPTER_H

#include <QImage>
#include <QMap>
#include <QDebug>
#include <QKeyEvent>
#include <QWheelEvent>
#include <QMouseEvent>
#include <QHoverEvent>

#include "vnclogger.h"
#include "viewervncauthhandler.h"

#include "viewer-core/RemoteViewerCore.h"
#include "viewer-core/CoreEventsAdapter.h"
#include "gui/DibFrameBuffer.h"
#define XK_MISCELLANY
#include "rfb/RfbKeySym.h"
#include "rfb/keysymdef.h"

class QVncViewerAdapter: public QObject, protected CoreEventsAdapter
{
    Q_OBJECT
public:
    QVncViewerAdapter();
    ~QVncViewerAdapter();

    QImage *bufferImage();

    int width();
    int height();
    int bitsPerPixel();

    void start(QString host, int port, QString password);
    void stop();
    void refresh();

    void keyEventHandler(QKeyEvent *e);
    void unpressModifiers();
    void wheelEventHandler(QWheelEvent *event);
    void mouseEventHandler(QMouseEvent *event);
    void hoverEventHandler(QHoverEvent *event);
protected:
    //
    // Bell event has been received from the server.
    //
    virtual void onBell();

    //
    // New cut text (clipboard) contents has been received from the server.
    //
    virtual void onCutText(const StringStorage *cutText);

    //
    // Connection has been established.
    //
    virtual void onEstablished();

    //
    // Protocol has entered the normal interactive phase (in other words,
    // protocol initialization has been completed).
    //
    // FIXME: document it.
    // Output need for capability, e.g. FT.
    virtual void onConnected(RfbOutputGate *output);

    //
    // RemoteViewerCore has been disconnected by calling stop()
    // or connection with server is disconnected.
    //
    // FIXME: now, onDisconnect not called after onError().
    // FIXME: change documentation or call onDisconnect() after onError().
    virtual void onDisconnect(const StringStorage *message);

    //
    // Authentication has been failed.
    // By default, onAuthError() call onError(exception).
    //
    virtual void onAuthError(const AuthException *exception);

    //
    // Error has been occured.
    //
    virtual void onError(const Exception *exception);

    // this event after update of frame buffer "fb" in rectangle "update".
    // guaranteed correct of frame buffer's area in rectangle "update".
    //
    // Frame buffer contents has been changed. During this callback,
    // the frame buffer is locked, and the rectangle is guaranteed to be valid
    // (no guarantees about other areas of the frame buffer).
    //
    virtual void onFrameBufferUpdate(const FrameBuffer *fb, const Rect *update);

    // changed properties of frame buffer.
    // In this moment frame buffer area is dirty and may be contained incorrect data
    //
    // Some properties of the frame buffer have been changed. Assume that new
    // frame buffer has been created and the old one has been destroyed. This
    // notification will be called on initial frame buffer allocation as well.
    //
    virtual void onFrameBufferPropChange(const FrameBuffer *fb);

    static const int MOUSE_LDOWN  = 1;
    static const int MOUSE_MDOWN  = 2;
    static const int MOUSE_RDOWN  = 4;
    static const int MOUSE_WUP    = 8;
    static const int MOUSE_WDOWN  = 16;
signals:

    void updateRect(const QRect rect);

    void onPropChanged();
private:
    void repaint(QRect rect);
private:
    VncLogger logger;

    int m_buttonMask;
    QMap<unsigned int, bool> m_mods;

    RemoteViewerCore *m_viewerCore;
    LocalMutex m_bufferLock;
    FrameBuffer m_framebuffer;
    QImage *m_bufferImage;
    QVector<QRgb> m_colorTable;
    Dimension m_serverDimension;
    ViewerVncAuthHandler m_vncAuthHandler;

    int m_width;
    int m_height;
    int m_bitsPerPixel;
};

#endif // QVNCVIEWERADAPTER_H
