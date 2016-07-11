#include "qvncvieweradapter.h"

QRect rectToQRect(Rect r)
{
    return QRect(r.left, r.top, r.right-r.left, r.bottom-r.top);
}

QVncViewerAdapter::QVncViewerAdapter():
    m_viewerCore(NULL),
    m_bufferImage(NULL),
    m_width(0),
    m_height(0),
    m_bitsPerPixel(0)
{

}

QVncViewerAdapter::~QVncViewerAdapter()
{
    stop();
}

QImage *QVncViewerAdapter::bufferImage()
{
    return m_bufferImage;
}

int QVncViewerAdapter::width()
{
    return m_width;
}

int QVncViewerAdapter::height()
{
    return m_height;
}

int QVncViewerAdapter::bitsPerPixel()
{
    return m_bitsPerPixel;
}

void QVncViewerAdapter::start(QString host, int port, QString password)
{
    if(m_viewerCore){
        delete m_viewerCore;
        m_viewerCore = NULL;
    }

    m_viewerCore = new RemoteViewerCore(&logger);
    m_viewerCore->setPreferredEncoding(EncodingDefs::ZRLE);
    m_vncAuthHandler.addAuthCapability(m_viewerCore);
    m_vncAuthHandler.setPassword(password.toStdWString());

    m_viewerCore->start(host.toStdWString().c_str(), port, this, true);
}

void QVncViewerAdapter::stop()
{
    if(m_viewerCore){
        m_viewerCore->stop();
        m_viewerCore->waitTermination();
        delete m_viewerCore;
        m_viewerCore = NULL;
    }
}

void QVncViewerAdapter::refresh()
{
    if(m_viewerCore)
        m_viewerCore->refreshFrameBuffer();
}

void QVncViewerAdapter::hoverEventHandler(QHoverEvent *event)
{
    const int x = event->pos().x();
    const int y = event->pos().y();

    Point point(x, y);

    if(m_viewerCore)
        m_viewerCore->sendPointerEvent(m_buttonMask, &point);
}

void QVncViewerAdapter::mouseEventHandler(QMouseEvent *e)
{
    if (e->type() != QEvent::MouseMove) {
        if ((e->type() == QEvent::MouseButtonPress) ||
                (e->type() == QEvent::MouseButtonDblClick)) {
            m_buttonMask = 0;

            if (e->button() & Qt::LeftButton)
                m_buttonMask |= MOUSE_LDOWN;
            if (e->button() & Qt::MidButton)
                m_buttonMask |= MOUSE_MDOWN;
            if (e->button() & Qt::RightButton)
                m_buttonMask |= MOUSE_RDOWN;
        } else if (e->type() == QEvent::MouseButtonRelease) {
            m_buttonMask = 0;

            if (e->button() & Qt::LeftButton)
                m_buttonMask &= MOUSE_WUP;
            if (e->button() & Qt::MidButton)
                m_buttonMask &= MOUSE_WUP;
            if (e->button() & Qt::RightButton)
                m_buttonMask &= MOUSE_WUP;
        }
    }

    Point point(e->x(), e->y());
    if(m_viewerCore)
        m_viewerCore->sendPointerEvent(m_buttonMask, &point);
}

void QVncViewerAdapter::repaint(QRect rect)
{
    emit updateRect(rect);
}

void QVncViewerAdapter::wheelEventHandler(QWheelEvent *event)
{
    int eb = 0;
    if (event->delta() < 0)
        eb |= 0x10;
    else
        eb |= 0x8;

    const int x = event->x();
    const int y = event->y();

    Point point(x, y);
    if(m_viewerCore){
        m_viewerCore->sendPointerEvent(eb | m_buttonMask, &point);
        m_viewerCore->sendPointerEvent(m_buttonMask, &point);
    }
}

