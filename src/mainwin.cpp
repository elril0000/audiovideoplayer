#include "mainwin.hpp"

using namespace std;

MainWin::MainWin(): QMainWindow()
{
	
}

void MainWin::init()
{
	_playback = QMediaPlaylist::Sequential;
	
	_mediaPlayer = new QMediaPlayer;
	createAction();
	createMenu();
	createToolbar();
	createDock();
	createMainWin();
	
	setProperties();
	

	connect(_mediaPlayer, SIGNAL(stateChanged(QMediaPlayer::State)), this, SLOT(currentState(QMediaPlayer::State))); 
	connect(_mediaPlayer, SIGNAL(durationChanged(qint64)), this, SLOT(durationChanged(qint64)));
	connect(_playlistModel->playlist(), SIGNAL(currentIndexChanged(int)), this, SLOT(changeSelection(int)));
	connect(_playlistModel->playlist(), SIGNAL(loaded()), this, SLOT(loaded()));
	connect(_playlistModel->playlist(), SIGNAL(loadFailed()), this, SLOT(loadFailed()));
	connect(_mediaPlayer, SIGNAL(volumeChanged(int)), this, SLOT(volumeChanged(int)));
	show();
}

void MainWin::volumeChanged(int volume)
{
	std::stringstream ss;
	ss << std::setfill('0') << std::setw(3) << volume << "%";
	string volumeString = ss.str();
	_volumeLabel->setText(QString(volumeString.c_str()));
}

void MainWin::loaded()
{
	if(_wasEmpty)
		_mediaPlayer->play();
}

void MainWin::loadFailed()
{
	cout << _playlistModel->playlist()->errorString().toStdString() << endl;
}

void MainWin::playback()
{
	switch(_playback)
	{
		case QMediaPlaylist::Sequential:
			_playback = QMediaPlaylist::CurrentItemInLoop;
			_playbackLabel->setText(tr("Repeat Song"));
			break;
			
		case QMediaPlaylist::CurrentItemInLoop:
			_playback = QMediaPlaylist::Loop;
			_playbackLabel->setText(tr("Repeat"));
			break;
			
		case QMediaPlaylist::Loop:
			_playback = QMediaPlaylist::Random;
			_playbackLabel->setText(tr("Shuffle"));
			break;
			
		case QMediaPlaylist::Random:
			_playback = QMediaPlaylist::Sequential;
			_playbackLabel->setText(tr("Normal"));
			break;
			
		default:
			break;
	}
	
	_playlistModel->playlist()->setPlaybackMode(_playback);
}

void MainWin::exitFullscreen(bool isFullscreen)
{
	if(!isFullscreen)
	{
		delete _videoFrame;
		createMainWin();
	}
}

void MainWin::addMedia()
{
	QStringList media = QFileDialog::getOpenFileNames(this, tr("Add Musics"), QDir::homePath(), tr("Music and Video(*.avi *.mp4 *.mkv *.wmv *.flv *.ogg *.mp3 *.wma *.wav *.flac *.m4a);;Music (*.mp3 *.wma *.wav *.flac *.m4a *.ogg);;Video (*.avi *.mp4 *.mkv *.wmv *.flv *.ogg)"));
	
	bool isEmpty;
	if(_playlistModel->playlist()->isEmpty())
		isEmpty = true;
	_playlistModel->addRows(media);
	
	if(isEmpty)
		_mediaPlayer->play();
}

void MainWin::removeMedia()
{
	QItemSelectionModel *selection = _playlistView->selectionModel();
	QModelIndexList selectionList = selection->selectedIndexes();
	
	if(!selectionList.isEmpty())
		_playlistModel->removeRows(selectionList.at(0).row(), selectionList.size());
}

void MainWin::changeSelection(int pos)
{
	//_playlistView->setCurrentIndex(_playlistModel->index(pos));
	setWindowTitle(tr("Simple Audio and Video Player - Playing : ") + _playlistModel->index(pos).data().toString());
	
	for(int i = 0 ; i < _playlistModel->rowCount() ; i++)
	{
		_playlistView->update(_playlistModel->index(i));
	}
}

