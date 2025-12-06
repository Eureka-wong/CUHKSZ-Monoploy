/****************************************************************************
** Meta object code from reading C++ file 'mainwindow.h'
**
** Created by: The Qt Meta Object Compiler version 69 (Qt 6.10.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../../mainwindow.h"
#include <QtGui/qtextcursor.h>
#include <QtCore/qmetatype.h>

#include <QtCore/qtmochelpers.h>

#include <memory>


#include <QtCore/qxptype_traits.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'mainwindow.h' doesn't include <QObject>."
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
struct qt_meta_tag_ZN10MainWindowE_t {};
} // unnamed namespace

template <> constexpr inline auto MainWindow::qt_create_metaobjectdata<qt_meta_tag_ZN10MainWindowE_t>()
{
    namespace QMC = QtMocConstants;
    QtMocHelpers::StringRefStorage qt_stringData {
        "MainWindow",
        "onRoundLabelChanged",
        "",
        "onMoneyChanged",
        "onRollDiceClicked",
        "onPropertiesClicked",
        "playerIndex",
        "forced",
        "amountDue",
        "onForceRaiseMoney",
        "payerIndex",
        "updatePropertyDisplay",
        "QLabel*",
        "playerInfo",
        "propertyInfo",
        "PropertyTile*",
        "property",
        "currentPlayerIndex",
        "showWarning",
        "message",
        "onTradeClicked",
        "onSellTrade",
        "onBuyTrade",
        "modifyTradeSelectionWidget",
        "Player",
        "currentPlayer",
        "std::vector<PropertyTile*>",
        "myProperties",
        "QDialog&",
        "dialog",
        "QWidget*",
        "propertiesContainer",
        "QVBoxLayout*",
        "propertiesLayout",
        "askTradeDecision",
        "Player*",
        "offeringPlayer",
        "targetPlayer",
        "amount",
        "card",
        "offerBuy",
        "onHintClicked",
        "onChatroomClicked",
        "onEndTurnClicked",
        "onPlayerTurnStarted",
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
        "onrentPaymentRequired",
        "rent",
        "fromPlayer",
        "toPlayer",
        "ontaxPaymentRequired",
        "taxName",
        "tax",
        "onLandOnSelfProperty",
        "onOpenChanceCard",
        "cardDiscription",
        "onOpenCommunityCard",
        "onLandOnFreeParking",
        "type",
        "onLandOnGoTile",
        "onLandOnGoToJailTile",
        "onEnableEndTurnAndDisableRoll"
    };

    QtMocHelpers::UintData qt_methods {
        // Slot 'onRoundLabelChanged'
        QtMocHelpers::SlotData<void()>(1, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'onMoneyChanged'
        QtMocHelpers::SlotData<void()>(3, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'onRollDiceClicked'
        QtMocHelpers::SlotData<void()>(4, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'onPropertiesClicked'
        QtMocHelpers::SlotData<void(int, bool, int)>(5, 2, QMC::AccessPrivate, QMetaType::Void, {{
            { QMetaType::Int, 6 }, { QMetaType::Bool, 7 }, { QMetaType::Int, 8 },
        }}),
        // Slot 'onPropertiesClicked'
        QtMocHelpers::SlotData<void(int, bool)>(5, 2, QMC::AccessPrivate | QMC::MethodCloned, QMetaType::Void, {{
            { QMetaType::Int, 6 }, { QMetaType::Bool, 7 },
        }}),
        // Slot 'onPropertiesClicked'
        QtMocHelpers::SlotData<void(int)>(5, 2, QMC::AccessPrivate | QMC::MethodCloned, QMetaType::Void, {{
            { QMetaType::Int, 6 },
        }}),
        // Slot 'onPropertiesClicked'
        QtMocHelpers::SlotData<void()>(5, 2, QMC::AccessPrivate | QMC::MethodCloned, QMetaType::Void),
        // Slot 'onForceRaiseMoney'
        QtMocHelpers::SlotData<void(int, int)>(9, 2, QMC::AccessPrivate, QMetaType::Void, {{
            { QMetaType::Int, 10 }, { QMetaType::Int, 8 },
        }}),
        // Slot 'updatePropertyDisplay'
        QtMocHelpers::SlotData<void(QLabel *, QLabel *, PropertyTile *, int)>(11, 2, QMC::AccessPrivate, QMetaType::Void, {{
            { 0x80000000 | 12, 13 }, { 0x80000000 | 12, 14 }, { 0x80000000 | 15, 16 }, { QMetaType::Int, 17 },
        }}),
        // Slot 'showWarning'
        QtMocHelpers::SlotData<void(const QString &)>(18, 2, QMC::AccessPrivate, QMetaType::Void, {{
            { QMetaType::QString, 19 },
        }}),
        // Slot 'onTradeClicked'
        QtMocHelpers::SlotData<void()>(20, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'onSellTrade'
        QtMocHelpers::SlotData<void()>(21, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'onBuyTrade'
        QtMocHelpers::SlotData<void()>(22, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'modifyTradeSelectionWidget'
        QtMocHelpers::SlotData<void(const Player &, const std::vector<PropertyTile*> &, QDialog &, QWidget *, QVBoxLayout *)>(23, 2, QMC::AccessPrivate, QMetaType::Void, {{
            { 0x80000000 | 24, 25 }, { 0x80000000 | 26, 27 }, { 0x80000000 | 28, 29 }, { 0x80000000 | 30, 31 },
            { 0x80000000 | 32, 33 },
        }}),
        // Slot 'askTradeDecision'
        QtMocHelpers::SlotData<void(Player *, Player *, int, bool, PropertyTile *, bool)>(34, 2, QMC::AccessPrivate, QMetaType::Void, {{
            { 0x80000000 | 35, 36 }, { 0x80000000 | 35, 37 }, { QMetaType::Int, 38 }, { QMetaType::Bool, 39 },
            { 0x80000000 | 15, 16 }, { QMetaType::Bool, 40 },
        }}),
        // Slot 'onHintClicked'
        QtMocHelpers::SlotData<void(int)>(41, 2, QMC::AccessPrivate, QMetaType::Void, {{
            { QMetaType::Int, 6 },
        }}),
        // Slot 'onChatroomClicked'
        QtMocHelpers::SlotData<void()>(42, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'onEndTurnClicked'
        QtMocHelpers::SlotData<void()>(43, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'onPlayerTurnStarted'
        QtMocHelpers::SlotData<void(int)>(44, 2, QMC::AccessPrivate, QMetaType::Void, {{
            { QMetaType::Int, 6 },
        }}),
        // Slot 'onPlayerTurnEnded'
        QtMocHelpers::SlotData<void(int)>(45, 2, QMC::AccessPrivate, QMetaType::Void, {{
            { QMetaType::Int, 6 },
        }}),
        // Slot 'onDiceRolled'
        QtMocHelpers::SlotData<void(int, int)>(46, 2, QMC::AccessPrivate, QMetaType::Void, {{
            { QMetaType::Int, 6 }, { QMetaType::Int, 47 },
        }}),
        // Slot 'onPlayerMoved'
        QtMocHelpers::SlotData<void(int, int, int)>(48, 2, QMC::AccessPrivate, QMetaType::Void, {{
            { QMetaType::Int, 6 }, { QMetaType::Int, 49 }, { QMetaType::Int, 50 },
        }}),
        // Slot 'onPurchaseOpportunity'
        QtMocHelpers::SlotData<void(const QString &, int, int)>(51, 2, QMC::AccessPrivate, QMetaType::Void, {{
            { QMetaType::QString, 52 }, { QMetaType::Int, 53 }, { QMetaType::Int, 6 },
        }}),
        // Slot 'onGameLogMessage'
        QtMocHelpers::SlotData<void(const QString &)>(54, 2, QMC::AccessPrivate, QMetaType::Void, {{
            { QMetaType::QString, 19 },
        }}),
        // Slot 'onPlayerBankrupt'
        QtMocHelpers::SlotData<void(int)>(55, 2, QMC::AccessPrivate, QMetaType::Void, {{
            { QMetaType::Int, 6 },
        }}),
        // Slot 'onrentPaymentRequired'
        QtMocHelpers::SlotData<void(const QString &, int, int, int)>(56, 2, QMC::AccessPrivate, QMetaType::Void, {{
            { QMetaType::QString, 52 }, { QMetaType::Int, 57 }, { QMetaType::Int, 58 }, { QMetaType::Int, 59 },
        }}),
        // Slot 'ontaxPaymentRequired'
        QtMocHelpers::SlotData<void(const QString &, int, int)>(60, 2, QMC::AccessPrivate, QMetaType::Void, {{
            { QMetaType::QString, 61 }, { QMetaType::Int, 62 }, { QMetaType::Int, 58 },
        }}),
        // Slot 'onLandOnSelfProperty'
        QtMocHelpers::SlotData<void(int, const QString &)>(63, 2, QMC::AccessPrivate, QMetaType::Void, {{
            { QMetaType::Int, 6 }, { QMetaType::QString, 52 },
        }}),
        // Slot 'onOpenChanceCard'
        QtMocHelpers::SlotData<void(int, const QString &)>(64, 2, QMC::AccessPrivate, QMetaType::Void, {{
            { QMetaType::Int, 6 }, { QMetaType::QString, 65 },
        }}),
        // Slot 'onOpenCommunityCard'
        QtMocHelpers::SlotData<void(int, const QString &)>(66, 2, QMC::AccessPrivate, QMetaType::Void, {{
            { QMetaType::Int, 6 }, { QMetaType::QString, 65 },
        }}),
        // Slot 'onLandOnFreeParking'
        QtMocHelpers::SlotData<void(int, int)>(67, 2, QMC::AccessPrivate, QMetaType::Void, {{
            { QMetaType::Int, 6 }, { QMetaType::Int, 68 },
        }}),
        // Slot 'onLandOnGoTile'
        QtMocHelpers::SlotData<void(int)>(69, 2, QMC::AccessPrivate, QMetaType::Void, {{
            { QMetaType::Int, 6 },
        }}),
        // Slot 'onLandOnGoToJailTile'
        QtMocHelpers::SlotData<void(int)>(70, 2, QMC::AccessPrivate, QMetaType::Void, {{
            { QMetaType::Int, 6 },
        }}),
        // Slot 'onEnableEndTurnAndDisableRoll'
        QtMocHelpers::SlotData<void()>(71, 2, QMC::AccessPrivate, QMetaType::Void),
    };
    QtMocHelpers::UintData qt_properties {
    };
    QtMocHelpers::UintData qt_enums {
    };
    return QtMocHelpers::metaObjectData<MainWindow, qt_meta_tag_ZN10MainWindowE_t>(QMC::MetaObjectFlag{}, qt_stringData,
            qt_methods, qt_properties, qt_enums);
}
Q_CONSTINIT const QMetaObject MainWindow::staticMetaObject = { {
    QMetaObject::SuperData::link<QMainWindow::staticMetaObject>(),
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN10MainWindowE_t>.stringdata,
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN10MainWindowE_t>.data,
    qt_static_metacall,
    nullptr,
    qt_staticMetaObjectRelocatingContent<qt_meta_tag_ZN10MainWindowE_t>.metaTypes,
    nullptr
} };

void MainWindow::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    auto *_t = static_cast<MainWindow *>(_o);
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: _t->onRoundLabelChanged(); break;
        case 1: _t->onMoneyChanged(); break;
        case 2: _t->onRollDiceClicked(); break;
        case 3: _t->onPropertiesClicked((*reinterpret_cast<std::add_pointer_t<int>>(_a[1])),(*reinterpret_cast<std::add_pointer_t<bool>>(_a[2])),(*reinterpret_cast<std::add_pointer_t<int>>(_a[3]))); break;
        case 4: _t->onPropertiesClicked((*reinterpret_cast<std::add_pointer_t<int>>(_a[1])),(*reinterpret_cast<std::add_pointer_t<bool>>(_a[2]))); break;
        case 5: _t->onPropertiesClicked((*reinterpret_cast<std::add_pointer_t<int>>(_a[1]))); break;
        case 6: _t->onPropertiesClicked(); break;
        case 7: _t->onForceRaiseMoney((*reinterpret_cast<std::add_pointer_t<int>>(_a[1])),(*reinterpret_cast<std::add_pointer_t<int>>(_a[2]))); break;
        case 8: _t->updatePropertyDisplay((*reinterpret_cast<std::add_pointer_t<QLabel*>>(_a[1])),(*reinterpret_cast<std::add_pointer_t<QLabel*>>(_a[2])),(*reinterpret_cast<std::add_pointer_t<PropertyTile*>>(_a[3])),(*reinterpret_cast<std::add_pointer_t<int>>(_a[4]))); break;
        case 9: _t->showWarning((*reinterpret_cast<std::add_pointer_t<QString>>(_a[1]))); break;
        case 10: _t->onTradeClicked(); break;
        case 11: _t->onSellTrade(); break;
        case 12: _t->onBuyTrade(); break;
        case 13: _t->modifyTradeSelectionWidget((*reinterpret_cast<std::add_pointer_t<Player>>(_a[1])),(*reinterpret_cast<std::add_pointer_t<std::vector<PropertyTile*>>>(_a[2])),(*reinterpret_cast<std::add_pointer_t<QDialog&>>(_a[3])),(*reinterpret_cast<std::add_pointer_t<QWidget*>>(_a[4])),(*reinterpret_cast<std::add_pointer_t<QVBoxLayout*>>(_a[5]))); break;
        case 14: _t->askTradeDecision((*reinterpret_cast<std::add_pointer_t<Player*>>(_a[1])),(*reinterpret_cast<std::add_pointer_t<Player*>>(_a[2])),(*reinterpret_cast<std::add_pointer_t<int>>(_a[3])),(*reinterpret_cast<std::add_pointer_t<bool>>(_a[4])),(*reinterpret_cast<std::add_pointer_t<PropertyTile*>>(_a[5])),(*reinterpret_cast<std::add_pointer_t<bool>>(_a[6]))); break;
        case 15: _t->onHintClicked((*reinterpret_cast<std::add_pointer_t<int>>(_a[1]))); break;
        case 16: _t->onChatroomClicked(); break;
        case 17: _t->onEndTurnClicked(); break;
        case 18: _t->onPlayerTurnStarted((*reinterpret_cast<std::add_pointer_t<int>>(_a[1]))); break;
        case 19: _t->onPlayerTurnEnded((*reinterpret_cast<std::add_pointer_t<int>>(_a[1]))); break;
        case 20: _t->onDiceRolled((*reinterpret_cast<std::add_pointer_t<int>>(_a[1])),(*reinterpret_cast<std::add_pointer_t<int>>(_a[2]))); break;
        case 21: _t->onPlayerMoved((*reinterpret_cast<std::add_pointer_t<int>>(_a[1])),(*reinterpret_cast<std::add_pointer_t<int>>(_a[2])),(*reinterpret_cast<std::add_pointer_t<int>>(_a[3]))); break;
        case 22: _t->onPurchaseOpportunity((*reinterpret_cast<std::add_pointer_t<QString>>(_a[1])),(*reinterpret_cast<std::add_pointer_t<int>>(_a[2])),(*reinterpret_cast<std::add_pointer_t<int>>(_a[3]))); break;
        case 23: _t->onGameLogMessage((*reinterpret_cast<std::add_pointer_t<QString>>(_a[1]))); break;
        case 24: _t->onPlayerBankrupt((*reinterpret_cast<std::add_pointer_t<int>>(_a[1]))); break;
        case 25: _t->onrentPaymentRequired((*reinterpret_cast<std::add_pointer_t<QString>>(_a[1])),(*reinterpret_cast<std::add_pointer_t<int>>(_a[2])),(*reinterpret_cast<std::add_pointer_t<int>>(_a[3])),(*reinterpret_cast<std::add_pointer_t<int>>(_a[4]))); break;
        case 26: _t->ontaxPaymentRequired((*reinterpret_cast<std::add_pointer_t<QString>>(_a[1])),(*reinterpret_cast<std::add_pointer_t<int>>(_a[2])),(*reinterpret_cast<std::add_pointer_t<int>>(_a[3]))); break;
        case 27: _t->onLandOnSelfProperty((*reinterpret_cast<std::add_pointer_t<int>>(_a[1])),(*reinterpret_cast<std::add_pointer_t<QString>>(_a[2]))); break;
        case 28: _t->onOpenChanceCard((*reinterpret_cast<std::add_pointer_t<int>>(_a[1])),(*reinterpret_cast<std::add_pointer_t<QString>>(_a[2]))); break;
        case 29: _t->onOpenCommunityCard((*reinterpret_cast<std::add_pointer_t<int>>(_a[1])),(*reinterpret_cast<std::add_pointer_t<QString>>(_a[2]))); break;
        case 30: _t->onLandOnFreeParking((*reinterpret_cast<std::add_pointer_t<int>>(_a[1])),(*reinterpret_cast<std::add_pointer_t<int>>(_a[2]))); break;
        case 31: _t->onLandOnGoTile((*reinterpret_cast<std::add_pointer_t<int>>(_a[1]))); break;
        case 32: _t->onLandOnGoToJailTile((*reinterpret_cast<std::add_pointer_t<int>>(_a[1]))); break;
        case 33: _t->onEnableEndTurnAndDisableRoll(); break;
        default: ;
        }
    }
    if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        switch (_id) {
        default: *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType(); break;
        case 8:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType(); break;
            case 1:
            case 0:
                *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType::fromType< QLabel* >(); break;
            }
            break;
        case 13:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType(); break;
            case 4:
                *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType::fromType< QVBoxLayout* >(); break;
            case 3:
                *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType::fromType< QWidget* >(); break;
            }
            break;
        }
    }
}

const QMetaObject *MainWindow::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *MainWindow::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_staticMetaObjectStaticContent<qt_meta_tag_ZN10MainWindowE_t>.strings))
        return static_cast<void*>(this);
    return QMainWindow::qt_metacast(_clname);
}

int MainWindow::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QMainWindow::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 34)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 34;
    }
    if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 34)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 34;
    }
    return _id;
}
QT_WARNING_POP
