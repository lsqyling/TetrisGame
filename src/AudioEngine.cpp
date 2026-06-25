#include "AudioEngine.h"

#include <QAudioSink>
#include <QMediaDevices>
#include <QAudioFormat>
#include <QBuffer>
#include <QTimer>
#include <cmath>
#include <cstring>

// ============================================================
//  音符频率表（平均律 A4=440Hz）/ Note frequency table
// ============================================================
const QHash<QString, double> AudioEngine::s_notes = {
    {"C3", 130.81}, {"Db3", 138.59}, {"D3", 146.83}, {"Eb3", 155.56},
    {"E3", 164.81}, {"F3", 174.61}, {"Gb3", 185.00}, {"G3", 196.00},
    {"Ab3", 207.65}, {"A3", 220.00}, {"Bb3", 233.08}, {"B3", 246.94},
    {"C4", 261.63}, {"Db4", 277.18}, {"D4", 293.66}, {"Eb4", 311.13},
    {"E4", 329.63}, {"F4", 349.23}, {"Gb4", 369.99}, {"G4", 392.00},
    {"Ab4", 415.30}, {"A4", 440.00}, {"Bb4", 466.16}, {"B4", 493.88},
    {"C5", 523.25}, {"Db5", 554.37}, {"D5", 587.33}, {"Eb5", 622.25},
    {"E5", 659.25}, {"F5", 698.46}, {"Gb5", 739.99}, {"G5", 783.99},
    {"Ab5", 830.61}, {"A5", 880.00}, {"Bb5", 932.33}, {"B5", 987.77},
    {"C6", 1046.50}, {"_R", 0.0}
};

// ============================================================
//  KOROBEINIKI（俄罗斯方块主题曲）/ Tetris theme
//  旋律 (8分音符步进) + 低音 (4分音符)
//  Melody (8th-note steps) + Bass (quarter-note beats)
// ============================================================
const QList<AudioEngine::Step> AudioEngine::s_melody = {
    // Part A (16)
    {"E5",0.5},{"_R",0.3},{"E5",0.5},{"Eb5",0.5},
    {"E5",0.5},{"_R",0.3},{"E5",0.5},{"Eb5",0.5},
    {"E5",0.5},{"B4",0.5},{"D5",0.5},{"C5",0.7},
    {"A4",0.7},{"C5",0.5},{"E4",0.5},{"A4",0.5},
    {"B4",0.5},{"C5",0.5},{"D5",0.5},{"E5",0.8},
    // Part A repeat (16)
    {"E5",0.5},{"_R",0.3},{"E5",0.5},{"Eb5",0.5},
    {"E5",0.5},{"_R",0.3},{"E5",0.5},{"Eb5",0.5},
    {"E5",0.5},{"B4",0.5},{"D5",0.5},{"C5",0.7},
    {"A4",0.7},{"C5",0.5},{"E4",0.5},{"A4",0.5},
    {"B4",0.5},{"C5",0.5},{"D5",0.5},{"E5",0.8},
    // Part B (18)
    {"_R",0.3},{"E5",0.5},{"E5",0.5},{"Eb5",0.5},
    {"E5",0.5},{"_R",0.3},{"E5",0.5},{"B4",0.5},
    {"D5",0.5},{"C5",0.8},{"A4",0.7},
    {"_R",0.3},{"A4",0.5},{"B4",0.5},{"C5",0.5},
    {"D5",0.5},{"E5",0.5},{"F5",0.5},
    {"E5",0.5},{"D5",0.5},{"C5",0.7},{"B4",0.3},
    {"C5",0.5},{"D5",0.8},{"E5",0.8},
    // Part A final (20)
    {"E5",0.5},{"_R",0.3},{"E5",0.5},{"Eb5",0.5},
    {"E5",0.5},{"_R",0.3},{"E5",0.5},{"Eb5",0.5},
    {"E5",0.5},{"B4",0.5},{"D5",0.5},{"C5",0.7},
    {"A4",0.7},{"C5",0.5},{"E4",0.5},{"A4",0.5},
    {"B4",0.5},{"C5",0.5},{"D5",0.5},{"E5",0.8},
    // Final cadence (12)
    {"E5",0.5},{"B4",0.5},{"C5",0.5},{"D5",0.5},
    {"B4",0.8},{"A4",0.8},
    {"E4",0.5},{"A4",0.5},{"B4",0.5},{"C5",0.5},
    {"D5",0.8},{"E5",1.0}
};