void QVncViewerAdapter::keyEventHandler(QKeyEvent *_ke)
{
    bool pressed = _ke->type() == QEvent::KeyPress;

    int key = 0;
    switch( _ke->key() )
    {
    // modifiers are handled separately
    case Qt::Key_Shift: key = XK_Shift_L; break;
    case Qt::Key_Control: key = XK_Control_L; break;
    case Qt::Key_Meta: key = XK_Meta_L; break;
    case Qt::Key_Alt: key = XK_Alt_L; break;
    case Qt::Key_Escape: key = XK_Escape; break;
    case Qt::Key_Tab: key = XK_Tab; break;
    case Qt::Key_Backtab: key = XK_Tab; break;
    case Qt::Key_Backspace: key = XK_BackSpace; break;
    case Qt::Key_Return: key = XK_Return; break;
    case Qt::Key_Insert: key = XK_Insert; break;
    case Qt::Key_Delete: key = XK_Delete; break;
    case Qt::Key_Pause: key = XK_Pause; break;
    case Qt::Key_Print: key = XK_Print; break;
    case Qt::Key_Home: key = XK_Home; break;
    case Qt::Key_End: key = XK_End; break;
    case Qt::Key_Left: key = XK_Left; break;
    case Qt::Key_Up: key = XK_Up; break;
    case Qt::Key_Right: key = XK_Right; break;
    case Qt::Key_Down: key = XK_Down; break;
    case Qt::Key_PageUp: key = XK_Prior; break;
    case Qt::Key_PageDown: key = XK_Next; break;
    case Qt::Key_CapsLock: key = XK_Caps_Lock; break;
    case Qt::Key_NumLock: key = XK_Num_Lock; break;
    case Qt::Key_ScrollLock: key = XK_Scroll_Lock; break;
    case Qt::Key_Super_L: key = XK_Super_L; break;
    case Qt::Key_Super_R: key = XK_Super_R; break;
    case Qt::Key_Menu: key = XK_Menu; break;
    case Qt::Key_Hyper_L: key = XK_Hyper_L; break;
    case Qt::Key_Hyper_R: key = XK_Hyper_R; break;
    case Qt::Key_Help: key = XK_Help; break;
        //case Qt::Key_AltGr: key = XK_ISO_Level3_Shift; break;
    case Qt::Key_Multi_key: key = XK_Multi_key; break;
    case Qt::Key_SingleCandidate: key = XK_SingleCandidate; break;
    case Qt::Key_MultipleCandidate: key = XK_MultipleCandidate; break;
    case Qt::Key_PreviousCandidate: key = XK_PreviousCandidate; break;
    case Qt::Key_Mode_switch: key = XK_Mode_switch; break;
    case Qt::Key_Kanji: key = XK_Kanji; break;
    case Qt::Key_Muhenkan: key = XK_Muhenkan; break;
    case Qt::Key_Henkan: key = XK_Henkan; break;
    case Qt::Key_Romaji: key = XK_Romaji; break;
    case Qt::Key_Hiragana: key = XK_Hiragana; break;
    case Qt::Key_Katakana: key = XK_Katakana; break;
    case Qt::Key_Hiragana_Katakana: key = XK_Hiragana_Katakana; break;
    case Qt::Key_Zenkaku: key = XK_Zenkaku; break;
    case Qt::Key_Hankaku: key = XK_Hankaku; break;
    case Qt::Key_Zenkaku_Hankaku: key = XK_Zenkaku_Hankaku; break;
    case Qt::Key_Touroku: key = XK_Touroku; break;
    case Qt::Key_Massyo: key = XK_Massyo; break;
    case Qt::Key_Kana_Lock: key = XK_Kana_Lock; break;
    case Qt::Key_Kana_Shift: key = XK_Kana_Shift; break;
    case Qt::Key_Eisu_Shift: key = XK_Eisu_Shift; break;
    case Qt::Key_Eisu_toggle: key = XK_Eisu_toggle; break;
        //    case Qt::Key_Hangul: key = XK_Hangul; break;
        //    case Qt::Key_Hangul_Start: key = XK_Hangul_Start; break;
        //    case Qt::Key_Hangul_End: key = XK_Hangul_End; break;
        //    case Qt::Key_Hangul_Hanja: key = XK_Hangul_Hanja; break;
        //    case Qt::Key_Hangul_Jamo: key = XK_Hangul_Jamo; break;
        //    case Qt::Key_Hangul_Romaja: key = XK_Hangul_Romaja; break;
        //    case Qt::Key_Hangul_Jeonja: key = XK_Hangul_Jeonja; break;
        //    case Qt::Key_Hangul_Banja: key = XK_Hangul_Banja; break;
        //    case Qt::Key_Hangul_PreHanja: key = XK_Hangul_PreHanja; break;
        //    case Qt::Key_Hangul_PostHanja: key = XK_Hangul_PostHanja; break;
        //    case Qt::Key_Hangul_Special: key = XK_Hangul_Special; break;
        //    case Qt::Key_Dead_Grave: key = XK_dead_grave; break;
        //    case Qt::Key_Dead_Acute: key = XK_dead_acute; break;
        //    case Qt::Key_Dead_Circumflex: key = XK_dead_circumflex; break;
        //    case Qt::Key_Dead_Tilde: key = XK_dead_tilde; break;
        //    case Qt::Key_Dead_Macron: key = XK_dead_macron; break;
        //    case Qt::Key_Dead_Breve: key = XK_dead_breve; break;
        //    case Qt::Key_Dead_Abovedot: key = XK_dead_abovedot; break;
        //    case Qt::Key_Dead_Diaeresis: key = XK_dead_diaeresis; break;
        //    case Qt::Key_Dead_Abovering: key = XK_dead_abovering; break;
        //    case Qt::Key_Dead_Doubleacute: key = XK_dead_doubleacute; break;
        //    case Qt::Key_Dead_Caron: key = XK_dead_caron; break;
        //    case Qt::Key_Dead_Cedilla: key = XK_dead_cedilla; break;
        //    case Qt::Key_Dead_Ogonek: key = XK_dead_ogonek; break;
        //    case Qt::Key_Dead_Iota: key = XK_dead_iota; break;
        //    case Qt::Key_Dead_Voiced_Sound: key = XK_dead_voiced_sound; break;
        //    case Qt::Key_Dead_Semivoiced_Sound: key = XK_dead_semivoiced_sound; break;
        //    case Qt::Key_Dead_Belowdot: key = XK_dead_belowdot; break;
    }

    if( _ke->key() >= Qt::Key_F1 && _ke->key() <= Qt::Key_F35 )
    {
        key = XK_F1 + _ke->key() - Qt::Key_F1;
    }
    else if( key == 0 )
    {
        if( m_mods.contains( XK_Control_L ) &&
                QKeySequence( _ke->key() ).toString().length() == 1 )
        {
            QString s = QKeySequence( _ke->key() ).toString();
            if( !m_mods.contains( XK_Shift_L ) )
            {
                s = s.toLower();
            }
            key = s.utf16()[0];
        }
        else
        {
            key = _ke->text().utf16()[0];
        }

    }
    // handle modifiers
    if( key == XK_Shift_L || key == XK_Control_L || key == XK_Meta_L ||
            key == XK_Alt_L )
    {
        if( pressed )
        {
            m_mods[key] = true;
        }
        else if( m_mods.contains( key ) )
        {
            m_mods.remove( key );
        }
        else
        {
            unpressModifiers();
        }
    }

    if( key )
    {
        if(m_viewerCore)
            m_viewerCore->sendKeyboardEvent(pressed,key );
        _ke->accept();
    }
}

