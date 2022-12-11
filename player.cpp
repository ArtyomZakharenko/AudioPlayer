#include "player.h"
#include "qmediaplaylist.h"
#include "ui_player.h"
#include "QFileDialog"
#include "QTime"
#include "QString"
#include "QMediaMetaData"
#include "QMediaPlaylist"
#include "QMessageBox"
#include <iostream>

Player::Player(QWidget *parent)
        : QMainWindow(parent), ui(new Ui::Player) {
    ui->setupUi(this);
    player = new QMediaPlayer(this);
    playlist = new QMediaPlaylist(player);
    player->setPlaylist(playlist);
    player->setVolume(ui->volumeBar->value());
    playlist->setPlaybackMode(QMediaPlaylist::Loop);

    connect(player, &QMediaPlayer::stateChanged, this, &Player::stateChanged);
    connect(player, &QMediaPlayer::positionChanged, this, &Player::positionChanged);
    connect(playlist, &QMediaPlaylist::playbackModeChanged, this, &Player::playbackModeChanged);
    connect(playlist, &QMediaPlaylist::currentIndexChanged, this, &Player::currentIndexChanged);

}

Player::~Player() {
    delete ui;
    delete player;
    delete playlist;
}

void Player::stateChanged(QMediaPlayer::State state) {
    if (state == QMediaPlayer::PlayingState) {
        ui->playButton->setIcon(QIcon(":/buttons/pause.png"));
        ui->currentSong->setText(player->currentMedia().request().url().fileName());
    } else {
        ui->playButton->setIcon(QIcon(":/buttons/play.png"));
        ui->currentSong->setText("Paused");
    }
}

void Player::positionChanged(qint64 position) {
    if (ui->seekBar->maximum() != player->duration()) {
        ui->seekBar->setMaximum(player->duration());
    }
    ui->seekBar->setValue(position);

    int seconds = (position / 1000) % 60;
    int minutes = (position / 60000) % 60;

    int hours = (position / 3600000) % 24;
    QTime time(hours, minutes, seconds);
    QTime duration(0, 0, 0);
    duration = duration.addMSecs(player->duration());
    if (hours > 0) {
        ui->timeStamp->setText(time.toString("hh:mm:ss") + " / " + duration.toString("hh:mm:ss"));
    } else {
        ui->timeStamp->setText(time.toString("mm:ss") + " / " + duration.toString("mm:ss"));
    }
}

void Player::playbackModeChanged(QMediaPlaylist::PlaybackMode mode) {
    if (mode == QMediaPlaylist::Loop) {
        ui->shuffleButton->setChecked(false);
        ui->repeatButton->setChecked(false);
    } else if (mode == QMediaPlaylist::CurrentItemInLoop) {
        ui->shuffleButton->setChecked(false);
        ui->repeatButton->setChecked(true);
    } else if (mode == QMediaPlaylist::Random) {
        ui->shuffleButton->setChecked(true);
        ui->repeatButton->setChecked(false);
    }
}

void Player::currentIndexChanged(int index) {
    ui->playlistView->setCurrentRow(index);
    ui->currentSong->setText(playlist->currentMedia().request().url().fileName());
}

void Player::openFile() {
    QStringList files = QFileDialog::getOpenFileNames(
            this,
            "Open a file",
            QString(),
            "Audio files (*.mp3 *.wav *m3u)"
    );
    if (files.isEmpty()) {
        return;
    }
    if (files.first().endsWith(".m3u")) {
        playlist->clear();
        playlist->load(QUrl::fromLocalFile(files.first()));
        ui->playlistView->clear();
        for (int i = 0; i < playlist->mediaCount(); i++) {
            ui->playlistView->addItem(playlist->media(i).request().url().fileName());
        }
    } else {
        foreach(QString
        file, files) {
            playlist->addMedia(QUrl::fromLocalFile(file));
            QFileInfo fileInfo(file);
            ui->playlistView->addItem(fileInfo.fileName());
        }
    }
}

void Player::savePlaylist() {
    QString file = QFileDialog::getSaveFileName(
            this,
            "Save playlist",
            QString(),
            "Playlist (*.m3u)"
    );
    if (file.isEmpty()) {
        return;
    }
    playlist->save(QUrl::fromLocalFile(file), "m3u");
}

void Player::clearPlaylist() {
    playlist->clear();
    ui->playlistView->clear();
}

void Player::on_addButton_clicked() {
    openFile();
}

void Player::on_saveButton_clicked() {
    savePlaylist();
}

void Player::on_removeButton_clicked() {
    int index = ui->playlistView->currentRow();
    if (index == -1) {
        return;
    }
    playlist->removeMedia(index);
    delete ui->playlistView->takeItem(index);
}


void Player::on_clearButton_clicked() {
    clearPlaylist();
}

void Player::on_playlistView_itemDoubleClicked() {
    int row = ui->playlistView->currentRow();
    playlist->setCurrentIndex(row);
    player->play();
}

void Player::on_playButton_clicked() {
    if (player->state() == QMediaPlayer::PlayingState) {
        player->pause();
    } else {
        player->play();
    }
}

void Player::on_seekBar_sliderMoved(int position) {
    player->setPosition(position);
}

void Player::on_volumeBar_sliderMoved(int position) {
    player->setVolume(position);
    if (player->volume() == 0) {
        ui->muteButton->setIcon(QIcon(":/buttons/mute.png"));
    } else {
        ui->muteButton->setIcon(QIcon(":/buttons/volume.png"));
    }
}

void Player::on_muteButton_clicked() {
    if (player->isMuted()) {
        player->setMuted(false);
        ui->muteButton->setIcon(QIcon(":/buttons/volume.png"));
    } else {
        player->setMuted(true);
        ui->muteButton->setIcon(QIcon(":/buttons/mute.png"));
    }
}

void Player::on_volumeBar_valueChanged(int value) {
    player->setVolume(value);
}

void Player::on_nextButton_clicked() {
    playlist->next();
}

void Player::on_prevButton_clicked() {
    playlist->previous();
}

void Player::on_shuffleButton_clicked() {
    if (ui->shuffleButton->isChecked()) {
        playlist->setPlaybackMode(QMediaPlaylist::Random);
    } else {
        playlist->setPlaybackMode(QMediaPlaylist::Loop);
    }
}

void Player::on_repeatButton_clicked() {
    if (ui->repeatButton->isChecked()) {
        playlist->setPlaybackMode(QMediaPlaylist::CurrentItemInLoop);
    } else {
        playlist->setPlaybackMode(QMediaPlaylist::Loop);
    }
}

void Player::on_searchBar_textChanged() {
    QString search = ui->searchBar->text();
    for (int i = 0; i < ui->playlistView->count(); i++) {
        QListWidgetItem *item = ui->playlistView->item(i);
        if (item->text().contains(search, Qt::CaseInsensitive)) {
            item->setHidden(false);
        } else {
            item->setHidden(true);
        }
    }
}

void Player::on_actionOpenFile_triggered() {
    openFile();
}

void Player::on_actionSavePlaylist_triggered() {
    savePlaylist();
}

void Player::on_actionClearQueue_triggered() {
    clearPlaylist();
}

void Player::on_actionExit_triggered() {
    QApplication::quit();
}

void Player::on_actionInfo_triggered() {
    QMessageBox::information(this, "О программе", "Аудиоплеер на фреймворке Qt\n"
                                                  "Автор: Артём Захаренко\n"
                                                  "Группа: 050541\n"
                                                  "Выполнено в рамках курсового проекта по дисциплине \"Конструирование программ и языки программирования\"\n");
}