void MainWin::durationChanged(qint64 duration)
{
	_seekSlider->setRange(0, duration);
	int hour, minutes, seconds;
	toTime(duration, minutes, seconds);
	std::stringstream ss;
	if(minutes >= 60)
	{
		hour = minutes / 60;
		minutes = minutes%60;
		ss << std::setfill('0') << std::setw(2);
		ss << hour;
		ss << ":";
		ss << std::setfill('0') << std::setw(2);
		ss << minutes;
		ss << ":";
		ss << std::setfill('0') << std::setw(2);
		ss << seconds;
	}
	else
	{
		ss << std::setfill('0') << std::setw(2);
		ss << minutes;
		ss << ":";
		ss << std::setfill('0') << std::setw(2);
		ss << seconds;
	}
	
	cout << ss.str() << endl;
	_durationLabel->setText(ss.str().c_str());
}

void MainWin::toTime(const qint64 time, int& minutes, int& seconds)
{
	minutes = (time / 1000) / 60;
	seconds = (time / 1000)%60;
}

void MainWin::createMainWin()
{
	_videoFrame = new VideoWidget(this);
	setCentralWidget(_videoFrame);
	_mediaPlayer->setVideoOutput(_videoFrame);
}

void MainWin::createAction()
{
	_playPauseAction = new QAction(tr("Play/Pause"), this);
	_playPauseAction->setIcon(QIcon::fromTheme("media-playback-start"));
	_playPauseAction->setShortcut(QKeySequence("Space"));
	
	_previousAction = new QAction(tr("Previous"), this);
	_previousAction->setIcon(QIcon::fromTheme("media-seek-backward"));
	
	_stopAction = new QAction(tr("Stop"), this);
	_stopAction->setIcon(QIcon::fromTheme("media-playback-stop"));
	
	_nextAction = new QAction(tr("Next"), this);
	_nextAction->setIcon(QIcon::fromTheme("media-seek-forward"));
	
	_muteAction = new QAction(tr("Mute"), this);
	_muteAction->setIcon(QIcon::fromTheme("audio-volume-high"));
	_muteAction->setCheckable(true);
	
	_fullscreenAction = new QAction(tr("Fullscreen"), this);
	_fullscreenAction->setIcon(QIcon::fromTheme("view-fullscreen"));
	
	_openPlaylistAction = new QAction(tr("Open a playlist"), this);
	_openPlaylistAction->setIcon(QIcon::fromTheme("document-open"));
	
	_savePlaylistAction = new QAction(tr("Save the playlist"), this);
	_savePlaylistAction->setIcon(QIcon::fromTheme("document-save"));
	
	_addMediaAction = new QAction(tr("Add to playlist..."), this);
	_addMediaAction->setIcon(QIcon::fromTheme("list-add"));
	
	_removeMediaAction = new QAction(tr("Remove from playlist"), this);
	_removeMediaAction->setIcon(QIcon::fromTheme("list-remove"));
	
	_hideAction = new QAction(tr("Hide"), this);
	
	_quitAction = new QAction(tr("Quit"), this);
	
	connect(_playPauseAction, SIGNAL(triggered()), this, SLOT(playPause()));
	connect(_stopAction, SIGNAL(triggered(bool)), this, SLOT(stop()));
	connect(_nextAction, SIGNAL(triggered(bool)), this, SLOT(next()));
	connect(_previousAction, SIGNAL(triggered(bool)), this, SLOT(previous()));
	connect(_muteAction, SIGNAL(triggered(bool)), _mediaPlayer, SLOT(setMuted(bool)));
	connect(_fullscreenAction, SIGNAL(triggered(bool)), this, SLOT(fullscreen()));
	connect(_addMediaAction, SIGNAL(triggered(bool)), this, SLOT(addMedia()));
	connect(_removeMediaAction, SIGNAL(triggered(bool)), this, SLOT(removeMedia()));
	connect(_openPlaylistAction, SIGNAL(triggered(bool)), this, SLOT(openPlaylist()));
	connect(_savePlaylistAction, SIGNAL(triggered(bool)), this, SLOT(savePlaylist()));
	connect(_quitAction, SIGNAL(triggered(bool)), qApp, SLOT(quit()));
	connect(_hideAction, SIGNAL(triggered(bool)), this, SLOT(hide()));
}

