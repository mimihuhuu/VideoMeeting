#pragma once
#include <QWidget>

class CSmallVideWidget : public QWidget
{
	Q_OBJECT

public:
	explicit CSmallVideWidget(unsigned int memberUid, QWidget* parent = nullptr);
	~CSmallVideWidget() override;

	unsigned int memberUid() const { return m_memberUid; }
	HWND videoHwnd() const;

protected:
	void resizeEvent(QResizeEvent* event) override;
	void showEvent(QShowEvent* event) override;

signals:
	void sig_videoSurfaceChanged();

private:
	unsigned int m_memberUid = 0;
};
