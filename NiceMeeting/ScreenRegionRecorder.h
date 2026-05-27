#pragma once

#include <QObject>
#include <QRect>
#include <QSize>
#include <QString>
#include <QTimer>

class ScreenRegionRecorder : public QObject
{
	Q_OBJECT

public:
	explicit ScreenRegionRecorder(QObject* parent = nullptr);
	~ScreenRegionRecorder() override;

	bool isActive() const { return m_active; }
	QString outputPath() const { return m_outputPath; }
	bool start(const QRect& globalRegion, const QString& outputPath, QString* errorOut = nullptr);
	void stop();

private slots:
	void onCaptureTick();

private:
	struct AviMjpegWriter;

	QPixmap captureRegion() const;

	QTimer m_timer;
	QRect m_globalRegion;
	QSize m_outputSize;
	QString m_outputPath;
	AviMjpegWriter* m_writer = nullptr;
	bool m_active = false;
};
