#include <QString>
#include <QDataStream>
#include <QByteArray>
#include <QtTest>
#include <drumstick/qsmf.h>

using namespace drumstick::File;

class FileTest : public QObject
{
    Q_OBJECT
public:
    explicit FileTest(QObject* parent = nullptr);

    static const char test_mid[];
    static const int test_mid_len;
    static const int FORMAT;
    static const int TRACKS;
    static const int DIVISION;
    static const int TEMPO;
    static const QString COPYRIGHT;
    static const QByteArray GSRESET;
    static const QList<int> NOTES;

public Q_SLOTS:
    void errorHandler(const QString& errorStr);
    void trackHandler(int track);
    void headerEvent(int format, int ntrks, int division);
    void trackStartEvent();
    void trackEndEvent();
    void endOfTrackEvent();
    void noteOnEvent(int chan, int pitch, int vol);
    void noteOffEvent(int chan, int pitch, int vol);
    void keyPressEvent(int chan, int pitch, int press);
    void ctlChangeEvent(int chan, int ctl, int value);
    void pitchBendEvent(int chan, int value);
    void programEvent(int chan, int patch);
    void chanPressEvent(int chan, int press);
    void sysexEvent(const QByteArray& data);
    void textEvent(int typ, const QString& data);
    void timeSigEvent(int b0, int b1, int b2, int b3);
    void keySigEvent(int b0, int b1);
    void tempoEvent(int tempo);

private Q_SLOTS:
    void testCaseWriteSmf();
    void testCaseReadSmf();
    void initTestCase();
    void cleanupTestCase();

private:
    QSmf *m_engine;
    int m_numNoteOn;
    int m_lastNoteOn;
    int m_numNoteOff;
    int m_lastNoteOff;
    int m_lastKeyPress;
    int m_currentTrack;
    int m_endOfTrack;
    int m_lastCtl;
    int m_lastProgram;
    int m_lastChanPress;
    int m_lastPitchBend;
    int m_lastTempo;
    QByteArray m_testData;
    QByteArray m_lastSysex;
    QString m_lastError;
    QString m_header;
    QString m_trackEnd;
    QString m_lastTextEvent;
    QString m_lastTimeSig;
    QString m_lastKeySig;
};

FileTest::FileTest(QObject* parent): QObject(parent),
    m_engine(nullptr),
    m_numNoteOn(0),
    m_lastNoteOn(0),
    m_numNoteOff(0),
    m_lastNoteOff(0),
    m_lastKeyPress(0),
    m_currentTrack(0),
    m_endOfTrack(0),
    m_lastCtl(0),
    m_lastProgram(0),
    m_lastChanPress(0),
    m_lastPitchBend(0),
    m_lastTempo(0)
{
    m_engine = new QSmf(this);
    m_engine->setTextCodec(QTextCodec::codecForName("UTF-8"));

    connect(m_engine, &QSmf::signalSMFError, this, &FileTest::errorHandler);
    connect(m_engine, &QSmf::signalSMFWriteTrack, this, &FileTest::trackHandler);

    connect(m_engine, &QSmf::signalSMFHeader, this, &FileTest::headerEvent);
    connect(m_engine, &QSmf::signalSMFTrackStart, this, &FileTest::trackStartEvent);
    connect(m_engine, &QSmf::signalSMFTrackEnd, this, &FileTest::trackEndEvent);
    connect(m_engine, &QSmf::signalSMFNoteOn, this, &FileTest::noteOnEvent);
    connect(m_engine, &QSmf::signalSMFNoteOff, this, &FileTest::noteOffEvent);
    connect(m_engine, &QSmf::signalSMFKeyPress, this, &FileTest::keyPressEvent);
    connect(m_engine, &QSmf::signalSMFCtlChange, this, &FileTest::ctlChangeEvent);
    connect(m_engine, &QSmf::signalSMFPitchBend, this, &FileTest::pitchBendEvent);
    connect(m_engine, &QSmf::signalSMFProgram, this, &FileTest::programEvent);
    connect(m_engine, &QSmf::signalSMFChanPress, this, &FileTest::chanPressEvent);
    connect(m_engine, &QSmf::signalSMFSysex, this, &FileTest::sysexEvent);
    connect(m_engine, &QSmf::signalSMFText, this, &FileTest::textEvent);
    connect(m_engine, &QSmf::signalSMFendOfTrack, this, &FileTest::endOfTrackEvent);
    connect(m_engine, &QSmf::signalSMFTimeSig, this, &FileTest::timeSigEvent);
    connect(m_engine, &QSmf::signalSMFKeySig, this, &FileTest::keySigEvent);
    connect(m_engine, &QSmf::signalSMFTempo, this, &FileTest::tempoEvent);
}

