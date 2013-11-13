#pragma once

#include <QVideoWidget>
#include <QKeyEvent>
#include <QMediaPlayer>

class MainWin;
class VideoWidgetWindow;

class VideoWidget : public QVideoWidget
{
	Q_OBJECT
public:
    VideoWidget(MainWin* parent, VideoWidgetWindow* videoWidgetWindow = 0);
	
public slots:
	void hideVideoWidget();
	
protected:
	virtual void keyPressEvent(QKeyEvent* event);
	virtual void mousePressEvent(QMouseEvent* event);
	virtual void mouseMoveEvent(QMouseEvent * event);
	
private:
	MainWin* _parent;
	VideoWidgetWindow* _videoWidgetWindow;
	QTimer timer;
	
};