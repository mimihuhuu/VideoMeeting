#include "CAgoraObject.h"
#include "RecordingSettings.h"
#include "agoraconfig.h"
#include "commons.h"
#include <QDateTime>
#include <QDir>
#include <QImage>
#include <QJsonDocument>
#include <QJsonObject>
#include <QMessageBox>
#include <QProcess>
#include <QRect>
#include <QSize>
#include <QStandardPaths>
#include <QStringList>
#include <windows.h>

#define APPID   "5c3ae3f88921453f8d97f60c955af82f"
#define APP_TOKEN   "007eJxTYLh5ydhZ/LRF2OlHR7dKpzp+mvz066eun3fNlxv6ZC/3eSqgwGCabJyYapxmYWFpZGhiCmSkWJqnmRkkW5qaJqZZGKUdXyWW1RDIyLDQex4TIwMEgvgsDIZGxiYMDAD/4SC5"

CAgoraConfig gAgoraConfig;

class NiceMeetingMediaRecorderObserver : public agora::media::IMediaRecorderObserver
{
public:
	void onRecorderStateChanged(const char* channelId, agora::rtc::uid_t uid,
		agora::media::RecorderState state, agora::media::RecorderReasonCode reason) override
	{
		Q_UNUSED(channelId);
		Q_UNUSED(uid);
		Q_UNUSED(state);
		Q_UNUSED(reason);
	}

	void onRecorderInfoUpdated(const char* channelId, agora::rtc::uid_t uid,
		const agora::media::RecorderInfo& info) override
	{
		Q_UNUSED(channelId);
		Q_UNUSED(uid);
		Q_UNUSED(info);
	}
};

class AgoraRtcEngineEvent : public agora::rtc::IRtcEngineEventHandler
{
	CAgoraObject& m_pInstance;
public:
	explicit AgoraRtcEngineEvent(CAgoraObject& engine)
		: m_pInstance(engine)
	{
	}

	void onJoinChannelSuccess(const char* channel, uid_t uid, int elapsed) override
	{
		emit m_pInstance.sender_joinedChannelSuccess(QString(channel), uid, elapsed);
	}

	void onUserJoined(uid_t uid, int elapsed) override
	{
		emit m_pInstance.sender_userJoined(uid, elapsed);
	}

	void onUserOffline(uid_t uid, USER_OFFLINE_REASON_TYPE reason) override
	{
		emit m_pInstance.sender_userOffline(uid, reason);
	}

	void onError(int err, const char* msg) override
	{
		Q_UNUSED(msg);
		emit m_pInstance.sender_joinChannelFailed(err);
	}

	void onStreamMessage(uid_t uid, int streamId, const char* data, size_t length, uint64_t sentTs) override
	{
		Q_UNUSED(streamId);
		Q_UNUSED(sentTs);
		if (!data || length == 0) {
			return;
		}

		const QJsonDocument doc = QJsonDocument::fromJson(QByteArray(data, static_cast<int>(length)));
		if (!doc.isObject()) {
			return;
		}

		const QJsonObject obj = doc.object();
		const unsigned int targetUid = static_cast<unsigned int>(obj.value(QStringLiteral("targetUid")).toInt());
		const QString type = obj.value(QStringLiteral("type")).toString();
		if (type == QStringLiteral("muteAudio") || type == QStringLiteral("muteVideo")
			|| type == QStringLiteral("kick")
			|| type == QStringLiteral("connectMic") || type == QStringLiteral("disconnectMic")) {
			emit m_pInstance.sender_controlCommandReceived(
				static_cast<unsigned int>(uid), targetUid, type);
			return;
		}

		const QString text = obj.value(QStringLiteral("text")).toString();
		if (text.isEmpty()) {
			return;
		}

		emit m_pInstance.sender_chatMessageReceived(static_cast<unsigned int>(uid), targetUid, text);
	}
};

