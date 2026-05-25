#pragma once
#include <QObject>
#include "commons.h"
#include "IAgoraRtcEngine.h"
#include "AgoraMediaBase.h"
using namespace agora::rtc;
using namespace agora::media::base;
using namespace std;

class CAgoraObject : public QObject
{
	Q_OBJECT

public:
	CAgoraObject();
	~CAgoraObject();
	int init();
	int joinChannel(const QString& channel, uint uid);
	BOOL LocalVideoPreview(HWND hVideoWnd, BOOL bPreviewOn, RENDER_MODE_TYPE renderType = RENDER_MODE_FIT);
	BOOL RemoteVideoRender(uid_t uid, HWND hVideoWnd, RENDER_MODE_TYPE renderType =RENDER_MODE_HIDDEN);
	int enableVideo(bool enabled);
	int muteLocalAudio(bool muted);
	void ShareScreen(VecWindowShareInfo& VecWindowShare);
	int start_share_screen(int type, int64_t sourceId);

signals:
	void sender_joinedChannelSuccess(const QString& qsChannel, unsigned int uid, int elapsed);
	void sender_userJoined(unsigned int uid, int elapsed);
	void sender_userOffline(unsigned int uid, USER_OFFLINE_REASON_TYPE reason);
	void sender_joinChannelFailed(int err);
 
private:
	IRtcEngine* m_rtcEngine;
	unique_ptr<agora::rtc::IRtcEngineEventHandler> m_eventHandler;
};

