/****************************************************************************
** Meta object code from reading C++ file 'Game.h'
**
** Created by: The Qt Meta Object Compiler version 69 (Qt 6.10.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../../Game.h"
#include <QtCore/qmetatype.h>

#include <QtCore/qtmochelpers.h>

#include <memory>


#include <QtCore/qxptype_traits.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'Game.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 69
#error "This file was generated using the moc from 6.10.0. It"
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
        "roundLabelChanged",
        "moneyChanged",
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
        "taxPaymentRequired",
        "taxName",
        "tax",
        "landOnSelfProperty",
        "openChanceCard",
        "cardDiscription",
        "openCommunityCard",
        "landOnFreeParking",
        "type",
        "landOnGoTile",
        "landOnGoToJailTile",
        "rentPaid",
        "rentAmount",
        "gameLogMessage",
        "message",
        "forceRaiseMoney",
        "amount",
        "playerBankrupt",
        "tradeProposed",
        "propertyIndex",
        "warningSignal",
        "enableEndTurnAndDisableRoll"
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
        // Signal 'roundLabelChanged'
        QtMocHelpers::SignalData<void()>(8, 2, QMC::AccessPublic, QMetaType::Void),
        // Signal 'moneyChanged'
        QtMocHelpers::SignalData<void()>(9, 2, QMC::AccessPublic, QMetaType::Void),
        // Signal 'playerTurnStarted'
        QtMocHelpers::SignalData<void(int)>(10, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::Int, 11 },
        }}),
        // Signal 'playerTurnEnded'
        QtMocHelpers::SignalData<void(int)>(12, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::Int, 11 },
        }}),
        // Signal 'diceRolled'
        QtMocHelpers::SignalData<void(int, int)>(13, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::Int, 11 }, { QMetaType::Int, 14 },
        }}),
        // Signal 'playerMoved'
        QtMocHelpers::SignalData<void(int, int, int)>(15, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::Int, 11 }, { QMetaType::Int, 16 }, { QMetaType::Int, 17 },
        }}),
        // Signal 'playerSentToJail'
        QtMocHelpers::SignalData<void(int)>(18, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::Int, 11 },
        }}),
        // Signal 'playerReleasedFromJail'
        QtMocHelpers::SignalData<void(int)>(19, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::Int, 11 },
        }}),
        // Signal 'tileEventTriggered'
        QtMocHelpers::SignalData<void(int, const QString &)>(20, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::Int, 21 }, { QMetaType::QString, 22 },
        }}),
        // Signal 'purchaseOpportunity'
        QtMocHelpers::SignalData<void(const QString &, int, int)>(23, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::QString, 24 }, { QMetaType::Int, 25 }, { QMetaType::Int, 11 },
        }}),
        // Signal 'rentPaymentRequired'
        QtMocHelpers::SignalData<void(const QString &, int, int, int)>(26, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::QString, 24 }, { QMetaType::Int, 27 }, { QMetaType::Int, 28 }, { QMetaType::Int, 29 },
        }}),
        // Signal 'taxPaymentRequired'
        QtMocHelpers::SignalData<void(const QString &, int, int)>(30, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::QString, 31 }, { QMetaType::Int, 32 }, { QMetaType::Int, 28 },
        }}),
        // Signal 'landOnSelfProperty'
        QtMocHelpers::SignalData<void(int, const QString &)>(33, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::Int, 11 }, { QMetaType::QString, 24 },
        }}),
        // Signal 'openChanceCard'
        QtMocHelpers::SignalData<void(int, const QString &)>(34, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::Int, 11 }, { QMetaType::QString, 35 },
        }}),
        // Signal 'openCommunityCard'
        QtMocHelpers::SignalData<void(int, const QString &)>(36, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::Int, 11 }, { QMetaType::QString, 35 },
        }}),
        // Signal 'landOnFreeParking'
        QtMocHelpers::SignalData<void(int, int)>(37, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::Int, 11 }, { QMetaType::Int, 38 },
        }}),
        // Signal 'landOnGoTile'
        QtMocHelpers::SignalData<void(int)>(39, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::Int, 11 },
        }}),
        // Signal 'landOnGoToJailTile'
        QtMocHelpers::SignalData<void(int)>(40, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::Int, 11 },
        }}),
        // Signal 'rentPaid'
        QtMocHelpers::SignalData<void(const QString &, int, const QString &)>(41, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::QString, 28 }, { QMetaType::Int, 42 }, { QMetaType::QString, 29 },
        }}),
        // Signal 'gameLogMessage'
        QtMocHelpers::SignalData<void(const QString &)>(43, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::QString, 44 },
        }}),
        // Signal 'forceRaiseMoney'
        QtMocHelpers::SignalData<void(int, int)>(45, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::Int, 11 }, { QMetaType::Int, 46 },
        }}),
        // Signal 'playerBankrupt'
        QtMocHelpers::SignalData<void(int)>(47, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::Int, 11 },
        }}),
        // Signal 'tradeProposed'
        QtMocHelpers::SignalData<void(int, int, int, int)>(48, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::Int, 28 }, { QMetaType::Int, 29 }, { QMetaType::Int, 49 }, { QMetaType::Int, 46 },
        }}),
        // Signal 'warningSignal'
        QtMocHelpers::SignalData<void(const QString &)>(50, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::QString, 44 },
        }}),
        // Signal 'enableEndTurnAndDisableRoll'
        QtMocHelpers::SignalData<void()>(51, 2, QMC::AccessPublic, QMetaType::Void),
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
        case 3: _t->roundLabelChanged(); break;
        case 4: _t->moneyChanged(); break;
        case 5: _t->playerTurnStarted((*reinterpret_cast<std::add_pointer_t<int>>(_a[1]))); break;
        case 6: _t->playerTurnEnded((*reinterpret_cast<std::add_pointer_t<int>>(_a[1]))); break;
        case 7: _t->diceRolled((*reinterpret_cast<std::add_pointer_t<int>>(_a[1])),(*reinterpret_cast<std::add_pointer_t<int>>(_a[2]))); break;
        case 8: _t->playerMoved((*reinterpret_cast<std::add_pointer_t<int>>(_a[1])),(*reinterpret_cast<std::add_pointer_t<int>>(_a[2])),(*reinterpret_cast<std::add_pointer_t<int>>(_a[3]))); break;
        case 9: _t->playerSentToJail((*reinterpret_cast<std::add_pointer_t<int>>(_a[1]))); break;
        case 10: _t->playerReleasedFromJail((*reinterpret_cast<std::add_pointer_t<int>>(_a[1]))); break;
        case 11: _t->tileEventTriggered((*reinterpret_cast<std::add_pointer_t<int>>(_a[1])),(*reinterpret_cast<std::add_pointer_t<QString>>(_a[2]))); break;
        case 12: _t->purchaseOpportunity((*reinterpret_cast<std::add_pointer_t<QString>>(_a[1])),(*reinterpret_cast<std::add_pointer_t<int>>(_a[2])),(*reinterpret_cast<std::add_pointer_t<int>>(_a[3]))); break;
        case 13: _t->rentPaymentRequired((*reinterpret_cast<std::add_pointer_t<QString>>(_a[1])),(*reinterpret_cast<std::add_pointer_t<int>>(_a[2])),(*reinterpret_cast<std::add_pointer_t<int>>(_a[3])),(*reinterpret_cast<std::add_pointer_t<int>>(_a[4]))); break;
        case 14: _t->taxPaymentRequired((*reinterpret_cast<std::add_pointer_t<QString>>(_a[1])),(*reinterpret_cast<std::add_pointer_t<int>>(_a[2])),(*reinterpret_cast<std::add_pointer_t<int>>(_a[3]))); break;
        case 15: _t->landOnSelfProperty((*reinterpret_cast<std::add_pointer_t<int>>(_a[1])),(*reinterpret_cast<std::add_pointer_t<QString>>(_a[2]))); break;
        case 16: _t->openChanceCard((*reinterpret_cast<std::add_pointer_t<int>>(_a[1])),(*reinterpret_cast<std::add_pointer_t<QString>>(_a[2]))); break;
        case 17: _t->openCommunityCard((*reinterpret_cast<std::add_pointer_t<int>>(_a[1])),(*reinterpret_cast<std::add_pointer_t<QString>>(_a[2]))); break;
        case 18: _t->landOnFreeParking((*reinterpret_cast<std::add_pointer_t<int>>(_a[1])),(*reinterpret_cast<std::add_pointer_t<int>>(_a[2]))); break;
        case 19: _t->landOnGoTile((*reinterpret_cast<std::add_pointer_t<int>>(_a[1]))); break;
        case 20: _t->landOnGoToJailTile((*reinterpret_cast<std::add_pointer_t<int>>(_a[1]))); break;
        case 21: _t->rentPaid((*reinterpret_cast<std::add_pointer_t<QString>>(_a[1])),(*reinterpret_cast<std::add_pointer_t<int>>(_a[2])),(*reinterpret_cast<std::add_pointer_t<QString>>(_a[3]))); break;
        case 22: _t->gameLogMessage((*reinterpret_cast<std::add_pointer_t<QString>>(_a[1]))); break;
        case 23: _t->forceRaiseMoney((*reinterpret_cast<std::add_pointer_t<int>>(_a[1])),(*reinterpret_cast<std::add_pointer_t<int>>(_a[2]))); break;
        case 24: _t->playerBankrupt((*reinterpret_cast<std::add_pointer_t<int>>(_a[1]))); break;
        case 25: _t->tradeProposed((*reinterpret_cast<std::add_pointer_t<int>>(_a[1])),(*reinterpret_cast<std::add_pointer_t<int>>(_a[2])),(*reinterpret_cast<std::add_pointer_t<int>>(_a[3])),(*reinterpret_cast<std::add_pointer_t<int>>(_a[4]))); break;
        case 26: _t->warningSignal((*reinterpret_cast<std::add_pointer_t<QString>>(_a[1]))); break;
        case 27: _t->enableEndTurnAndDisableRoll(); break;
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
        if (QtMocHelpers::indexOfMethod<void (Game::*)()>(_a, &Game::roundLabelChanged, 3))
            return;
        if (QtMocHelpers::indexOfMethod<void (Game::*)()>(_a, &Game::moneyChanged, 4))
            return;
        if (QtMocHelpers::indexOfMethod<void (Game::*)(int )>(_a, &Game::playerTurnStarted, 5))
            return;
        if (QtMocHelpers::indexOfMethod<void (Game::*)(int )>(_a, &Game::playerTurnEnded, 6))
            return;
        if (QtMocHelpers::indexOfMethod<void (Game::*)(int , int )>(_a, &Game::diceRolled, 7))
            return;
        if (QtMocHelpers::indexOfMethod<void (Game::*)(int , int , int )>(_a, &Game::playerMoved, 8))
            return;
        if (QtMocHelpers::indexOfMethod<void (Game::*)(int )>(_a, &Game::playerSentToJail, 9))
            return;
        if (QtMocHelpers::indexOfMethod<void (Game::*)(int )>(_a, &Game::playerReleasedFromJail, 10))
            return;
        if (QtMocHelpers::indexOfMethod<void (Game::*)(int , const QString & )>(_a, &Game::tileEventTriggered, 11))
            return;
        if (QtMocHelpers::indexOfMethod<void (Game::*)(const QString & , int , int )>(_a, &Game::purchaseOpportunity, 12))
            return;
        if (QtMocHelpers::indexOfMethod<void (Game::*)(const QString & , int , int , int )>(_a, &Game::rentPaymentRequired, 13))
            return;
        if (QtMocHelpers::indexOfMethod<void (Game::*)(const QString & , int , int )>(_a, &Game::taxPaymentRequired, 14))
            return;
        if (QtMocHelpers::indexOfMethod<void (Game::*)(int , const QString & )>(_a, &Game::landOnSelfProperty, 15))
            return;
        if (QtMocHelpers::indexOfMethod<void (Game::*)(int , const QString & )>(_a, &Game::openChanceCard, 16))
            return;
        if (QtMocHelpers::indexOfMethod<void (Game::*)(int , const QString & )>(_a, &Game::openCommunityCard, 17))
            return;
        if (QtMocHelpers::indexOfMethod<void (Game::*)(int , int )>(_a, &Game::landOnFreeParking, 18))
            return;
        if (QtMocHelpers::indexOfMethod<void (Game::*)(int )>(_a, &Game::landOnGoTile, 19))
            return;
        if (QtMocHelpers::indexOfMethod<void (Game::*)(int )>(_a, &Game::landOnGoToJailTile, 20))
            return;
        if (QtMocHelpers::indexOfMethod<void (Game::*)(const QString & , int , const QString & )>(_a, &Game::rentPaid, 21))
            return;
        if (QtMocHelpers::indexOfMethod<void (Game::*)(const QString & )>(_a, &Game::gameLogMessage, 22))
            return;
        if (QtMocHelpers::indexOfMethod<void (Game::*)(int , int )>(_a, &Game::forceRaiseMoney, 23))
            return;
        if (QtMocHelpers::indexOfMethod<void (Game::*)(int )>(_a, &Game::playerBankrupt, 24))
            return;
        if (QtMocHelpers::indexOfMethod<void (Game::*)(int , int , int , int )>(_a, &Game::tradeProposed, 25))
            return;
        if (QtMocHelpers::indexOfMethod<void (Game::*)(const QString & )>(_a, &Game::warningSignal, 26))
            return;
        if (QtMocHelpers::indexOfMethod<void (Game::*)()>(_a, &Game::enableEndTurnAndDisableRoll, 27))
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
        if (_id < 28)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 28;
    }
    if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 28)
            *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType();
        _id -= 28;
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
void Game::roundLabelChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 3, nullptr);
}

// SIGNAL 4
void Game::moneyChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 4, nullptr);
}

// SIGNAL 5
void Game::playerTurnStarted(int _t1)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 5, nullptr, _t1);
}

// SIGNAL 6
void Game::playerTurnEnded(int _t1)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 6, nullptr, _t1);
}

// SIGNAL 7
void Game::diceRolled(int _t1, int _t2)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 7, nullptr, _t1, _t2);
}

// SIGNAL 8
void Game::playerMoved(int _t1, int _t2, int _t3)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 8, nullptr, _t1, _t2, _t3);
}

// SIGNAL 9
void Game::playerSentToJail(int _t1)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 9, nullptr, _t1);
}

// SIGNAL 10
void Game::playerReleasedFromJail(int _t1)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 10, nullptr, _t1);
}

// SIGNAL 11
void Game::tileEventTriggered(int _t1, const QString & _t2)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 11, nullptr, _t1, _t2);
}

// SIGNAL 12
void Game::purchaseOpportunity(const QString & _t1, int _t2, int _t3)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 12, nullptr, _t1, _t2, _t3);
}

// SIGNAL 13
void Game::rentPaymentRequired(const QString & _t1, int _t2, int _t3, int _t4)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 13, nullptr, _t1, _t2, _t3, _t4);
}

// SIGNAL 14
void Game::taxPaymentRequired(const QString & _t1, int _t2, int _t3)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 14, nullptr, _t1, _t2, _t3);
}

// SIGNAL 15
void Game::landOnSelfProperty(int _t1, const QString & _t2)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 15, nullptr, _t1, _t2);
}

// SIGNAL 16
void Game::openChanceCard(int _t1, const QString & _t2)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 16, nullptr, _t1, _t2);
}

// SIGNAL 17
void Game::openCommunityCard(int _t1, const QString & _t2)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 17, nullptr, _t1, _t2);
}

// SIGNAL 18
void Game::landOnFreeParking(int _t1, int _t2)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 18, nullptr, _t1, _t2);
}

// SIGNAL 19
void Game::landOnGoTile(int _t1)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 19, nullptr, _t1);
}

// SIGNAL 20
void Game::landOnGoToJailTile(int _t1)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 20, nullptr, _t1);
}

// SIGNAL 21
void Game::rentPaid(const QString & _t1, int _t2, const QString & _t3)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 21, nullptr, _t1, _t2, _t3);
}

// SIGNAL 22
void Game::gameLogMessage(const QString & _t1)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 22, nullptr, _t1);
}

// SIGNAL 23
void Game::forceRaiseMoney(int _t1, int _t2)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 23, nullptr, _t1, _t2);
}

// SIGNAL 24
void Game::playerBankrupt(int _t1)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 24, nullptr, _t1);
}

// SIGNAL 25
void Game::tradeProposed(int _t1, int _t2, int _t3, int _t4)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 25, nullptr, _t1, _t2, _t3, _t4);
}

// SIGNAL 26
void Game::warningSignal(const QString & _t1)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 26, nullptr, _t1);
}

// SIGNAL 27
void Game::enableEndTurnAndDisableRoll()
{
    QMetaObject::activate(this, &staticMetaObject, 27, nullptr);
}
QT_WARNING_POP
