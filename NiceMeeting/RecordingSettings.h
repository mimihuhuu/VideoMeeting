#pragma once

#include <QSize>
#include <QString>
#include "commons.h"


enum class RecordingCodec {
	H264 = 0,
	H265 = 1
};

enum class RecordingQualityPreset {
	SD_640x360 = 0,
	HD_1280x720 = 1,
	FHD_1920x1080 = 2,
	RemoteNative = 3
};

class RecordingSettings
{
public:
	static RecordingSettings& instance();

	void load();
	void save();

	int streamFps() const { return m_streamFps; }
	int screenFps() const { return m_screenFps; }
	int screenJpegQuality() const { return m_screenJpegQuality; }
	RecordingCodec streamCodec() const { return m_streamCodec; }
	RecordingQualityPreset localStreamQuality() const { return m_localStreamQuality; }
	RecordingQualityPreset remoteStreamQuality() const { return m_remoteStreamQuality; }
	RecordingQualityPreset screenOutputQuality() const { return m_screenOutputQuality; }

	void setStreamFps(int fps);
	void setScreenFps(int fps);
	void setScreenJpegQuality(int quality);
	void setStreamCodec(RecordingCodec codec);
	void setLocalStreamQuality(RecordingQualityPreset preset);
	void setRemoteStreamQuality(RecordingQualityPreset preset);
	void setScreenOutputQuality(RecordingQualityPreset preset);

	QSize streamVideoSize(bool isRemote) const;
	QSize scaledScreenOutputSize(const QSize& captureSize) const;

	static QSize presetToSize(RecordingQualityPreset preset);
	static QString codecDisplayName(RecordingCodec codec);
	static QString qualityDisplayName(RecordingQualityPreset preset);

private:
	RecordingSettings();

	int m_streamFps = RECORDING_VIDEO_FPS;
	int m_screenFps = RECORDING_VIDEO_FPS;
	int m_screenJpegQuality = 85;
	RecordingCodec m_streamCodec = RecordingCodec::H264;
	RecordingQualityPreset m_localStreamQuality = RecordingQualityPreset::HD_1280x720;
	RecordingQualityPreset m_remoteStreamQuality = RecordingQualityPreset::RemoteNative;
	RecordingQualityPreset m_screenOutputQuality = RecordingQualityPreset::HD_1280x720;
};

