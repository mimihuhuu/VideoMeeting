#include "CmainWidget.h"
#include "CSmallVideWidget.h"
#include "commons.h"
#include "CAgoraObject.h"
#include "ShareScreenDialog.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QMessageBox>
#include <QGuiApplication>
#include <QScreen>
#include <QShowEvent>
#include <QEvent>


CmainWidget::CmainWidget(QWidget* p)
    : CFrameLessWidgetBase(p)
{
	initUI();
	m_pAgoraObject = new CAgoraObject();
	if (0 != m_pAgoraObject->init())
	{
		QMessageBox::information(this, u8"\u63d0\u793a", u8"agora init failed");
		exit(EXIT_FAILURE);
	}

	connect(m_pAgoraObject, &CAgoraObject::sender_joinedChannelSuccess,
			this, &CmainWidget::on_LocalJoinSuccess);
	connect(m_pAgoraObject, &CAgoraObject::sender_userJoined,
			this, &CmainWidget::on_RemoteJoined);

}

CmainWidget::~CmainWidget()
{
}

void CmainWidget::joinroom(const QString& roomID)
{
	m_pAgoraObject->joinChannel(roomID, 123456);
}

void CmainWidget::initUI()
{
	m_pTitleBar = new CTitleBar(this);
	m_pBigVideoWidget = new CBigVideoWidget(this);
	m_pLeftVideoList = new CLeftVideoList(this);
	m_pBottonBar = new CBottonBar(this);

	QVBoxLayout* pMainLayout = new QVBoxLayout(this);
	pMainLayout->setSpacing(0);
	pMainLayout->addWidget(m_pTitleBar);

	QHBoxLayout* pHlay = new QHBoxLayout();
	pHlay->setSpacing(0);
	pHlay->addWidget(m_pLeftVideoList);

	QWidget* pVideoPanel = new QWidget(this);
	pVideoPanel->setAttribute(Qt::WA_StyledBackground, true);
	pVideoPanel->setStyleSheet("background-color: rgb(26, 26, 26);");

	QVBoxLayout* pVlay = new QVBoxLayout(pVideoPanel);
	pVlay->setSpacing(0);
	pVlay->setContentsMargins(0, 0, 0, 0);
	pVlay->addWidget(m_pBigVideoWidget, 1);
	pVlay->addWidget(m_pBottonBar, 0, Qt::AlignBottom);

	pHlay->addWidget(pVideoPanel, 1);
	pMainLayout->addLayout(pHlay);

	pMainLayout->setContentsMargins(0, 0, 0, 0);

	connect(m_pTitleBar, &CTitleBar::sig_close, this, &QWidget::close);
	connect(m_pBottonBar, &CBottonBar::sig_EndMeeting, this, &CmainWidget::on_EndMeeting);
	connect(m_pBottonBar, &CBottonBar::sig_enableVideo, this, &CmainWidget::on_enableVideo);
	connect(m_pBottonBar, &CBottonBar::sig_enableAudio, this, &CmainWidget::on_enableAudio);
	connect(m_pBottonBar, &CBottonBar::sig_shareScreen, this, &CmainWidget::on_shareScreen);

	resize(MAIN_WINDOW_DEFAULT_WIDTH, MAIN_WINDOW_DEFAULT_HEIGHT);
}

void CmainWidget::showEvent(QShowEvent* event)
{
	CFrameLessWidgetBase::showEvent(event);
	if (m_centeredOnShow || isMaximized()) {
		return;
	}
	QScreen* screen = QGuiApplication::primaryScreen();
	if (!screen) {
		return;
	}
	m_centeredOnShow = true;
	const QRect avail = screen->availableGeometry();
	move(avail.center() - rect().center());
}

void CmainWidget::changeEvent(QEvent* event)
{
	CFrameLessWidgetBase::changeEvent(event);
	if (event->type() == QEvent::WindowStateChange) {
		updateMainVideoRenderMode();
	}
}

void CmainWidget::updateMainVideoRenderMode()
{
	if (!m_localPreviewActive) {
		return;
	}
	const RENDER_MODE_TYPE mode = isMaximized() ? RENDER_MODE_HIDDEN : RENDER_MODE_FIT;
	m_pAgoraObject->LocalVideoPreview(m_pBigVideoWidget->getHWND(), true, mode);
}

void CmainWidget::on_LocalJoinSuccess(const QString& qsChannel, unsigned int uid, int elapsed)
{
	Q_UNUSED(qsChannel);
	Q_UNUSED(uid);
	Q_UNUSED(elapsed);
	m_localPreviewActive = true;
	updateMainVideoRenderMode();
}

void CmainWidget::on_RemoteJoined(uid_t uid, int elapsed)
{
	Q_UNUSED(elapsed);
	CSmallVideWidget* pSmall = new CSmallVideWidget();
	m_pLeftVideoList->addVideoWidget(pSmall);
	m_pAgoraObject->RemoteVideoRender(uid, (HWND)(pSmall->winId()), RENDER_MODE_HIDDEN);
}


void CmainWidget::on_enableVideo(bool enabled)
{
	if (!m_pAgoraObject) {
		return;
	}
	if (0 != m_pAgoraObject->enableVideo(enabled)) {
		QMessageBox::warning(this, u8"\u63d0\u793a",
			enabled ? u8"\u5f00\u542f\u6444\u50cf\u5934\u5931\u8d25" : u8"\u5173\u95ed\u6444\u50cf\u5934\u5931\u8d25");
		return;
	}
	if (!m_localPreviewActive) {
		return;
	}
	if (enabled) {
		updateMainVideoRenderMode();
	} else {
		m_pAgoraObject->LocalVideoPreview(m_pBigVideoWidget->getHWND(), FALSE);
	}
}

void CmainWidget::on_enableAudio(bool enabled)
{
	if (!m_pAgoraObject) {
		return;
	}
	// enabled=true ???enabled=false ????? muteLocalAudioStream(true)?
	if (0 != m_pAgoraObject->muteLocalAudio(!enabled)) {
		QMessageBox::warning(this, u8"\u63d0\u793a",
			enabled ? u8"\u5f00\u542f\u9ea6\u514b\u98ce\u5931\u8d25" : u8"\u5173\u95ed\u9ea6\u514b\u98ce\u5931\u8d25");
	}
}

void CmainWidget::on_EndMeeting()
{
	close();
}

void CmainWidget::on_shareScreen()
{
	if (!m_pAgoraObject) {
		return;
	}

	VecWindowShareInfo vecWindowShareInfo;
	m_pAgoraObject->ShareScreen(vecWindowShareInfo);

	ShareScreenDialog share(this);
	share.initListWidget(vecWindowShareInfo);

	connect(&share, &ShareScreenDialog::sig_StartShare, this, &CmainWidget::statShareScreen);

	share.exec();
}

void CmainWidget::statShareScreen(int type, qint64 sourceId)
{
	if (0 != m_pAgoraObject->start_share_screen(type, sourceId))
	{
		QMessageBox::information(this, u8"\u63d0\u793a", u8"\u5c4f\u5e55\u5171\u4eab\u5931\u8d25");
	}
}
