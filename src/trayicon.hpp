#pragma once

#include <QSystemTrayIcon>
#include "mainwin.hpp"

class TrayIcon : public QSystemTrayIcon
{
	Q_OBJECT
public:
    TrayIcon(MainWin* parent);
	
public slots:
	void clickTray(QSystemTrayIcon::ActivationReason reason);
	void showApp();
	
private:
	MainWin *_parent;
	QMenu *_trayIconMenu;
	
	QAction *_quitAction;
	QAction *_showAction;
	
};