const char FileTest::test_mid[] = {
  '\x4d','\x54','\x68','\x64','\x00','\x00','\x00','\x06','\x00','\x00','\x00','\x01',
  '\x00','\x78','\x4d','\x54','\x72','\x6b','\x00','\x00','\x00','\x99','\x00','\xff',
  '\x02','\x2f','\x43','\x6f','\x70','\x79','\x72','\x69','\x67','\x68','\x74','\x20',
  '\x28','\x43','\x29','\x20','\x32','\x30','\x30','\x36','\x2d','\x32','\x30','\x32',
  '\x30','\x20','\x50','\x65','\x64','\x72','\x6f','\x20','\x4c','\xc3','\xb3','\x70',
  '\x65','\x7a','\x2d','\x43','\x61','\x62','\x61','\x6e','\x69','\x6c','\x6c','\x61',
  '\x73','\x00','\xff','\x51','\x03','\x09','\x27','\xc0','\x00','\xff','\x58','\x04',
  '\x03','\x02','\x24','\x08','\x00','\xff','\x59','\x02','\x02','\x00','\x00','\xf0',
  '\x0a','\x41','\x10','\x42','\x12','\x40','\x00','\x7f','\x00','\x41','\xf7','\x00',
  '\x90','\x3c','\x78','\x3c','\x80','\x3c','\x00','\x00','\x90','\x3e','\x78','\x3c',
  '\x80','\x3e','\x00','\x00','\x90','\x40','\x78','\x3c','\x80','\x40','\x00','\x00',
  '\x90','\x41','\x78','\x3c','\x80','\x41','\x00','\x00','\x90','\x43','\x78','\x3c',
  '\x80','\x43','\x00','\x00','\x90','\x45','\x78','\x3c','\x80','\x45','\x00','\x00',
  '\x90','\x47','\x78','\x3c','\x80','\x47','\x00','\x00','\x90','\x48','\x78','\x3c',
  '\x80','\x48','\x00','\x00','\xff','\x2f','\x00'
};
const int FileTest::test_mid_len = sizeof(test_mid); //175;
const QString FileTest::COPYRIGHT = QStringLiteral("Copyright (C) 2006-2020 Pedro López-Cabanillas");
const QByteArray FileTest::GSRESET = QByteArrayLiteral( "f04110421240007f0041f7" );
const QList<int> FileTest::NOTES = { 60, 62, 64, 65, 67, 69, 71, 72 };
const int FileTest::FORMAT = 0;
const int FileTest::TRACKS = 1;
const int FileTest::DIVISION = 120;
const int FileTest::TEMPO = 100;

void FileTest::errorHandler(const QString& errorStr)
{
    m_lastError = errorStr;
    qWarning() << errorStr;
}

void FileTest::trackHandler(int )
{
    int i;
    // Text event
    m_engine->writeMetaEvent(0, copyright_notice, COPYRIGHT);
    m_engine->writeBpmTempo(0, TEMPO);
    m_engine->writeTimeSignature(0, 3, 2, 36, 8);  // ts = 3/4
    m_engine->writeKeySignature(0, 2, major_mode); // D major (2 sharps)
    // system exclusive event
    QByteArray gsreset = QByteArray::fromHex( GSRESET );
    m_engine->writeMidiEvent(0, system_exclusive, long(gsreset.size()), gsreset.data());
    // some note events
    for(i = 0; i < NOTES.length(); ++i)
    {
        m_engine->writeMidiEvent(0,  note_on,  0, NOTES[i], 120);
        m_engine->writeMidiEvent(60, note_off, 0, NOTES[i], 0);
    }
    // final event
    m_engine->writeMetaEvent(0, end_of_track);
}

