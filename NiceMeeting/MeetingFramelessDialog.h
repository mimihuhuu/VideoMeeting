#pragma once
#include <QDialog>

class QLabel;
class QPushButton;
class QVBoxLayout;
class QMouseEvent;

class MeetingFramelessDialog : public QDialog
{
	Q_OBJECT

public:
	explicit MeetingFramelessDialog(const QString& title, QWidget* parent = nullptr);
	~MeetingFramelessDialog() override;

	QVBoxLayout* bodyLayout() const;
	QWidget* titleBarWidget() const { return m_pTitleBar; }

protected:
	void mousePressEvent(QMouseEvent* event) override;
	void mouseMoveEvent(QMouseEvent* event) override;
	void mouseReleaseEvent(QMouseEvent* event) override;

private:
	QWidget* m_pTitleBar = nullptr;
	QLabel* m_pTitleLabel = nullptr;
	QWidget* m_pBody = nullptr;
	QVBoxLayout* m_pBodyLayout = nullptr;

	QPoint m_windowPos;
	QPoint m_mousePos;
	bool m_dragging = false;
};
