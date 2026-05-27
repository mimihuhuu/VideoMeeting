#include "CLogDialog.h"
#include <QtWidgets/QApplication>
#include "CmainWidget.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    CLogDialog window;
	CmainWidget mainWidget; 

	//����exec()������ʾģ̬�Ի���ֻ�е��û�����˶Ի����ϵİ�ť���Ż����ִ�к���Ĵ���
    if (window.exec() == QDialog::Accepted)
    {
        mainWidget.setJoinMediaOptions(window.isCameraEnabled(), window.isMicEnabled());
        mainWidget.show(); 
        mainWidget.joinroom(window.getRoomID());
        return app.exec();
    }
}
