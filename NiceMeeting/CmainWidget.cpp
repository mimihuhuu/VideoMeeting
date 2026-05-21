#include "CmainWidget.h"
#include "CSmallVideWidget.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QMessageBox>

CmainWidget::CmainWidget(QWidget* p)
    : CFrameLessWidgetBase(p)
{
	initUI();
	m_pAgoraObject = new CAgoraObject();
	if (0 != m_pAgoraObject->init())
	{
		QMessageBox::information(this, u8"提示", u8"agora init failed");
		exit(EXIT_FAILURE);
	}

	connect(m_pAgoraObject, &CAgoraObject::sender_joinedChannelSuccess, 
			this, &CmainWidget::on_LocalJoinSuccess);
	connect(m_pAgoraObject, &CAgoraObject::sender_userJoined, 
			this, &CmainWidget::on_RemoteJoined);
}

CmainWidget::~CmainWidget()
{ 
    

}

void CmainWidget::joinroom(const QString& roomID)
{
	m_pAgoraObject->joinChannel(roomID, 123456);
}

void CmainWidget::initUI()
{
    m_pTitleBar = new CTitleBar(this);
	connect(m_pTitleBar, &CTitleBar::sig_close, this, &QWidget::close);
    m_pBigVideoWidget = new CBigVideoWidget(this);
    m_pLeftVideoList = new CLeftVideoList(this);
    m_pBottonBar = new CBottonBar(this);

	QVBoxLayout* pMainLayout = new QVBoxLayout(this);

	pMainLayout->addWidget(m_pTitleBar);
	pMainLayout->setSpacing(0);
	QHBoxLayout* pVideoLayout = new QHBoxLayout();  
	pVideoLayout->addWidget(m_pLeftVideoList);
	pVideoLayout->addWidget(m_pBigVideoWidget);

	pMainLayout->addLayout(pVideoLayout);
	pMainLayout->addWidget(m_pBottonBar);

	pMainLayout->setContentsMargins(0, 0, 0, 0);
}

//频道加入成功回调
void CmainWidget::on_LocalJoinSuccess(const QString& qsChannel, unsigned int uid, int elapsed)
{
	m_pAgoraObject->LocalVideoPreview(m_pBigVideoWidget->getHWND(), true);
}
//远端用户加入回调 HWND为远端用户视频显示窗口句柄
void CmainWidget::on_RemoteJoined(uid_t uid, int elapsed)
{
	CSmallVideWidget* pSmall = new CSmallVideWidget();	
	m_pLeftVideoList->addVideoWidget(pSmall);	
	m_pAgoraObject->RemoteVideoRender(uid, (HWND)(pSmall->winId()));
}