namespace {

bool shouldExcludeRecordWindow(int64_t sourceId, const ScreenCaptureSourceInfo& info,
	const QList<int64_t>& excludeWindowIds)
{
	if (excludeWindowIds.contains(sourceId)) {
		return true;
	}

	HWND hwnd = reinterpret_cast<HWND>(static_cast<intptr_t>(sourceId));
	if (!hwnd || !::IsWindow(hwnd)) {
		return false;
	}

	wchar_t title[256] = {};
	::GetWindowTextW(hwnd, title, 255);
	const QString windowTitle = QString::fromWCharArray(title);
	if (windowTitle.contains(QStringLiteral("NiceMeeting"), Qt::CaseInsensitive)) {
		return true;
	}

	DWORD pid = 0;
	::GetWindowThreadProcessId(hwnd, &pid);
	if (pid == ::GetCurrentProcessId()) {
		return true;
	}

	const DWORD exStyle = static_cast<DWORD>(::GetWindowLongPtr(hwnd, GWL_EXSTYLE));
	const DWORD style = static_cast<DWORD>(::GetWindowLongPtr(hwnd, GWL_STYLE));
	if (exStyle & WS_EX_TOOLWINDOW) {
		return true;
	}
	if ((style & WS_POPUP) && !(style & WS_CAPTION)) {
		return true;
	}

	Q_UNUSED(info);
	return false;
}

} // namespace

CAgoraObject::CAgoraObject()
{
}

CAgoraObject::~CAgoraObject()
{
	stopMeetingRecording();
}

int CAgoraObject::init()
{
	m_rtcEngine = createAgoraRtcEngine();
	m_eventHandler.reset(new AgoraRtcEngineEvent(*this));

	agora::rtc::RtcEngineContext context;
	context.eventHandler = m_eventHandler.get();
	QByteArray temp;
	if (strlen(APPID)) {
		context.appId = APPID;
	} else {
		QString strAppId = gAgoraConfig.getAppId();
		temp = strAppId.toUtf8();
		context.appId = temp.constData();
	}
	if (!context.appId || *context.appId == '\0') {
		QMessageBox::critical(nullptr, ("AgoraOpenLive"), ("You must specify APP ID before using the demo"));
		return -1;
	}

	if (0 != m_rtcEngine->initialize(context)) {
		return -1;
	}

	m_rtcEngine->enableAudio();
	applyRecordingEncoderConfiguration();
	m_rtcEngine->enableVideo();
	m_rtcEngine->enableLocalVideo(false);
	m_rtcEngine->setAudioProfile(AUDIO_PROFILE_DEFAULT, AUDIO_SCENARIO_DEFAULT);
	m_rtcEngine->setChannelProfile(agora::CHANNEL_PROFILE_TYPE::CHANNEL_PROFILE_COMMUNICATION);
	return 0;
}

int CAgoraObject::joinChannel(const QString& channel, uint uid)
{
	if (channel.isEmpty()) {
		QMessageBox::warning(nullptr, ("AgoraHighSound"), ("channelname is empty"));
		return -1;
	}

	QString token = gAgoraConfig.getAppToken();
	if (token.isEmpty()) {
		token = QString(APP_TOKEN);
	}

	const QByteArray tokenUtf8 = token.toUtf8();
	const QByteArray channelUtf8 = channel.toUtf8();
	const char* tokenPtr = tokenUtf8.isEmpty() ? nullptr : tokenUtf8.constData();

	m_joinedChannelUtf8 = channelUtf8;
	m_localUid = uid;

	return m_rtcEngine->joinChannel(tokenPtr, channelUtf8.constData(), nullptr, uid);
}

int CAgoraObject::leaveChannel()
{
	if (!m_rtcEngine) {
		return -1;
	}
	stopMeetingRecording();
	stopScreenRecord();
	m_chatStreamId = -1;
	return m_rtcEngine->leaveChannel();
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
		if (nRet == 0) {
			nRet = m_rtcEngine->startPreview();
		}
	} else {
		nRet = m_rtcEngine->stopPreview();
	}
	return nRet == 0 ? TRUE : FALSE;
}

BOOL CAgoraObject::RemoteVideoRender(uid_t uid, HWND hVideoWnd, RENDER_MODE_TYPE renderType)
{
	VideoCanvas vc;
	vc.uid = uid;
	vc.view = hVideoWnd;
	vc.renderMode = renderType;
	m_rtcEngine->setupRemoteVideo(vc);
	return TRUE;
}

