#pragma once

#include <QAbstractListModel>
#include <QMediaPlaylist>
#include <sstream>
#include <iomanip>
#include <QIcon>

class PlaylistModel : public QAbstractListModel
{
	Q_OBJECT
public:
	PlaylistModel(QObject *parent = 0) : QAbstractListModel(parent), _playlist(new QMediaPlaylist){
		_playlist->setPlaybackMode(QMediaPlaylist::Sequential);
	}
	
	int rowCount(const QModelIndex &parent = QModelIndex()) const;
	QVariant data(const QModelIndex &index, int role) const;
	QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;
	
	Qt::ItemFlags flags(const QModelIndex &index) const;
	bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole);
	
	bool insertRow(int position, QMediaContent content, const QModelIndex &index = QModelIndex());
	bool insertRow(int position, QString content, const QModelIndex &index = QModelIndex());
	bool insertRows(int position, QStringList content, const QModelIndex &index = QModelIndex());
	bool removeRow(int position, const QModelIndex &index = QModelIndex());
	bool removeRows(int position, int rows, const QModelIndex &index = QModelIndex());
	bool addRow(QMediaContent content);
	bool addRow(QString content);
	bool addRows(QStringList content);
	QMediaPlaylist* playlist();
	
	void load(const QUrl& location, const char* format = 0);
	
public slots:
	void next();
	void previous();
	
	
private:
	
	QMediaPlaylist *_playlist;
	
};