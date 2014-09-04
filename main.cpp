#include <QtGui/QApplication>
#include "myview.h"
#include <QtCore/QTextCodec>
#include <QTime>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);

	QTextCodec::setCodecForTr(QTextCodec::codecForLocale());
	qsrand(QTime(0,0,0).secsTo(QTime::currentTime()));
	MyView view;
	view.show();

	return a.exec();
}
