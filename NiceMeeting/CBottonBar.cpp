#include "CBottonBar.h"
#include "commons.h"
#include <QHBoxLayout>
#include <QList>

namespace {
const char* kBottomBarRes = ":/bottomBar/resources/mainwidget/bottomBar/";

const char* kEndMeetingStyle =
	"QPushButton {"
	"    background-color: #FFFFFF;"
	"    color: #E53935;"
	"    border: 1px solid #FFCDD2;"
	"    font-family: 'Microsoft YaHei';"
	"    font-size: 14px;"
	"    padding: 0 18px;"
	"}"
	"QPushButton:hover {"
	"    background-color: #FFEBEE;"
	"    border-color: #EF9A9A;"
	"}"
	"QPushButton:pressed {"
	"    background-color: #FFCDD2;"
	"}";

} // namespace

CBottonBar::CBottonBar(QWidget* p)
	: QWidget(p)
{
	setFixedHeight(Bottonbar_HEIGHT);
	setAttribute(Qt::WA_StyledBackground, true);
	setStyleSheet("background-color: #FAF8F5;");

	m_pAudioToolButton = new CCustomButton(u8"\u58f0\u97f3",
		QString(kBottomBarRes) + "microfeng_open.svg",
		QString(kBottomBarRes) + "microfeng_open_hover.svg",
		QString(kBottomBarRes) + "microfeng_close.svg",
		QString(kBottomBarRes) + "microfeng_close_hover.svg",
		true, this);

	m_pCameraToolButton = new CCustomButton(u8"\u6444\u50cf\u5934",
		QString(kBottomBarRes) + "camera_open.svg",
		QString(kBottomBarRes) + "camera_open_hover.svg",
		QString(kBottomBarRes) + "camera_close.svg",
		QString(kBottomBarRes) + "camera_close_hover.svg",
		true, this);

	m_pShareSreenToolButton = new CCustomButton(u8"\u5171\u4eab\u5c4f\u5e55",
		QString(kBottomBarRes) + "screen_share.svg",
		QString(kBottomBarRes) + "screen_share_hover.svg",
		QString(kBottomBarRes) + "screen_share.svg",
		QString(kBottomBarRes) + "screen_share_hover.svg",
		true, this);

	m_pSecurityToolButton = new CCustomButton(u8"\u5b89\u5168",
		QString(kBottomBarRes) + "anquan.svg",
		QString(kBottomBarRes) + "anquan_hover.svg",
		QString(kBottomBarRes) + "anquan.svg",
		QString(kBottomBarRes) + "anquan_hover.svg",
		true, this);

	m_pInviationToolButton = new CCustomButton(u8"\u9080\u8bf7",
		QString(kBottomBarRes) + "yaoqing.svg",
		QString(kBottomBarRes) + "yaoqing_hover.svg",
		QString(kBottomBarRes) + "yaoqing.svg",
		QString(kBottomBarRes) + "yaoqing_hover.svg",
		true, this);

	m_pManageMembersToolButton = new CCustomButton(u8"\u7ba1\u7406\u6210\u5458",
		QString(kBottomBarRes) + "manage_member.svg",
		QString(kBottomBarRes) + "manage_member_hover.svg",
		QString(kBottomBarRes) + "manage_member.svg",
		QString(kBottomBarRes) + "manage_member_hover.svg",
		true, this);

	m_pChatToolButton = new CCustomButton(u8"\u804a\u5929",
		QString(kBottomBarRes) + "chat.svg",
		QString(kBottomBarRes) + "chat_hover.svg",
		QString(kBottomBarRes) + "chat.svg",
		QString(kBottomBarRes) + "chat_hover.svg",
		true, this);

	m_pRecordToolButton = new CCustomButton(u8"\u5f55\u5236",
		QString(kBottomBarRes) + "rec.svg",
		QString(kBottomBarRes) + "rec_hover.svg",
		QString(kBottomBarRes) + "rec.svg",
		QString(kBottomBarRes) + "rec_hover.svg",
		true, this);

	m_pGorupDiscussionToolButton = new CCustomButton(u8"\u5206\u7ec4\u8ba8\u8bba",
		QString(kBottomBarRes) + "group.svg",
		QString(kBottomBarRes) + "group_hover.svg",
		QString(kBottomBarRes) + "group.svg",
		QString(kBottomBarRes) + "group_hover.svg",
		true, this);

	m_pApplicationToolButton = new CCustomButton(u8"\u5e94\u7528",
		QString(kBottomBarRes) + "application.svg",
		QString(kBottomBarRes) + "application_hover.svg",
		QString(kBottomBarRes) + "application.svg",
		QString(kBottomBarRes) + "application_hover.svg",
		true, this);

	m_pSettingsToolButton = new CCustomButton(u8"\u8bbe\u7f6e",
		QString(kBottomBarRes) + "ctrlwidget_set.svg",
		QString(kBottomBarRes) + "ctrlwidget_set_hover.svg",
		QString(kBottomBarRes) + "ctrlwidget_set.svg",
		QString(kBottomBarRes) + "ctrlwidget_set_hover.svg",
		true, this);

	m_pEndMeetingToolButton = new QPushButton(u8"\u7ed3\u675f\u4f1a\u8bae", this);
	m_pEndMeetingToolButton->setFixedSize(BOTTONBAR_END_BTN_WIDTH, BOTTONBAR_END_BTN_HEIGHT);
	m_pEndMeetingToolButton->setCursor(Qt::PointingHandCursor);
	m_pEndMeetingToolButton->setStyleSheet(kEndMeetingStyle);

	QHBoxLayout* pHlayout = new QHBoxLayout(this);
	pHlayout->setContentsMargins(BOTTONBAR_H_MARGIN, 0, BOTTONBAR_H_MARGIN, 0);
	pHlayout->setSpacing(BOTTONBAR_ITEM_SPACING);
	pHlayout->setAlignment(Qt::AlignVCenter);

	const QList<QWidget*> toolButtons = {
		m_pAudioToolButton, m_pCameraToolButton, m_pShareSreenToolButton,
		m_pSecurityToolButton, m_pInviationToolButton, m_pManageMembersToolButton,
		m_pChatToolButton, m_pRecordToolButton, m_pGorupDiscussionToolButton,
		m_pApplicationToolButton, m_pSettingsToolButton
	};
	for (QWidget* btn : toolButtons) {
		pHlayout->addWidget(btn, 0, Qt::AlignVCenter);
	}
	pHlayout->addStretch(1);
	pHlayout->addWidget(m_pEndMeetingToolButton, 0, Qt::AlignVCenter);
			
	connect(m_pEndMeetingToolButton, &QPushButton::clicked, this, &CBottonBar::sig_EndMeeting);
	connect(m_pAudioToolButton, &CCustomButton::sig_clicked, this, [this]() {
		m_audioEnabled = !m_audioEnabled;
		m_pAudioToolButton->setOpen(m_audioEnabled);
		emit sig_enableAudio(m_audioEnabled);
	});
	connect(m_pCameraToolButton, &CCustomButton::sig_clicked, this, [this]() {
		m_cameraEnabled = !m_cameraEnabled;
		m_pCameraToolButton->setOpen(m_cameraEnabled);
		emit sig_enableVideo(m_cameraEnabled);
	});
	connect(m_pShareSreenToolButton, &CCustomButton::sig_clicked, this,&CBottonBar::sig_shareScreen);
}

CBottonBar::~CBottonBar()
{
}
