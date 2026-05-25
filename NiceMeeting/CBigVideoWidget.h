#pragma once
#include <QWidget>

class CBigVideoWidget : public QWidget
{
	Q_OBJECT

public:
	CBigVideoWidget(QWidget* p = nullptr);
	~CBigVideoWidget() override;

	HWND getHWND() const;

protected:
	bool hasHeightForWidth() const override;
	int heightForWidth(int w) const override;
};
