#pragma once

#include <QDialog>
#include "commons.h"
#include <QListWidget>
#include <QLabel>
#include <QPushButton>
#include <map>

class ShareScreenDialog : public QDialog
{
	Q_OBJECT

public:
	explicit ShareScreenDialog(QWidget* parent = nullptr);
	~ShareScreenDialog() override;

	void initListWidget(const VecWindowShareInfo& vec);

private:
	void clearMap();
	void clearLists();

	void mousePressEvent(QMouseEvent* event) override;
	void mouseMoveEvent(QMouseEvent* event) override;
	void mouseReleaseEvent(QMouseEvent* event) override;

signals:
	void sig_StartShare(int type, qint64 sourceId);

private slots:
	void onMin();
	void onClose();
	void onCancel();
	void onStartShare();
	void onDesktopItemSelected(QListWidgetItem* item);
	void onAppItemSelected(QListWidgetItem* item);

private:
	QWidget* m_pTitleWidget = nullptr;
	QLabel* m_pTitleLabel = nullptr;
	QPushButton* m_pBtnMin = nullptr;
	QPushButton* m_pBtnClose = nullptr;
	QLabel* m_pLabel_Screen = nullptr;
	QListWidget* m_pLWScreen = nullptr;
	QLabel* m_pLabel_App = nullptr;
	QListWidget* m_pLWApp = nullptr;
	QPushButton* m_pBtnCancel = nullptr;
	QPushButton* m_pBtnStart = nullptr;

	QPoint m_windowPos;
	QPoint m_mousePos;
	QPoint m_dPos;
	bool m_dragging = false;

	int m_selectedType = -1;   // 0=Screen,1=App
	std::map<int, qint64> m_mapScreen;
	std::map<int, qint64> m_mapApp;
};
