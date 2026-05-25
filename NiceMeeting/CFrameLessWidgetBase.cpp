#include "CFrameLessWidgetBase.h"

#include <QPoint>
#include <windows.h>
#include <windowsx.h>

CFrameLessWidgetBase::CFrameLessWidgetBase(QWidget* p)
    : QWidget(p)
{
    setWindowFlags(Qt::FramelessWindowHint | Qt::WindowMinMaxButtonsHint);
    setAttribute(Qt::WA_Hover);
}
    
bool CFrameLessWidgetBase::nativeEvent(const QByteArray& eventType, void* message, long* result)
{
    if (eventType != "windows_generic_MSG" && eventType != "windows_dispatcher_MSG") {
        return QWidget::nativeEvent(eventType, message, result);
    }

    MSG* msg = static_cast<MSG*>(message);
    if (!msg || msg->message != WM_NCHITTEST) {
        return QWidget::nativeEvent(eventType, message, result);
    }

    const QPoint pos = mapFromGlobal(QPoint(GET_X_LPARAM(msg->lParam), GET_Y_LPARAM(msg->lParam)));
    const int x = pos.x();
    const int y = pos.y();
    const int bw = m_nBorderWidth;
    const int w = width();
    const int h = height();

    const bool onLeft = x >= 0 && x < bw;
    const bool onRight = x > w - bw && x < w;
    const bool onTop = y >= 0 && y < bw;
    const bool onBottom = y > h - bw && y < h;

    if (onTop && onLeft) {
        *result = HTTOPLEFT;
        return true;
    }
    if (onTop && onRight) {
        *result = HTTOPRIGHT;
        return true;
    }
    if (onBottom && onLeft) {
        *result = HTBOTTOMLEFT;
        return true;
    }
    if (onBottom && onRight) {
        *result = HTBOTTOMRIGHT;
        return true;
    }
    if (onLeft) {
        *result = HTLEFT;
        return true;
    }
    if (onRight) {
        *result = HTRIGHT;
        return true;
    }
    if (onTop) {
        *result = HTTOP;
        return true;
    }
    if (onBottom) {
        *result = HTBOTTOM;
        return true;
    }

    if (x >= bw && x <= w - bw && y >= bw && y <= h - bw) {
        if (childAt(x, y) != nullptr) {
            return QWidget::nativeEvent(eventType, message, result);
        }
        *result = HTCLIENT;
        return true;
    }

    return QWidget::nativeEvent(eventType, message, result);
}
