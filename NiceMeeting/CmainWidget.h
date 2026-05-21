#pragma once
#include <QObject>
#include <Qwidget>
#include "CFrameLessWidgetBase.h"
#include "CBigVideoWidget.h"
#include "CLeftVideoList.h"
#include "CBottonBar.h"
#include "CTitleBar.h"
#include "CAgoraObject.h"

class CmainWidget : public CFrameLessWidgetBase
{
	Q_OBJECT

public:
	CmainWidget(QWidget* p = nullptr);
	~CmainWidget();

	void joinroom(const QString& roomID);

private:
	void initUI();

private slots:
	void on_LocalJoinSuccess(const QString& qsChannel, unsigned int uid, int elapsed);
	void on_RemoteJoined(uid_t uid, int elapsed);

private:	
	CBigVideoWidget* m_pBigVideoWidget = nullptr;
	CLeftVideoList* m_pLeftVideoList = nullptr;
	CBottonBar* m_pBottonBar = nullptr;
	CTitleBar* m_pTitleBar = nullptr;
	
	CAgoraObject* m_pAgoraObject = nullptr;

};

