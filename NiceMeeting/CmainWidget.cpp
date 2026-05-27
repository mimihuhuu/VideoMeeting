#include "CmainWidget.h"
#include "ConnectMicDialog.h"
#include "CSmallVideWidget.h"
#include "commons.h"
#include "CAgoraObject.h"
#include "ShareScreenDialog.h"
#include "MemberManageDialog.h"
#include "RecordingManageDialog.h"
#include "RecordingSettingsDialog.h"
#include "InviteDialog.h"
#include "AgoraRestAuth.h"
#include "agoraconfig.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QMessageBox>
#include <QClipboard>
#include <QGuiApplication>
#include <QScreen>
#include <QShowEvent>
#include <QEvent>
#include <QDateTime>
#include <QDir>
#include <QRandomGenerator>
#include <QTimer>

extern CAgoraConfig gAgoraConfig;

CmainWidget::CmainWidget(QWidget* p)
    : CFrameLessWidgetBase(p)
{
	initUI();
	m_pAgoraObject = new CAgoraObject();
	m_pKickService = new CAgoraKickService(this);
	m_pScreenRegionRecorder = new ScreenRegionRecorder(this);
	if (0 != m_pAgoraObject->init())
	{
		QMessageBox::information(this, u8"\u63d0\u793a", u8"agora init failed");
		exit(EXIT_FAILURE);
	}

	connect(m_pAgoraObject, &CAgoraObject::sender_joinedChannelSuccess,
			this, &CmainWidget::on_LocalJoinSuccess);
	connect(m_pAgoraObject, &CAgoraObject::sender_userJoined,
			this, &CmainWidget::on_RemoteJoined);
	connect(m_pAgoraObject, &CAgoraObject::sender_userOffline,
			this, &CmainWidget::on_RemoteOffline);
	connect(m_pKickService, &CAgoraKickService::kickFinished,
			this, &CmainWidget::on_kickFinished);
	connect(m_pAgoraObject, &CAgoraObject::sender_chatMessageReceived,
			this, &CmainWidget::on_chatMessageReceived);
	connect(m_pAgoraObject, &CAgoraObject::sender_controlCommandReceived,
			this, &CmainWidget::on_controlCommandReceived);
}

CmainWidget::~CmainWidget()
{
}

void CmainWidget::joinroom(const QString& roomID)
{
	m_roomId = roomID.trimmed();
	const unsigned int joinUid = static_cast<unsigned int>(
		QRandomGenerator::global()->bounded(100001u, 999999999u));
	m_pAgoraObject->joinChannel(m_roomId, joinUid);
}

