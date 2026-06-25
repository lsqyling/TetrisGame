/****************************************************************************
** Meta object code from reading C++ file 'AudioEngine.h'
**
** Created by: The Qt Meta Object Compiler version 68 (Qt 6.8.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../../src/AudioEngine.h"
#include <QtCore/qmetatype.h>

#include <QtCore/qtmochelpers.h>

#include <memory>


#include <QtCore/qxptype_traits.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'AudioEngine.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 68
#error "This file was generated using the moc from 6.8.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

#ifndef Q_CONSTINIT
#define Q_CONSTINIT
#endif

QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
QT_WARNING_DISABLE_GCC("-Wuseless-cast")
namespace {
struct qt_meta_tag_ZN11AudioEngineE_t {};
} // unnamed namespace


#ifdef QT_MOC_HAS_STRINGDATA
static constexpr auto qt_meta_stringdata_ZN11AudioEngineE = QtMocHelpers::stringData(
    "AudioEngine",
    "musicEnabledChanged",
    "",
    "sfxEnabledChanged",
    "setMusicEnabled",
    "enabled",
    "setSfxEnabled",
    "playMove",
    "playRotate",
    "playDrop",
    "playClear",
    "linesCleared",
    "playLevelUp",
    "playGameOver",
    "startMusic",
    "stopMusic",
    "pauseMusic",
    "resumeMusic",
    "toggleMusic",
    "musicEnabled",
    "sfxEnabled"
);
#else  // !QT_MOC_HAS_STRINGDATA
#error "qtmochelpers.h not found or too old."
#endif // !QT_MOC_HAS_STRINGDATA

Q_CONSTINIT static const uint qt_meta_data_ZN11AudioEngineE[] = {

 // content:
      12,       // revision
       0,       // classname
       0,    0, // classinfo
      15,   14, // methods
       2,  125, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       2,       // signalCount

 // signals: name, argc, parameters, tag, flags, initial metatype offsets
       1,    0,  104,    2, 0x06,    3 /* Public */,
       3,    0,  105,    2, 0x06,    4 /* Public */,

 // slots: name, argc, parameters, tag, flags, initial metatype offsets
       4,    1,  106,    2, 0x0a,    5 /* Public */,
       6,    1,  109,    2, 0x0a,    7 /* Public */,

 // methods: name, argc, parameters, tag, flags, initial metatype offsets
       7,    0,  112,    2, 0x02,    9 /* Public */,
       8,    0,  113,    2, 0x02,   10 /* Public */,
       9,    0,  114,    2, 0x02,   11 /* Public */,
      10,    1,  115,    2, 0x02,   12 /* Public */,
      12,    0,  118,    2, 0x02,   14 /* Public */,
      13,    0,  119,    2, 0x02,   15 /* Public */,
      14,    0,  120,    2, 0x02,   16 /* Public */,
      15,    0,  121,    2, 0x02,   17 /* Public */,
      16,    0,  122,    2, 0x02,   18 /* Public */,
      17,    0,  123,    2, 0x02,   19 /* Public */,
      18,    0,  124,    2, 0x02,   20 /* Public */,

 // signals: parameters
    QMetaType::Void,
    QMetaType::Void,

 // slots: parameters
    QMetaType::Void, QMetaType::Bool,    5,
    QMetaType::Void, QMetaType::Bool,    5,

 // methods: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int,   11,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

 // properties: name, type, flags, notifyId, revision
      19, QMetaType::Bool, 0x00015103, uint(0), 0,
      20, QMetaType::Bool, 0x00015103, uint(1), 0,

       0        // eod
};