int CAgoraObject::enableVideo(bool enabled)
{
	if (!m_rtcEngine) {
		return -1;
	}

	const int ret = m_rtcEngine->enableLocalVideo(enabled);
	if (ret != 0) {
		return ret;
	}

	if (enabled) {
		return m_rtcEngine->muteLocalVideoStream(false);
	}

	m_rtcEngine->stopPreview();
	VideoCanvas vc;
	vc.uid = 0;
	vc.view = nullptr;
	m_rtcEngine->setupLocalVideo(vc);
	return m_rtcEngine->muteLocalVideoStream(true);
}

int CAgoraObject::muteLocalAudio(bool muted)
{
	if (!m_rtcEngine) {
		return -1;
	}
	return m_rtcEngine->muteLocalAudioStream(muted);
}

int CAgoraObject::muteRemoteAudio(unsigned int uid, bool muted)
{
	if (!m_rtcEngine || uid == 0) {
		return -1;
	}
	return m_rtcEngine->muteRemoteAudioStream(static_cast<uid_t>(uid), muted);
}

int CAgoraObject::muteRemoteVideo(unsigned int uid, bool muted)
{
	if (!m_rtcEngine || uid == 0) {
		return -1;
	}

	const int ret = m_rtcEngine->muteRemoteVideoStream(static_cast<uid_t>(uid), muted);
	if (ret != 0) {
		return ret;
	}

	if (muted) {
		VideoCanvas vc;
		vc.uid = static_cast<uid_t>(uid);
		vc.view = nullptr;
		vc.renderMode = RENDER_MODE_HIDDEN;
		m_rtcEngine->setupRemoteVideo(vc);
	}
	return 0;
}

int CAgoraObject::unbindRemoteVideo(unsigned int uid)
{
	if (!m_rtcEngine || uid == 0) {
		return -1;
	}

	VideoCanvas vc;
	vc.uid = static_cast<uid_t>(uid);
	vc.view = nullptr;
	vc.renderMode = RENDER_MODE_HIDDEN;
	return m_rtcEngine->setupRemoteVideo(vc);
}

int CAgoraObject::muteAllRemoteAudio(bool muted)
{
	if (!m_rtcEngine) {
		return -1;
	}
	return m_rtcEngine->muteAllRemoteAudioStreams(muted);
}

void CAgoraObject::fillScreenCaptureSources(VecWindowShareInfo& out, const QList<int64_t>* excludeWindowIds)
{
	if (!m_rtcEngine) {
		return;
	}

	out.clear();

	SIZE size;
	size.cx = 360;
	size.cy = 240;

	IScreenCaptureSourceList* infos = m_rtcEngine->getScreenCaptureSources(size, size, true);
	if (!infos) {
		return;
	}

	const int infoCount = infos->getCount();
	for (int i = 0; i < infoCount; ++i) {
		const ScreenCaptureSourceInfo info = infos->getSourceInfo(i);

		window_share_info window;
		if (info.type == ScreenCaptureSourceType_Screen) {
			window.windowType = window_share_info::Screen;
		} else if (info.type == ScreenCaptureSourceType_Window) {
			if (excludeWindowIds && shouldExcludeRecordWindow(info.sourceId, info, *excludeWindowIds)) {
				continue;
			}
			window.windowType = window_share_info::Window;
		} else {
			continue;
		}

		window.name = QString::fromUtf8(info.sourceName ? info.sourceName : "");
		window.sourceId = info.sourceId;

		const ThumbImageBuffer& thumb = info.thumbImage;
		if (thumb.buffer && thumb.width > 0 && thumb.height > 0 && thumb.length > 0) {
			const int bytesPerLine = static_cast<int>(thumb.length / thumb.height);
			if (bytesPerLine >= static_cast<int>(thumb.width * 4)) {
				QImage image(
					reinterpret_cast<const uchar*>(thumb.buffer),
					static_cast<int>(thumb.width),
					static_cast<int>(thumb.height),
					bytesPerLine,
					QImage::Format_ARGB32);
				window.pixmap = QPixmap::fromImage(image.copy());
			}
		}

		out.push_back(window);
	}

	infos->release();
}

void CAgoraObject::ShareScreen(VecWindowShareInfo& VecWindowShare)
{
	fillScreenCaptureSources(VecWindowShare, nullptr);
}

