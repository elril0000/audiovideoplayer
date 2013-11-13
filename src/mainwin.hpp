#pragma once

#include <QtWidgets>
#include <QRect>
#include <QtMultimediaWidgets>
#include <QtMultimedia>
#include <iostream>
#include <iomanip>
#include <string>
#include <cstdio>
#include <sstream>
#include "playlistmodel.hpp"
#include "videowidget.hpp"

class VideoWidget;

class VideoWidgetWindow;

class MainWin : public QMainWindow
{
	Q_OBJECT
public:
	MainWin();
	
public slots:
	void positionChanged(qint64 pos);
	void seekChanged(int pos);
	void playPause();
	void next();
	void previous();
	void stop();
	void currentState(QMediaPlayer::State state);
	void durationChanged(qint64 duration);
	void toTime(const qint64 time, int& minutes, int& seconds);
	void changeSelection(int pos);
	void addMedia();
	void removeMedia();
	void doubleClickPlaylist(const QModelIndex& index);
	void fullscreen();
	void exitFullscreen(bool isFullscreen);
	void playback();
	void savePlaylist();
	void openPlaylist();
	void loaded();
	void loadFailed();
	virtual void show();
	void init();
	void volumeChanged(int volume);
	void quitFullscreen();
	
signals:
	void done();
	
private:
	void createMainWin();
	void createAction();
	void createToolbar();
	void createDock();
	void createMenu();
	void setProperties();
	
	QAction *_playPauseAction;
	QAction *_previousAction;
	QAction *_stopAction;
	QAction *_nextAction;
	QAction *_muteAction;
	QAction *_fullscreenAction;
	QAction *_addMediaAction;
	QAction *_removeMediaAction;
	QAction *_openPlaylistAction;
	QAction *_savePlaylistAction;
	//QAction *_optionAction;
	QAction *_hideAction;
	QAction *_quitAction;
	
	QMenu *_fileMenu;
	QMenu *_mediaControlMenu; 
	
	QPushButton *_addMediaButton;
	QPushButton *_removeMediaButton;
	QPushButton *_playbackButton;
	QPushButton *_openPlaylistButton;
	QPushButton *_saveButton;
	
	QListView *_playlistView;
	
	QMediaPlaylist::PlaybackMode _playback;
	
	bool _wasEmpty;
	
	QSlider *_seekSlider;
	QSlider *_volumeSlider;
	
	QLabel *_durationLabel;
	QLabel *_elapsedLabel;
	QLabel *_volumeLabel;
	QLabel *_playbackLabel;
	
	VideoWidget *_videoFrame;

	
	QMediaPlayer *_mediaPlayer;
	
	PlaylistModel *_playlistModel;
	
	VideoWidgetWindow *_videoWidgetWindow;
	
};

class VideoWidgetWindow : public QWidget
{
	Q_OBJECT
public:
    VideoWidgetWindow(QMediaPlayer* mediaPlayer, PlaylistModel* playlistModel, VideoWidget* videoframe, MainWin* parent);
	
public slots:
	void currentState(QMediaPlayer::State state);
	void positionChanged(qint64 pos);
	void seekChanged(int pos);
	void durationChanged(qint64 duration);
	void volumeChanged(int volume);
	QTimer* timer() const;
	void hideVideoWidget();
	
signals:
	void hiding();
	
protected:
	virtual void mouseMoveEvent(QMouseEvent * event);
	virtual void keyPressEvent(QKeyEvent* event);
	
private:
	MainWin *_parent;
	QPushButton *_playPauseButton;
	QPushButton *_previousButton;
	QPushButton *_stopButton;
	QPushButton *_nextButton;
	QPushButton *_muteButton;
	QPushButton *_fullscreenButton;
	
	QSlider *_seekSlider;
	QSlider *_volumeSlider;
	
	QLabel *_durationLabel;
	QLabel *_elapsedLabel;
	QLabel *_volumeLabel;
	QLabel *_playbackLabel;
	
	VideoWidget *_videoFrame;

	
	QMediaPlayer *_mediaPlayer;
	
	PlaylistModel *_playlistModel;
	
	QTimer *_timer;
	
};