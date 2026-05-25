#include "CAgoraObject.h"
#include "agoraconfig.h"
#include "commons.h"
#include <QMessageBox>
#include <QImage>
#include <QProcess>
#include <windows.h>
    
//Specify your APP ID here
#define APPID   "5c3ae3f88921453f8d97f60c955af82f"
#define APP_TOKEN   "007eJxTYLjx/3/kRYcw9R9/E98ue6xa8vXX+iVrt22rvnzl9CKxONnlCgymycaJqcZpFhaWRoYmpkBGiqV5mplBsqWpaWKahVEa1w2hrIZARobJffmMjAwQCOKzMBgaGZswMAAAxEcilw=="
    
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
	// Set the channel profile as communication to enable both video and audio call features.
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
// Enables the video module.
int CAgoraObject::enableVideo(bool enabled)
{
    if (!m_rtcEngine) {
        return -1;
    }
    return enabled ? m_rtcEngine->enableVideo() : m_rtcEngine->disableVideo();
}

int CAgoraObject::muteLocalAudio(bool muted)
{
    if (!m_rtcEngine) {
        return -1;
    }
    return m_rtcEngine->muteLocalAudioStream(muted);
}

void CAgoraObject::ShareScreen(VecWindowShareInfo& VecWindowShare)
{
	if (!m_rtcEngine) {
		return;
	}

	VecWindowShare.clear();

    SIZE size;
	size.cx = 1280;
	size.cy = 720;

	IScreenCaptureSourceList* infos = m_rtcEngine->getScreenCaptureSources(size, size, true);
	if (!infos) {
		return;
	}

	const int info_count = infos->getCount();
	if (info_count <= 0) {
		infos->release();
		return;
	}

    for (int i = 0; i < info_count; ++i)
    {

		const ScreenCaptureSourceInfo info = infos->getSourceInfo(i);

		window_share_info window;
		if (info.type == ScreenCaptureSourceType_Screen) {
			window.windowType = window_share_info::Screen;
		} else if (info.type == ScreenCaptureSourceType_Window) {
			window.windowType = window_share_info::Window;
		} else {
			continue;
		}

		window.name = QString::fromUtf8(info.sourceName ? info.sourceName : "");
		window.sourceId = info.sourceId;

		const ThumbImageBuffer& thumb = info.thumbImage;
		if (thumb.buffer && thumb.width > 0 && thumb.height > 0 && thumb.length > 0)
		{
			const int bytesPerLine = static_cast<int>(thumb.length / thumb.height);
			if (bytesPerLine >= static_cast<int>(thumb.width * 4))
			{
				QImage image(
					reinterpret_cast<const uchar*>(thumb.buffer),
					static_cast<int>(thumb.width),
					static_cast<int>(thumb.height),
					bytesPerLine,
					QImage::Format_ARGB32);
				// 必须深拷贝：infos->release() 后 SDK 缓冲区会失效
				window.pixmap = QPixmap::fromImage(image.copy());
			}
		}

		VecWindowShare.push_back(window);
    }

	infos->release();
}

int CAgoraObject::start_share_screen(int type, int64_t sourceId)
{
	if (!m_rtcEngine) {
		return -1;
	}

    ScreenCaptureParameters captureParam;
    captureParam.frameRate = 25;
    captureParam.windowFocus = true;
    captureParam.captureMouseCursor = true;

    if (type == 0)
    {
        agora::rtc::Rectangle region;
        region.x = 0;
        region.y = 0;
        region.width = 0;
        region.height = 0;
        captureParam.dimensions.width = 1280;
        captureParam.dimensions.height = 720;
        return m_rtcEngine->startScreenCaptureByDisplayId(sourceId, region, captureParam);
    }

    if (type == 1)
    {
        HWND hwnd = reinterpret_cast<HWND>(static_cast<intptr_t>(sourceId));
        RECT rect = {};
        WINDOWINFO winfo = {};
        winfo.cbSize = sizeof(WINDOWINFO);
        if (!::GetWindowInfo(hwnd, &winfo)) {
            return -1;
        }
        rect = winfo.rcWindow;

        const int win_w = abs(rect.right - rect.left);
        const int win_h = abs(rect.bottom - rect.top);

        agora::rtc::Rectangle region(0, 0, win_w, win_h);
        captureParam.dimensions.width = win_w;
        captureParam.dimensions.height = win_h;
        return m_rtcEngine->startScreenCaptureByWindowId(sourceId, region, captureParam);
    }

    return -1;
}