void CAgoraObject::getRecordableSources(VecWindowShareInfo& out, const QList<int64_t>& excludeWindowIds)
{
	fillScreenCaptureSources(out, &excludeWindowIds);
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

	if (type == 0) {
		agora::rtc::Rectangle region;
		region.x = 0;
		region.y = 0;
		region.width = 0;
		region.height = 0;
		captureParam.dimensions.width = 1280;
		captureParam.dimensions.height = 720;
		return m_rtcEngine->startScreenCaptureByDisplayId(sourceId, region, captureParam);
	}

	if (type == 1) {
		HWND hwnd = reinterpret_cast<HWND>(static_cast<intptr_t>(sourceId));
		WINDOWINFO winfo = {};
		winfo.cbSize = sizeof(WINDOWINFO);
		if (!::GetWindowInfo(hwnd, &winfo)) {
			return -1;
		}

		const RECT rect = winfo.rcWindow;
		const int winW = abs(rect.right - rect.left);
		const int winH = abs(rect.bottom - rect.top);

		agora::rtc::Rectangle region(0, 0, winW, winH);
		captureParam.dimensions.width = winW;
		captureParam.dimensions.height = winH;
		return m_rtcEngine->startScreenCaptureByWindowId(sourceId, region, captureParam);
	}

	return -1;
}

int CAgoraObject::publishScreenTrack(bool publish)
{
	if (!m_rtcEngine) {
		return -1;
	}

	ChannelMediaOptions options;
	options.publishCameraTrack = agora::Optional<bool>(!publish);
	options.publishScreenTrack = agora::Optional<bool>(publish);
	options.publishMicrophoneTrack = agora::Optional<bool>(true);
	return m_rtcEngine->updateChannelMediaOptions(options);
}

int CAgoraObject::startScreenRecord(int type, int64_t sourceId)
{
	if (!m_rtcEngine) {
		return -1;
	}

	if (m_screenRecording) {
		stopScreenRecord();
	}

	ScreenCaptureParameters captureParam;
	captureParam.frameRate = 15;
	captureParam.windowFocus = true;
	captureParam.captureMouseCursor = true;

	agora::rtc::Rectangle region;
	region.x = 0;
	region.y = 0;
	region.width = 0;
	region.height = 0;

	int ret = -1;
	if (type == 0) {
		captureParam.dimensions.width = 1920;
		captureParam.dimensions.height = 1080;
		ret = m_rtcEngine->startScreenCaptureByDisplayId(sourceId, region, captureParam);
	} else if (type == 1) {
		HWND hwnd = reinterpret_cast<HWND>(static_cast<intptr_t>(sourceId));
		WINDOWINFO winfo = {};
		winfo.cbSize = sizeof(WINDOWINFO);
		if (!::GetWindowInfo(hwnd, &winfo)) {
			return -1;
		}

		const RECT rect = winfo.rcWindow;
		const int winW = abs(rect.right - rect.left);
		const int winH = abs(rect.bottom - rect.top);
		region.width = winW;
		region.height = winH;
		captureParam.dimensions.width = winW;
		captureParam.dimensions.height = winH;
		ret = m_rtcEngine->startScreenCaptureByWindowId(sourceId, region, captureParam);
	}

	if (ret != 0) {
		return ret;
	}

	ret = publishScreenTrack(true);
	if (ret != 0) {
		m_rtcEngine->stopScreenCapture();
		return ret;
	}

	m_screenRecording = true;
	return 0;
}

int CAgoraObject::stopScreenRecord()
{
	if (!m_rtcEngine || !m_screenRecording) {
		return 0;
	}

	m_rtcEngine->stopScreenCapture();
	publishScreenTrack(false);
	m_screenRecording = false;
	return 0;
}

void CAgoraObject::applyRecordingEncoderConfiguration()
{
	if (!m_rtcEngine) {
		return;
	}

	const RecordingSettings& settings = RecordingSettings::instance();
	const QSize videoSize = settings.streamVideoSize(false);

	VideoEncoderConfiguration encoderConfig(
		videoSize.width(),
		videoSize.height(),
		settings.streamFps(),
		STANDARD_BITRATE,
		ORIENTATION_MODE_ADAPTIVE);
	encoderConfig.codecType = settings.streamCodec() == RecordingCodec::H265
		? VIDEO_CODEC_H265 : VIDEO_CODEC_H264;
	encoderConfig.degradationPreference = MAINTAIN_QUALITY;
	encoderConfig.advanceOptions = AdvanceOptions(PREFER_SOFTWARE, PREFER_COMPRESSION_AUTO, false);
	m_rtcEngine->setVideoEncoderConfiguration(encoderConfig);
}