void MainWin::createMenu()
{
	_fileMenu = new QMenu(tr("&File"));
	_fileMenu->addAction(_openPlaylistAction);
	_fileMenu->addAction(_savePlaylistAction);
	_fileMenu->addSeparator();
	_fileMenu->addAction(_addMediaAction);
	_fileMenu->addAction(_removeMediaAction);
	_fileMenu->addSeparator();
	_fileMenu->addAction(_hideAction);
	_fileMenu->addAction(_quitAction);
	
	_mediaControlMenu = new QMenu(tr("&Media Control"));
	_mediaControlMenu->addAction(_playPauseAction);
	_mediaControlMenu->addAction(_previousAction);
	_mediaControlMenu->addAction(_stopAction);
	_mediaControlMenu->addAction(_nextAction);
	_mediaControlMenu->addSeparator();
	_mediaControlMenu->addAction(_fullscreenAction);
	
	menuBar()->addMenu(_fileMenu);
	menuBar()->addMenu(_mediaControlMenu);
	
}

void MainWin::fullscreen()
{
	delete _videoFrame;
	_videoWidgetWindow = new VideoWidgetWindow(_mediaPlayer, _playlistModel, _videoFrame, this);
	_videoFrame = new VideoWidget(this, _videoWidgetWindow);
	_mediaPlayer->setVideoOutput(_videoFrame);
	_videoFrame->setWindowState(Qt::WindowMaximized);
	_videoFrame->setWindowFlags(Qt::FramelessWindowHint | Qt::X11BypassWindowManagerHint | Qt::WindowStaysOnTopHint);
	_videoFrame->show();
	QTimer::singleShot(1000, this, SLOT(setCenter()));
}

void MainWin::quitFullscreen()
{
	delete _videoFrame;
	createMainWin();
	_videoWidgetWindow->deleteLater();
}

void MainWin::setCenter()
{
	QDesktopWidget *desktop = QApplication::desktop();
	_videoWidgetWindow->setGeometry((desktop->screen(desktop->screenNumber(_videoFrame))->size().width() / 2) - (900 / 2) + desktop->screen(desktop->screenNumber(_videoFrame))->geometry().x(), desktop->screen(desktop->screenNumber(_videoFrame))->size().height() - 75 + desktop->screen(desktop->screenNumber(_videoFrame))->geometry().y(), 900, 75);
	_videoWidgetWindow->show();
	
}

void MainWin::createToolbar()
{
	_seekSlider = new QSlider(Qt::Horizontal);
	
	_volumeSlider = new QSlider(Qt::Horizontal);
	_volumeSlider->setRange(0, 100);
	_volumeSlider->setValue(100);
	_volumeSlider->setFixedWidth(100);
	
	_volumeLabel = new QLabel(tr("100%"));
	
	_elapsedLabel = new QLabel(tr("--:--"));
	
	_durationLabel = new QLabel(tr("--:--"));
	
	_playbackLabel = new QLabel(tr("Normal"));
	
	QWidget *spacer = new QWidget();
	spacer->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
	
	QToolBar *mediaControlToolbar = new QToolBar(tr("Media Control"));
	mediaControlToolbar->addAction(_playPauseAction);
	mediaControlToolbar->addAction(_previousAction);
	mediaControlToolbar->addAction(_stopAction);
	mediaControlToolbar->addAction(_nextAction);
	mediaControlToolbar->addWidget(spacer);
	mediaControlToolbar->addAction(_fullscreenAction);
	mediaControlToolbar->addAction(_muteAction);
	mediaControlToolbar->addWidget(_volumeSlider);
	mediaControlToolbar->addWidget(_volumeLabel);
	mediaControlToolbar->setOrientation(Qt::Horizontal);
	
	QToolBar *seekControlToolbar = addToolBar(tr("Seek Control"));
	seekControlToolbar->addWidget(_elapsedLabel);
	seekControlToolbar->addWidget(_seekSlider);
	seekControlToolbar->addWidget(_durationLabel);
	seekControlToolbar->setOrientation(Qt::Horizontal);
	
	addToolBar(Qt::BottomToolBarArea, mediaControlToolbar);
	addToolBar(Qt::BottomToolBarArea, seekControlToolbar);
	insertToolBarBreak(seekControlToolbar);
	
	_seekSlider->setRange(0, _mediaPlayer->duration());
	connect(_mediaPlayer, SIGNAL(positionChanged(qint64)), this, SLOT(positionChanged(qint64)));
	connect(_seekSlider, SIGNAL(sliderMoved(int)), this, SLOT(seekChanged(int)));
	connect(_volumeSlider, SIGNAL(sliderMoved(int)), _mediaPlayer, SLOT(setVolume(int)));
}

