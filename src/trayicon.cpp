#include "trayicon.hpp"

TrayIcon::TrayIcon(MainWin* parent): QSystemTrayIcon(parent), _parent(parent)
{
	_trayIconMenu = new QMenu(_parent);
	
	_showAction = new QAction(tr("Show"), _parent);
	_quitAction = new QAction(tr("Quit"), _parent);
	
	_trayIconMenu->addAction(_showAction);
	_trayIconMenu->addAction(_quitAction);
	
	setContextMenu(_trayIconMenu);
	setToolTip(tr("Audio Video Player"));
	setIcon(QIcon::fromTheme("applications-multimedia"));
	connect(this, SIGNAL(activated(QSystemTrayIcon::ActivationReason)), this, SLOT(clickTray(QSystemTrayIcon::ActivationReason)));
	connect(_showAction, SIGNAL(triggered(bool)), this, SLOT(showApp()));
	connect(_quitAction, SIGNAL(triggered(bool)), qApp, SLOT(quit()));
}

void TrayIcon::showApp()
{
	if(!_parent->isVisible())
		_parent->setVisible(true);
	else
		_parent->setWindowState(Qt::WindowActive);
}

void TrayIcon::clickTray(QSystemTrayIcon::ActivationReason reason)
{
	if(reason == QSystemTrayIcon::Trigger)
	{
		if(!_parent->isVisible())
			_parent->setVisible(true);
		else
			_parent->setWindowState(Qt::WindowActive);
	}
}

