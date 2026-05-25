#pragma once
#include <QToolButton>
#include "commons.h"

class CCustomButton : public QToolButton
{
	Q_OBJECT

public:
	CCustomButton(QString text, QString normalImageUrl, QString normalhoverImageUrl,
				QString clickedImageUrl, QString clickedImagehoverUrl, bool clicked, QWidget* parent = nullptr);
	~CCustomButton();

	bool event(QEvent* event) override;
	
	void setOpen(bool open)
	{
		m_StateOpen = open;
		setChecked(false);
		setIcon(QIcon(m_StateOpen ? m_normalImageUrl : m_clickedImageUrl));
	}

signals:
	void sig_clicked();

private:
	QString m_text;
	QString m_normalImageUrl;
	QString m_normalhoverImageUrl;
	QString m_clickedImageUrl;
	QString m_clickedImagehoverUrl;
	bool m_StateOpen = false;
};