void MainWin::playPause()
{
	if(_mediaPlayer->state() == QMediaPlayer::PlayingState)
	{
		_mediaPlayer->pause();
		
	}
	else if(_mediaPlayer->state() == QMediaPlayer::PausedState)
	{
		_mediaPlayer->play();
	}
	else if(_mediaPlayer->state() == QMediaPlayer::StoppedState)
	{
		_mediaPlayer->play();
	}
}

void MainWin::currentState(QMediaPlayer::State state)
{
	switch(state)
	{
		case QMediaPlayer::PausedState:
			_playPauseAction->setIcon(QIcon::fromTheme("media-playback-start"));
			setWindowTitle(tr("Simple Audio and Video Player - Paused"));
			break;
		
		case QMediaPlayer::PlayingState:
			_playPauseAction->setIcon(QIcon::fromTheme("media-playback-pause"));
			setWindowTitle(tr("Simple Audio and Video Player - Playing : ") + _playlistModel->index(_playlistModel->playlist()->currentIndex()).data().toString());
			break;
			
		case QMediaPlayer::StoppedState:
			_playPauseAction->setIcon(QIcon::fromTheme("media-playback-start"));
			setWindowTitle(tr("Simple Audio and Video Player - Stopped"));
			break;
			
		default:
			break;
	}
}

void MainWin::next()
{
	_playlistModel->next();
}

void MainWin::previous()
{
	_playlistModel->previous();
}

void MainWin::stop()
{
	_mediaPlayer->stop();
}

void MainWin::positionChanged(qint64 pos)
{
	_seekSlider->setValue(pos);
	int hour, minutes, seconds;
	toTime(pos, minutes, seconds);
	
	std::stringstream ss;
	if(minutes >= 60)
	{
		hour = minutes / 60;
		minutes = minutes % 60;
		ss << std::setfill('0') << std::setw(2);
		ss << hour;
		ss << ":";
		ss << std::setfill('0') << std::setw(2);
		ss << minutes;
		ss << ":";
		ss << std::setfill('0') << std::setw(2);
		ss << seconds;
	}
	else
	{
		ss << std::setfill('0') << std::setw(2);
		ss << minutes;
		ss << ":";
		ss << std::setfill('0') << std::setw(2);
		ss << seconds;
	}
	
	_elapsedLabel->setText(ss.str().c_str());
	
}

void MainWin::seekChanged(int pos)
{
	_mediaPlayer->setPosition(pos);
}

void MainWin::setProperties()
{
	setMinimumSize(350, 60);
	
	resize(800, 600);
	
	setWindowTitle(tr("Simple Audio and Video Player - Stopped"));
}

void MainWin::show()
{
	QWidget::show();
	emit done();
}

