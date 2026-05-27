#include "ScreenRegionRecorder.h"

#include <QBuffer>
#include <QFile>
#include <QGuiApplication>
#include <QImage>
#include <QPixmap>
#include <QScreen>
#include <QVector>

#include "commons.h"
#include "RecordingSettings.h"

namespace {

struct AviIndexEntry {
	quint32 offset = 0;
	quint32 size = 0;
};

} // namespace

struct ScreenRegionRecorder::AviMjpegWriter
{
	~AviMjpegWriter()
	{
		close();
	}

	bool open(const QString& path, int width, int height, int fps)
	{
		close();
		if (width <= 0 || height <= 0 || fps <= 0) {
			return false;
		}

		m_file.setFileName(path);
		if (!m_file.open(QIODevice::WriteOnly)) {
			return false;
		}

		m_width = width;
		m_height = height;
		m_fps = fps;
		m_frameCount = 0;
		m_index.clear();

		writeHeaderPlaceholder();
		m_file.write("LIST", 4);
		m_moviSizeOffset = m_file.pos();
		writeLe32(0);
		m_file.write("movi", 4);
		return true;
	}

	bool writeFrame(const QImage& image)
	{
		if (!m_file.isOpen()) {
			return false;
		}

		QByteArray jpeg;
		QBuffer buffer(&jpeg);
		buffer.open(QIODevice::WriteOnly);
		if (!image.save(&buffer, "JPG", m_jpegQuality)) {
			return false;
		}

		AviIndexEntry entry;
		entry.offset = static_cast<quint32>(m_file.pos() - m_moviDataStart + 4);
		entry.size = static_cast<quint32>(jpeg.size());

		m_file.write("00dc", 4);
		writeLe32(static_cast<quint32>(jpeg.size()));
		m_file.write(jpeg);
		if (jpeg.size() % 2 != 0) {
			m_file.putChar('\0');
		}

		m_index.push_back(entry);
		++m_frameCount;
		return true;
	}

	void setJpegQuality(int quality)
	{
		m_jpegQuality = qBound(40, quality, 100);
	}

	void close()
	{
		if (!m_file.isOpen()) {
			return;
		}

		const qint64 moviEnd = m_file.pos();
		const quint32 moviSize = static_cast<quint32>(moviEnd - m_moviDataStart + 4);
		m_file.seek(m_moviSizeOffset);
		writeLe32(moviSize);
		m_file.seek(moviEnd);

		m_file.write("idx1", 4);
		writeLe32(static_cast<quint32>(m_index.size()) * 16);
		for (const AviIndexEntry& entry : m_index) {
			m_file.write("00dc", 4);
			writeLe32(0x10);
			m_file.write(reinterpret_cast<const char*>(&entry.offset), 4);
			m_file.write(reinterpret_cast<const char*>(&entry.size), 4);
		}

		const qint64 fileEnd = m_file.pos();
		const quint32 riffSize = static_cast<quint32>(fileEnd - 8);
		m_file.seek(m_riffSizeOffset);
		writeLe32(riffSize);

		const quint32 microSecPerFrame = 1000000 / static_cast<quint32>(m_fps);
		m_file.seek(m_avihFrameOffset);
		writeLe32(microSecPerFrame);
		m_file.seek(m_avihTotalFramesOffset);
		writeLe32(static_cast<quint32>(m_frameCount));

		m_file.close();
		m_index.clear();
		m_frameCount = 0;
	}

private:
	void writeLe32(quint32 value)
	{
		m_file.write(reinterpret_cast<const char*>(&value), 4);
	}

	void writeLe16(quint16 value)
	{
		m_file.write(reinterpret_cast<const char*>(&value), 2);
	}

