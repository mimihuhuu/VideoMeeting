#pragma once

#include <QWidget>
#include <QShowEvent>
#include <QEvent>
#include "CFrameLessWidgetBase.h"
#include "CBigVideoWidget.h"
#include "CLeftVideoList.h"
#include "CBottonBar.h"
#include "CTitleBar.h"
#include "CAgoraObject.h"

class CmainWidget : public CFrameLessWidgetBase
{
	Q_OBJECT

public:
	CmainWidget(QWidget* p = nullptr);
	~CmainWidget() override;

	void joinroom(const QString& roomID);

private:
	void initUI();
	void showEvent(QShowEvent* event) override;
	void changeEvent(QEvent* event) override;
	void updateMainVideoRenderMode();

private slots:
	void on_LocalJoinSuccess(const QString& qsChannel, unsigned int uid, int elapsed);
	void on_RemoteJoined(uid_t uid, int elapsed);
	void on_enableVideo(bool enabled);
	void on_enableAudio(bool enabled);
	void on_EndMeeting();
	void on_shareScreen();

private:
	void statShareScreen(int type, qint64 sourceId);

private:
	CBigVideoWidget* m_pBigVideoWidget = nullptr;
	CLeftVideoList* m_pLeftVideoList = nullptr;
	CBottonBar* m_pBottonBar = nullptr;
	CTitleBar* m_pTitleBar = nullptr;

	CAgoraObject* m_pAgoraObject = nullptr;
	bool m_centeredOnShow = false;
	bool m_localPreviewActive = false;
};