void MainWin::createDock()
{
	_addMediaButton = new QPushButton;
	_addMediaButton->setIcon(QIcon::fromTheme("list-add"));
	
	_removeMediaButton = new QPushButton;
	_removeMediaButton->setIcon(QIcon::fromTheme("list-remove"));
	
	_playbackLabel = new QLabel(tr("Normal"));
	_playbackButton = new QPushButton;
	_playbackButton->setIcon(QIcon::fromTheme("media-playlist-shuffle"));
	
	_openPlaylistButton = new QPushButton;
	_openPlaylistButton->setIcon(QIcon::fromTheme("document-open"));
	
	_saveButton = new QPushButton;
	_saveButton->setIcon(QIcon::fromTheme("document-save"));
	
	QWidget *spacer = new QWidget();
	spacer->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Minimum);
	
	QBoxLayout *buttonLayout = new QHBoxLayout;
	buttonLayout->addWidget(_openPlaylistButton);
	buttonLayout->addWidget(_saveButton);
	buttonLayout->addWidget(spacer);
	buttonLayout->addWidget(_addMediaButton);
	buttonLayout->addWidget(_removeMediaButton);
	
	QBoxLayout *bottomLayout = new QHBoxLayout;
	bottomLayout->setAlignment(Qt::AlignRight);
	bottomLayout->addWidget(_playbackLabel);
	bottomLayout->addWidget(_playbackButton);
	
	_playlistModel = new PlaylistModel();
	
	_playlistView = new QListView;
	_playlistView->setModel(_playlistModel);
	_playlistView->setGridSize(QSize(_playlistView->width(), 25));
	
	QBoxLayout *dockLayout = new QVBoxLayout;
	dockLayout->addLayout(buttonLayout);
	dockLayout->addWidget(_playlistView);
	dockLayout->addLayout(bottomLayout);
	
	QWidget *dockCountainer = new QWidget;
	dockCountainer->setLayout(dockLayout);
	
	QDockWidget *playlistWidget = new QDockWidget(tr("Playlist"));
	playlistWidget->setWidget(dockCountainer);
	
	addDockWidget(Qt::RightDockWidgetArea, playlistWidget);
	
	_playlistView->setSelectionMode(QAbstractItemView::ExtendedSelection);
	_playlistView->setEditTriggers(QAbstractItemView::NoEditTriggers);
	
	_mediaPlayer->setPlaylist(_playlistModel->playlist());
	//*
	connect(_addMediaButton, SIGNAL(clicked(bool)), this, SLOT(addMedia()));
	connect(_removeMediaButton, SIGNAL(clicked(bool)), this, SLOT(removeMedia()));
	connect(_playlistView, SIGNAL(doubleClicked(QModelIndex)), this, SLOT(doubleClickPlaylist(QModelIndex)));
	connect(_playbackButton, SIGNAL(clicked(bool)), this, SLOT(playback()));
	connect(_openPlaylistButton, SIGNAL(clicked(bool)), this, SLOT(openPlaylist()));
	connect(_saveButton, SIGNAL(clicked(bool)), this, SLOT(savePlaylist()));//*/
}

void MainWin::openPlaylist()
{
	_wasEmpty = _playlistModel->playlist()->isEmpty();
	QString openLocation = QFileDialog::getOpenFileName(this, tr("Open Playlist"), QString(), tr("Playlist (*.m3u)"));
	_playlistModel->load(QUrl::fromLocalFile(openLocation), openLocation.section(".", -1).toStdString().c_str());
}

void MainWin::savePlaylist()
{
	QString saveLocation = QFileDialog::getSaveFileName(this, tr("Save Playlist"), QString(), tr("Playlist (*.m3u)"));
	if(_playlistModel->playlist()->save(QUrl::fromLocalFile(saveLocation), saveLocation.section(".", -1).toStdString().c_str()))
		cout << "Saved Successfully" << endl;
	else
		cout << _playlistModel->playlist()->errorString().toStdString() << endl;
}

void MainWin::doubleClickPlaylist(const QModelIndex& index)
{
	_playlistModel->playlist()->setCurrentIndex(index.row());
	_mediaPlayer->play();
}