QString CAgoraObject::defaultRecordingsDirectory()
{
	const QString base = QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation);
	return QDir(base).filePath(QStringLiteral("NiceMeeting/recordings"));
}

int CAgoraObject::startMainVideoRecording(QString* errorOut)
{
	if (m_screenRecording) {
		stopScreenRecord();
	}
	return startMeetingRecording(m_localUid, errorOut);
}

int CAgoraObject::startRemoteVideoRecording(uid_t recordUid, QString* errorOut)
{
	if (recordUid == 0 || recordUid == m_localUid) {
		if (errorOut) {
			*errorOut = QString::fromUtf8(u8"\u8bf7\u6307\u5b9a\u6709\u6548\u7684\u8fdc\u7aef\u7528\u6237 UID\u3002");
		}
		return -1;
	}
	return startMeetingRecording(recordUid, errorOut);
}

agora::media::MediaRecorderConfiguration CAgoraObject::buildRecorderConfiguration(
	const QByteArray& storagePathUtf8, bool isRemote) const
{
	const RecordingSettings& settings = RecordingSettings::instance();
	const QSize videoSize = settings.streamVideoSize(isRemote);

	agora::media::MediaRecorderConfiguration config;
	config.storagePath = storagePathUtf8.constData();
	config.containerFormat = agora::media::FORMAT_MP4;
	config.streamType = agora::media::STREAM_TYPE_BOTH;
	config.maxDurationMs = 4 * 60 * 60 * 1000;
	config.recorderInfoUpdateInterval = 2000;
	config.sample_rate = RECORDING_AUDIO_SAMPLE_RATE;
	config.channel_num = RECORDING_AUDIO_CHANNELS;
	config.fps = settings.streamFps();
	config.width = videoSize.width();
	config.height = videoSize.height();

	if (isRemote) {
		config.videoSourceType = VIDEO_SOURCE_REMOTE;
	} else {
		config.videoSourceType = VIDEO_SOURCE_CAMERA_PRIMARY;
	}
	return config;
}

int CAgoraObject::startMeetingRecording(uid_t recordUid, QString* errorOut)
{
	if (!m_rtcEngine) {
		if (errorOut) {
			*errorOut = QString::fromUtf8(u8"RTC \u5f15\u64ce\u672a\u521d\u59cb\u5316\u3002");
		}
		return -1;
	}

	if (m_joinedChannelUtf8.isEmpty()) {
		if (errorOut) {
			*errorOut = QString::fromUtf8(u8"\u5c1a\u672a\u52a0\u5165\u4f1a\u8bae\u9891\u9053\u3002");
		}
		return -1;
	}

	if (m_localUid == 0) {
		if (errorOut) {
			*errorOut = QString::fromUtf8(u8"\u672c\u5730\u7528\u6237 ID \u672a\u5c31\u7eea\u3002");
		}
		return -1;
	}

	const uid_t targetUid = recordUid != 0 ? recordUid : m_localUid;
	const bool isRemote = targetUid != m_localUid;
	if (m_meetingRecording) {
		if (m_recordingUid == targetUid) {
			return 0;
		}
		stopMeetingRecording();
	}

	if (m_screenRecording) {
		stopScreenRecord();
	}

	if (!isRemote) {
		applyRecordingEncoderConfiguration();
	}

	const QString outputDir = defaultRecordingsDirectory();
	if (!QDir().mkpath(outputDir)) {
		if (errorOut) {
			*errorOut = QString::fromUtf8(u8"\u65e0\u6cd5\u521b\u5efa\u5f55\u5236\u76ee\u5f55\uff1a") + outputDir;
		}
		return -1;
	}

	const QString fileName = QStringLiteral("NiceMeeting_%1_uid%2_%3.mp4")
		.arg(QString::fromUtf8(m_joinedChannelUtf8))
		.arg(targetUid)
		.arg(QDateTime::currentDateTime().toString(QStringLiteral("yyyyMMdd_HHmmss")));
	const QString filePath = QDir(outputDir).filePath(fileName);
	const QByteArray storagePathUtf8 = QDir::toNativeSeparators(filePath).toUtf8();

	RecorderStreamInfo streamInfo(m_joinedChannelUtf8.constData(), targetUid);
	m_mediaRecorder = m_rtcEngine->createMediaRecorder(streamInfo);
	if (!m_mediaRecorder) {
		if (errorOut) {
			*errorOut = QString::fromUtf8(u8"\u521b\u5efa\u5f55\u5236\u5668\u5931\u8d25\u3002");
		}
		return -1;
	}

	if (!m_mediaRecorderObserver) {
		m_mediaRecorderObserver.reset(new NiceMeetingMediaRecorderObserver());
	}

	if (0 != m_mediaRecorder->setMediaRecorderObserver(m_mediaRecorderObserver.get())) {
		m_rtcEngine->destroyMediaRecorder(m_mediaRecorder);
		m_mediaRecorder = nullptr;
		if (errorOut) {
			*errorOut = QString::fromUtf8(u8"\u6ce8\u518c\u5f55\u5236\u56de\u8c03\u5931\u8d25\u3002");
		}
		return -1;
	}

	const agora::media::MediaRecorderConfiguration config =
		buildRecorderConfiguration(storagePathUtf8, isRemote);
	const int ret = m_mediaRecorder->startRecording(config);
	if (ret != 0) {
		m_rtcEngine->destroyMediaRecorder(m_mediaRecorder);
		m_mediaRecorder = nullptr;
		if (errorOut) {
			*errorOut = QString::fromUtf8(u8"\u542f\u52a8\u5f55\u5236\u5931\u8d25\uff0c\u9519\u8bef\u7801\uff1a") + QString::number(ret);
		}
		return ret;
	}

	m_meetingRecording = true;
	m_recordingUid = targetUid;
	m_recordingSource = isRemote ? RecordingSource::Remote : RecordingSource::LocalCamera;
	m_recordingStoragePathUtf8 = storagePathUtf8;
	m_lastRecordingFilePath = filePath;
	return 0;
}

