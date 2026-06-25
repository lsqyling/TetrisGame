/****************************************************************************
** Meta object code from reading C++ file 'GameEngine.h'
**
** Created by: The Qt Meta Object Compiler version 68 (Qt 6.8.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../../src/GameEngine.h"
#include <QtCore/qmetatype.h>

#include <QtCore/qtmochelpers.h>

#include <memory>


#include <QtCore/qxptype_traits.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'GameEngine.h' doesn't include <QObject>."
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
struct qt_meta_tag_ZN10GameEngineE_t {};
} // unnamed namespace


#ifdef QT_MOC_HAS_STRINGDATA
static constexpr auto qt_meta_stringdata_ZN10GameEngineE = QtMocHelpers::stringData(
    "GameEngine",
    "stateChanged",
    "",
    "linesCleared",
    "count",
    "tick",
    "startGame",
    "restartGame",
    "moveLeft",
    "moveRight",
    "moveDown",
    "rotatePiece",
    "hardDrop",
    "togglePause",
    "cellColor",
    "row",
    "col",
    "ghostVisible",
    "ghostColor",
    "pieceColor",
    "type",
    "pieceShape",
    "QVariantList",
    "boardCell",
    "currentPieceCell",
    "ghostCell",
    "score",
    "level",
    "lines",
    "gameRunning",
    "gameOver",
    "paused",
    "boardData",
    "nextPieceType",
    "currentPieceType",
    "currentPieceX",
    "currentPieceY",
    "ghostY",
    "currentPieceShape",
    "stateRevision",
    "cols",
    "rows",
    "blockSize",
    "pieceColors"
);
#else  // !QT_MOC_HAS_STRINGDATA
#error "qtmochelpers.h not found or too old."
#endif // !QT_MOC_HAS_STRINGDATA

Q_CONSTINIT static const uint qt_meta_data_ZN10GameEngineE[] = {

 // content:
      12,       // revision
       0,       // classname
       0,    0, // classinfo
      19,   14, // methods
      18,  173, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       2,       // signalCount

 // signals: name, argc, parameters, tag, flags, initial metatype offsets
       1,    0,  128,    2, 0x06,   19 /* Public */,
       3,    1,  129,    2, 0x06,   20 /* Public */,

 // slots: name, argc, parameters, tag, flags, initial metatype offsets
       5,    0,  132,    2, 0x08,   22 /* Private */,

 // methods: name, argc, parameters, tag, flags, initial metatype offsets
       6,    0,  133,    2, 0x02,   23 /* Public */,
       7,    0,  134,    2, 0x02,   24 /* Public */,
       8,    0,  135,    2, 0x02,   25 /* Public */,
       9,    0,  136,    2, 0x02,   26 /* Public */,
      10,    0,  137,    2, 0x02,   27 /* Public */,
      11,    0,  138,    2, 0x02,   28 /* Public */,
      12,    0,  139,    2, 0x02,   29 /* Public */,
      13,    0,  140,    2, 0x02,   30 /* Public */,
      14,    2,  141,    2, 0x102,   31 /* Public | MethodIsConst  */,
      17,    2,  146,    2, 0x102,   34 /* Public | MethodIsConst  */,
      18,    0,  151,    2, 0x102,   37 /* Public | MethodIsConst  */,
      19,    1,  152,    2, 0x102,   38 /* Public | MethodIsConst  */,
      21,    1,  155,    2, 0x102,   40 /* Public | MethodIsConst  */,
      23,    2,  158,    2, 0x102,   42 /* Public | MethodIsConst  */,
      24,    2,  163,    2, 0x102,   45 /* Public | MethodIsConst  */,
      25,    2,  168,    2, 0x102,   48 /* Public | MethodIsConst  */,

 // signals: parameters
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int,    4,

 // slots: parameters
    QMetaType::Void,

 // methods: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::QString, QMetaType::Int, QMetaType::Int,   15,   16,
    QMetaType::Bool, QMetaType::Int, QMetaType::Int,   15,   16,
    QMetaType::QString,
    QMetaType::QString, QMetaType::Int,   20,
    0x80000000 | 22, QMetaType::Int,   20,
    QMetaType::Int, QMetaType::Int, QMetaType::Int,   15,   16,
    QMetaType::Int, QMetaType::Int, QMetaType::Int,   15,   16,
    QMetaType::Int, QMetaType::Int, QMetaType::Int,   15,   16,

 // properties: name, type, flags, notifyId, revision
      26, QMetaType::Int, 0x00015001, uint(0), 0,
      27, QMetaType::Int, 0x00015001, uint(0), 0,
      28, QMetaType::Int, 0x00015001, uint(0), 0,
      29, QMetaType::Bool, 0x00015001, uint(0), 0,
      30, QMetaType::Bool, 0x00015001, uint(0), 0,
      31, QMetaType::Bool, 0x00015001, uint(0), 0,
      32, 0x80000000 | 22, 0x00015009, uint(0), 0,
      33, QMetaType::Int, 0x00015001, uint(0), 0,
      34, QMetaType::Int, 0x00015001, uint(0), 0,
      35, QMetaType::Int, 0x00015001, uint(0), 0,
      36, QMetaType::Int, 0x00015001, uint(0), 0,
      37, QMetaType::Int, 0x00015001, uint(0), 0,
      38, 0x80000000 | 22, 0x00015009, uint(0), 0,
      39, QMetaType::Int, 0x00015001, uint(0), 0,
      40, QMetaType::Int, 0x00015401, uint(-1), 0,
      41, QMetaType::Int, 0x00015401, uint(-1), 0,
      42, QMetaType::Int, 0x00015401, uint(-1), 0,
      43, QMetaType::QStringList, 0x00015401, uint(-1), 0,

       0        // eod
};