VideoWidgetWindow::VideoWidgetWindow(QMediaPlayer* mediaPlayer, PlaylistModel* playlistModel, VideoWidget* videoframe, MainWin* mainWin): _mainWin(mainWin)
{
	_playlistModel = playlistModel;
	_videoFrame = videoframe;
	_mediaPlayer = mediaPlayer;
	_playPauseButton = new QPushButton();
	_playPauseButton->setIcon(QIcon::fromTheme("media-playback-start"));
	_playPauseButton->setShortcut(QKeySequence("Space"));
	
	_previousButton = new QPushButton();
	_previousButton->setIcon(QIcon::fromTheme("media-seek-backward"));
	
	_stopButton = new QPushButton();
	_stopButton->setIcon(QIcon::fromTheme("media-playback-stop"));
	
	_nextButton = new QPushButton();
	_nextButton->setIcon(QIcon::fromTheme("media-seek-forward"));
	
	_muteButton = new QPushButton();
	_muteButton->setIcon(QIcon::fromTheme("audio-volume-high"));
	_muteButton->setCheckable(true);
	
	_fullscreenButton = new QPushButton();
	_fullscreenButton->setIcon(QIcon::fromTheme("view-restore"));
	
	_elapsedLabel = new QLabel(tr("--:--"));
	_durationLabel = new QLabel(tr("--:--"));
	
	_volumeLabel = new QLabel(tr("100%"));
	_seekSlider = new QSlider(Qt::Horizontal);
	
	_volumeSlider = new QSlider(Qt::Horizontal);
	_volumeSlider->setRange(0, 100);
	_volumeSlider->setValue(100);
	_volumeSlider->setFixedWidth(100);
	
	QWidget *spacer = new QWidget;
	spacer->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
	
	QBoxLayout *seekLayout = new QHBoxLayout;
	seekLayout->addWidget(_elapsedLabel);
	seekLayout->addWidget(_seekSlider);
	seekLayout->addWidget(_durationLabel);
	
	QBoxLayout *mediaControlLayout = new QHBoxLayout;
	mediaControlLayout->addWidget(_playPauseButton);
	mediaControlLayout->addWidget(_previousButton);
	mediaControlLayout->addWidget(_stopButton);
	mediaControlLayout->addWidget(_nextButton);
	mediaControlLayout->addWidget(spacer);
	mediaControlLayout->addWidget(_fullscreenButton);
	mediaControlLayout->addWidget(_muteButton);
	mediaControlLayout->addWidget(_volumeSlider);
	mediaControlLayout->addWidget(_volumeLabel);
	
	QBoxLayout *videoWidgetLayout = new QVBoxLayout;
	videoWidgetLayout->addLayout(seekLayout);
	videoWidgetLayout->addLayout(mediaControlLayout);
	
	setLayout(videoWidgetLayout);
	_timer = new QTimer;
	setMouseTracking(true);
	
		
	Qt::WindowFlags flags = windowFlags();
	setWindowFlags(Qt::FramelessWindowHint | Qt::X11BypassWindowManagerHint | Qt::WindowStaysOnTopHint);
	
	setCurrentState(_mediaPlayer->state());
	
	durationChanged(_mediaPlayer->duration());
	connect(_playPauseButton, SIGNAL(clicked()), _mainWin, SLOT(playPause()));
	connect(_stopButton, SIGNAL(clicked(bool)), _mainWin, SLOT(stop()));
	connect(_nextButton, SIGNAL(clicked(bool)), _mainWin, SLOT(next()));
	connect(_previousButton, SIGNAL(clicked(bool)), _mainWin, SLOT(previous()));
	connect(_muteButton, SIGNAL(clicked(bool)), _mediaPlayer, SLOT(setMuted(bool)));
	connect(_fullscreenButton, SIGNAL(clicked(bool)), _mainWin, SLOT(quitFullscreen()));
	connect(_mediaPlayer, SIGNAL(stateChanged(QMediaPlayer::State)), this, SLOT(currentState(QMediaPlayer::State)));
	connect(_mediaPlayer, SIGNAL(positionChanged(qint64)), this, SLOT(positionChanged(qint64)));
	connect(_seekSlider, SIGNAL(sliderMoved(int)), this, SLOT(seekChanged(int)));
	connect(_volumeSlider, SIGNAL(sliderMoved(int)), _mediaPlayer, SLOT(setVolume(int)));
	connect(_mediaPlayer, SIGNAL(durationChanged(qint64)), this, SLOT(durationChanged(qint64)));
	connect(_mediaPlayer, SIGNAL(volumeChanged(int)), this, SLOT(volumeChanged(int)));
	connect(_timer, SIGNAL(timeout()), this, SLOT(hideVideoWidget()));
}

