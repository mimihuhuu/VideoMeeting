#include "RecordingSettings.h"
#include "commons.h"

#include <QCoreApplication>
#include <QDir>
#include <QSettings>
#include <QStandardPaths>
#include <QtGlobal>

namespace {

QString settingsFilePath()
{
	const QString dir = QStandardPaths::writableLocation(QStandardPaths::AppConfigLocation);
	if (dir.isEmpty()) {
		return QString();
	}
	QDir().mkpath(dir);
	return QDir(dir).filePath(QStringLiteral("recording_settings.ini"));
}

QSize fitInside(const QSize& source, const QSize& maxSize)
{
	if (source.isEmpty() || maxSize.isEmpty()) {
		return source;
	}
	if (source.width() <= maxSize.width() && source.height() <= maxSize.height()) {
		return source;
	}
	return source.scaled(maxSize, Qt::KeepAspectRatio);
}

} // namespace

RecordingSettings& RecordingSettings::instance()
{
	static RecordingSettings settings;
	return settings;
}

RecordingSettings::RecordingSettings()
{
	load();
}

void RecordingSettings::load()
{
	const QString path = settingsFilePath();
	if (path.isEmpty()) {
		return;
	}

	QSettings ini(path, QSettings::IniFormat);
	m_streamFps = ini.value(QStringLiteral("stream/fps"), RECORDING_VIDEO_FPS).toInt();
	m_screenFps = ini.value(QStringLiteral("screen/fps"), RECORDING_VIDEO_FPS).toInt();
	m_screenJpegQuality = ini.value(QStringLiteral("screen/jpegQuality"), 85).toInt();
	m_streamCodec = static_cast<RecordingCodec>(
		ini.value(QStringLiteral("stream/codec"), static_cast<int>(RecordingCodec::H264)).toInt());
	m_localStreamQuality = static_cast<RecordingQualityPreset>(
		ini.value(QStringLiteral("stream/localQuality"), static_cast<int>(RecordingQualityPreset::HD_1280x720)).toInt());
	m_remoteStreamQuality = static_cast<RecordingQualityPreset>(
		ini.value(QStringLiteral("stream/remoteQuality"), static_cast<int>(RecordingQualityPreset::RemoteNative)).toInt());
	m_screenOutputQuality = static_cast<RecordingQualityPreset>(
		ini.value(QStringLiteral("screen/outputQuality"), static_cast<int>(RecordingQualityPreset::HD_1280x720)).toInt());

	m_streamFps = qBound(5, m_streamFps, 30);
	m_screenFps = qBound(5, m_screenFps, 30);
	m_screenJpegQuality = qBound(40, m_screenJpegQuality, 100);
}

void RecordingSettings::save()
{
	const QString path = settingsFilePath();
	if (path.isEmpty()) {
		return;
	}

	QSettings ini(path, QSettings::IniFormat);
	ini.setValue(QStringLiteral("stream/fps"), m_streamFps);
	ini.setValue(QStringLiteral("screen/fps"), m_screenFps);
	ini.setValue(QStringLiteral("screen/jpegQuality"), m_screenJpegQuality);
	ini.setValue(QStringLiteral("stream/codec"), static_cast<int>(m_streamCodec));
	ini.setValue(QStringLiteral("stream/localQuality"), static_cast<int>(m_localStreamQuality));
	ini.setValue(QStringLiteral("stream/remoteQuality"), static_cast<int>(m_remoteStreamQuality));
	ini.setValue(QStringLiteral("screen/outputQuality"), static_cast<int>(m_screenOutputQuality));
	ini.sync();
}

void RecordingSettings::setStreamFps(int fps)
{
	m_streamFps = qBound(5, fps, 30);
}

void RecordingSettings::setScreenFps(int fps)
{
	m_screenFps = qBound(5, fps, 30);
}

void RecordingSettings::setScreenJpegQuality(int quality)
{
	m_screenJpegQuality = qBound(40, quality, 100);
}

void RecordingSettings::setStreamCodec(RecordingCodec codec)
{
	m_streamCodec = codec;
}

void RecordingSettings::setLocalStreamQuality(RecordingQualityPreset preset)
{
	m_localStreamQuality = preset;
}

void RecordingSettings::setRemoteStreamQuality(RecordingQualityPreset preset)
{
	m_remoteStreamQuality = preset;
}

void RecordingSettings::setScreenOutputQuality(RecordingQualityPreset preset)
{
	m_screenOutputQuality = preset;
}

QSize RecordingSettings::presetToSize(RecordingQualityPreset preset)
{
	switch (preset) {
	case RecordingQualityPreset::SD_640x360:
		return QSize(640, 360);
	case RecordingQualityPreset::HD_1280x720:
		return QSize(RECORDING_VIDEO_WIDTH, RECORDING_VIDEO_HEIGHT);
	case RecordingQualityPreset::FHD_1920x1080:
		return QSize(1920, 1080);
	case RecordingQualityPreset::RemoteNative:
	default:
		return QSize(RECORDING_REMOTE_VIDEO_WIDTH, RECORDING_REMOTE_VIDEO_HEIGHT);
	}
}

QSize RecordingSettings::streamVideoSize(bool isRemote) const
{
	const RecordingQualityPreset preset = isRemote ? m_remoteStreamQuality : m_localStreamQuality;
	return presetToSize(preset);
}

QSize RecordingSettings::scaledScreenOutputSize(const QSize& captureSize) const
{
	if (captureSize.isEmpty()) {
		return captureSize;
	}

	const QSize maxSize = presetToSize(m_screenOutputQuality);
	if (maxSize.width() >= captureSize.width() && maxSize.height() >= captureSize.height()) {
		return captureSize;
	}
	return fitInside(captureSize, maxSize);
}

QString RecordingSettings::codecDisplayName(RecordingCodec codec)
{
	return codec == RecordingCodec::H265 ? QStringLiteral("H.265 (HEVC)") : QStringLiteral("H.264 (AVC)");
}

QString RecordingSettings::qualityDisplayName(RecordingQualityPreset preset)
{
	switch (preset) {
	case RecordingQualityPreset::SD_640x360:
		return QString::fromUtf8(u8"\u6807\u6e05 640\u00d7360");
	case RecordingQualityPreset::HD_1280x720:
		return QString::fromUtf8(u8"\u9ad8\u6e05 1280\u00d7720");
	case RecordingQualityPreset::FHD_1920x1080:
		return QString::fromUtf8(u8"\u8d85\u6e05 1920\u00d71080");
	case RecordingQualityPreset::RemoteNative:
	default:
		return QString::fromUtf8(u8"\u8fdc\u7aef\u539f\u59cb %1\u00d7%2")
			.arg(RECORDING_REMOTE_VIDEO_WIDTH)
			.arg(RECORDING_REMOTE_VIDEO_HEIGHT);
	}
}