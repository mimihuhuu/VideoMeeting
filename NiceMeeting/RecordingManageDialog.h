#pragma once
#include "MeetingFramelessDialog.h"
#include <QHash>
#include <QList>

class QPushButton;
class QVBoxLayout;
class QWidget;

class RecordingManageDialog : public MeetingFramelessDialog
{
	Q_OBJECT

public:
	RecordingManageDialog(unsigned int localUid,
		const QList<unsigned int>& remoteUids,
		unsigned int recordingUid,
		bool screenRecordingActive = false,
		QWidget* parent = nullptr);

	void setRecordingUid(unsigned int uid);
	void setScreenRecordingActive(bool active);
	void updateRemoteMembers(unsigned int localUid, const QList<unsigned int>& remoteUids);

signals:
	void sig_startRecording(unsigned int uid);
	void sig_stopRecording(unsigned int uid);
	void sig_startScreenRecording();
	void sig_stopScreenRecording();

private slots:
	void onToggleClicked();
	void onScreenToggleClicked();

private:
	void rebuildStreamRows(unsigned int localUid, const QList<unsigned int>& remoteUids);

	unsigned int m_localUid = 0;
	unsigned int m_recordingUid = 0;
	bool m_screenRecordingActive = false;
	QWidget* m_pStreamContainer = nullptr;
	QVBoxLayout* m_pStreamLayout = nullptr;
	QHash<unsigned int, QPushButton*> m_toggleButtons;
	QPushButton* m_pScreenToggleButton = nullptr;
};