void FileTest::headerEvent(int format, int ntrks, int division)
{
    m_header = QString("Format=%1, Tracks=%2, Division=%3").arg(format).arg(ntrks).arg(division);
}

void FileTest::trackStartEvent()
{
    m_currentTrack++;
}

void FileTest::trackEndEvent()
{
    m_trackEnd = QString("End: %1").arg(m_currentTrack);
}

void FileTest::endOfTrackEvent()
{
    m_endOfTrack++;
}

void FileTest::noteOnEvent(int , int pitch, int )
{
    m_numNoteOn++;
    m_lastNoteOn = pitch;
}

void FileTest::noteOffEvent(int , int pitch, int )
{
    m_numNoteOff++;
    m_lastNoteOff = pitch;
}

void FileTest::keyPressEvent(int , int pitch, int )
{
    m_lastKeyPress = pitch;
}

void FileTest::ctlChangeEvent(int , int ctl, int )
{
    m_lastCtl = ctl;
}

void FileTest::pitchBendEvent(int , int value)
{
    m_lastPitchBend = value;
}

void FileTest::programEvent(int , int patch)
{
    m_lastProgram = patch;
}

void FileTest::chanPressEvent(int , int press)
{
    m_lastChanPress = press;
}

void FileTest::sysexEvent(const QByteArray& data)
{
    m_lastSysex = data;
}

void FileTest::textEvent(int , const QString& data)
{
    m_lastTextEvent = data;
}

void FileTest::timeSigEvent(int b0, int b1, int b2, int b3)
{
    m_lastTimeSig = QString("%1, %2, %3, %4").arg(b0).arg(b1).arg(b2).arg(b3);
}

void FileTest::keySigEvent(int b0, int b1)
{
    m_lastKeySig = QString("%1, %2").arg(b0).arg(b1);
}

void FileTest::tempoEvent(int tempo)
{
    m_lastTempo = static_cast<int>( 6e7 / tempo );
}

void FileTest::initTestCase()
{
    m_testData = QByteArray::fromRawData(test_mid, test_mid_len);
}

void FileTest::cleanupTestCase()
{
    m_testData.clear();
}

void FileTest::testCaseWriteSmf()
{
    QByteArray data;
    QDataStream stream(&data,  QIODevice::ReadWrite);
    m_engine->setDivision(DIVISION);
    m_engine->setFileFormat(FORMAT);
    m_engine->setTracks(TRACKS);
    m_engine->writeToStream(&stream);
    if (!m_lastError.isEmpty()) {
        QFAIL(m_lastError.toLocal8Bit());
    }
    QCOMPARE(data, m_testData);
}

void FileTest::testCaseReadSmf()
{
    QDataStream stream(&m_testData,  QIODevice::ReadWrite);
    m_engine->readFromStream(&stream);
    if (!m_lastError.isEmpty()) {
        QFAIL(m_lastError.toLocal8Bit());
    }
    QCOMPARE(m_engine->getFileFormat(), FORMAT);
    QCOMPARE(m_engine->getDivision(), DIVISION);
    QCOMPARE(m_engine->getTracks(), TRACKS);
    QCOMPARE(m_lastTempo, TEMPO);
    QCOMPARE(m_lastTextEvent, COPYRIGHT);
    QCOMPARE(m_lastSysex, QByteArray::fromHex(GSRESET));
    QCOMPARE(m_numNoteOn, NOTES.length());
    QCOMPARE(m_numNoteOff, NOTES.length());
    QCOMPARE(m_lastNoteOn, NOTES.last());
    QCOMPARE(m_lastNoteOff, NOTES.last());
    QCOMPARE(m_currentTrack, TRACKS);
    QCOMPARE(m_endOfTrack, TRACKS);
}

QTEST_APPLESS_MAIN(FileTest)

#include "filetest.moc"
