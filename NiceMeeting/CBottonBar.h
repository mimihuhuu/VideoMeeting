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

signals:
	void sig_EndMeeting();
	void sig_enableVideo(bool enabled);
	void sig_enableAudio(bool enabled);
	void sig_shareScreen();
	
private:
	CCustomButton* m_pAudioToolButton;
	CCustomButton* m_pCameraToolButton;
	CCustomButton* m_pShareSreenToolButton;

	CCustomButton* m_pSecurityToolButton;
	CCustomButton* m_pInviationToolButton;
	CCustomButton* m_pManageMembersToolButton;
	CCustomButton* m_pChatToolButton;
	CCustomButton* m_pRecordToolButton;
	CCustomButton* m_pGorupDiscussionToolButton;
	CCustomButton* m_pApplicationToolButton;
	CCustomButton* m_pSettingsToolButton;
	QPushButton* m_pEndMeetingToolButton;

	bool m_audioEnabled = true;
	bool m_cameraEnabled = true;
};
