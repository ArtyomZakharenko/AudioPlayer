#ifndef PLAYER_H
#define PLAYER_H

#include <QMainWindow>
#include <QMediaPlayer>
#include <QTimer>
#include <QMediaPlaylist>

QT_BEGIN_NAMESPACE
namespace Ui { class Player; }
QT_END_NAMESPACE

class Player : public QMainWindow {
    Q_OBJECT

public:
    Player(QWidget *parent = nullptr);

    ~Player();

private
    slots:

    void on_addButton_clicked();

    void on_removeButton_clicked();

    void on_saveButton_clicked();

    void on_playlistView_itemDoubleClicked();

    void on_playButton_clicked();

    void on_seekBar_sliderMoved(int position);

    void on_volumeBar_sliderMoved(int position);

    void on_muteButton_clicked();

    void on_nextButton_clicked();

    void on_prevButton_clicked();

    void on_shuffleButton_clicked();

    void on_repeatButton_clicked();

    void on_volumeBar_valueChanged(int value);

    void on_searchBar_textChanged();

    void on_clearButton_clicked();

    void on_actionOpenFile_triggered();

    void on_actionSavePlaylist_triggered();

    void on_actionClearQueue_triggered();

    void on_actionExit_triggered();

    void on_actionInfo_triggered();

private:
    Ui::Player *ui;
    QMediaPlayer *player;
    QMediaPlaylist *playlist;

    void stateChanged(QMediaPlayer::State state);

    void positionChanged(qint64 position);

    void playbackModeChanged(QMediaPlaylist::PlaybackMode mode);

    void currentIndexChanged(int index);

    void openFile();

    void savePlaylist();

    void clearPlaylist();

};
#endif // PLAYER_H
