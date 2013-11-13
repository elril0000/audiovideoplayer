#include "mainwin.hpp"

VideoWidget::VideoWidget(MainWin* parent, VideoWidgetWindow* videoWidgetWindow): _parent(parent), _videoWidgetWindow(videoWidgetWindow)
{
	setMouseTracking(true);
	if(_videoWidgetWindow != 0)
	{
		_videoWidgetWindow->timer()->start(5000);
		connect(_videoWidgetWindow, SIGNAL(hiding()), this, SLOT(hideVideoWidget()));
	}
}

void VideoWidget::hideVideoWidget()
{
	setWindowState(Qt::WindowActive | Qt::WindowMaximized);
	activateWindow();
}

void VideoWidget::keyPressEvent(QKeyEvent* event)
{
	switch(event->key())
	{
		case Qt::Key::Key_Escape:
			if(windowState() == Qt::WindowActive | Qt::WindowMaximized)
				_parent->quitFullscreen();
			break;
		
		case Qt::Key::Key_Space:
			_parent->playPause();
			break;
			
		case Qt::Key::Key_Right:
			_parent->next();
			break;
			
		case Qt::Key::Key_Left:
			_parent->previous();
			break;
			
		default:
			QWidget::keyPressEvent(event);
			break;
	}
	if(_videoWidgetWindow != 0)
	{
		_videoWidgetWindow->setVisible(true);
		_videoWidgetWindow->setWindowState(Qt::WindowActive);
		_videoWidgetWindow->timer()->start(5000);
	}
}

void VideoWidget::mousePressEvent(QMouseEvent* event)
{
	if(_videoWidgetWindow != 0)
	{
		_videoWidgetWindow->setVisible(true);
		_videoWidgetWindow->setWindowState(Qt::WindowActive);
		_videoWidgetWindow->activateWindow();
		_videoWidgetWindow->timer()->start(5000);
	}
	QWidget::mousePressEvent(event);
	
}

void VideoWidget::mouseMoveEvent(QMouseEvent* event)
{
    if(_videoWidgetWindow != 0)
	{
		_videoWidgetWindow->setVisible(true);
		_videoWidgetWindow->setWindowState(Qt::WindowActive);
		_videoWidgetWindow->activateWindow();
		_videoWidgetWindow->timer()->start(5000);
	}
	QWidget::mouseMoveEvent(event);
}
