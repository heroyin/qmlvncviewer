#ifndef QQUICKVNCVIEWER_H
#define QQUICKVNCVIEWER_H

#include <QQuickItem>
#include <QQuickPaintedItem>

#include "qvncvieweradapter.h"

class QQuickVncViewer : public QQuickPaintedItem
{
    Q_OBJECT
    Q_DISABLE_COPY(QQuickVncViewer)
    Q_PROPERTY(QString host READ host WRITE setHost)
    Q_PROPERTY(int port READ port WRITE setPort)
    Q_PROPERTY(QString password READ password WRITE setPassword)
    Q_PROPERTY(int serverWidth READ getServerWidth CONSTANT)
    Q_PROPERTY(int serverHeight READ getServerHeight CONSTANT)

public:
    QQuickVncViewer(QQuickItem *parent = 0);
    ~QQuickVncViewer();

    QString host() const;
    void setHost(const QString &value);

    int port() const;
    void setPort(const int &value);

    QString password() const;
    void setPassword(const QString &value);

    int getServerWidth();

    int getServerHeight();

    void paint(QPainter *painter);

    Q_INVOKABLE void start();
    Q_INVOKABLE void stop();
    Q_INVOKABLE void refresh();

protected:
    bool event(QEvent *event);

signals:
    void onPropChanged();

public slots:
    void onUpdateRect(const QRect rect);
    void handlePropChanged();

private:
    QString m_host;
    QString m_password;
    int m_port;

    QVncViewerAdapter m_viewerAdapter;
};

#endif // QQUICKVNCVIEWER_H
