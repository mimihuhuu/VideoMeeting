#include "CAgoraObject.h"
#include "agoraconfig.h"
#include <QMessageBox> 
#include <QProcess>
    
//Specify your APP ID here
#define APPID   "5c3ae3f88921453f8d97f60c955af82f"
#define APP_TOKEN   "007eJxTYHgtkPLwba/dHacdqWKCohZ/t04QyZB/mXfvzeT4K2wM8msUGEyTjRNTjdMsLCyNDE1MgYwUS/M0M4NkS1PTxDQLozQvaf6shkBGBttOZhZGBggE8VkYDI2MTRgYAP32HEs="
    
CAgoraConfig gAgoraConfig;

class AgoraRtcEngineEvent : public agora::rtc::IRtcEngineEventHandler
{
    CAgoraObject& m_pInstance;
public:
    AgoraRtcEngineEvent(CAgoraObject& engine)
        :m_pInstance(engine)
    {
    } 
    virtual void onJoinChannelSuccess(const char* channel, uid_t uid, int elapsed) override
    {
        emit m_pInstance.sender_joinedChannelSuccess(QString(channel), uid, elapsed);
    }
    virtual void onUserJoined(uid_t uid, int elapsed) override
    {
        emit m_pInstance.sender_userJoined(uid, elapsed);
    }
    virtual void onUserOffline(uid_t uid, USER_OFFLINE_REASON_TYPE reason) override
    {
        emit m_pInstance.sender_userOffline(uid, reason);
    }
    virtual void onError(int err, const char* msg) override
    {
        Q_UNUSED(msg);
        emit m_pInstance.sender_joinChannelFailed(err);
    }
};

CAgoraObject::CAgoraObject()
{

}

CAgoraObject::~CAgoraObject()
{
}

int CAgoraObject::init()
{
	m_rtcEngine = createAgoraRtcEngine();
	m_eventHandler.reset(new AgoraRtcEngineEvent(*this));		

    // Declare a RTC engine context
    agora::rtc::RtcEngineContext context;
    // Retrieve event handler
    context.eventHandler = m_eventHandler.get();
    QByteArray temp;
    // Pass appId to the context
    if (strlen(APPID))
        context.appId = APPID;
    else {
        QString strAppId = gAgoraConfig.getAppId();
        if (strAppId.length() == 0) {
            gAgoraConfig.setAppId(QString(""));
        }
        temp = strAppId.toUtf8();
        context.appId = const_cast<const char*>(temp.data());
    }
    if (*context.appId == '\0')
    {
        QMessageBox::critical(nullptr, ("AgoraOpenLive"),
            ("You must specify APP ID before using the demo"));
        QProcess process;
        process.startDetached("notepad.exe", { "D:\OpenVideoCall-Windows\AgoraConfigOpenVideoCall.ini" }, "");
        ExitProcess(0);
    }
    // initialize the RtcEngine with the context
    
    if (0 != m_rtcEngine->initialize(context))
    {
        return -1;
    }

    m_rtcEngine->enableAudio();
    m_rtcEngine->enableVideo();
    m_rtcEngine->setChannelProfile(agora::CHANNEL_PROFILE_TYPE::CHANNEL_PROFILE_COMMUNICATION);
        
	return 0;
}

int CAgoraObject::joinChannel( const QString& channel, uint uid)
{
    if (channel.isEmpty()) {
        QMessageBox::warning(nullptr, ("AgoraHighSound"), ("channelname is empty"));
        return -1;
    }

	QString token = gAgoraConfig.getAppToken();
    if (token.isEmpty())token = QString(APP_TOKEN);

    QByteArray tokenUtf8 = token.toUtf8();
    QByteArray channelUtf8 = channel.toUtf8();
    const char* tokenPtr = tokenUtf8.isEmpty() ? nullptr : tokenUtf8.constData();

    int r = m_rtcEngine->joinChannel(tokenPtr, channelUtf8.constData(), nullptr, uid);
    return r;
}

BOOL CAgoraObject::LocalVideoPreview(HWND hVideoWnd, BOOL bPreviewOn, RENDER_MODE_TYPE renderType)
{
    int nRet = 0;

    if (bPreviewOn) {
        VideoCanvas vc;

        vc.uid = 0;
        vc.view = hVideoWnd;
        vc.renderMode = renderType;

        nRet = m_rtcEngine->setupLocalVideo(vc);
        if (nRet == 0)
            nRet = m_rtcEngine->startPreview();
    }
    else
        nRet = m_rtcEngine->stopPreview();

    return nRet == 0 ? TRUE : FALSE;
}

// Plays the remote video
BOOL CAgoraObject::RemoteVideoRender(uid_t uid, HWND hVideoWnd, RENDER_MODE_TYPE renderType/* = RENDER_MODE_TYPE::RENDER_MODE_HIDDEN*/)
{
    int nRet = 0;

    VideoCanvas vc;

    vc.uid = uid;
    vc.view = hVideoWnd;
    vc.renderMode = renderType;

    m_rtcEngine->setupRemoteVideo(vc);

    return nRet == 0 ? TRUE : FALSE;
}