	void writeHeaderPlaceholder()
	{
		m_file.write("RIFF", 4);
		m_riffSizeOffset = m_file.pos();
		writeLe32(0);
		m_file.write("AVI ", 4);

		m_file.write("LIST", 4);
		writeLe32(192);
		m_file.write("hdrl", 4);

		m_file.write("avih", 4);
		writeLe32(56);
		m_avihFrameOffset = m_file.pos();
		writeLe32(microSecPerFrameFromFps());
		writeLe32(0);
		writeLe32(0);
		writeLe32(0);
		writeLe32(0);
		m_avihTotalFramesOffset = m_file.pos();
		writeLe32(0);
		writeLe32(0);
		writeLe32(0);
		writeLe32(static_cast<quint32>(m_width));
		writeLe32(static_cast<quint32>(m_height));
		writeLe32(0);
		writeLe32(0);
		writeLe32(0);
		writeLe32(0);

		m_file.write("LIST", 4);
		writeLe32(116);
		m_file.write("strl", 4);

		m_file.write("strh", 4);
		writeLe32(56);
		m_file.write("vids", 4);
		m_file.write("MJPG", 4);
		writeLe32(0);
		writeLe32(0);
		writeLe32(0);
		writeLe32(microSecPerFrameFromFps());
		writeLe32(0);
		writeLe32(0);
		writeLe32(0);
		writeLe32(0);
		writeLe32(static_cast<quint32>(m_height));
		writeLe32(static_cast<quint32>(m_width));
		writeLe32(0);
		writeLe32(0);
		writeLe32(0);
		writeLe32(0);

		m_file.write("strf", 4);
		writeLe32(40);
		writeLe32(40);
		writeLe32(static_cast<quint32>(m_width));
		writeLe32(static_cast<quint32>(m_height));
		writeLe16(1);
		writeLe16(24);
		m_file.write("MJPG", 4);
		writeLe32(static_cast<quint32>(m_width * m_height * 3));
		writeLe32(0);
		writeLe32(0);
		writeLe32(0);
		writeLe32(0);

		m_moviDataStart = m_file.pos() + 8;
	}

	quint32 microSecPerFrameFromFps() const
	{
		return 1000000 / static_cast<quint32>(m_fps > 0 ? m_fps : 15);
	}

	QFile m_file;
	QVector<AviIndexEntry> m_index;
	int m_width = 0;
	int m_height = 0;
	int m_fps = 15;
	int m_jpegQuality = 85;
	int m_frameCount = 0;
	qint64 m_riffSizeOffset = 0;
	qint64 m_avihFrameOffset = 0;
	qint64 m_avihTotalFramesOffset = 0;
	qint64 m_moviSizeOffset = 0;
	qint64 m_moviDataStart = 0;
};

ScreenRegionRecorder::ScreenRegionRecorder(QObject* parent)
	: QObject(parent)
{
	connect(&m_timer, &QTimer::timeout, this, &ScreenRegionRecorder::onCaptureTick);
}

ScreenRegionRecorder::~ScreenRegionRecorder()
{
	stop();
}

bool ScreenRegionRecorder::start(const QRect& globalRegion, const QString& outputPath, QString* errorOut)
{
	if (m_active) {
		stop();
	}

	if (globalRegion.width() <= 0 || globalRegion.height() <= 0) {
		if (errorOut) {
			*errorOut = QString::fromUtf8(u8"\u5f55\u5236\u533a\u57df\u65e0\u6548\u3002");
		}
		return false;
	}

	m_writer = new AviMjpegWriter();
	const RecordingSettings& settings = RecordingSettings::instance();
	const QSize outputSize = settings.scaledScreenOutputSize(globalRegion.size());
	const int fps = settings.screenFps();
	if (!m_writer->open(outputPath, outputSize.width(), outputSize.height(), fps)) {
		delete m_writer;
		m_writer = nullptr;
		if (errorOut) {
			*errorOut = QString::fromUtf8(u8"\u65e0\u6cd5\u521b\u5efa\u5f55\u5236\u6587\u4ef6\uff1a") + outputPath;
		}
		return false;
	}

	m_writer->setJpegQuality(settings.screenJpegQuality());
	m_outputSize = outputSize;
	m_globalRegion = globalRegion;
	m_outputPath = outputPath;
	m_active = true;
	m_timer.start(fps > 0 ? (1000 / fps) : 66);
	onCaptureTick();
	return true;
}

void ScreenRegionRecorder::stop()
{
	if (!m_active) {
		return;
	}

	m_timer.stop();
	if (m_writer) {
		m_writer->close();
		delete m_writer;
		m_writer = nullptr;
	}
	m_active = false;
}

QPixmap ScreenRegionRecorder::captureRegion() const
{
	QScreen* screen = QGuiApplication::screenAt(m_globalRegion.center());
	if (!screen) {
		screen = QGuiApplication::primaryScreen();
	}
	if (!screen) {
		return QPixmap();
	}

	const QRect screenGeometry = screen->geometry();
	const int localX = m_globalRegion.x() - screenGeometry.x();
	const int localY = m_globalRegion.y() - screenGeometry.y();
	return screen->grabWindow(0, localX, localY, m_globalRegion.width(), m_globalRegion.height());
}

void ScreenRegionRecorder::onCaptureTick()
{
	if (!m_active || !m_writer) {
		return;
	}

	const QPixmap pixmap = captureRegion();
	if (pixmap.isNull()) {
		return;
	}

	QImage image = pixmap.toImage().convertToFormat(QImage::Format_RGB888);
	if (!m_outputSize.isEmpty() && image.size() != m_outputSize) {
		image = image.scaled(m_outputSize, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
	}
	m_writer->writeFrame(image);
}
