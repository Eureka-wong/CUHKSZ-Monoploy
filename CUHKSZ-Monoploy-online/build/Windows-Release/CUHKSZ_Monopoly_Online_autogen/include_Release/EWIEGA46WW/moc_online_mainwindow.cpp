/****************************************************************************
** Meta object code from reading C++ file 'online_mainwindow.h'
**
** Created by: The Qt Meta Object Compiler version 69 (Qt 6.10.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../../../online_mainwindow.h"
#include <QtGui/qtextcursor.h>
#include <QtCore/qmetatype.h>

#include <QtCore/qtmochelpers.h>

#include <memory>


#include <QtCore/qxptype_traits.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'online_mainwindow.h' doesn't include <QObject>."
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
struct qt_meta_tag_ZN16OnlineMainWindowE_t {};
} // unnamed namespace

template <> constexpr inline auto OnlineMainWindow::qt_create_metaobjectdata<qt_meta_tag_ZN16OnlineMainWindowE_t>()
{
    namespace QMC = QtMocConstants;
    QtMocHelpers::StringRefStorage qt_stringData {
        "OnlineMainWindow",
        "onConnectToServer",
        "",
        "onCreateRoom",
        "onJoinRoom",
        "onRefreshRoomList",
        "onRoomSelected",
        "onRollDiceClicked",
        "onBuyPropertyClicked",
        "onSellPropertyClicked",
        "onPayBailClicked",
        "onEndTurnClicked",
        "onHintClicked",
        "onStartGameClicked",
        "onViewPropertyClicked",
        "onChatroomClicked",
        "onSendChatMessage",
        "onConnected",
        "onDisconnected",
        "onError",
        "error",
        "onJoinedRoom",
        "roomId",
        "playerId",
        "monopoly::StateSnapshot",
        "snapshot",
        "onRoomListReceived",
        "std::vector<monopoly::RoomSummary>",
        "rooms",
        "onStateUpdate",
        "onGameEvent",
        "monopoly::GameEvent",
        "event",
        "onChatReceived",
        "playerName",
        "message",
        "onPlayerTurnStarted",
        "playerIndex",
        "onPlayerTurnEnded",
        "onDiceRolled",
        "diceValue",
        "onPlayerMoved",
        "oldPosition",
        "newPosition",
        "onPurchaseOpportunity",
        "propertyName",
        "price",
        "onGameLogMessage",
        "onPlayerBankrupt",
        "onRentPaymentRequired",
        "rent",
        "fromPlayer",
        "toPlayer",
        "onForceRaiseMoney",
        "payerIndex",
        "amountDue"
    };

    QtMocHelpers::UintData qt_methods {
        // Slot 'onConnectToServer'
        QtMocHelpers::SlotData<void()>(1, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'onCreateRoom'
        QtMocHelpers::SlotData<void()>(3, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'onJoinRoom'
        QtMocHelpers::SlotData<void()>(4, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'onRefreshRoomList'
        QtMocHelpers::SlotData<void()>(5, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'onRoomSelected'
        QtMocHelpers::SlotData<void()>(6, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'onRollDiceClicked'
        QtMocHelpers::SlotData<void()>(7, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'onBuyPropertyClicked'
        QtMocHelpers::SlotData<void()>(8, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'onSellPropertyClicked'
        QtMocHelpers::SlotData<void()>(9, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'onPayBailClicked'
        QtMocHelpers::SlotData<void()>(10, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'onEndTurnClicked'
        QtMocHelpers::SlotData<void()>(11, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'onHintClicked'
        QtMocHelpers::SlotData<void()>(12, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'onStartGameClicked'
        QtMocHelpers::SlotData<void()>(13, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'onViewPropertyClicked'
        QtMocHelpers::SlotData<void()>(14, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'onChatroomClicked'
        QtMocHelpers::SlotData<void()>(15, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'onSendChatMessage'
        QtMocHelpers::SlotData<void()>(16, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'onConnected'
        QtMocHelpers::SlotData<void()>(17, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'onDisconnected'
        QtMocHelpers::SlotData<void()>(18, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'onError'
        QtMocHelpers::SlotData<void(const QString &)>(19, 2, QMC::AccessPrivate, QMetaType::Void, {{
            { QMetaType::QString, 20 },
        }}),
        // Slot 'onJoinedRoom'
        QtMocHelpers::SlotData<void(const QString &, int, const monopoly::StateSnapshot &)>(21, 2, QMC::AccessPrivate, QMetaType::Void, {{
            { QMetaType::QString, 22 }, { QMetaType::Int, 23 }, { 0x80000000 | 24, 25 },
        }}),
        // Slot 'onRoomListReceived'
        QtMocHelpers::SlotData<void(const std::vector<monopoly::RoomSummary> &)>(26, 2, QMC::AccessPrivate, QMetaType::Void, {{
            { 0x80000000 | 27, 28 },
        }}),
        // Slot 'onStateUpdate'
        QtMocHelpers::SlotData<void(const monopoly::StateSnapshot &)>(29, 2, QMC::AccessPrivate, QMetaType::Void, {{
            { 0x80000000 | 24, 25 },
        }}),
        // Slot 'onGameEvent'
        QtMocHelpers::SlotData<void(const monopoly::GameEvent &)>(30, 2, QMC::AccessPrivate, QMetaType::Void, {{
            { 0x80000000 | 31, 32 },
        }}),
        // Slot 'onChatReceived'
        QtMocHelpers::SlotData<void(int, const QString &, const QString &)>(33, 2, QMC::AccessPrivate, QMetaType::Void, {{
            { QMetaType::Int, 23 }, { QMetaType::QString, 34 }, { QMetaType::QString, 35 },
        }}),
        // Slot 'onPlayerTurnStarted'
        QtMocHelpers::SlotData<void(int)>(36, 2, QMC::AccessPrivate, QMetaType::Void, {{
            { QMetaType::Int, 37 },
        }}),
        // Slot 'onPlayerTurnEnded'
        QtMocHelpers::SlotData<void(int)>(38, 2, QMC::AccessPrivate, QMetaType::Void, {{
            { QMetaType::Int, 37 },
        }}),
        // Slot 'onDiceRolled'
        QtMocHelpers::SlotData<void(int, int)>(39, 2, QMC::AccessPrivate, QMetaType::Void, {{
            { QMetaType::Int, 37 }, { QMetaType::Int, 40 },
        }}),
        // Slot 'onPlayerMoved'
        QtMocHelpers::SlotData<void(int, int, int)>(41, 2, QMC::AccessPrivate, QMetaType::Void, {{
            { QMetaType::Int, 37 }, { QMetaType::Int, 42 }, { QMetaType::Int, 43 },
        }}),
        // Slot 'onPurchaseOpportunity'
        QtMocHelpers::SlotData<void(const QString &, int, int)>(44, 2, QMC::AccessPrivate, QMetaType::Void, {{
            { QMetaType::QString, 45 }, { QMetaType::Int, 46 }, { QMetaType::Int, 37 },
        }}),
        // Slot 'onGameLogMessage'
        QtMocHelpers::SlotData<void(const QString &)>(47, 2, QMC::AccessPrivate, QMetaType::Void, {{
            { QMetaType::QString, 35 },
        }}),
        // Slot 'onPlayerBankrupt'
        QtMocHelpers::SlotData<void(int)>(48, 2, QMC::AccessPrivate, QMetaType::Void, {{
            { QMetaType::Int, 37 },
        }}),
        // Slot 'onRentPaymentRequired'
        QtMocHelpers::SlotData<void(const QString &, int, int, int)>(49, 2, QMC::AccessPrivate, QMetaType::Void, {{
            { QMetaType::QString, 45 }, { QMetaType::Int, 50 }, { QMetaType::Int, 51 }, { QMetaType::Int, 52 },
        }}),
        // Slot 'onForceRaiseMoney'
        QtMocHelpers::SlotData<void(int, int)>(53, 2, QMC::AccessPrivate, QMetaType::Void, {{
            { QMetaType::Int, 54 }, { QMetaType::Int, 55 },
        }}),
    };
    QtMocHelpers::UintData qt_properties {
    };
    QtMocHelpers::UintData qt_enums {
    };
    return QtMocHelpers::metaObjectData<OnlineMainWindow, qt_meta_tag_ZN16OnlineMainWindowE_t>(QMC::MetaObjectFlag{}, qt_stringData,
            qt_methods, qt_properties, qt_enums);
}
Q_CONSTINIT const QMetaObject OnlineMainWindow::staticMetaObject = { {
    QMetaObject::SuperData::link<QMainWindow::staticMetaObject>(),
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN16OnlineMainWindowE_t>.stringdata,
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN16OnlineMainWindowE_t>.data,
    qt_static_metacall,
    nullptr,
    qt_staticMetaObjectRelocatingContent<qt_meta_tag_ZN16OnlineMainWindowE_t>.metaTypes,
    nullptr
} };

void OnlineMainWindow::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    auto *_t = static_cast<OnlineMainWindow *>(_o);
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: _t->onConnectToServer(); break;
        case 1: _t->onCreateRoom(); break;
        case 2: _t->onJoinRoom(); break;
        case 3: _t->onRefreshRoomList(); break;
        case 4: _t->onRoomSelected(); break;
        case 5: _t->onRollDiceClicked(); break;
        case 6: _t->onBuyPropertyClicked(); break;
        case 7: _t->onSellPropertyClicked(); break;
        case 8: _t->onPayBailClicked(); break;
        case 9: _t->onEndTurnClicked(); break;
        case 10: _t->onHintClicked(); break;
        case 11: _t->onStartGameClicked(); break;
        case 12: _t->onViewPropertyClicked(); break;
        case 13: _t->onChatroomClicked(); break;
        case 14: _t->onSendChatMessage(); break;
        case 15: _t->onConnected(); break;
        case 16: _t->onDisconnected(); break;
        case 17: _t->onError((*reinterpret_cast<std::add_pointer_t<QString>>(_a[1]))); break;
        case 18: _t->onJoinedRoom((*reinterpret_cast<std::add_pointer_t<QString>>(_a[1])),(*reinterpret_cast<std::add_pointer_t<int>>(_a[2])),(*reinterpret_cast<std::add_pointer_t<monopoly::StateSnapshot>>(_a[3]))); break;
        case 19: _t->onRoomListReceived((*reinterpret_cast<std::add_pointer_t<std::vector<monopoly::RoomSummary>>>(_a[1]))); break;
        case 20: _t->onStateUpdate((*reinterpret_cast<std::add_pointer_t<monopoly::StateSnapshot>>(_a[1]))); break;
        case 21: _t->onGameEvent((*reinterpret_cast<std::add_pointer_t<monopoly::GameEvent>>(_a[1]))); break;
        case 22: _t->onChatReceived((*reinterpret_cast<std::add_pointer_t<int>>(_a[1])),(*reinterpret_cast<std::add_pointer_t<QString>>(_a[2])),(*reinterpret_cast<std::add_pointer_t<QString>>(_a[3]))); break;
        case 23: _t->onPlayerTurnStarted((*reinterpret_cast<std::add_pointer_t<int>>(_a[1]))); break;
        case 24: _t->onPlayerTurnEnded((*reinterpret_cast<std::add_pointer_t<int>>(_a[1]))); break;
        case 25: _t->onDiceRolled((*reinterpret_cast<std::add_pointer_t<int>>(_a[1])),(*reinterpret_cast<std::add_pointer_t<int>>(_a[2]))); break;
        case 26: _t->onPlayerMoved((*reinterpret_cast<std::add_pointer_t<int>>(_a[1])),(*reinterpret_cast<std::add_pointer_t<int>>(_a[2])),(*reinterpret_cast<std::add_pointer_t<int>>(_a[3]))); break;
        case 27: _t->onPurchaseOpportunity((*reinterpret_cast<std::add_pointer_t<QString>>(_a[1])),(*reinterpret_cast<std::add_pointer_t<int>>(_a[2])),(*reinterpret_cast<std::add_pointer_t<int>>(_a[3]))); break;
        case 28: _t->onGameLogMessage((*reinterpret_cast<std::add_pointer_t<QString>>(_a[1]))); break;
        case 29: _t->onPlayerBankrupt((*reinterpret_cast<std::add_pointer_t<int>>(_a[1]))); break;
        case 30: _t->onRentPaymentRequired((*reinterpret_cast<std::add_pointer_t<QString>>(_a[1])),(*reinterpret_cast<std::add_pointer_t<int>>(_a[2])),(*reinterpret_cast<std::add_pointer_t<int>>(_a[3])),(*reinterpret_cast<std::add_pointer_t<int>>(_a[4]))); break;
        case 31: _t->onForceRaiseMoney((*reinterpret_cast<std::add_pointer_t<int>>(_a[1])),(*reinterpret_cast<std::add_pointer_t<int>>(_a[2]))); break;
        default: ;
        }
    }
}

const QMetaObject *OnlineMainWindow::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *OnlineMainWindow::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_staticMetaObjectStaticContent<qt_meta_tag_ZN16OnlineMainWindowE_t>.strings))
        return static_cast<void*>(this);
    return QMainWindow::qt_metacast(_clname);
}

int OnlineMainWindow::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QMainWindow::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 32)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 32;
    }
    if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 32)
            *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType();
        _id -= 32;
    }
    return _id;
}
QT_WARNING_POP