void VideoWidgetWindow::currentState(QMediaPlayer::State state)
{
	switch(state)
	{
		case QMediaPlayer::PausedState:
			_playPauseButton->setIcon(QIcon::fromTheme("media-playback-start"));
			break;
		
		case QMediaPlayer::PlayingState:
			_playPauseButton->setIcon(QIcon::fromTheme("media-playback-pause"));
			
			break;
			
		case QMediaPlayer::StoppedState:
			_playPauseButton->setIcon(QIcon::fromTheme("media-playback-start"));
			_mainWin->quitFullscreen();
			break;
			
		default:
			break;
	}
}

void VideoWidgetWindow::setCurrentState(QMediaPlayer::State state)
{
	switch(state)
	{
		case QMediaPlayer::PausedState:
			_playPauseButton->setIcon(QIcon::fromTheme("media-playback-start"));
			break;
		
		case QMediaPlayer::PlayingState:
			_playPauseButton->setIcon(QIcon::fromTheme("media-playback-pause"));
			
			break;
			
		case QMediaPlayer::StoppedState:
			_playPauseButton->setIcon(QIcon::fromTheme("media-playback-start"));
			break;
			
		default:
			break;
	}
}

void VideoWidgetWindow::positionChanged(qint64 pos)
{
	_seekSlider->setValue(pos);
	int hour, minutes, seconds;
	_mainWin->toTime(pos, minutes, seconds);
	
	std::stringstream ss;
	if(minutes >= 60)
	{
		hour = minutes / 60;
		minutes = minutes % 60;
		ss << std::setfill('0') << std::setw(2);
		ss << hour;
		ss << ":";
		ss << std::setfill('0') << std::setw(2);
		ss << minutes;
		ss << ":";
		ss << std::setfill('0') << std::setw(2);
		ss << seconds;
	}
	else
	{
		ss << std::setfill('0') << std::setw(2);
		ss << minutes;
		ss << ":";
		ss << std::setfill('0') << std::setw(2);
		ss << seconds;
	}
	
	_elapsedLabel->setText(ss.str().c_str());
}

void VideoWidgetWindow::seekChanged(int pos)
{
	_mediaPlayer->setPosition(pos);
}

void VideoWidgetWindow::durationChanged(qint64 duration)
{
	_seekSlider->setRange(0, duration);
	int hour, minutes, seconds;
	_mainWin->toTime(duration, minutes, seconds);
	std::stringstream ss;
	if(minutes >= 60)
	{
		hour = minutes / 60;
		minutes = minutes%60;
		ss << std::setfill('0') << std::setw(2);
		ss << hour;
		ss << ":";
		ss << std::setfill('0') << std::setw(2);
		ss << minutes;
		ss << ":";
		ss << std::setfill('0') << std::setw(2);
		ss << seconds;
	}
	else
	{
		ss << std::setfill('0') << std::setw(2);
		ss << minutes;
		ss << ":";
		ss << std::setfill('0') << std::setw(2);
		ss << seconds;
	}
	
	cout << ss.str() << endl;
	_durationLabel->setText(ss.str().c_str());
}

void VideoWidgetWindow::volumeChanged(int volume)
{
	std::stringstream ss;
	ss << std::setfill('0') << std::setw(3) << volume << "%";
	string volumeString = ss.str();
	_volumeLabel->setText(QString(volumeString.c_str()));
}

void VideoWidgetWindow::mouseMoveEvent(QMouseEvent* event)
{
    _timer->start(5000);
	QWidget::mouseMoveEvent(event);
}

void VideoWidgetWindow::hideVideoWidget()
{
	releaseMouse();
	setVisible(false);
	emit hiding();
}

QTimer* VideoWidgetWindow::timer() const
{
	return _timer;
}

void VideoWidgetWindow::keyPressEvent(QKeyEvent* event)
{
	switch(event->key())
	{
		case Qt::Key::Key_Escape:
			if(windowState() == Qt::WindowActive | Qt::WindowMaximized)
				_mainWin->quitFullscreen();
			break;
		
		case Qt::Key::Key_Space:
			_mainWin->playPause();
			break;
			
		case Qt::Key::Key_Right:
			_mainWin->next();
			break;
			
		case Qt::Key::Key_Left:
			_mainWin->previous();
			break;
			
		default:
			QWidget::keyPressEvent(event);
			break;
	}
    _timer->start(5000);
}