int CAgoraObject::stopMeetingRecording()
{
	if (!m_meetingRecording) {
		return 0;
	}

	if (m_mediaRecorder) {
		m_mediaRecorder->stopRecording();
		if (m_rtcEngine) {
			m_rtcEngine->destroyMediaRecorder(m_mediaRecorder);
		}
		m_mediaRecorder = nullptr;
	}

	m_meetingRecording = false;
	m_recordingUid = 0;
	m_recordingSource = RecordingSource::None;
	m_recordingStoragePathUtf8.clear();
	return 0;
}

void CAgoraObject::prepareChatAfterJoin(unsigned int uid)
{
	m_localUid = uid;
	ensureChatStream();
}

void CAgoraObject::ensureChatStream()
{
	if (!m_rtcEngine || m_chatStreamId >= 0) {
		return;
	}

	int streamId = -1;
	if (0 == m_rtcEngine->createDataStream(&streamId, false, false)) {
		m_chatStreamId = streamId;
	}
}

int CAgoraObject::sendChatMessage(const QString& text, unsigned int targetUid)
{
	if (!m_rtcEngine || m_chatStreamId < 0 || text.isEmpty()) {
		return -1;
	}

	QJsonObject obj;
	obj.insert(QStringLiteral("targetUid"), static_cast<int>(targetUid));
	obj.insert(QStringLiteral("text"), text);
	const QByteArray payload = QJsonDocument(obj).toJson(QJsonDocument::Compact);
	return m_rtcEngine->sendStreamMessage(m_chatStreamId, payload.constData(), payload.size());
}

int CAgoraObject::sendControlMessage(const QString& command, unsigned int targetUid)
{
	if (!m_rtcEngine || m_chatStreamId < 0 || command.isEmpty()) {
		return -1;
	}
	if (targetUid == 0
		&& command != QStringLiteral("connectMic")
		&& command != QStringLiteral("disconnectMic")) {
		return -1;
	}

	QJsonObject obj;
	obj.insert(QStringLiteral("type"), command);
	obj.insert(QStringLiteral("targetUid"), static_cast<int>(targetUid));
	const QByteArray payload = QJsonDocument(obj).toJson(QJsonDocument::Compact);
	return m_rtcEngine->sendStreamMessage(m_chatStreamId, payload.constData(), payload.size());
}
