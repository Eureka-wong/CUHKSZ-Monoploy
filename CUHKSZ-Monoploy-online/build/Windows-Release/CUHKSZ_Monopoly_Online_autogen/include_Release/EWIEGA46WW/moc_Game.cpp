/****************************************************************************
** Meta object code from reading C++ file 'Game.h'
**
** Created by: The Qt Meta Object Compiler version 69 (Qt 6.10.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../../../Game.h"
#include <QtCore/qmetatype.h>

#include <QtCore/qtmochelpers.h>

#include <memory>


#include <QtCore/qxptype_traits.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'Game.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 69
#error "This file was generated using the moc from 6.10.1. It"
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
struct qt_meta_tag_ZN4GameE_t {};
} // unnamed namespace

template <> constexpr inline auto Game::qt_create_metaobjectdata<qt_meta_tag_ZN4GameE_t>()
{
    namespace QMC = QtMocConstants;
    QtMocHelpers::StringRefStorage qt_stringData {
        "Game",
        "gameStarted",
        "",
        "gameStateChanged",
        "GameState",
        "newState",
        "gameOver",
        "winnerPlayerIndex",
        "playerTurnStarted",
        "playerIndex",
        "playerTurnEnded",
        "diceRolled",
        "diceValue",
        "playerMoved",
        "oldPosition",
        "newPosition",
        "playerSentToJail",
        "playerReleasedFromJail",
        "tileEventTriggered",
        "tileIndex",
        "eventMessage",
        "purchaseOpportunity",
        "propertyName",
        "price",
        "rentPaymentRequired",
        "rent",
        "fromPlayer",
        "toPlayer",
        "rentPaid",
        "rentAmount",
        "gameLogMessage",
        "message",
        "forceRaiseMoney",
        "amount",
        "playerBankrupt",
        "tradeProposed",
        "propertyIndex",
        "warningSignal"
    };

    QtMocHelpers::UintData qt_methods {
        // Signal 'gameStarted'
        QtMocHelpers::SignalData<void()>(1, 2, QMC::AccessPublic, QMetaType::Void),
        // Signal 'gameStateChanged'
        QtMocHelpers::SignalData<void(GameState)>(3, 2, QMC::AccessPublic, QMetaType::Void, {{
            { 0x80000000 | 4, 5 },
        }}),
        // Signal 'gameOver'
        QtMocHelpers::SignalData<void(int)>(6, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::Int, 7 },
        }}),
        // Signal 'playerTurnStarted'
        QtMocHelpers::SignalData<void(int)>(8, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::Int, 9 },
        }}),
        // Signal 'playerTurnEnded'
        QtMocHelpers::SignalData<void(int)>(10, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::Int, 9 },
        }}),
        // Signal 'diceRolled'
        QtMocHelpers::SignalData<void(int, int)>(11, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::Int, 9 }, { QMetaType::Int, 12 },
        }}),
        // Signal 'playerMoved'
        QtMocHelpers::SignalData<void(int, int, int)>(13, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::Int, 9 }, { QMetaType::Int, 14 }, { QMetaType::Int, 15 },
        }}),
        // Signal 'playerSentToJail'
        QtMocHelpers::SignalData<void(int)>(16, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::Int, 9 },
        }}),
        // Signal 'playerReleasedFromJail'
        QtMocHelpers::SignalData<void(int)>(17, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::Int, 9 },
        }}),
        // Signal 'tileEventTriggered'
        QtMocHelpers::SignalData<void(int, const QString &)>(18, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::Int, 19 }, { QMetaType::QString, 20 },
        }}),
        // Signal 'purchaseOpportunity'
        QtMocHelpers::SignalData<void(const QString &, int, int)>(21, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::QString, 22 }, { QMetaType::Int, 23 }, { QMetaType::Int, 9 },
        }}),
        // Signal 'rentPaymentRequired'
        QtMocHelpers::SignalData<void(const QString &, int, int, int)>(24, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::QString, 22 }, { QMetaType::Int, 25 }, { QMetaType::Int, 26 }, { QMetaType::Int, 27 },
        }}),
        // Signal 'rentPaid'
        QtMocHelpers::SignalData<void(const QString &, int, const QString &)>(28, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::QString, 26 }, { QMetaType::Int, 29 }, { QMetaType::QString, 27 },
        }}),
        // Signal 'gameLogMessage'
        QtMocHelpers::SignalData<void(const QString &)>(30, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::QString, 31 },
        }}),
        // Signal 'forceRaiseMoney'
        QtMocHelpers::SignalData<void(int, int)>(32, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::Int, 9 }, { QMetaType::Int, 33 },
        }}),
        // Signal 'playerBankrupt'
        QtMocHelpers::SignalData<void(int)>(34, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::Int, 9 },
        }}),
        // Signal 'tradeProposed'
        QtMocHelpers::SignalData<void(int, int, int, int)>(35, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::Int, 26 }, { QMetaType::Int, 27 }, { QMetaType::Int, 36 }, { QMetaType::Int, 33 },
        }}),
        // Signal 'warningSignal'
        QtMocHelpers::SignalData<void(const QString &)>(37, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::QString, 31 },
        }}),
    };
    QtMocHelpers::UintData qt_properties {
    };
    QtMocHelpers::UintData qt_enums {
    };
    return QtMocHelpers::metaObjectData<Game, qt_meta_tag_ZN4GameE_t>(QMC::MetaObjectFlag{}, qt_stringData,
            qt_methods, qt_properties, qt_enums);
}
Q_CONSTINIT const QMetaObject Game::staticMetaObject = { {
    QMetaObject::SuperData::link<QObject::staticMetaObject>(),
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN4GameE_t>.stringdata,
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN4GameE_t>.data,
    qt_static_metacall,
    nullptr,
    qt_staticMetaObjectRelocatingContent<qt_meta_tag_ZN4GameE_t>.metaTypes,
    nullptr
} };

void Game::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    auto *_t = static_cast<Game *>(_o);
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: _t->gameStarted(); break;
        case 1: _t->gameStateChanged((*reinterpret_cast<std::add_pointer_t<GameState>>(_a[1]))); break;
        case 2: _t->gameOver((*reinterpret_cast<std::add_pointer_t<int>>(_a[1]))); break;
        case 3: _t->playerTurnStarted((*reinterpret_cast<std::add_pointer_t<int>>(_a[1]))); break;
        case 4: _t->playerTurnEnded((*reinterpret_cast<std::add_pointer_t<int>>(_a[1]))); break;
        case 5: _t->diceRolled((*reinterpret_cast<std::add_pointer_t<int>>(_a[1])),(*reinterpret_cast<std::add_pointer_t<int>>(_a[2]))); break;
        case 6: _t->playerMoved((*reinterpret_cast<std::add_pointer_t<int>>(_a[1])),(*reinterpret_cast<std::add_pointer_t<int>>(_a[2])),(*reinterpret_cast<std::add_pointer_t<int>>(_a[3]))); break;
        case 7: _t->playerSentToJail((*reinterpret_cast<std::add_pointer_t<int>>(_a[1]))); break;
        case 8: _t->playerReleasedFromJail((*reinterpret_cast<std::add_pointer_t<int>>(_a[1]))); break;
        case 9: _t->tileEventTriggered((*reinterpret_cast<std::add_pointer_t<int>>(_a[1])),(*reinterpret_cast<std::add_pointer_t<QString>>(_a[2]))); break;
        case 10: _t->purchaseOpportunity((*reinterpret_cast<std::add_pointer_t<QString>>(_a[1])),(*reinterpret_cast<std::add_pointer_t<int>>(_a[2])),(*reinterpret_cast<std::add_pointer_t<int>>(_a[3]))); break;
        case 11: _t->rentPaymentRequired((*reinterpret_cast<std::add_pointer_t<QString>>(_a[1])),(*reinterpret_cast<std::add_pointer_t<int>>(_a[2])),(*reinterpret_cast<std::add_pointer_t<int>>(_a[3])),(*reinterpret_cast<std::add_pointer_t<int>>(_a[4]))); break;
        case 12: _t->rentPaid((*reinterpret_cast<std::add_pointer_t<QString>>(_a[1])),(*reinterpret_cast<std::add_pointer_t<int>>(_a[2])),(*reinterpret_cast<std::add_pointer_t<QString>>(_a[3]))); break;
        case 13: _t->gameLogMessage((*reinterpret_cast<std::add_pointer_t<QString>>(_a[1]))); break;
        case 14: _t->forceRaiseMoney((*reinterpret_cast<std::add_pointer_t<int>>(_a[1])),(*reinterpret_cast<std::add_pointer_t<int>>(_a[2]))); break;
        case 15: _t->playerBankrupt((*reinterpret_cast<std::add_pointer_t<int>>(_a[1]))); break;
        case 16: _t->tradeProposed((*reinterpret_cast<std::add_pointer_t<int>>(_a[1])),(*reinterpret_cast<std::add_pointer_t<int>>(_a[2])),(*reinterpret_cast<std::add_pointer_t<int>>(_a[3])),(*reinterpret_cast<std::add_pointer_t<int>>(_a[4]))); break;
        case 17: _t->warningSignal((*reinterpret_cast<std::add_pointer_t<QString>>(_a[1]))); break;
        default: ;
        }
    }
    if (_c == QMetaObject::IndexOfMethod) {
        if (QtMocHelpers::indexOfMethod<void (Game::*)()>(_a, &Game::gameStarted, 0))
            return;
        if (QtMocHelpers::indexOfMethod<void (Game::*)(GameState )>(_a, &Game::gameStateChanged, 1))
            return;
        if (QtMocHelpers::indexOfMethod<void (Game::*)(int )>(_a, &Game::gameOver, 2))
            return;
        if (QtMocHelpers::indexOfMethod<void (Game::*)(int )>(_a, &Game::playerTurnStarted, 3))
            return;
        if (QtMocHelpers::indexOfMethod<void (Game::*)(int )>(_a, &Game::playerTurnEnded, 4))
            return;
        if (QtMocHelpers::indexOfMethod<void (Game::*)(int , int )>(_a, &Game::diceRolled, 5))
            return;
        if (QtMocHelpers::indexOfMethod<void (Game::*)(int , int , int )>(_a, &Game::playerMoved, 6))
            return;
        if (QtMocHelpers::indexOfMethod<void (Game::*)(int )>(_a, &Game::playerSentToJail, 7))
            return;
        if (QtMocHelpers::indexOfMethod<void (Game::*)(int )>(_a, &Game::playerReleasedFromJail, 8))
            return;
        if (QtMocHelpers::indexOfMethod<void (Game::*)(int , const QString & )>(_a, &Game::tileEventTriggered, 9))
            return;
        if (QtMocHelpers::indexOfMethod<void (Game::*)(const QString & , int , int )>(_a, &Game::purchaseOpportunity, 10))
            return;
        if (QtMocHelpers::indexOfMethod<void (Game::*)(const QString & , int , int , int )>(_a, &Game::rentPaymentRequired, 11))
            return;
        if (QtMocHelpers::indexOfMethod<void (Game::*)(const QString & , int , const QString & )>(_a, &Game::rentPaid, 12))
            return;
        if (QtMocHelpers::indexOfMethod<void (Game::*)(const QString & )>(_a, &Game::gameLogMessage, 13))
            return;
        if (QtMocHelpers::indexOfMethod<void (Game::*)(int , int )>(_a, &Game::forceRaiseMoney, 14))
            return;
        if (QtMocHelpers::indexOfMethod<void (Game::*)(int )>(_a, &Game::playerBankrupt, 15))
            return;
        if (QtMocHelpers::indexOfMethod<void (Game::*)(int , int , int , int )>(_a, &Game::tradeProposed, 16))
            return;
        if (QtMocHelpers::indexOfMethod<void (Game::*)(const QString & )>(_a, &Game::warningSignal, 17))
            return;
    }
}

const QMetaObject *Game::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *Game::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_staticMetaObjectStaticContent<qt_meta_tag_ZN4GameE_t>.strings))
        return static_cast<void*>(this);
    return QObject::qt_metacast(_clname);
}

int Game::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 18)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 18;
    }
    if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 18)
            *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType();
        _id -= 18;
    }
    return _id;
}

// SIGNAL 0
void Game::gameStarted()
{
    QMetaObject::activate(this, &staticMetaObject, 0, nullptr);
}

// SIGNAL 1
void Game::gameStateChanged(GameState _t1)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 1, nullptr, _t1);
}

// SIGNAL 2
void Game::gameOver(int _t1)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 2, nullptr, _t1);
}

// SIGNAL 3
void Game::playerTurnStarted(int _t1)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 3, nullptr, _t1);
}

// SIGNAL 4
void Game::playerTurnEnded(int _t1)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 4, nullptr, _t1);
}

// SIGNAL 5
void Game::diceRolled(int _t1, int _t2)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 5, nullptr, _t1, _t2);
}

// SIGNAL 6
void Game::playerMoved(int _t1, int _t2, int _t3)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 6, nullptr, _t1, _t2, _t3);
}

// SIGNAL 7
void Game::playerSentToJail(int _t1)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 7, nullptr, _t1);
}

// SIGNAL 8
void Game::playerReleasedFromJail(int _t1)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 8, nullptr, _t1);
}

// SIGNAL 9
void Game::tileEventTriggered(int _t1, const QString & _t2)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 9, nullptr, _t1, _t2);
}

// SIGNAL 10
void Game::purchaseOpportunity(const QString & _t1, int _t2, int _t3)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 10, nullptr, _t1, _t2, _t3);
}

// SIGNAL 11
void Game::rentPaymentRequired(const QString & _t1, int _t2, int _t3, int _t4)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 11, nullptr, _t1, _t2, _t3, _t4);
}

// SIGNAL 12
void Game::rentPaid(const QString & _t1, int _t2, const QString & _t3)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 12, nullptr, _t1, _t2, _t3);
}

// SIGNAL 13
void Game::gameLogMessage(const QString & _t1)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 13, nullptr, _t1);
}

// SIGNAL 14
void Game::forceRaiseMoney(int _t1, int _t2)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 14, nullptr, _t1, _t2);
}

// SIGNAL 15
void Game::playerBankrupt(int _t1)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 15, nullptr, _t1);
}

// SIGNAL 16
void Game::tradeProposed(int _t1, int _t2, int _t3, int _t4)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 16, nullptr, _t1, _t2, _t3, _t4);
}

// SIGNAL 17
void Game::warningSignal(const QString & _t1)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 17, nullptr, _t1);
}
QT_WARNING_POP
