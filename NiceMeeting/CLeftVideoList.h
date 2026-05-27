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

	void addVideoWidget(unsigned int uid, CSmallVideWidget* pSmall);
	bool removeVideoWidget(unsigned int uid);
	bool hasVideoWidget(unsigned int uid) const;
	CSmallVideWidget* videoWidget(unsigned int uid) const;
	QList<unsigned int> remoteMemberUids() const;

private:
	QListWidget* m_pVideoListWidget = nullptr;
};