const QList<QString> AudioEngine::s_bass = {
    "A3","A3","E3","E3","A3","A3","E3","E3",
    "A3","A3","E3","E3","A3","A3","E3","E3",
    "F3","F3","C3","C3","D3","D3","E3","E3",
    "A3","A3","E3","E3","A3","A3",
    "E3","A3"
};

// ============================================================
//  构造 / Constructor
// ============================================================

AudioEngine::AudioEngine(QObject *parent)
    : QObject(parent)
{
    QAudioFormat format;
    format.setSampleRate(44100);
    format.setChannelCount(1);
    format.setSampleFormat(QAudioFormat::Int16);

    m_audioSink = new QAudioSink(QMediaDevices::defaultAudioOutput(), format, this);
    generateBGM();   // 预渲染整个 BGM 循环 / Pre-render entire loop
}

AudioEngine::~AudioEngine()
{
    stopMusic();
}

// ============================================================
//  波形生成 / Waveform generation
// ============================================================

QByteArray AudioEngine::generateSFX(const QList<double> &frequencies,
                                     double noteDuration, double volume)
{
    const int sampleRate = 44100;
    int totalSamples = static_cast<int>(sampleRate * noteDuration * frequencies.size());
    if (totalSamples <= 0) return {};

    QByteArray data;
    data.resize(totalSamples * sizeof(qint16));
    auto *ptr = reinterpret_cast<qint16*>(data.data());

    int samplesPerNote = static_cast<int>(sampleRate * noteDuration);
    double envelopeLen = qMin(0.005, noteDuration * 0.5);

    for (int n = 0; n < frequencies.size(); ++n) {
        double freq = frequencies[n];
        int offset = n * samplesPerNote;

        for (int i = 0; i < samplesPerNote; ++i) {
            double t = static_cast<double>(i) / sampleRate;
            double value = 0.0;

            if (freq > 0) {
                // 方波 + 正弦混合 = 芯片音色 / Square + sine = chiptune
                double phase = fmod(t * freq, 1.0);
                value = (phase < 0.5) ? 1.0 : -1.0;
                value = value * 0.7 + sin(2.0 * M_PI * freq * t) * 0.3;
            }

            // 包络（淡入淡出）/ Envelope (fade in/out)
            double envelope = 1.0;
            if (t < envelopeLen)
                envelope = t / envelopeLen;
            if (t > noteDuration - envelopeLen)
                envelope = (noteDuration - t) / envelopeLen;

            ptr[offset + i] = static_cast<qint16>(value * volume * 32767.0 * envelope);
        }
    }

    return data;
}

// ============================================================
//  BGM 生成 — 预渲染 Korobeiniki 循环到缓冲区
//  BGM generation — render entire loop to buffer
// ============================================================

