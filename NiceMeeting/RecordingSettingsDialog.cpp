#include "RecordingSettingsDialog.h"
#include "RecordingSettings.h"

#include <QComboBox>
#include <QFormLayout>
#include <QGroupBox>
#include <QLabel>
#include <QMessageBox>
#include <QPushButton>

namespace {

QComboBox* makeFpsCombo(int currentFps, QWidget* parent)
{
	auto* combo = new QComboBox(parent);
	const QList<int> fpsOptions = { 10, 15, 24, 30 };
	for (int fps : fpsOptions) {
		combo->addItem(QString::number(fps), fps);
		if (fps == currentFps) {
			combo->setCurrentIndex(combo->count() - 1);
		}
	}
	return combo;
}

QComboBox* makeQualityCombo(bool includeRemoteNative, RecordingQualityPreset current, QWidget* parent)
{
	auto* combo = new QComboBox(parent);
	const QList<RecordingQualityPreset> presets = {
		RecordingQualityPreset::SD_640x360,
		RecordingQualityPreset::HD_1280x720,
		RecordingQualityPreset::FHD_1920x1080
	};
	for (RecordingQualityPreset preset : presets) {
		combo->addItem(RecordingSettings::qualityDisplayName(preset), static_cast<int>(preset));
	}
	if (includeRemoteNative) {
		combo->addItem(RecordingSettings::qualityDisplayName(RecordingQualityPreset::RemoteNative),
			static_cast<int>(RecordingQualityPreset::RemoteNative));
	}
	const int index = combo->findData(static_cast<int>(current));
	if (index >= 0) {
		combo->setCurrentIndex(index);
	}
	return combo;
}

}

