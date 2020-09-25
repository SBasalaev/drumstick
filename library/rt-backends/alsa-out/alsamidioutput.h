/*
    Drumstick RT Backend using the ALSA Sequencer
    Copyright (C) 2009-2020 Pedro Lopez-Cabanillas <plcl@users.sf.net>

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program. If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef ALSAMIDIOUTPUT_H
#define ALSAMIDIOUTPUT_H

#include <QObject>
#include <drumstick/rtmidioutput.h>

namespace drumstick {
namespace rt {

    class ALSAMIDIOutput : public MIDIOutput
    {
        Q_OBJECT
        Q_PLUGIN_METADATA(IID "net.sourceforge.drumstick.rt.MIDIOutput/2.0")
        Q_INTERFACES(drumstick::rt::MIDIOutput)
    public:
        ALSAMIDIOutput(QObject *parent = 0);
        virtual ~ALSAMIDIOutput();

        virtual void initialize(QSettings* settings);
        virtual QString backendName();
        virtual QString publicName();
        virtual void setPublicName(QString name);
        virtual QList<MIDIConnection> connections(bool advanced);
        virtual void setExcludedConnections(QStringList conns);
        virtual void open(const MIDIConnection& name);
        virtual void close();
        virtual MIDIConnection currentConnection();

    public slots:
        virtual void sendNoteOn(int chan, int note, int vel);
        virtual void sendNoteOff(int chan, int note, int vel);
        virtual void sendController(int chan, int control, int value);
        virtual void sendKeyPressure(int chan, int note, int value);
        virtual void sendProgram(int chan, int program);
        virtual void sendChannelPressure(int chan, int value);
        virtual void sendPitchBend(int chan, int value);
        virtual void sendSysex(const QByteArray& data);
        virtual void sendSystemMsg(const int status);

    private:
        class ALSAMIDIOutputPrivate;
        ALSAMIDIOutputPrivate *d;
    };

}}

#endif /* ALSAMIDIOUTPUT_H */
