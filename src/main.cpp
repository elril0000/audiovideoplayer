#include <iostream>
#include <QApplication>
#include <QWidget>
#include "mainwin.hpp"
#include "Config.hpp"
#include "trayicon.hpp"

int main(int argc, char* argv[])
{
	std::cout << NAME << " v" << VERSION << std::endl;
	QApplication app(argc, argv);
	
	MainWin win;
	win.init();
	
	TrayIcon icon(&win);
	qApp->processEvents();
	
	QTimer::singleShot(1200, &icon, SLOT(show()));
	
	return app.exec();
}
