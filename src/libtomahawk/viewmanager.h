/* === This file is part of Tomahawk Player - <http://tomahawk-player.org> ===
 *
 *   Copyright 2010-2011, Christian Muehlhaeuser <muesli@tomahawk-player.org>
 *
 *   Tomahawk is free software: you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License as published by
 *   the Free Software Foundation, either version 3 of the License, or
 *   (at your option) any later version.
 *
 *   Tomahawk is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 *   GNU General Public License for more details.
 *
 *   You should have received a copy of the GNU General Public License
 *   along with Tomahawk. If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef PLAYLISTMANAGER_H
#define PLAYLISTMANAGER_H

#include <QObject>
#include <QHash>
#include <QStackedWidget>

#include "artist.h"
#include "collection.h"
#include "playlistinterface.h"
#include "playlist/queueview.h"
#include "viewpage.h"
#include "widgets/welcomewidget.h"
#include "widgets/whatshotwidget.h"

#include "dllmacro.h"

class AnimatedSplitter;
class AlbumModel;
class AlbumView;
class AlbumInfoWidget;
class ArtistInfoWidget;
class ArtistView;
class CollectionModel;
class CollectionFlatModel;
class CollectionView;
class ContextWidget;
class PlaylistModel;
class PlaylistView;
class TrackProxyModel;
class TrackModel;
class TreeProxyModel;
class TreeModel;
class TrackView;
class SourceInfoWidget;
class InfoBar;
class TopBar;
class WelcomeWidget;
class WhatsHotWidget;
class QPushButton;

namespace Tomahawk
{
    class DynamicWidget;
}

class DLLEXPORT ViewManager : public QObject
{
Q_OBJECT

public:
    static ViewManager* instance();

    explicit ViewManager( QObject* parent = 0 );
    ~ViewManager();

    QWidget* widget() const { return m_widget; }
    InfoBar* infobar() const { return m_infobar; }
    ContextWidget* context() const { return m_contextWidget; }

    PlaylistView* queue() const { return m_queue->queue(); }
    void setQueue( QueueView* queue ) { m_queue = queue; }

    bool isSuperCollectionVisible() const;
    bool isNewPlaylistPageVisible() const;

    Tomahawk::PlaylistInterface* currentPlaylistInterface() const;
    Tomahawk::ViewPage* currentPage() const;
    Tomahawk::ViewPage* pageForInterface( Tomahawk::PlaylistInterface* interface ) const;

    Tomahawk::ViewPage* show( Tomahawk::ViewPage* page );

    Tomahawk::ViewPage* welcomeWidget() const { return m_welcomeWidget; }
    Tomahawk::ViewPage* whatsHotWidget() const { return m_whatsHotWidget; }
    ArtistView* superCollectionView() const { return m_superCollectionView; }

    /// Get the view page for the given item. Not pretty...
    Tomahawk::ViewPage* pageForPlaylist( const Tomahawk::playlist_ptr& pl ) const;
    Tomahawk::ViewPage* pageForDynPlaylist( const Tomahawk::dynplaylist_ptr& pl ) const;
    Tomahawk::ViewPage* pageForCollection( const Tomahawk::collection_ptr& pl ) const;

    /// Get a playlist (or dynamic playlist ) from a ViewPage* if the page is PlaylistView or DynamicWidget.
    /// Lives here but used by SourcesModel
    Tomahawk::playlist_ptr playlistForPage( Tomahawk::ViewPage* ) const;

    // only use this is you need to create a playlist and show it directly and want it to be
    // linked to the sidebar. call it right after creating the playlist
    PlaylistView* createPageForPlaylist( const Tomahawk::playlist_ptr& pl );

signals:
    void numSourcesChanged( unsigned int sources );
    void numTracksChanged( unsigned int tracks );
    void numArtistsChanged( unsigned int artists );
    void numShownChanged( unsigned int shown );

    void repeatModeChanged( Tomahawk::PlaylistInterface::RepeatMode mode );
    void shuffleModeChanged( bool enabled );

    void statsAvailable( bool b );
    void modesAvailable( bool b );
    void filterAvailable( bool b );
    void autoUpdateAvailable( bool b );
    void modeChanged( Tomahawk::PlaylistInterface::ViewMode mode );

    void playClicked();
    void pauseClicked();

    void tempPageActivated( Tomahawk::ViewPage* );
    void viewPageActivated( Tomahawk::ViewPage* );

    void showQueueRequested();
    void hideQueueRequested();

public slots:
    Tomahawk::ViewPage* showSuperCollection();
    Tomahawk::ViewPage* showWelcomePage();
    Tomahawk::ViewPage* showWhatsHotPage();
    void showCurrentTrack();

    // Returns the shown viewpage
    Tomahawk::ViewPage* show( const Tomahawk::playlist_ptr& playlist );
    Tomahawk::ViewPage* show( const Tomahawk::dynplaylist_ptr& playlist );
    Tomahawk::ViewPage* show( const Tomahawk::artist_ptr& artist );
    Tomahawk::ViewPage* show( const Tomahawk::album_ptr& album, Tomahawk::ModelMode withInitialMode = Tomahawk::InfoSystemMode );
    Tomahawk::ViewPage* show( const Tomahawk::collection_ptr& collection );
    Tomahawk::ViewPage* show( const Tomahawk::source_ptr& source );

    void historyBack();
    void removeFromHistory( Tomahawk::ViewPage* p );

    void showQueue() { emit showQueueRequested(); }
    void hideQueue() { emit hideQueueRequested(); }

    void setTreeMode();
    void setTableMode();
    void setAlbumMode();

    void setRepeatMode( Tomahawk::PlaylistInterface::RepeatMode mode );
    void setShuffled( bool enabled );

    void playlistInterfaceChanged( Tomahawk::PlaylistInterface* );

    // called by the playlist creation dbcmds
    void createPlaylist( const Tomahawk::source_ptr& src, const QVariant& contents );
    void createDynamicPlaylist( const Tomahawk::source_ptr& src, const QVariant& contents );

private slots:
    void setFilter( const QString& filter );
    void applyFilter();

    void autoUpdateChanged( int );

    void onWidgetDestroyed( QWidget* widget );

private:
    void setPage( Tomahawk::ViewPage* page, bool trackHistory = true );
    void updateView();
    void unlinkPlaylist();
    void saveCurrentPlaylistSettings();
    void loadCurrentPlaylistSettings();

    Tomahawk::playlist_ptr playlistForInterface( Tomahawk::PlaylistInterface* interface ) const;
    Tomahawk::dynplaylist_ptr dynamicPlaylistForInterface( Tomahawk::PlaylistInterface* interface ) const;
    Tomahawk::collection_ptr collectionForInterface( Tomahawk::PlaylistInterface* interface ) const;

    QWidget* m_widget;
    InfoBar* m_infobar;
    ContextWidget* m_contextWidget;
    QStackedWidget* m_stack;
    AnimatedSplitter* m_splitter;

    AlbumModel* m_superAlbumModel;
    AlbumView* m_superAlbumView;
    TreeModel* m_superCollectionModel;
    ArtistView* m_superCollectionView;
    QueueView* m_queue;
    WelcomeWidget* m_welcomeWidget;
    WhatsHotWidget* m_whatsHotWidget;

    QList< Tomahawk::collection_ptr > m_superCollections;

    QHash< Tomahawk::dynplaylist_ptr, QWeakPointer<Tomahawk::DynamicWidget> > m_dynamicWidgets;
    QHash< Tomahawk::collection_ptr, QWeakPointer<CollectionView> > m_collectionViews;
    QHash< Tomahawk::collection_ptr, QWeakPointer<ArtistView> > m_treeViews;
    QHash< Tomahawk::collection_ptr, QWeakPointer<AlbumView> > m_collectionAlbumViews;
    QHash< Tomahawk::artist_ptr, QWeakPointer<ArtistInfoWidget> > m_artistViews;
    QHash< Tomahawk::album_ptr, QWeakPointer<AlbumInfoWidget> > m_albumViews;
    QHash< Tomahawk::playlist_ptr, QWeakPointer<PlaylistView> > m_playlistViews;
    QHash< Tomahawk::source_ptr, QWeakPointer<SourceInfoWidget> > m_sourceViews;

    QList<Tomahawk::ViewPage*> m_pageHistory;

    Tomahawk::collection_ptr m_currentCollection;
    int m_currentMode;

    QTimer m_filterTimer;
    QString m_filter;

    static ViewManager* s_instance;
};

#endif // PLAYLISTMANAGER_H
