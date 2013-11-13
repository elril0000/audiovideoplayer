#include "playlistmodel.hpp"

QMediaPlaylist* PlaylistModel::playlist()
{
	return _playlist;
}

void PlaylistModel::load(const QUrl& location, const char* format)
{
	int rows = rowCount();
	_playlist->load(location, format);
	beginInsertRows(QModelIndex(), rows, rowCount() - 1);
	endInsertRows();
}

void PlaylistModel::next()
{
	_playlist->next();
}

void PlaylistModel::previous()
{
	_playlist->previous();
}

int PlaylistModel::rowCount(const QModelIndex& parent) const
{
	return _playlist->mediaCount();
}

QVariant PlaylistModel::data(const QModelIndex& index, int role) const
{
    if (!index.isValid())
        return QVariant();

    if (index.row() >= _playlist->mediaCount())
        return QVariant();

    if (role == Qt::DisplayRole || role == Qt::EditRole)
	{
		return _playlist->media(index.row()).canonicalUrl().path().section("/", -1);
	}
	if(role == Qt::DecorationRole)
	{
		if(_playlist->media(index.row()) == _playlist->currentMedia())
			return QIcon::fromTheme("arrow-right");
		else
		{
			QPixmap pixmap(22, 22);
			QColor white(255, 255, 255, 0);
			pixmap.fill(white);
			return pixmap;
		}
	}
    else
        return QVariant();
}

QVariant PlaylistModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role != Qt::DisplayRole)
        return QVariant();

    if (orientation == Qt::Horizontal)
        return QString("Column %1").arg(section);
    else
        return QString("Row %1").arg(section);
}

Qt::ItemFlags PlaylistModel::flags(const QModelIndex& index) const
{
    if (!index.isValid())
        return Qt::ItemIsEnabled;

    return QAbstractItemModel::flags(index) | Qt::ItemIsEditable;
}

bool PlaylistModel::setData(const QModelIndex& index, const QVariant& value, int role)
{
	if (index.isValid() && role == Qt::EditRole)
	{
		_playlist->media(index.row()).canonicalUrl().setPath(value.toString());
		emit dataChanged(index, index);
		return true;
	}
	return false;
}

bool PlaylistModel::insertRow(int position, QMediaContent content, const QModelIndex& index)
{
	beginInsertRows(QModelIndex(), position, position);
	
	_playlist->insertMedia(position, content);
	
	endInsertRows();
	return true;
}

bool PlaylistModel::insertRow(int position, QString content, const QModelIndex& index)
{
	beginInsertRows(QModelIndex(), position, position);
	_playlist->insertMedia(position, QMediaContent(QUrl::fromLocalFile(content)));
	
	endInsertRows();
	return true;
}

bool PlaylistModel::insertRows(int position, QStringList content, const QModelIndex& index)
{
	beginInsertRows(QModelIndex(), position, position + content.count() - 1);
	for(int i = 0 ; i < content.count() ; i++)
	{
		_playlist->insertMedia(position + i, QMediaContent(QUrl::fromLocalFile(content.at(i))));
	}
	
	endInsertRows();
	return true;
}

bool PlaylistModel::removeRow(int position, const QModelIndex& index)
{
	beginRemoveRows(QModelIndex(), position, position);
	
	_playlist->removeMedia(position);
	
	endRemoveRows();
	return true;
}

bool PlaylistModel::removeRows(int position, int rows, const QModelIndex& index)
{
	beginRemoveRows(QModelIndex(), position, position+rows-1);

	for (int row = 0; row < rows; ++row)
	{
		_playlist->removeMedia(position);
	}

	endRemoveRows();
	return true;
}

bool PlaylistModel::addRow(QMediaContent content)
{
	beginInsertRows(QModelIndex(), rowCount(), rowCount());
	
	_playlist->addMedia(content);
	
	endInsertRows();
	return true;
}

bool PlaylistModel::addRow(QString content)
{
	beginInsertRows(QModelIndex(), rowCount(), rowCount());
	
	_playlist->addMedia(QUrl::fromLocalFile(content));
	
	endInsertRows();
	return true;
}

bool PlaylistModel::addRows(QStringList content)
{
	beginInsertColumns(QModelIndex(), rowCount(), rowCount() + content.count() - 1);
	
	for(int i = 0 ; i < content.count() ; i++)
	{
		_playlist->addMedia(QUrl::fromLocalFile(content.at(i)));
	}
	
	endInsertRows();
	return true;
}

