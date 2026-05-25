#pragma once
#include <QWidget>

class CFrameLessWidgetBase : public QWidget
{
public:
    CFrameLessWidgetBase(QWidget* p = nullptr);
    ~CFrameLessWidgetBase() override = default;

protected:
    int m_nBorderWidth = 8;

private:
    bool nativeEvent(const QByteArray& eventType, void* message, long* result) override;
};
