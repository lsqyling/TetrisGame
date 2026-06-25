#ifndef AUDIOENGINE_H
#define AUDIOENGINE_H

#include <QObject>
#include <QByteArray>
#include <QHash>
#include <QTimer>

QT_BEGIN_NAMESPACE
class QAudioSink;
QT_END_NAMESPACE

// ============================================================
//  AudioEngine — 程序化芯片音效合成 / Chiptune synthesis
//
//  所有音频（音效 + BGM）通过 PCM 波形生成，使用 QAudioSink
//  播放，不依赖任何外部音频文件。
//  All audio (SFX + BGM) generated via PCM waveform synthesis
//  and played through QAudioSink. No external audio files.
// ============================================================
class AudioEngine : public QObject
{
    Q_OBJECT

    Q_PROPERTY(bool musicEnabled READ musicEnabled WRITE setMusicEnabled NOTIFY musicEnabledChanged)
    Q_PROPERTY(bool sfxEnabled   READ sfxEnabled   WRITE setSfxEnabled   NOTIFY sfxEnabledChanged)

public:
    explicit AudioEngine(QObject *parent = nullptr);
    ~AudioEngine() override;

    bool musicEnabled() const { return m_musicEnabled; }
    bool sfxEnabled()   const { return m_sfxEnabled; }

    // --- 音效 / Sound Effects ---
    Q_INVOKABLE void playMove();
    Q_INVOKABLE void playRotate();
    Q_INVOKABLE void playDrop();
    Q_INVOKABLE void playClear(int linesCleared);
    Q_INVOKABLE void playLevelUp();
    Q_INVOKABLE void playGameOver();

    // --- 背景音乐 / Background Music ---
    Q_INVOKABLE void startMusic();
    Q_INVOKABLE void stopMusic();
    Q_INVOKABLE void pauseMusic();    // 暂停（保留播放位置 / keep position）
    Q_INVOKABLE void resumeMusic();   // 继续（从断点续播 / resume from position）
    Q_INVOKABLE void toggleMusic();

public slots:
    void setMusicEnabled(bool enabled);
    void setSfxEnabled(bool enabled);

signals:
    void musicEnabledChanged();
    void sfxEnabledChanged();

private:
    // 播放缓冲区 / Play a PCM buffer
    void playBuffer(const QByteArray &buffer);
    // 生成音效波形 / Generate SFX waveform
    QByteArray generateSFX(const QList<double> &frequencies,
                           double noteDuration, double volume);
    // 生成 BGM 缓冲区 / Generate entire BGM loop buffer
    void generateBGM();
    // 开始 BGM 播放 / Begin BGM playback
    void startBGMPlayback();

    // --- 状态 / State ---
    QAudioSink  *m_audioSink   = nullptr;
    QByteArray   m_bgmBuffer;                // 预渲染的 BGM 循环 / Pre-rendered loop
    bool         m_musicEnabled = true;
    bool         m_sfxEnabled   = true;
    bool         m_musicPlaying = false;
    bool         m_musicPaused  = false;     // 暂停标志 / Paused flag

    // 音符频率表（平均律, A4=440Hz）/ Note frequency table (equal temperament)
    static const QHash<QString, double> s_notes;
    // Korobeiniki 旋律数据 / Korobeiniki melody data
    struct Step { QString note; double sustain; };
    static const QList<Step> s_melody;
    static const QList<QString> s_bass;
};

#endif // AUDIOENGINE_H