void AudioEngine::generateBGM()
{
    const int sampleRate = 44100;
    const double bpm = 130.0;
    const double stepSec = 60.0 / bpm / 2.0;     // 一个 8 分音符 / one 8th-note
    const double bassStepSec = stepSec * 4;       // 一个 4 分音符 / one quarter-note

    int totalMelodySamples = static_cast<int>(sampleRate * stepSec * s_melody.size());
    int totalBassSamples = static_cast<int>(sampleRate * bassStepSec * s_bass.size());
    int totalSamples = qMax(totalMelodySamples, totalBassSamples);

    if (totalSamples <= 0) return;

    m_bgmBuffer.resize(totalSamples * sizeof(qint16));
    auto *buffer = reinterpret_cast<qint16*>(m_bgmBuffer.data());
    memset(buffer, 0, m_bgmBuffer.size());   // 初始化为静音 / init silence

    // 渲染旋律（方波）/ Render melody (square wave)
    for (int i = 0; i < s_melody.size(); ++i) {
        double freq = s_notes.value(s_melody[i].note, 0.0);
        double dur = stepSec * s_melody[i].sustain;
        double vol = 0.055;
        int startSample = static_cast<int>(i * stepSec * sampleRate);
        int numSamples = static_cast<int>(dur * sampleRate);

        for (int s = 0; s < numSamples && (startSample + s) < totalSamples; ++s) {
            double t = static_cast<double>(s) / sampleRate;
            double value = 0.0;
            if (freq > 0) {
                double phase = fmod(t * freq, 1.0);
                value = (phase < 0.5) ? 1.0 : -1.0;
                value = value * 0.7 + sin(2.0 * M_PI * freq * t) * 0.3;
            }
            double envelope = 1.0;
            double envLen = qMin(0.003, dur * 0.5);
            if (t < envLen) envelope = t / envLen;
            if (t > dur - envLen) envelope = (dur - t) / envLen;

            buffer[startSample + s] +=
                static_cast<qint16>(value * vol * 32767.0 * envelope);
        }
    }

    // 渲染低音（三角波）/ Render bass (triangle wave)
    for (int i = 0; i < s_bass.size(); ++i) {
        double freq = s_notes.value(s_bass[i], 0.0);
        double dur = bassStepSec * 0.9;
        double vol = 0.09;
        int startSample = static_cast<int>(i * bassStepSec * sampleRate);
        int numSamples = static_cast<int>(dur * sampleRate);

        for (int s = 0; s < numSamples && (startSample + s) < totalSamples; ++s) {
            double t = static_cast<double>(s) / sampleRate;
            double value = 0.0;
            if (freq > 0) {
                double phase = fmod(t * freq, 1.0);
                value = 4.0 * qAbs(phase - 0.5) - 1.0;
            }
            double envelope = 1.0;
            double envLen = qMin(0.005, dur * 0.5);
            if (t < envLen) envelope = t / envLen;
            if (t > dur - envLen) envelope = (dur - t) / envLen;

            buffer[startSample + s] +=
                static_cast<qint16>(value * vol * 32767.0 * envelope);
        }
    }
}

// ============================================================
//  播放 / Playback
// ============================================================

void AudioEngine::playBuffer(const QByteArray &buffer)
{
    if (buffer.isEmpty() || !m_sfxEnabled) return;

    // 记录 BGM 是否正在活跃播放（非暂停）/ Remember if actively playing
    bool musicWasActive = m_musicPlaying && !m_musicPaused
                          && (m_audioSink->state() != QAudio::StoppedState);

    // 停止当前播放（可能是 BGM 或前一个 SFX）
    // Stop current playback (could be BGM or previous SFX)
    if (m_audioSink->state() != QAudio::StoppedState)
        m_audioSink->stop();

    // 播放 SFX 缓冲区 / Play the SFX buffer
    auto *source = new QBuffer(this);
    source->setData(buffer);
    source->open(QIODevice::ReadOnly);
    source->moveToThread(thread());

    m_audioSink->start(source);

    // SFX 结束后清理，若 BGM 之前活跃则恢复
    // Clean up when done; resume BGM if it was active
    connect(m_audioSink, &QAudioSink::stateChanged, this,
            [this, source, musicWasActive](QAudio::State state) {
                if (state == QAudio::IdleState || state == QAudio::StoppedState) {
                    source->deleteLater();
                    if (musicWasActive && m_musicPlaying && !m_musicPaused) {
                        // 延迟恢复以处理快速连续 SFX
                        // Delay to handle rapid-fire SFX
                        QTimer::singleShot(50, this, [this]() {
                            if (m_musicPlaying && !m_musicPaused)
                                startBGMPlayback();
                        });
                    }
                }
            }, Qt::SingleShotConnection);
}

// ============================================================
//  音效 / Sound Effects
// ============================================================

void AudioEngine::playMove()
{
    if (!m_sfxEnabled) return;
    playBuffer(generateSFX({220.0}, 0.06, 0.035));
}

void AudioEngine::playRotate()
{
    if (!m_sfxEnabled) return;
    playBuffer(generateSFX({330.0, 440.0}, 0.07, 0.04));
}

void AudioEngine::playDrop()
{
    if (!m_sfxEnabled) return;
    playBuffer(generateSFX({130.81, 261.63}, 0.12, 0.065));
}