RecordingSettingsDialog::RecordingSettingsDialog(bool recordingBusy, QWidget* parent)
	: MeetingFramelessDialog(QString::fromUtf8(u8"\u8bbe\u7f6e"), parent)
	, m_recordingBusy(recordingBusy)
{
	setMinimumWidth(460);

	RecordingSettings& settings = RecordingSettings::instance();

	auto* streamGroup = new QGroupBox(
		QString::fromUtf8(u8"\u89c6\u9891\u6d41\u5f55\u5236\uff08MP4\u3001\u672c\u5730\u4e3b\u89c6\u9891 / \u8fdc\u7aef\u5c0f\u7a97\uff09"), this);
	auto* streamForm = new QFormLayout(streamGroup);

	m_streamFpsCombo = makeFpsCombo(settings.streamFps(), this);
	m_localQualityCombo = makeQualityCombo(false, settings.localStreamQuality(), this);
	m_remoteQualityCombo = makeQualityCombo(true, settings.remoteStreamQuality(), this);
	m_codecCombo = new QComboBox(this);
	m_codecCombo->addItem(RecordingSettings::codecDisplayName(RecordingCodec::H264),
		static_cast<int>(RecordingCodec::H264));
	m_codecCombo->addItem(RecordingSettings::codecDisplayName(RecordingCodec::H265),
		static_cast<int>(RecordingCodec::H265));
	m_codecCombo->setCurrentIndex(m_codecCombo->findData(static_cast<int>(settings.streamCodec())));

	streamForm->addRow(QString::fromUtf8(u8"\u5e27\u7387 (fps)"), m_streamFpsCombo);
	streamForm->addRow(QString::fromUtf8(u8"\u672c\u5730\u4e3b\u89c6\u9891\u6e05\u6670\u5ea6"), m_localQualityCombo);
	streamForm->addRow(QString::fromUtf8(u8"\u8fdc\u7aef\u5c0f\u7a97\u6e05\u6670\u5ea6"), m_remoteQualityCombo);
	streamForm->addRow(QString::fromUtf8(u8"\u89c6\u9891\u7f16\u7801"), m_codecCombo);

	auto* screenGroup = new QGroupBox(
		QString::fromUtf8(u8"\u4f1a\u8bae\u753b\u9762\u5f55\u5236\uff08AVI / MJPEG\u3001\u6240\u89c1\u5373\u6240\u5f55\uff09"), this);
	auto* screenForm = new QFormLayout(screenGroup);

	m_screenFpsCombo = makeFpsCombo(settings.screenFps(), this);
	m_screenQualityCombo = makeQualityCombo(false, settings.screenOutputQuality(), this);
	m_jpegQualityCombo = new QComboBox(this);
	m_jpegQualityCombo->addItem(QString::fromUtf8(u8"\u6807\u6e05 (60)"), 60);
	m_jpegQualityCombo->addItem(QString::fromUtf8(u8"\u9ad8\u6e05 (85)"), 85);
	m_jpegQualityCombo->addItem(QString::fromUtf8(u8"\u6781\u81f4 (95)"), 95);
	const int jpegIndex = m_jpegQualityCombo->findData(settings.screenJpegQuality());
	m_jpegQualityCombo->setCurrentIndex(jpegIndex >= 0 ? jpegIndex : 1);

	screenForm->addRow(QString::fromUtf8(u8"\u5e27\u7387 (fps)"), m_screenFpsCombo);
	screenForm->addRow(QString::fromUtf8(u8"\u8f93\u51fa\u6700\u5927\u6e05\u6670\u5ea6"), m_screenQualityCombo);
	screenForm->addRow(QString::fromUtf8(u8"JPEG \u538b\u7f29\u8d28\u91cf"), m_jpegQualityCombo);

	auto* hintLabel = new QLabel(this);
	hintLabel->setText(m_recordingBusy
		? QString::fromUtf8(u8"\u5f53\u524d\u6b63\u5728\u5f55\u5236\uff0c\u65b0\u8bbe\u7f6e\u5728\u4e0b\u6b21\u5f00\u59cb\u5f55\u5236\u65f6\u751f\u6548\u3002")
		: QString::fromUtf8(u8"\u8bbe\u7f6e\u4fdd\u5b58\u540e\u7acb\u5373\u751f\u6548\u4e8e\u540e\u7eed\u5f55\u5236\u3002"));
	hintLabel->setWordWrap(true);

	auto* saveButton = new QPushButton(QString::fromUtf8(u8"\u4fdd\u5b58"), this);
	connect(saveButton, &QPushButton::clicked, this, &RecordingSettingsDialog::onAccept);

	auto* buttonRow = new QHBoxLayout();
	buttonRow->addStretch();
	buttonRow->addWidget(saveButton);

	bodyLayout()->addWidget(streamGroup);
	bodyLayout()->addWidget(screenGroup);
	bodyLayout()->addWidget(hintLabel);
	bodyLayout()->addLayout(buttonRow);
}

void RecordingSettingsDialog::onAccept()
{
	RecordingSettings& settings = RecordingSettings::instance();
	settings.setStreamFps(m_streamFpsCombo->currentData().toInt());
	settings.setLocalStreamQuality(static_cast<RecordingQualityPreset>(m_localQualityCombo->currentData().toInt()));
	settings.setRemoteStreamQuality(static_cast<RecordingQualityPreset>(m_remoteQualityCombo->currentData().toInt()));
	settings.setStreamCodec(static_cast<RecordingCodec>(m_codecCombo->currentData().toInt()));
	settings.setScreenFps(m_screenFpsCombo->currentData().toInt());
	settings.setScreenOutputQuality(static_cast<RecordingQualityPreset>(m_screenQualityCombo->currentData().toInt()));
	settings.setScreenJpegQuality(m_jpegQualityCombo->currentData().toInt());
	settings.save();

	if (m_recordingBusy) {
		QMessageBox::information(this, QString::fromUtf8(u8"\u63d0\u793a"),
			QString::fromUtf8(u8"\u5f55\u5236\u8bbe\u7f6e\u5df2\u4fdd\u5b58\uff0c\u5c06\u5728\u4e0b\u6b21\u5f00\u59cb\u5f55\u5236\u65f6\u751f\u6548\u3002"));
	}
	accept();
}