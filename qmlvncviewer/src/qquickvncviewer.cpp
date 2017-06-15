#include "qquickvncviewer.h"

#include <QPainter>

QQuickVncViewer::QQuickVncViewer(QQuickItem *parent):
    QQuickPaintedItem(parent),
    m_host(""),
    m_port(0),
    m_password("")
{
    // By default, QQuickItem does not draw anything. If you subclass
    // QQuickItem to create a visual item, you will need to uncomment the
    // following line and re-implement updatePaintNode()

    // setFlag(ItemHasContents, true);

    connect(&m_viewerAdapter, SIGNAL(updateRect(QRect)), this, SLOT(onUpdateRect(QRect)), Qt::QueuedConnection);
    connect(&m_viewerAdapter, SIGNAL(onPropChanged()), this, SLOT(handlePropChanged()), Qt::QueuedConnection);

    setAcceptedMouseButtons(Qt::AllButtons);
    setAcceptHoverEvents(true);
    setRenderTarget(QQuickPaintedItem::FramebufferObject);
    setOpaquePainting(false);
}

QQuickVncViewer::~QQuickVncViewer()
{

}

QString QQuickVncViewer::host() const
{
    return m_host;
}

void QQuickVncViewer::setHost(const QString &value)
{
    m_host = value;
}

int QQuickVncViewer::port() const
{
    return m_port;
}

void QQuickVncViewer::setPort(const int &value)
{
    m_port = value;
}

QString QQuickVncViewer::password() const
{
    return m_password;
}

void QQuickVncViewer::setPassword(const QString &value)
{
    m_password = value;
}

int QQuickVncViewer::getServerWidth()
{
    return m_viewerAdapter.width();
}

int QQuickVncViewer::getServerHeight()
{
    return m_viewerAdapter.height();
}

void QQuickVncViewer::paint(QPainter *painter)
{
    if(!m_viewerAdapter.bufferImage())
        return;

  //  int width = m_viewerAdapter.width();
 //   int height = m_viewerAdapter.height();
  //  int bitsPerPixel = m_viewerAdapter.bitsPerPixel();
    QRect rect = painter->clipBoundingRect().toRect();
    rect = m_viewerAdapter.bufferImage()->rect().intersected(rect);

    painter->drawImage(rect.topLeft(), m_viewerAdapter.bufferImage()->copy(rect).convertToFormat(QImage::Format_ARGB32));

    //        QPen pen(QColor::fromRgb(255, 0, 0), 2);
    //        painter->setPen(pen);
    //        painter->setRenderHints(QPainter::Antialiasing, true);
    //        painter->drawRect(rect);


}

void QQuickVncViewer::start()
{
    m_viewerAdapter.start(m_host, m_port, m_password);
}

void QQuickVncViewer::stop()
{
    m_viewerAdapter.stop();
}

void QQuickVncViewer::refresh()
{
    m_viewerAdapter.refresh();
}

bool QQuickVncViewer::event(QEvent *event)
{
    switch (event->type()) {
    case QEvent::KeyPress:
    case QEvent::KeyRelease:
        m_viewerAdapter.keyEventHandler(static_cast<QKeyEvent*>(event));
        return event->isAccepted();
    case QEvent::MouseButtonDblClick:
    case QEvent::MouseButtonPress:
    case QEvent::MouseButtonRelease:
    case QEvent::MouseMove:
        m_viewerAdapter.mouseEventHandler(static_cast<QMouseEvent*>(event));
        return true;
    case QEvent::HoverMove:
        m_viewerAdapter.hoverEventHandler(static_cast<QHoverEvent*>(event));
        return true;
    case QEvent::Wheel:
        m_viewerAdapter.wheelEventHandler(static_cast<QWheelEvent*>(event));
        return true;
    default:
        return QQuickItem::event(event);
    }
}

void QQuickVncViewer::onUpdateRect(const QRect rect)
{
    update(rect);
}

void QQuickVncViewer::handlePropChanged()
{
    setWidth(m_viewerAdapter.width());
    setHeight(m_viewerAdapter.height());
    update();

    emit onPropChanged();
}
