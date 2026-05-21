//????????��?????
#pragma once
#include <QWidget>
#include <QListWidget>

class CSmallVideWidget;

class CLeftVideoList : public QWidget
{
	Q_OBJECT
public:
	CLeftVideoList(QWidget* p = nullptr);
	~CLeftVideoList();

public:
	void addVideoWidget(CSmallVideWidget* pSmall);

private:
	QListWidget* m_pVideoListWidget = nullptr;
	
};