Q_CONSTINIT const QMetaObject AudioEngine::staticMetaObject = { {
    QMetaObject::SuperData::link<QObject::staticMetaObject>(),
    qt_meta_stringdata_ZN11AudioEngineE.offsetsAndSizes,
    qt_meta_data_ZN11AudioEngineE,
    qt_static_metacall,
    nullptr,
    qt_incomplete_metaTypeArray<qt_meta_tag_ZN11AudioEngineE_t,
        // property 'musicEnabled'
        QtPrivate::TypeAndForceComplete<bool, std::true_type>,
        // property 'sfxEnabled'
        QtPrivate::TypeAndForceComplete<bool, std::true_type>,
        // Q_OBJECT / Q_GADGET
        QtPrivate::TypeAndForceComplete<AudioEngine, std::true_type>,
        // method 'musicEnabledChanged'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'sfxEnabledChanged'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'setMusicEnabled'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<bool, std::false_type>,
        // method 'setSfxEnabled'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<bool, std::false_type>,
        // method 'playMove'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'playRotate'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'playDrop'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'playClear'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<int, std::false_type>,
        // method 'playLevelUp'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'playGameOver'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'startMusic'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'stopMusic'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'pauseMusic'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'resumeMusic'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'toggleMusic'
        QtPrivate::TypeAndForceComplete<void, std::false_type>
    >,
    nullptr
} };

void AudioEngine::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    auto *_t = static_cast<AudioEngine *>(_o);
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: _t->musicEnabledChanged(); break;
        case 1: _t->sfxEnabledChanged(); break;
        case 2: _t->setMusicEnabled((*reinterpret_cast< std::add_pointer_t<bool>>(_a[1]))); break;
        case 3: _t->setSfxEnabled((*reinterpret_cast< std::add_pointer_t<bool>>(_a[1]))); break;
        case 4: _t->playMove(); break;
        case 5: _t->playRotate(); break;
        case 6: _t->playDrop(); break;
        case 7: _t->playClear((*reinterpret_cast< std::add_pointer_t<int>>(_a[1]))); break;
        case 8: _t->playLevelUp(); break;
        case 9: _t->playGameOver(); break;
        case 10: _t->startMusic(); break;
        case 11: _t->stopMusic(); break;
        case 12: _t->pauseMusic(); break;
        case 13: _t->resumeMusic(); break;
        case 14: _t->toggleMusic(); break;
        default: ;
        }
    }
    if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _q_method_type = void (AudioEngine::*)();
            if (_q_method_type _q_method = &AudioEngine::musicEnabledChanged; *reinterpret_cast<_q_method_type *>(_a[1]) == _q_method) {
                *result = 0;
                return;
            }
        }
        {
            using _q_method_type = void (AudioEngine::*)();
            if (_q_method_type _q_method = &AudioEngine::sfxEnabledChanged; *reinterpret_cast<_q_method_type *>(_a[1]) == _q_method) {
                *result = 1;
                return;
            }
        }
    }
    if (_c == QMetaObject::ReadProperty) {
        void *_v = _a[0];
        switch (_id) {
        case 0: *reinterpret_cast< bool*>(_v) = _t->musicEnabled(); break;
        case 1: *reinterpret_cast< bool*>(_v) = _t->sfxEnabled(); break;
        default: break;
        }
    }
    if (_c == QMetaObject::WriteProperty) {
        void *_v = _a[0];
        switch (_id) {
        case 0: _t->setMusicEnabled(*reinterpret_cast< bool*>(_v)); break;
        case 1: _t->setSfxEnabled(*reinterpret_cast< bool*>(_v)); break;
        default: break;
        }
    }
}

const QMetaObject *AudioEngine::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *AudioEngine::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_ZN11AudioEngineE.stringdata0))
        return static_cast<void*>(this);
    return QObject::qt_metacast(_clname);
}

int AudioEngine::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 15)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 15;
    }
    if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 15)
            *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType();
        _id -= 15;
    }
    if (_c == QMetaObject::ReadProperty || _c == QMetaObject::WriteProperty
            || _c == QMetaObject::ResetProperty || _c == QMetaObject::BindableProperty
            || _c == QMetaObject::RegisterPropertyMetaType) {
        qt_static_metacall(this, _c, _id, _a);
        _id -= 2;
    }
    return _id;
}

// SIGNAL 0
void AudioEngine::musicEnabledChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 0, nullptr);
}

// SIGNAL 1
void AudioEngine::sfxEnabledChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 1, nullptr);
}
QT_WARNING_POP
