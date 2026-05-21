#include "CFrameLessWidgetBase.h"
#include <windows.h>
#include <windowsx.h>

CFrameLessWidgetBase::CFrameLessWidgetBase(QWidget* p)
{
    this->setWindowFlags(Qt::FramelessWindowHint | Qt::WindowMinMaxButtonsHint);
    setAttribute(Qt::WA_Hover);
}

bool CFrameLessWidgetBase::nativeEvent(const QByteArray& eventType, void* message, long* result)
{
    MSG* param = static_cast<MSG*>(message);
    switch (param->message)
    {
    case WM_NCHITTEST:
    {
        int nX = GET_X_LPARAM(param->lParam) - this->geometry().x();
        int nY = GET_Y_LPARAM(param->lParam) - this->geometry().y();

        if ((nX > m_nBorderWith) && (nX < this->width() - m_nBorderWith) && (nY > m_nBorderWith) && (nY < this->height() - m_nBorderWith))
        {
            if (childAt(nX, nY) != nullptr) return QWidget::nativeEvent(eventType, message, result);
        }
        if ((nX > 0) && (nX < m_nBorderWith)) *result = HTLEFT;
        if ((nX > this->width() - m_nBorderWith) && (nX < this->width())) *result = HTRIGHT;
        if ((nY > 0) && (nY < m_nBorderWith)) *result = HTTOP;
        if ((nY > this->height() - m_nBorderWith) && (nY < this->height())) *result = HTBOTTOM;
        if ((nX > 0) && (nX < m_nBorderWith) && (nY > 0) && (nY < m_nBorderWith)) *result = HTTOPLEFT;
        if ((nX > this->width() - m_nBorderWith) && (nX < this->width()) && (nY > 0) && (nY < m_nBorderWith)) *result = HTTOPRIGHT;
        if ((nX > 0) && (nX < m_nBorderWith) && (nY > this->height() - m_nBorderWith) && (nY < this->height())) *result = HTBOTTOMLEFT;
        if ((nX > this->width() - m_nBorderWith) && (nX < this->width()) && (nY > this->height() - m_nBorderWith) && (nY < this->height())) *result = HTBOTTOMRIGHT;
        return true;
    }
    }
    return false;
}
