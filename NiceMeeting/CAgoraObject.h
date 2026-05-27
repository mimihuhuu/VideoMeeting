#pragma once

#include <QObject>

#include <QList>

#include "commons.h"

#include "IAgoraRtcEngine.h"

#include "IAgoraMediaRecorder.h"

#include "AgoraMediaBase.h"

using namespace agora::rtc;

using namespace agora::media::base;

using namespace std;



class CAgoraObject : public QObject

{

	Q_OBJECT



public:

	enum class RecordingSource {

		None,

		LocalCamera,

		Remote

	};



	CAgoraObject();

	~CAgoraObject();

	int init();

	int joinChannel(const QString& channel, uint uid);

	int leaveChannel();

	BOOL LocalVideoPreview(HWND hVideoWnd, BOOL bPreviewOn, RENDER_MODE_TYPE renderType = RENDER_MODE_FIT);

	BOOL RemoteVideoRender(uid_t uid, HWND hVideoWnd, RENDER_MODE_TYPE renderType =RENDER_MODE_HIDDEN);

	int enableVideo(bool enabled);

	int muteLocalAudio(bool muted);

	int muteRemoteAudio(unsigned int uid, bool muted);

	int muteRemoteVideo(unsigned int uid, bool muted);

	int unbindRemoteVideo(unsigned int uid);

	int muteAllRemoteAudio(bool muted);

	void ShareScreen(VecWindowShareInfo& VecWindowShare);

	void getRecordableSources(VecWindowShareInfo& out, const QList<int64_t>& excludeWindowIds);

	int start_share_screen(int type, int64_t sourceId);

	int startScreenRecord(int type, int64_t sourceId);

	int stopScreenRecord();

	bool isScreenRecording() const { return m_screenRecording; }

	int startMainVideoRecording(QString* errorOut = nullptr);

	int startRemoteVideoRecording(uid_t recordUid, QString* errorOut = nullptr);

	int startMeetingRecording(uid_t recordUid, QString* errorOut = nullptr);

	int stopMeetingRecording();

	bool isMeetingRecording() const { return m_meetingRecording; }

	uid_t recordingUid() const { return m_recordingUid; }

	RecordingSource recordingSource() const { return m_recordingSource; }

	QString lastRecordingFilePath() const { return m_lastRecordingFilePath; }

	static QString defaultRecordingsDirectory();

	int sendChatMessage(const QString& text, unsigned int targetUid = 0);

	int sendControlMessage(const QString& command, unsigned int targetUid);

	unsigned int localUid() const { return m_localUid; }

	void prepareChatAfterJoin(unsigned int uid);

	void applyRecordingEncoderConfiguration();



signals:

	void sender_joinedChannelSuccess(const QString& qsChannel, unsigned int uid, int elapsed);

	void sender_userJoined(unsigned int uid, int elapsed);

	void sender_userOffline(unsigned int uid, USER_OFFLINE_REASON_TYPE reason);

	void sender_joinChannelFailed(int err);

	void sender_chatMessageReceived(unsigned int fromUid, unsigned int targetUid, const QString& text);

	void sender_controlCommandReceived(unsigned int fromUid, unsigned int targetUid, const QString& command);



private:

	void ensureChatStream();

	agora::media::MediaRecorderConfiguration buildRecorderConfiguration(

		const QByteArray& storagePathUtf8, bool isRemote) const;

	void fillScreenCaptureSources(VecWindowShareInfo& out, const QList<int64_t>* excludeWindowIds);

	int publishScreenTrack(bool publish);



	IRtcEngine* m_rtcEngine;

	unique_ptr<agora::rtc::IRtcEngineEventHandler> m_eventHandler;

	int m_chatStreamId = -1;

	unsigned int m_localUid = 0;

	bool m_screenRecording = false;

	bool m_meetingRecording = false;

	uid_t m_recordingUid = 0;

	RecordingSource m_recordingSource = RecordingSource::None;

	QByteArray m_joinedChannelUtf8;

	QByteArray m_recordingStoragePathUtf8;

	QString m_lastRecordingFilePath;

	agora::agora_refptr<agora::rtc::IMediaRecorder> m_mediaRecorder;

	std::unique_ptr<agora::media::IMediaRecorderObserver> m_mediaRecorderObserver;

};