void AudioEngine::playClear(int linesCleared)
{
    if (!m_sfxEnabled) return;
    if (linesCleared >= 4) {
        // Tetris 庆祝音效 / Tetris fanfare
        QByteArray buf;
        buf.append(generateSFX({523.25, 659.25, 783.99, 1046.50, 783.99, 1046.50}, 0.08, 0.07));
        playBuffer(buf);
    } else {
        // 普通消行音效（上行音符）/ Ascending notes
        QByteArray buf;
        buf.append(generateSFX({523.25, 659.25, 783.99, 1046.50}, 0.08, 0.06));
        playBuffer(buf);
    }
}

void AudioEngine::playLevelUp()
{
    if (!m_sfxEnabled) return;
    QByteArray buf;
    buf.append(generateSFX({440.0, 550.0, 660.0, 770.0, 880.0}, 0.09, 0.07));
    playBuffer(buf);
}

void AudioEngine::playGameOver()
{
    if (!m_sfxEnabled) return;
    QByteArray buf;
    buf.append(generateSFX({200.0, 160.0, 120.0}, 0.3, 0.07));
    playBuffer(buf);
}

// ============================================================
//  背景音乐 / Background Music
// ============================================================

void AudioEngine::startMusic()
{
    if (!m_musicEnabled) return;
    // 已播放且非暂停则忽略 / Skip if already playing (not paused)
    if (m_musicPlaying && !m_musicPaused) return;
    m_musicPlaying = true;
    m_musicPaused  = false;
    startBGMPlayback();
}

void AudioEngine::stopMusic()
{
    m_musicPlaying = false;
    m_musicPaused  = false;
    if (m_audioSink->state() != QAudio::StoppedState)
        m_audioSink->stop();
}

void AudioEngine::pauseMusic()
{
    if (!m_musicPlaying || m_musicPaused) return;
    m_musicPaused = true;
    auto state = m_audioSink->state();
    if (state == QAudio::ActiveState || state == QAudio::IdleState)
        m_audioSink->suspend();    // 保留播放位置 / preserve position
}

void AudioEngine::resumeMusic()
{
    if (!m_musicPaused) return;
    m_musicPaused = false;
    if (m_audioSink->state() == QAudio::SuspendedState) {
        m_audioSink->resume();     // 从断点续播 / resume from position
    } else {
        // SFX 在暂停期间打断了 BGM，回退重新开始
        // BGM was fully stopped (e.g. SFX), fallback to restart
        if (m_musicPlaying)
            startBGMPlayback();
    }
}

void AudioEngine::toggleMusic()
{
    if (m_musicPlaying)
        stopMusic();
    else
        startMusic();
}

void AudioEngine::startBGMPlayback()
{
    if (m_bgmBuffer.isEmpty()) return;

    auto *source = new QBuffer(this);
    source->setData(m_bgmBuffer);
    source->open(QIODevice::ReadOnly);

    m_audioSink->start(source);

    // 循环 BGM；若被打断则清理 / Loop BGM; clean up if interrupted
    connect(m_audioSink, &QAudioSink::stateChanged, this,
            [this, source](QAudio::State state) {
                if (!m_musicPlaying) {
                    source->deleteLater();
                    return;
                }
                if (state == QAudio::IdleState) {
                    // 自然结束——循环 / Natural finish — loop
                    source->deleteLater();
                    if (m_musicPlaying)
                        startBGMPlayback();
                } else if (state == QAudio::StoppedState) {
                    // 被 SFX 打断——清理，playBuffer 将恢复
                    // Interrupted by SFX — clean up, playBuffer will resume
                    source->deleteLater();
                }
            }, Qt::SingleShotConnection);
}

// ============================================================
//  属性设置器 / Property setters
// ============================================================

void AudioEngine::setMusicEnabled(bool enabled)
{
    if (m_musicEnabled == enabled) return;
    m_musicEnabled = enabled;
    if (!enabled) stopMusic();
    emit musicEnabledChanged();
}

void AudioEngine::setSfxEnabled(bool enabled)
{
    if (m_sfxEnabled == enabled) return;
    m_sfxEnabled = enabled;
    emit sfxEnabledChanged();
}