void CmainWidget::setJoinMediaOptions(bool cameraEnabled, bool micEnabled)
{
	m_joinWithCamera = cameraEnabled;
	m_joinWithMic = micEnabled;
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
	connect(m_pBottonBar, &CBottonBar::sig_connectMic, this, &CmainWidget::on_connectMic);
	connect(m_pBottonBar, &CBottonBar::sig_shareScreen, this, &CmainWidget::on_shareScreen);
	connect(m_pBottonBar, &CBottonBar::sig_invite, this, &CmainWidget::on_invite);
	connect(m_pBottonBar, &CBottonBar::sig_manageMembers, this, &CmainWidget::on_manageMembers);
	connect(m_pBottonBar, &CBottonBar::sig_chat, this, &CmainWidget::on_chat);
	connect(m_pBottonBar, &CBottonBar::sig_recordScreen, this, &CmainWidget::on_recordScreen);
	connect(m_pBottonBar, &CBottonBar::sig_settings, this, &CmainWidget::on_settings);

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

QString CmainWidget::currentAppId() const
{
	const QString appId = gAgoraConfig.getAppId();
	return appId.isEmpty() ? QStringLiteral(AGORA_APP_ID) : appId;
}

void CmainWidget::syncLocalMicUi(bool micEnabled)
{
	if (m_pBottonBar) {
		m_pBottonBar->setAudioEnabled(micEnabled);
	}
}

void CmainWidget::applyInitialAudioPolicy()
{
	if (!m_pAgoraObject) {
		return;
	}

	m_pAgoraObject->muteAllRemoteAudio(true);
}

void CmainWidget::applyJoinMediaOptions()
{
	if (!m_pAgoraObject) {
		return;
	}

	if (m_joinWithCamera) {
		updateMainVideoRenderMode();
	} else {
		m_pAgoraObject->enableVideo(false);
	}
	if (m_pBottonBar) {
		m_pBottonBar->setCameraEnabled(m_joinWithCamera);
	}

	m_pAgoraObject->muteLocalAudio(!m_joinWithMic);
	if (m_pBottonBar) {
		m_pBottonBar->setAudioEnabled(m_joinWithMic);
	}
}

void CmainWidget::on_LocalJoinSuccess(const QString& qsChannel, unsigned int uid, int elapsed)
{
	Q_UNUSED(elapsed);
	if (!qsChannel.isEmpty()) {
		m_roomId = qsChannel;
	}
	m_pAgoraObject->prepareChatAfterJoin(uid);
	m_localPreviewActive = true;
	applyInitialAudioPolicy();
	applyJoinMediaOptions();
}

void CmainWidget::on_RemoteJoined(uid_t uid, int elapsed)
{
	Q_UNUSED(elapsed);
	if (uid == 0 || !m_pAgoraObject || uid == m_pAgoraObject->localUid() || !m_pLeftVideoList) {
		return;
	}

	const unsigned int remoteUid = static_cast<unsigned int>(uid);
	const bool isRejoin = m_pLeftVideoList->hasVideoWidget(remoteUid);
	if (isRejoin) {
		m_pAgoraObject->unbindRemoteVideo(remoteUid);
		m_pLeftVideoList->removeVideoWidget(remoteUid);
	}

	if (!isRejoin && m_pLeftVideoList->remoteMemberUids().size() >= MAX_REMOTE_VIDEO_WIDGETS) {
		m_pAgoraObject->muteRemoteVideo(remoteUid, true);
		m_pAgoraObject->muteRemoteAudio(remoteUid, true);
		refreshMemberDialogs();
		return;
	}

	auto* pSmall = new CSmallVideWidget(remoteUid);
	connect(pSmall, &CSmallVideWidget::sig_videoSurfaceChanged, this, [this, remoteUid, pSmall]() {
		if (!m_pAgoraObject || !pSmall || !m_pLeftVideoList
			|| !m_pLeftVideoList->hasVideoWidget(remoteUid)) {
			return;
		}
		m_pAgoraObject->RemoteVideoRender(remoteUid, pSmall->videoHwnd(), RENDER_MODE_HIDDEN);
	});
	m_pLeftVideoList->addVideoWidget(remoteUid, pSmall);
	QTimer::singleShot(0, this, [this, remoteUid, pSmall]() {
		if (!m_pAgoraObject || !pSmall) {
			return;
		}
		m_pAgoraObject->RemoteVideoRender(remoteUid, pSmall->videoHwnd(), RENDER_MODE_HIDDEN);
	});
	m_pAgoraObject->muteRemoteAudio(remoteUid, true);
	refreshMemberDialogs();
}

void CmainWidget::on_RemoteOffline(unsigned int uid, USER_OFFLINE_REASON_TYPE reason)
{
	Q_UNUSED(reason);
	if (!m_pLeftVideoList || !m_pAgoraObject || uid == 0) {
		return;
	}

	if (m_pAgoraObject->isMeetingRecording() && m_pAgoraObject->recordingUid() == uid) {
		m_pAgoraObject->stopMeetingRecording();
		m_meetingRecordingActive = false;
		if (m_pBottonBar) {
			m_pBottonBar->setRecordingActive(false);
		}
	}

	m_pAgoraObject->unbindRemoteVideo(uid);
	m_pLeftVideoList->removeVideoWidget(uid);
	refreshMemberDialogs();
}

void CmainWidget::refreshMemberDialogs()
{
	if (!m_pLeftVideoList || !m_pAgoraObject) {
		return;
	}

	const unsigned int localUid = m_pAgoraObject->localUid();
	const QList<unsigned int> memberUids = m_pLeftVideoList->remoteMemberUids();

	if (m_pChatDialog) {
		m_pChatDialog->updateMemberList(memberUids);
	}
	if (m_pMemberDialog) {
		m_pMemberDialog->updateMemberList(memberUids);
	}
	if (m_pRecordingDialog) {
		m_pRecordingDialog->updateRemoteMembers(localUid, memberUids);
	}
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
	if (0 != m_pAgoraObject->muteLocalAudio(!enabled)) {
		QMessageBox::warning(this, u8"\u63d0\u793a",
			enabled ? u8"\u5f00\u542f\u9ea6\u514b\u98ce\u5931\u8d25" : u8"\u5173\u95ed\u9ea6\u514b\u98ce\u5931\u8d25");
		return;
	}
	syncLocalMicUi(enabled);
}

void CmainWidget::applyConnectMicRemoteAudio(unsigned int fromUid, unsigned int targetUid, bool unmute)
{
	if (!m_pAgoraObject || targetUid == 0) {
		return;
	}

	const unsigned int localUid = m_pAgoraObject->localUid();
	if (fromUid != localUid) {
		m_pAgoraObject->muteRemoteAudio(fromUid, !unmute);
	}
	if (targetUid != localUid) {
		m_pAgoraObject->muteRemoteAudio(targetUid, !unmute);
	}
}

void CmainWidget::on_connectMic(bool enabled)
{
	if (!m_pAgoraObject || !m_pLeftVideoList) {
		return;
	}

	if (enabled) {
		const QList<unsigned int> memberUids = m_pLeftVideoList->remoteMemberUids();

		ConnectMicDialog dialog(memberUids, this);
		if (dialog.exec() != QDialog::Accepted) {
			if (m_pBottonBar) {
				m_pBottonBar->setConnectMicActive(false);
			}
			return;
		}

		const unsigned int targetUid = dialog.selectedUid();
		if (targetUid == 0) {
			if (m_pBottonBar) {
				m_pBottonBar->setConnectMicActive(false);
			}
			return;
		}

		if (0 != m_pAgoraObject->sendControlMessage(QStringLiteral("connectMic"), targetUid)) {
			QMessageBox::warning(this, u8"\u63d0\u793a", u8"\u8fde\u9ea6\u8bf7\u6c42\u53d1\u9001\u5931\u8d25");
			if (m_pBottonBar) {
				m_pBottonBar->setConnectMicActive(false);
			}
			return;
		}

		m_connectMicTargetUid = targetUid;
		applyConnectMicRemoteAudio(m_pAgoraObject->localUid(), targetUid, true);
		return;
	}

	if (m_connectMicTargetUid == 0) {
		if (m_pBottonBar) {
			m_pBottonBar->setConnectMicActive(false);
		}
		return;
	}

	const unsigned int targetUid = m_connectMicTargetUid;
	if (0 != m_pAgoraObject->sendControlMessage(QStringLiteral("disconnectMic"), targetUid)) {
		QMessageBox::warning(this, u8"\u63d0\u793a", u8"\u65ad\u5f00\u8fde\u9ea6\u5931\u8d25");
		if (m_pBottonBar) {
			m_pBottonBar->setConnectMicActive(true);
		}
		return;
	}

	applyConnectMicRemoteAudio(m_pAgoraObject->localUid(), targetUid, false);
	m_connectMicTargetUid = 0;
}

void CmainWidget::on_EndMeeting()
{
	stopScreenRegionRecording(false);
	if (m_pAgoraObject && m_pAgoraObject->isMeetingRecording()) {
		m_pAgoraObject->stopMeetingRecording();
	}
	m_meetingRecordingActive = false;
	m_connectMicTargetUid = 0;
	if (m_pBottonBar) {
		m_pBottonBar->setRecordingActive(false);
		m_pBottonBar->setConnectMicActive(false);
	}
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

void CmainWidget::on_recordScreen()
{
	if (m_roomId.isEmpty()) {
		QMessageBox::warning(this, u8"\u63d0\u793a", u8"\u5f53\u524d\u672a\u52a0\u5165\u4f1a\u8bae\u3002");
		return;
	}

	if (!m_pAgoraObject || !m_pBottonBar || !m_pLeftVideoList) {
		return;
	}

	if (m_screenRegionRecordingActive) {
		stopScreenRegionRecording();
		return;
	}

	if (m_meetingRecordingActive) {
		on_toggleParticipantRecording(m_pAgoraObject->recordingUid());
		return;
	}

	showRecordingManageDialog();
}

void CmainWidget::on_settings()
{
	const bool recordingBusy = m_meetingRecordingActive
		|| m_screenRegionRecordingActive
		|| (m_pAgoraObject && m_pAgoraObject->isMeetingRecording());

	RecordingSettingsDialog dialog(recordingBusy, this);
	if (dialog.exec() != QDialog::Accepted) {
		return;
	}

	if (!recordingBusy && m_pAgoraObject) {
		m_pAgoraObject->applyRecordingEncoderConfiguration();
	}
}

void CmainWidget::showRecordingManageDialog()
{
	if (!m_pAgoraObject || !m_pLeftVideoList) {
		return;
	}

	const unsigned int localUid = m_pAgoraObject->localUid();
	const QList<unsigned int> remoteUids = m_pLeftVideoList->remoteMemberUids();
	const unsigned int recordingUid = m_meetingRecordingActive ? m_pAgoraObject->recordingUid() : 0;

	if (!m_pRecordingDialog) {
		m_pRecordingDialog = new RecordingManageDialog(
			localUid, remoteUids, recordingUid, m_screenRegionRecordingActive, this);
		m_pRecordingDialog->setAttribute(Qt::WA_DeleteOnClose);
		connect(m_pRecordingDialog, &RecordingManageDialog::sig_startRecording, this,
			[this](unsigned int uid) {
				stopScreenRegionRecording(false);
				on_toggleParticipantRecording(uid);
				if (m_meetingRecordingActive && m_pRecordingDialog) {
					m_pRecordingDialog->setRecordingUid(m_pAgoraObject->recordingUid());
				}
			});
		connect(m_pRecordingDialog, &RecordingManageDialog::sig_stopRecording, this,
			[this](unsigned int uid) {
				on_toggleParticipantRecording(uid);
				if (m_pRecordingDialog) {
					m_pRecordingDialog->setRecordingUid(
						m_meetingRecordingActive ? m_pAgoraObject->recordingUid() : 0);
				}
			});
		connect(m_pRecordingDialog, &RecordingManageDialog::sig_startScreenRecording, this,
			[this]() {
				if (m_pAgoraObject && m_pAgoraObject->isMeetingRecording()) {
					m_pAgoraObject->stopMeetingRecording();
					m_meetingRecordingActive = false;
				}
				startScreenRegionRecording();
				if (m_pRecordingDialog) {
					m_pRecordingDialog->setScreenRecordingActive(m_screenRegionRecordingActive);
				}
			});
		connect(m_pRecordingDialog, &RecordingManageDialog::sig_stopScreenRecording, this,
			[this]() {
				stopScreenRegionRecording();
				if (m_pRecordingDialog) {
					m_pRecordingDialog->setScreenRecordingActive(false);
				}
			});
		connect(m_pRecordingDialog, &QDialog::finished, this, [this]() {
			m_pRecordingDialog = nullptr;
		});
	} else {
		m_pRecordingDialog->updateRemoteMembers(localUid, remoteUids);
		m_pRecordingDialog->setRecordingUid(recordingUid);
		m_pRecordingDialog->setScreenRecordingActive(m_screenRegionRecordingActive);
	}

	m_pRecordingDialog->show();
	m_pRecordingDialog->raise();
	m_pRecordingDialog->activateWindow();
}

QRect CmainWidget::collectMeetingRecordRegion() const
{
	QRect region = frameGeometry();
	if (m_pChatDialog && m_pChatDialog->isVisible()) {
		region = region.united(m_pChatDialog->frameGeometry());
	}
	if (m_pMemberDialog && m_pMemberDialog->isVisible()) {
		region = region.united(m_pMemberDialog->frameGeometry());
	}
	return region;
}

void CmainWidget::startScreenRegionRecording()
{
	if (!m_pScreenRegionRecorder || m_screenRegionRecordingActive) {
		return;
	}

	const QRect captureRegion = collectMeetingRecordRegion();
	const QString outputDir = CAgoraObject::defaultRecordingsDirectory();
	if (!QDir().mkpath(outputDir)) {
		QMessageBox::warning(this, u8"\u63d0\u793a",
			QString::fromUtf8(u8"\u65e0\u6cd5\u521b\u5efa\u5f55\u5236\u76ee\u5f55\uff1a") + outputDir);
		return;
	}

	const QString fileName = QStringLiteral("NiceMeeting_%1_screen_%2.avi")
		.arg(m_roomId)
		.arg(QDateTime::currentDateTime().toString(QStringLiteral("yyyyMMdd_HHmmss")));
	const QString filePath = QDir(outputDir).filePath(fileName);

	QString error;
	if (!m_pScreenRegionRecorder->start(captureRegion, filePath, &error)) {
		QMessageBox::warning(this, u8"\u63d0\u793a",
			QString::fromUtf8(u8"\u5f00\u59cb\u4f1a\u8bae\u753b\u9762\u5f55\u5236\u5931\u8d25\uff1a\n%1").arg(error));
		return;
	}

	m_screenRegionRecordingActive = true;
	if (m_pBottonBar) {
		m_pBottonBar->setRecordingActive(true);
	}
}

void CmainWidget::stopScreenRegionRecording(bool notifyUser)
{
	if (!m_pScreenRegionRecorder || !m_screenRegionRecordingActive) {
		return;
	}

	const QString savedPath = m_pScreenRegionRecorder->outputPath();
	m_pScreenRegionRecorder->stop();
	m_screenRegionRecordingActive = false;
	if (m_pBottonBar) {
		m_pBottonBar->setRecordingActive(false);
	}
	if (notifyUser && !savedPath.isEmpty()) {
		QMessageBox::information(this, u8"\u63d0\u793a",
			QString::fromUtf8(u8"\u4f1a\u8bae\u753b\u9762\u5f55\u5236\u5df2\u505c\u6b62\u3002\n\u6587\u4ef6\u4fdd\u5b58\u4e8e\uff1a\n%1").arg(savedPath));
	}
}

void CmainWidget::on_toggleParticipantRecording(unsigned int uid)
{
	if (!m_pAgoraObject) {
		return;
	}

	if (m_pAgoraObject->isMeetingRecording() && m_pAgoraObject->recordingUid() == uid) {
		const QString savedPath = m_pAgoraObject->lastRecordingFilePath();
		m_pAgoraObject->stopMeetingRecording();
		m_meetingRecordingActive = false;
		if (m_pBottonBar) {
			m_pBottonBar->setRecordingActive(false);
		}
		if (!savedPath.isEmpty()) {
			QMessageBox::information(this, u8"\u63d0\u793a",
				QString::fromUtf8(u8"\u5f55\u5236\u5df2\u505c\u6b62\u3002\n\u6587\u4ef6\u4fdd\u5b58\u4e8e\uff1a\n%1").arg(savedPath));
		}
		return;
	}

	stopScreenRegionRecording(false);

	QString error;
	const int ret = (uid == m_pAgoraObject->localUid())
		? m_pAgoraObject->startMainVideoRecording(&error)
		: m_pAgoraObject->startRemoteVideoRecording(uid, &error);
	if (ret != 0) {
		QMessageBox::warning(this, u8"\u63d0\u793a",
			QString::fromUtf8(u8"\u5f00\u59cb\u5f55\u5236 UID %1 \u5931\u8d25\uff1a\n%2").arg(uid).arg(error));
		return;
	}

	m_meetingRecordingActive = true;
	if (m_pBottonBar) {
		m_pBottonBar->setRecordingActive(true);
	}
}

void CmainWidget::on_invite()
{
	if (m_roomId.isEmpty()) {
		QMessageBox::warning(this, u8"\u63d0\u793a", u8"\u5f53\u524d\u672a\u52a0\u5165\u4f1a\u8bae\uff0c\u65e0\u6cd5\u9080\u8bf7\u3002");
		return;
	}

	InviteDialog dialog(m_roomId, this);
	dialog.exec();
}

void CmainWidget::on_manageMembers()
{
	if (m_roomId.isEmpty()) {
		QMessageBox::warning(this, u8"\u63d0\u793a", u8"\u5f53\u524d\u672a\u52a0\u5165\u4f1a\u8bae\u3002");
		return;
	}

	const QList<unsigned int> memberUids = m_pLeftVideoList
		? m_pLeftVideoList->remoteMemberUids()
		: QList<unsigned int>();

	if (!m_pMemberDialog) {
		m_pMemberDialog = new MemberManageDialog(memberUids, this);
		m_pMemberDialog->setAttribute(Qt::WA_DeleteOnClose);
		connect(m_pMemberDialog, &MemberManageDialog::sig_kickUser,
			this, &CmainWidget::on_kickUser, Qt::DirectConnection);
		connect(m_pMemberDialog, &MemberManageDialog::sig_blockRemoteVideo,
			this, &CmainWidget::on_blockRemoteVideo, Qt::DirectConnection);
		connect(m_pMemberDialog, &MemberManageDialog::sig_blockRemoteAudio,
			this, &CmainWidget::on_blockRemoteAudio, Qt::DirectConnection);
		connect(m_pMemberDialog, &MemberManageDialog::sig_muteUserAudio,
			this, &CmainWidget::on_muteMemberAudio, Qt::DirectConnection);
		connect(m_pMemberDialog, &QDialog::finished, this, [this]() {
			m_pMemberDialog = nullptr;
		});
	} else {
		m_pMemberDialog->updateMemberList(memberUids);
	}

	m_pMemberDialog->show();
	m_pMemberDialog->raise();
	m_pMemberDialog->activateWindow();
}

void CmainWidget::on_chat()
{
	if (m_roomId.isEmpty()) {
		QMessageBox::warning(this, u8"\u63d0\u793a", u8"\u5f53\u524d\u672a\u52a0\u5165\u4f1a\u8bae\u3002");
		return;
	}

	if (!m_pChatDialog) {
		const unsigned int localUid = m_pAgoraObject ? m_pAgoraObject->localUid() : LOCAL_USER_UID;
		m_pChatDialog = new MeetingChatDialog(localUid, this);
		m_pChatDialog->setAttribute(Qt::WA_DeleteOnClose);
		connect(m_pChatDialog, &MeetingChatDialog::sig_sendMessage,
				this, &CmainWidget::on_chatSendMessage);
		connect(m_pChatDialog, &QDialog::finished, this, [this]() {
			m_pChatDialog = nullptr;
		});
	}

	m_pChatDialog->updateMemberList(m_pLeftVideoList->remoteMemberUids());
	m_pChatDialog->show();
	m_pChatDialog->raise();
	m_pChatDialog->activateWindow();
}

void CmainWidget::on_chatSendMessage(const QString& text, unsigned int targetUid)
{
	if (!m_pAgoraObject) {
		return;
	}

	const int ret = m_pAgoraObject->sendChatMessage(text, targetUid);
	if (ret != 0) {
		QMessageBox::warning(this, u8"\u63d0\u793a", u8"\u6d88\u606f\u53d1\u9001\u5931\u8d25\uff0c\u8bf7\u786e\u8ba4\u5df2\u52a0\u5165\u4f1a\u8bae\u3002");
		return;
	}

	if (m_pChatDialog) {
		m_pChatDialog->appendMessage(m_pAgoraObject->localUid(), targetUid, text);
	}
}

void CmainWidget::on_chatMessageReceived(unsigned int fromUid, unsigned int targetUid, const QString& text)
{
	if (!m_pChatDialog || !m_pAgoraObject) {
		return;
	}

	if (fromUid == m_pAgoraObject->localUid()) {
		return;
	}

	m_pChatDialog->appendMessage(fromUid, targetUid, text);
}

void CmainWidget::on_controlCommandReceived(unsigned int fromUid, unsigned int targetUid,
	const QString& command)
{
	if (!m_pAgoraObject) {
		return;
	}

	const unsigned int localUid = m_pAgoraObject->localUid();

	if (command == QStringLiteral("connectMic") || command == QStringLiteral("disconnectMic")) {
		if (targetUid == 0) {
			return;
		}
		applyConnectMicRemoteAudio(fromUid, targetUid, command == QStringLiteral("connectMic"));
		return;
	}

	if (targetUid != localUid) {
		return;
	}

	if (command == QStringLiteral("muteAudio")) {
		if (0 != m_pAgoraObject->muteLocalAudio(true)) {
			return;
		}
		syncLocalMicUi(false);
		QMessageBox::information(this, u8"\u63d0\u793a",
			u8"\u7ba1\u7406\u5458\u5df2\u5173\u95ed\u60a8\u7684\u9ea6\u514b\u98ce\u3002");
		return;
	}

	if (command == QStringLiteral("muteVideo")) {
		if (0 != m_pAgoraObject->enableVideo(false)) {
			return;
		}
		if (m_pBottonBar) {
			m_pBottonBar->setCameraEnabled(false);
		}
		if (m_localPreviewActive) {
			m_pAgoraObject->LocalVideoPreview(m_pBigVideoWidget->getHWND(), FALSE);
		}
		QMessageBox::information(this, u8"\u63d0\u793a",
			u8"\u7ba1\u7406\u5458\u5df2\u5173\u95ed\u60a8\u7684\u6444\u50cf\u5934\u3002");
		return;
	}

	if (command == QStringLiteral("kick")) {
		QMessageBox::warning(this, u8"\u63d0\u793a", u8"\u60a8\u5df2\u88ab\u7ba1\u7406\u5458\u79fb\u51fa\u4f1a\u8bae\u3002");
		stopScreenRegionRecording(false);
		if (m_pAgoraObject->isMeetingRecording()) {
			m_pAgoraObject->stopMeetingRecording();
		}
		m_meetingRecordingActive = false;
		m_pAgoraObject->leaveChannel();
		close();
	}
}

void CmainWidget::on_blockRemoteAudio(unsigned int uid)
{
	if (!m_pAgoraObject || uid == 0 || uid == m_pAgoraObject->localUid()) {
		return;
	}

	if (0 != m_pAgoraObject->muteRemoteAudio(uid, true)) {
		QMessageBox::warning(this, u8"\u63d0\u793a",
			u8"\u5c4f\u853d\u8fdc\u7aef\u58f0\u97f3\u5931\u8d25\uff0c\u8bf7\u786e\u8ba4\u5bf9\u65b9\u5728\u7ebf\u3002");
	}
}

void CmainWidget::on_muteMemberAudio(unsigned int uid)
{
	if (!m_pAgoraObject || uid == 0 || uid == m_pAgoraObject->localUid()) {
		return;
	}

	if (0 != m_pAgoraObject->sendControlMessage(QStringLiteral("muteAudio"), uid)) {
		QMessageBox::warning(this, u8"\u63d0\u793a",
			u8"\u6307\u4ee4\u53d1\u9001\u5931\u8d25\uff0c\u8bf7\u786e\u8ba4\u5bf9\u65b9\u5728\u7ebf\u4e14\u5df2\u52a0\u5165\u4f1a\u8bae\u3002");
	}
}

void CmainWidget::on_blockRemoteVideo(unsigned int uid)
{
	if (!m_pAgoraObject || uid == 0 || uid == m_pAgoraObject->localUid()) {
		return;
	}

	if (0 != m_pAgoraObject->muteRemoteVideo(uid, true)) {
		QMessageBox::warning(this, u8"\u63d0\u793a",
			u8"\u5c4f\u853d\u8fdc\u7aef\u89c6\u9891\u5931\u8d25\uff0c\u8bf7\u786e\u8ba4\u5bf9\u65b9\u5728\u7ebf\u3002");
	}
}

void CmainWidget::on_kickUser(unsigned int uid)
{
	if (!m_pAgoraObject || uid == 0 || uid == m_pAgoraObject->localUid()) {
		QMessageBox::warning(this, u8"\u63d0\u793a", u8"\u65e0\u6cd5\u8e22\u51fa\u8be5\u7528\u6237\u3002");
		return;
	}

	if (m_roomId.isEmpty()) {
		QMessageBox::warning(this, u8"\u63d0\u793a", u8"\u5f53\u524d\u672a\u52a0\u5165\u4f1a\u8bae\u3002");
		return;
	}

	m_pAgoraObject->sendControlMessage(QStringLiteral("kick"), uid);

	const AgoraRestCredentials creds = AgoraRestCredentials::loadDefault();
	if (!creds.isValid()) {
		QMessageBox::information(this, u8"\u63d0\u793a",
			u8"\u5df2\u5411\u5bf9\u65b9\u53d1\u9001\u79bb\u5f00\u6307\u4ee4\u3002\u82e5\u672a\u751f\u6548\uff0c\u8bf7\u914d\u7f6e key_and_secret.txt \u540e\u91cd\u8bd5\u3002");
		return;
	}

	m_pKickService->kickFromChannel(
		currentAppId(),
		m_roomId,
		uid,
		creds.customerId,
		creds.customerSecret);
}

void CmainWidget::on_kickFinished(bool success, const QString& message)
{
	if (success) {
		QMessageBox::information(this, u8"\u63d0\u793a", message);
	} else {
		QMessageBox::warning(this, u8"\u63d0\u793a", message);
	}
}
