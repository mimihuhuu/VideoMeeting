#include "CLogDialog.h"
#include <QtWidgets/QApplication>
#include "CmainWidget.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    CLogDialog window;
	CmainWidget mainWidget; 

	//调用exec()方法显示模态对话框，只有当用户点击了对话框上的按钮，才会继续执行后面的代码
    if (window.exec() == QDialog::Accepted)
    {
        mainWidget.show(); 
        mainWidget.joinroom(window.getRoomID());
        return app.exec();
    }
}
