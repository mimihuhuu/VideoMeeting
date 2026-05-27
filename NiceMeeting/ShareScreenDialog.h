#pragma once

#include <QDialog>
#include "commons.h"
#include <QLabel>
#include <QPixmap>
#include <QPushButton>
#include <QString>
#include <vector>

struct ShareScreenItem {
	QString name;
	QPixmap pixmap;
	qint64 sourceId = 0;
};

class ShareScreenDialog : public QDialog
{
	Q_OBJECT

public:
	explicit ShareScreenDialog(QWidget* parent = nullptr);
	~ShareScreenDialog() override;

	void initListWidget(const VecWindowShareInfo& vec);

private:
	void clearData();
	void updateScreenPreview();
	void updateAppPreview();
	void setSelectedColumn(int type);
	int wrapIndex(int index, int count, int delta) const;

	void mousePressEvent(QMouseEvent* event) override;
	void mouseMoveEvent(QMouseEvent* event) override;
	void mouseReleaseEvent(QMouseEvent* event) override;

signals:
	void sig_StartShare(int type, qint64 sourceId);

private slots:
	void onClose();
	void onStartShare();
	void onScreenPrev();
	void onScreenNext();
	void onAppPrev();
	void onAppNext();

private:
	QWidget* m_pTitleWidget = nullptr;
	QLabel* m_pTitleLabel = nullptr;
	QPushButton* m_pBtnClose = nullptr;

	QLabel* m_pLabel_Screen = nullptr;
	QLabel* m_pScreenPreview = nullptr;
	QLabel* m_pScreenName = nullptr;
	QPushButton* m_pBtnScreenPrev = nullptr;
	QPushButton* m_pBtnScreenNext = nullptr;

	QLabel* m_pLabel_App = nullptr;
	QLabel* m_pAppPreview = nullptr;
	QLabel* m_pAppName = nullptr;
	QPushButton* m_pBtnAppPrev = nullptr;
	QPushButton* m_pBtnAppNext = nullptr;

	QPushButton* m_pBtnStart = nullptr;

	QPoint m_windowPos;
	QPoint m_mousePos;
	QPoint m_dPos;
	bool m_dragging = false;

	int m_selectedType = -1;
	int m_screenIndex = -1;
	int m_appIndex = -1;
	std::vector<ShareScreenItem> m_screenItems;
	std::vector<ShareScreenItem> m_appItems;
};
