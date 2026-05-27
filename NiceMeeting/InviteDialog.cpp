#include "InviteDialog.h"

#include <QApplication>
#include <QClipboard>
#include <QLabel>
#include <QVBoxLayout>

InviteDialog::InviteDialog(const QString& roomId, QWidget* parent)
	: MeetingFramelessDialog(QString::fromUtf8(u8"\u9080\u8bf7"), parent)
{
	setMinimumSize(460, 180);

	QApplication::clipboard()->setText(roomId);

	auto* hintLabel = new QLabel(
		QString::fromUtf8(u8"\u623f\u95f4\u53f7\u5df2\u590d\u5236\u5230\u526a\u8d34\u677f\uff0c\u8bf7\u53d1\u9001\u7ed9\u5176\u4ed6\u4eba\u3002\n"
		u8"\u5bf9\u65b9\u5728\u767b\u5f55\u754c\u9762\u8f93\u5165\u8be5\u623f\u95f4\u53f7\u5373\u53ef\u52a0\u5165\u3002"), this);
	hintLabel->setWordWrap(true);

	m_pRoomLabel = new QLabel(this);
	m_pRoomLabel->setText(QString::fromUtf8(u8"\u623f\u95f4\u53f7\uff1a%1").arg(roomId));
	m_pRoomLabel->setStyleSheet(
		QStringLiteral("font-family: Microsoft YaHei; font-size: 24px; font-weight: bold; color: #333333;"));

	bodyLayout()->addWidget(hintLabel);
	bodyLayout()->addWidget(m_pRoomLabel);
	bodyLayout()->addStretch(1);
}