Q_CONSTINIT const QMetaObject GameEngine::staticMetaObject = { {
    QMetaObject::SuperData::link<QObject::staticMetaObject>(),
    qt_meta_stringdata_ZN10GameEngineE.offsetsAndSizes,
    qt_meta_data_ZN10GameEngineE,
    qt_static_metacall,
    nullptr,
    qt_incomplete_metaTypeArray<qt_meta_tag_ZN10GameEngineE_t,
        // property 'score'
        QtPrivate::TypeAndForceComplete<int, std::true_type>,
        // property 'level'
        QtPrivate::TypeAndForceComplete<int, std::true_type>,
        // property 'lines'
        QtPrivate::TypeAndForceComplete<int, std::true_type>,
        // property 'gameRunning'
        QtPrivate::TypeAndForceComplete<bool, std::true_type>,
        // property 'gameOver'
        QtPrivate::TypeAndForceComplete<bool, std::true_type>,
        // property 'paused'
        QtPrivate::TypeAndForceComplete<bool, std::true_type>,
        // property 'boardData'
        QtPrivate::TypeAndForceComplete<QVariantList, std::true_type>,
        // property 'nextPieceType'
        QtPrivate::TypeAndForceComplete<int, std::true_type>,
        // property 'currentPieceType'
        QtPrivate::TypeAndForceComplete<int, std::true_type>,
        // property 'currentPieceX'
        QtPrivate::TypeAndForceComplete<int, std::true_type>,
        // property 'currentPieceY'
        QtPrivate::TypeAndForceComplete<int, std::true_type>,
        // property 'ghostY'
        QtPrivate::TypeAndForceComplete<int, std::true_type>,
        // property 'currentPieceShape'
        QtPrivate::TypeAndForceComplete<QVariantList, std::true_type>,
        // property 'stateRevision'
        QtPrivate::TypeAndForceComplete<int, std::true_type>,
        // property 'cols'
        QtPrivate::TypeAndForceComplete<int, std::true_type>,
        // property 'rows'
        QtPrivate::TypeAndForceComplete<int, std::true_type>,
        // property 'blockSize'
        QtPrivate::TypeAndForceComplete<int, std::true_type>,
        // property 'pieceColors'
        QtPrivate::TypeAndForceComplete<QStringList, std::true_type>,
        // Q_OBJECT / Q_GADGET
        QtPrivate::TypeAndForceComplete<GameEngine, std::true_type>,
        // method 'stateChanged'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'linesCleared'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<int, std::false_type>,
        // method 'tick'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'startGame'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'restartGame'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'moveLeft'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'moveRight'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'moveDown'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'rotatePiece'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'hardDrop'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'togglePause'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'cellColor'
        QtPrivate::TypeAndForceComplete<QString, std::false_type>,
        QtPrivate::TypeAndForceComplete<int, std::false_type>,
        QtPrivate::TypeAndForceComplete<int, std::false_type>,
        // method 'ghostVisible'
        QtPrivate::TypeAndForceComplete<bool, std::false_type>,
        QtPrivate::TypeAndForceComplete<int, std::false_type>,
        QtPrivate::TypeAndForceComplete<int, std::false_type>,
        // method 'ghostColor'
        QtPrivate::TypeAndForceComplete<QString, std::false_type>,
        // method 'pieceColor'
        QtPrivate::TypeAndForceComplete<QString, std::false_type>,
        QtPrivate::TypeAndForceComplete<int, std::false_type>,
        // method 'pieceShape'
        QtPrivate::TypeAndForceComplete<QVariantList, std::false_type>,
        QtPrivate::TypeAndForceComplete<int, std::false_type>,
        // method 'boardCell'
        QtPrivate::TypeAndForceComplete<int, std::false_type>,
        QtPrivate::TypeAndForceComplete<int, std::false_type>,
        QtPrivate::TypeAndForceComplete<int, std::false_type>,
        // method 'currentPieceCell'
        QtPrivate::TypeAndForceComplete<int, std::false_type>,
        QtPrivate::TypeAndForceComplete<int, std::false_type>,
        QtPrivate::TypeAndForceComplete<int, std::false_type>,
        // method 'ghostCell'
        QtPrivate::TypeAndForceComplete<int, std::false_type>,
        QtPrivate::TypeAndForceComplete<int, std::false_type>,
        QtPrivate::TypeAndForceComplete<int, std::false_type>
    >,
    nullptr
} };