void QVncViewerAdapter::unpressModifiers()
{
    QList<unsigned int> keys = m_mods.keys();
    QList<unsigned int>::const_iterator it = keys.begin();
    while( it != keys.end() )
    {
        if(m_viewerCore)
            m_viewerCore->sendKeyboardEvent(false,*it );

        it++;
    }
    m_mods.clear();
}


void QVncViewerAdapter::onBell()
{
    qInfo() << "[VNC_EVENT] onBell";
}

void QVncViewerAdapter::onCutText(const StringStorage *cutText)
{
    qInfo() << "[VNC_EVENT] onCutText";

}

void QVncViewerAdapter::onEstablished()
{
    qInfo() << "[VNC_EVENT] onEstablished";

}

void QVncViewerAdapter::onConnected(RfbOutputGate *output)
{
    qInfo() << "[VNC_EVENT] onConnected";

}

void QVncViewerAdapter::onDisconnect(const StringStorage *message)
{
    qInfo() << "[VNC_EVENT] onDisconnect";

    if(m_bufferImage){
        delete m_bufferImage;
        m_bufferImage = NULL;
    }
    repaint(QRect(0, 0, m_width, m_height));

}

void QVncViewerAdapter::onAuthError(const AuthException *exception)
{
    qInfo() << "[VNC_EVENT] onAuthError";

}

void QVncViewerAdapter::onError(const Exception *exception)
{
    qInfo() << "[VNC_EVENT] onError";

}

void QVncViewerAdapter::onFrameBufferUpdate(const FrameBuffer *fb, const Rect *update)
{
    qInfo() << "[VNC_EVENT] onFrameBufferUpdate";

    AutoLock al(&m_bufferLock);
    if (!m_framebuffer.copyFrom(update, fb, update->left, update->top)) {
        qWarning() << "copy buffer failed!";
    }
    repaint(rectToQRect(*update));
}

void QVncViewerAdapter::onFrameBufferPropChange(const FrameBuffer *fb)
{
    qInfo() << "[VNC_EVENT] onFrameBufferPropChange";

    Dimension dimension = fb->getDimension();
    Dimension olddimension = m_framebuffer.getDimension();

    AutoLock al(&m_bufferLock);

    m_width = dimension.width;
    m_height = dimension.height;
    m_bitsPerPixel = fb->getBitsPerPixel();

    if (!dimension.isEmpty()) {
        int alignWidth = (dimension.width + 3) / 4;
        dimension.width = alignWidth * 4;
        int alignHeight = (dimension.height + 3) / 4;
        dimension.height = alignHeight * 4;
        m_framebuffer.setProperties(&dimension,
                                    &fb->getPixelFormat());
        m_framebuffer.setColor(0, 0, 0);
    } else {
        m_framebuffer.setColor(0, 0, 0);
    }

    if(m_bufferImage)
        delete m_bufferImage;

    switch(m_framebuffer.getBitsPerPixel()) {
    case 8:
        m_bufferImage = new QImage((uchar *)m_framebuffer.getBufferPtr(0, 0), dimension.width, dimension.height, QImage::Format_Indexed8);
        break;
    case 16:
        m_bufferImage = new QImage((uchar *)m_framebuffer.getBufferPtr(0, 0), dimension.width, dimension.height, QImage::Format_RGB16);
        break;
    case 32:
        m_bufferImage = new QImage((uchar *)m_framebuffer.getBufferPtr(0, 0), dimension.width, dimension.height, QImage::Format_RGB32);
        break;
    }

    if (dimension.isEmpty()) {
        QRect r(dimension.getRect().left, dimension.getRect().top,
                dimension.getRect().right-dimension.getRect().left,
                dimension.getRect().bottom-dimension.getRect().top);
        repaint(r);
    }
}

