#pragma once
#include <QWidget>
#include <QPushButton>
#include "CCustomButton.h"

class CBottonBar : public QWidget
{
	Q_OBJECT

public:
	CBottonBar(QWidget* p = nullptr);
	~CBottonBar();

	void setRecordingActive(bool active);
	void setAudioEnabled(bool enabled);
	void setCameraEnabled(bool enabled);
	void setConnectMicActive(bool active);

signals:
	void sig_EndMeeting();
	void sig_enableVideo(bool enabled);
	void sig_enableAudio(bool enabled);
	void sig_shareScreen();
	void sig_connectMic(bool enabled);
	void sig_invite();
	void sig_manageMembers();
	void sig_chat();
	void sig_recordScreen();
	void sig_settings();

private:
	CCustomButton* m_pAudioToolButton;
	CCustomButton* m_pCameraToolButton;
	CCustomButton* m_pShareSreenToolButton;
	CCustomButton* m_pConnectMicToolButton;
	CCustomButton* m_pInviationToolButton;
	CCustomButton* m_pManageMembersToolButton;
	CCustomButton* m_pChatToolButton;
	CCustomButton* m_pRecordToolButton;
	CCustomButton* m_pSettingsToolButton;
	QPushButton* m_pEndMeetingToolButton;

	bool m_audioEnabled = false;
	bool m_cameraEnabled = true;
	bool m_connectMicActive = false;
	bool m_recordingActive = false;
};