void GameEngine::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    auto *_t = static_cast<GameEngine *>(_o);
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: _t->stateChanged(); break;
        case 1: _t->linesCleared((*reinterpret_cast< std::add_pointer_t<int>>(_a[1]))); break;
        case 2: _t->tick(); break;
        case 3: _t->startGame(); break;
        case 4: _t->restartGame(); break;
        case 5: _t->moveLeft(); break;
        case 6: _t->moveRight(); break;
        case 7: _t->moveDown(); break;
        case 8: _t->rotatePiece(); break;
        case 9: _t->hardDrop(); break;
        case 10: _t->togglePause(); break;
        case 11: { QString _r = _t->cellColor((*reinterpret_cast< std::add_pointer_t<int>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<int>>(_a[2])));
            if (_a[0]) *reinterpret_cast< QString*>(_a[0]) = std::move(_r); }  break;
        case 12: { bool _r = _t->ghostVisible((*reinterpret_cast< std::add_pointer_t<int>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<int>>(_a[2])));
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = std::move(_r); }  break;
        case 13: { QString _r = _t->ghostColor();
            if (_a[0]) *reinterpret_cast< QString*>(_a[0]) = std::move(_r); }  break;
        case 14: { QString _r = _t->pieceColor((*reinterpret_cast< std::add_pointer_t<int>>(_a[1])));
            if (_a[0]) *reinterpret_cast< QString*>(_a[0]) = std::move(_r); }  break;
        case 15: { QVariantList _r = _t->pieceShape((*reinterpret_cast< std::add_pointer_t<int>>(_a[1])));
            if (_a[0]) *reinterpret_cast< QVariantList*>(_a[0]) = std::move(_r); }  break;
        case 16: { int _r = _t->boardCell((*reinterpret_cast< std::add_pointer_t<int>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<int>>(_a[2])));
            if (_a[0]) *reinterpret_cast< int*>(_a[0]) = std::move(_r); }  break;
        case 17: { int _r = _t->currentPieceCell((*reinterpret_cast< std::add_pointer_t<int>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<int>>(_a[2])));
            if (_a[0]) *reinterpret_cast< int*>(_a[0]) = std::move(_r); }  break;
        case 18: { int _r = _t->ghostCell((*reinterpret_cast< std::add_pointer_t<int>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<int>>(_a[2])));
            if (_a[0]) *reinterpret_cast< int*>(_a[0]) = std::move(_r); }  break;
        default: ;
        }
    }
    if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _q_method_type = void (GameEngine::*)();
            if (_q_method_type _q_method = &GameEngine::stateChanged; *reinterpret_cast<_q_method_type *>(_a[1]) == _q_method) {
                *result = 0;
                return;
            }
        }
        {
            using _q_method_type = void (GameEngine::*)(int );
            if (_q_method_type _q_method = &GameEngine::linesCleared; *reinterpret_cast<_q_method_type *>(_a[1]) == _q_method) {
                *result = 1;
                return;
            }
        }
    }
    if (_c == QMetaObject::ReadProperty) {
        void *_v = _a[0];
        switch (_id) {
        case 0: *reinterpret_cast< int*>(_v) = _t->score(); break;
        case 1: *reinterpret_cast< int*>(_v) = _t->level(); break;
        case 2: *reinterpret_cast< int*>(_v) = _t->lines(); break;
        case 3: *reinterpret_cast< bool*>(_v) = _t->gameRunning(); break;
        case 4: *reinterpret_cast< bool*>(_v) = _t->gameOver(); break;
        case 5: *reinterpret_cast< bool*>(_v) = _t->paused(); break;
        case 6: *reinterpret_cast< QVariantList*>(_v) = _t->boardData(); break;
        case 7: *reinterpret_cast< int*>(_v) = _t->nextPieceType(); break;
        case 8: *reinterpret_cast< int*>(_v) = _t->currentPieceType(); break;
        case 9: *reinterpret_cast< int*>(_v) = _t->currentPieceX(); break;
        case 10: *reinterpret_cast< int*>(_v) = _t->currentPieceY(); break;
        case 11: *reinterpret_cast< int*>(_v) = _t->ghostY(); break;
        case 12: *reinterpret_cast< QVariantList*>(_v) = _t->currentPieceShape(); break;
        case 13: *reinterpret_cast< int*>(_v) = _t->stateRevision(); break;
        case 14: *reinterpret_cast< int*>(_v) = _t->cols(); break;
        case 15: *reinterpret_cast< int*>(_v) = _t->rows(); break;
        case 16: *reinterpret_cast< int*>(_v) = _t->blockSize(); break;
        case 17: *reinterpret_cast< QStringList*>(_v) = _t->pieceColors(); break;
        default: break;
        }
    }
}

const QMetaObject *GameEngine::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *GameEngine::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_ZN10GameEngineE.stringdata0))
        return static_cast<void*>(this);
    return QObject::qt_metacast(_clname);
}

int GameEngine::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 19)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 19;
    }
    if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 19)
            *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType();
        _id -= 19;
    }
    if (_c == QMetaObject::ReadProperty || _c == QMetaObject::WriteProperty
            || _c == QMetaObject::ResetProperty || _c == QMetaObject::BindableProperty
            || _c == QMetaObject::RegisterPropertyMetaType) {
        qt_static_metacall(this, _c, _id, _a);
        _id -= 18;
    }
    return _id;
}

// SIGNAL 0
void GameEngine::stateChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 0, nullptr);
}

// SIGNAL 1
void GameEngine::linesCleared(int _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}
QT_WARNING_POP
