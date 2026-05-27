#pragma once
#include <QWidget>
#include "commons.h"

class QLabel;
class QMouseEvent;

class CCustomButton : public QWidget
{
	Q_OBJECT

public:
	CCustomButton(QString text, QString normalImageUrl, QString normalhoverImageUrl,
				QString clickedImageUrl, QString clickedImagehoverUrl, bool clicked, QWidget* parent = nullptr);
	~CCustomButton() override;

	void setText(const QString& text);
	void setOpen(bool open);

protected:
	void enterEvent(QEvent* event) override;
	void leaveEvent(QEvent* event) override;
	void mousePressEvent(QMouseEvent* event) override;

signals:
	void sig_clicked();

private:
	void updateIcon(bool hover);

	QString m_text;
	QString m_normalImageUrl;
	QString m_normalhoverImageUrl;
	QString m_clickedImageUrl;
	QString m_clickedImagehoverUrl;
	bool m_StateOpen = false;
	bool m_hovered = false;
	QLabel* m_pIconLabel = nullptr;
	QLabel* m_pTextLabel = nullptr;
};
