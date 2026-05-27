#pragma once
#include "MeetingFramelessDialog.h"

class QComboBox;

class RecordingSettingsDialog : public MeetingFramelessDialog
{
	Q_OBJECT

public:
	explicit RecordingSettingsDialog(bool recordingBusy, QWidget* parent = nullptr);

private slots:
	void onAccept();

private:
	bool m_recordingBusy = false;
	QComboBox* m_streamFpsCombo = nullptr;
	QComboBox* m_localQualityCombo = nullptr;
	QComboBox* m_remoteQualityCombo = nullptr;
	QComboBox* m_codecCombo = nullptr;
	QComboBox* m_screenFpsCombo = nullptr;
	QComboBox* m_screenQualityCombo = nullptr;
	QComboBox* m_jpegQualityCombo = nullptr;
};