/*
    SMF GUI Player test using the MIDI Sequencer C++ library
    Copyright (C) 2006-2010, Pedro Lopez-Cabanillas <plcl@users.sf.net>

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License along
    with this program; if not, write to the Free Software Foundation, Inc.,
    51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
*/

#ifndef INCLUDED_SMFPLAYER_H
#define INCLUDED_SMFPLAYER_H

#include <QObject>
#include <QString>
#include <QList>
#include <QMutex>
#include <QWidget>
#include <QProgressDialog>

#include "qsmf.h"
#include "alsaevent.h"
#include "alsaclient.h"
#include "alsaqueue.h"
#include "alsaport.h"

#include "ui_guiplayer.h"
#include "song.h"
#include "player.h"

using namespace drumstick;

const QString QSTR_DOMAIN("drumstick.sourceforge.net");
const QString QSTR_APPNAME("SMFPlayer");

class GUIPlayer : public QWidget
{
    Q_OBJECT

public:
    GUIPlayer(QWidget *parent = 0);
    ~GUIPlayer();

    void appendEvent(SequencerEvent* ev);
    void subscribe(const QString& portName);
    void updateTimeLabel(int mins, int secs, int cnts);
    void updateTempoLabel(float ftempo);
    void dragEnterEvent(QDragEnterEvent* event);
    void dropEvent(QDropEvent* event);
    void closeEvent(QCloseEvent* event);
    bool event(QEvent* event);
    void openFile(const QString& fileName);
    void readSettings();
    void writeSettings();

public slots:
    void play();
    void pause();
    void stop();
    void open();
    void setup();
    void tempoReset();
    void tempoSlider(int value);
    void quit();

    void songFinished();
    void playerStopped();
    void sequencerEvent(SequencerEvent* ev);
    void headerEvent(int format, int ntrks, int division);
    void noteOnEvent(int chan, int pitch, int vol);
    void noteOffEvent(int chan, int pitch, int vol);
    void keyPressEvent(int chan, int pitch, int press);
    void ctlChangeEvent(int chan, int ctl, int value);
    void pitchBendEvent(int chan, int value);
    void programEvent(int chan, int patch);
    void chanPressEvent(int chan, int press);
    void sysexEvent(const QByteArray& data);
    void textEvent(int type, const QString& data);
    void tempoEvent(int tempo);
    void errorHandler(const QString& errorStr);
    void updateLoadProgress();

private:
    int m_portId;
    int m_queueId;
    int m_initialTempo;
    float m_tempoFactor;
    unsigned long m_tick;

    QSmf* m_engine;
    MidiClient* m_Client;
    MidiPort* m_Port;
    MidiQueue* m_Queue;
    Player* m_player;

    QString m_subscription;
    QString m_lastDirectory;
    QString m_loadingMessages;
    Song m_song;
    Ui::GUIPlayerClass ui;
    QPointer<QProgressDialog> m_pd;
};

#endif // INCLUDED_SMFPLAYER_H