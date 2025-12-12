/****************************************************************************
** Meta object code from reading C++ file 'Tile.h'
**
** Created by: The Qt Meta Object Compiler version 69 (Qt 6.10.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../../../Tile.h"
#include <QtCore/qmetatype.h>

#include <QtCore/qtmochelpers.h>

#include <memory>


#include <QtCore/qxptype_traits.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'Tile.h' doesn't include <QObject>."
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
struct qt_meta_tag_ZN4TileE_t {};
} // unnamed namespace

template <> constexpr inline auto Tile::qt_create_metaobjectdata<qt_meta_tag_ZN4TileE_t>()
{
    namespace QMC = QtMocConstants;
    QtMocHelpers::StringRefStorage qt_stringData {
        "Tile"
    };

    QtMocHelpers::UintData qt_methods {
    };
    QtMocHelpers::UintData qt_properties {
    };
    QtMocHelpers::UintData qt_enums {
    };
    return QtMocHelpers::metaObjectData<Tile, qt_meta_tag_ZN4TileE_t>(QMC::MetaObjectFlag{}, qt_stringData,
            qt_methods, qt_properties, qt_enums);
}
Q_CONSTINIT const QMetaObject Tile::staticMetaObject = { {
    QMetaObject::SuperData::link<QObject::staticMetaObject>(),
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN4TileE_t>.stringdata,
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN4TileE_t>.data,
    qt_static_metacall,
    nullptr,
    qt_staticMetaObjectRelocatingContent<qt_meta_tag_ZN4TileE_t>.metaTypes,
    nullptr
} };

void Tile::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    auto *_t = static_cast<Tile *>(_o);
    (void)_t;
    (void)_c;
    (void)_id;
    (void)_a;
}

const QMetaObject *Tile::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *Tile::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_staticMetaObjectStaticContent<qt_meta_tag_ZN4TileE_t>.strings))
        return static_cast<void*>(this);
    return QObject::qt_metacast(_clname);
}

int Tile::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    return _id;
}
namespace {
struct qt_meta_tag_ZN6GoTileE_t {};
} // unnamed namespace

template <> constexpr inline auto GoTile::qt_create_metaobjectdata<qt_meta_tag_ZN6GoTileE_t>()
{
    namespace QMC = QtMocConstants;
    QtMocHelpers::StringRefStorage qt_stringData {
        "GoTile",
        "passedGo",
        "",
        "playerName",
        "reward"
    };

    QtMocHelpers::UintData qt_methods {
        // Signal 'passedGo'
        QtMocHelpers::SignalData<void(const QString &, int)>(1, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::QString, 3 }, { QMetaType::Int, 4 },
        }}),
    };
    QtMocHelpers::UintData qt_properties {
    };
    QtMocHelpers::UintData qt_enums {
    };
    return QtMocHelpers::metaObjectData<GoTile, qt_meta_tag_ZN6GoTileE_t>(QMC::MetaObjectFlag{}, qt_stringData,
            qt_methods, qt_properties, qt_enums);
}
Q_CONSTINIT const QMetaObject GoTile::staticMetaObject = { {
    QMetaObject::SuperData::link<Tile::staticMetaObject>(),
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN6GoTileE_t>.stringdata,
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN6GoTileE_t>.data,
    qt_static_metacall,
    nullptr,
    qt_staticMetaObjectRelocatingContent<qt_meta_tag_ZN6GoTileE_t>.metaTypes,
    nullptr
} };

void GoTile::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    auto *_t = static_cast<GoTile *>(_o);
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: _t->passedGo((*reinterpret_cast<std::add_pointer_t<QString>>(_a[1])),(*reinterpret_cast<std::add_pointer_t<int>>(_a[2]))); break;
        default: ;
        }
    }
    if (_c == QMetaObject::IndexOfMethod) {
        if (QtMocHelpers::indexOfMethod<void (GoTile::*)(const QString & , int )>(_a, &GoTile::passedGo, 0))
            return;
    }
}

const QMetaObject *GoTile::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *GoTile::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_staticMetaObjectStaticContent<qt_meta_tag_ZN6GoTileE_t>.strings))
        return static_cast<void*>(this);
    return Tile::qt_metacast(_clname);
}

int GoTile::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = Tile::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 1)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 1;
    }
    if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 1)
            *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType();
        _id -= 1;
    }
    return _id;
}

// SIGNAL 0
void GoTile::passedGo(const QString & _t1, int _t2)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 0, nullptr, _t1, _t2);
}
namespace {
struct qt_meta_tag_ZN12PropertyTileE_t {};
} // unnamed namespace

template <> constexpr inline auto PropertyTile::qt_create_metaobjectdata<qt_meta_tag_ZN12PropertyTileE_t>()
{
    namespace QMC = QtMocConstants;
    QtMocHelpers::StringRefStorage qt_stringData {
        "PropertyTile",
        "propertyAlreadyOwned",
        "",
        "propertyName",
        "ownerName",
        "propertyMortgaged",
        "buildingOpportunity",
        "housePrice",
        "playerName"
    };

    QtMocHelpers::UintData qt_methods {
        // Signal 'propertyAlreadyOwned'
        QtMocHelpers::SignalData<void(const QString &, const QString &)>(1, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::QString, 3 }, { QMetaType::QString, 4 },
        }}),
        // Signal 'propertyMortgaged'
        QtMocHelpers::SignalData<void(const QString &, const QString &)>(5, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::QString, 3 }, { QMetaType::QString, 4 },
        }}),
        // Signal 'buildingOpportunity'
        QtMocHelpers::SignalData<void(const QString &, int, const QString &)>(6, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::QString, 3 }, { QMetaType::Int, 7 }, { QMetaType::QString, 8 },
        }}),
    };
    QtMocHelpers::UintData qt_properties {
    };
    QtMocHelpers::UintData qt_enums {
    };
    return QtMocHelpers::metaObjectData<PropertyTile, qt_meta_tag_ZN12PropertyTileE_t>(QMC::MetaObjectFlag{}, qt_stringData,
            qt_methods, qt_properties, qt_enums);
}
Q_CONSTINIT const QMetaObject PropertyTile::staticMetaObject = { {
    QMetaObject::SuperData::link<Tile::staticMetaObject>(),
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN12PropertyTileE_t>.stringdata,
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN12PropertyTileE_t>.data,
    qt_static_metacall,
    nullptr,
    qt_staticMetaObjectRelocatingContent<qt_meta_tag_ZN12PropertyTileE_t>.metaTypes,
    nullptr
} };

void PropertyTile::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    auto *_t = static_cast<PropertyTile *>(_o);
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: _t->propertyAlreadyOwned((*reinterpret_cast<std::add_pointer_t<QString>>(_a[1])),(*reinterpret_cast<std::add_pointer_t<QString>>(_a[2]))); break;
        case 1: _t->propertyMortgaged((*reinterpret_cast<std::add_pointer_t<QString>>(_a[1])),(*reinterpret_cast<std::add_pointer_t<QString>>(_a[2]))); break;
        case 2: _t->buildingOpportunity((*reinterpret_cast<std::add_pointer_t<QString>>(_a[1])),(*reinterpret_cast<std::add_pointer_t<int>>(_a[2])),(*reinterpret_cast<std::add_pointer_t<QString>>(_a[3]))); break;
        default: ;
        }
    }
    if (_c == QMetaObject::IndexOfMethod) {
        if (QtMocHelpers::indexOfMethod<void (PropertyTile::*)(const QString & , const QString & )>(_a, &PropertyTile::propertyAlreadyOwned, 0))
            return;
        if (QtMocHelpers::indexOfMethod<void (PropertyTile::*)(const QString & , const QString & )>(_a, &PropertyTile::propertyMortgaged, 1))
            return;
        if (QtMocHelpers::indexOfMethod<void (PropertyTile::*)(const QString & , int , const QString & )>(_a, &PropertyTile::buildingOpportunity, 2))
            return;
    }
}

const QMetaObject *PropertyTile::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *PropertyTile::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_staticMetaObjectStaticContent<qt_meta_tag_ZN12PropertyTileE_t>.strings))
        return static_cast<void*>(this);
    return Tile::qt_metacast(_clname);
}

int PropertyTile::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = Tile::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 3)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 3;
    }
    if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 3)
            *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType();
        _id -= 3;
    }
    return _id;
}

// SIGNAL 0
void PropertyTile::propertyAlreadyOwned(const QString & _t1, const QString & _t2)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 0, nullptr, _t1, _t2);
}

// SIGNAL 1
void PropertyTile::propertyMortgaged(const QString & _t1, const QString & _t2)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 1, nullptr, _t1, _t2);
}

// SIGNAL 2
void PropertyTile::buildingOpportunity(const QString & _t1, int _t2, const QString & _t3)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 2, nullptr, _t1, _t2, _t3);
}
namespace {
struct qt_meta_tag_ZN15FreeParkingTileE_t {};
} // unnamed namespace

template <> constexpr inline auto FreeParkingTile::qt_create_metaobjectdata<qt_meta_tag_ZN15FreeParkingTileE_t>()
{
    namespace QMC = QtMocConstants;
    QtMocHelpers::StringRefStorage qt_stringData {
        "FreeParkingTile",
        "freeParkingLanded",
        "",
        "playerName"
    };

    QtMocHelpers::UintData qt_methods {
        // Signal 'freeParkingLanded'
        QtMocHelpers::SignalData<void(const QString &)>(1, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::QString, 3 },
        }}),
    };
    QtMocHelpers::UintData qt_properties {
    };
    QtMocHelpers::UintData qt_enums {
    };
    return QtMocHelpers::metaObjectData<FreeParkingTile, qt_meta_tag_ZN15FreeParkingTileE_t>(QMC::MetaObjectFlag{}, qt_stringData,
            qt_methods, qt_properties, qt_enums);
}
Q_CONSTINIT const QMetaObject FreeParkingTile::staticMetaObject = { {
    QMetaObject::SuperData::link<Tile::staticMetaObject>(),
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN15FreeParkingTileE_t>.stringdata,
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN15FreeParkingTileE_t>.data,
    qt_static_metacall,
    nullptr,
    qt_staticMetaObjectRelocatingContent<qt_meta_tag_ZN15FreeParkingTileE_t>.metaTypes,
    nullptr
} };

void FreeParkingTile::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    auto *_t = static_cast<FreeParkingTile *>(_o);
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: _t->freeParkingLanded((*reinterpret_cast<std::add_pointer_t<QString>>(_a[1]))); break;
        default: ;
        }
    }
    if (_c == QMetaObject::IndexOfMethod) {
        if (QtMocHelpers::indexOfMethod<void (FreeParkingTile::*)(const QString & )>(_a, &FreeParkingTile::freeParkingLanded, 0))
            return;
    }
}

const QMetaObject *FreeParkingTile::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *FreeParkingTile::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_staticMetaObjectStaticContent<qt_meta_tag_ZN15FreeParkingTileE_t>.strings))
        return static_cast<void*>(this);
    return Tile::qt_metacast(_clname);
}

int FreeParkingTile::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = Tile::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 1)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 1;
    }
    if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 1)
            *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType();
        _id -= 1;
    }
    return _id;
}

// SIGNAL 0
void FreeParkingTile::freeParkingLanded(const QString & _t1)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 0, nullptr, _t1);
}
namespace {
struct qt_meta_tag_ZN7TaxTileE_t {};
} // unnamed namespace

template <> constexpr inline auto TaxTile::qt_create_metaobjectdata<qt_meta_tag_ZN7TaxTileE_t>()
{
    namespace QMC = QtMocConstants;
    QtMocHelpers::StringRefStorage qt_stringData {
        "TaxTile",
        "taxDue",
        "",
        "tileName",
        "taxAmount",
        "playerName"
    };

    QtMocHelpers::UintData qt_methods {
        // Signal 'taxDue'
        QtMocHelpers::SignalData<void(const QString &, int, const QString &)>(1, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::QString, 3 }, { QMetaType::Int, 4 }, { QMetaType::QString, 5 },
        }}),
    };
    QtMocHelpers::UintData qt_properties {
    };
    QtMocHelpers::UintData qt_enums {
    };
    return QtMocHelpers::metaObjectData<TaxTile, qt_meta_tag_ZN7TaxTileE_t>(QMC::MetaObjectFlag{}, qt_stringData,
            qt_methods, qt_properties, qt_enums);
}
Q_CONSTINIT const QMetaObject TaxTile::staticMetaObject = { {
    QMetaObject::SuperData::link<Tile::staticMetaObject>(),
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN7TaxTileE_t>.stringdata,
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN7TaxTileE_t>.data,
    qt_static_metacall,
    nullptr,
    qt_staticMetaObjectRelocatingContent<qt_meta_tag_ZN7TaxTileE_t>.metaTypes,
    nullptr
} };

void TaxTile::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    auto *_t = static_cast<TaxTile *>(_o);
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: _t->taxDue((*reinterpret_cast<std::add_pointer_t<QString>>(_a[1])),(*reinterpret_cast<std::add_pointer_t<int>>(_a[2])),(*reinterpret_cast<std::add_pointer_t<QString>>(_a[3]))); break;
        default: ;
        }
    }
    if (_c == QMetaObject::IndexOfMethod) {
        if (QtMocHelpers::indexOfMethod<void (TaxTile::*)(const QString & , int , const QString & )>(_a, &TaxTile::taxDue, 0))
            return;
    }
}

const QMetaObject *TaxTile::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *TaxTile::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_staticMetaObjectStaticContent<qt_meta_tag_ZN7TaxTileE_t>.strings))
        return static_cast<void*>(this);
    return Tile::qt_metacast(_clname);
}

int TaxTile::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = Tile::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 1)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 1;
    }
    if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 1)
            *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType();
        _id -= 1;
    }
    return _id;
}

// SIGNAL 0
void TaxTile::taxDue(const QString & _t1, int _t2, const QString & _t3)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 0, nullptr, _t1, _t2, _t3);
}
namespace {
struct qt_meta_tag_ZN12GoToJailTileE_t {};
} // unnamed namespace

template <> constexpr inline auto GoToJailTile::qt_create_metaobjectdata<qt_meta_tag_ZN12GoToJailTileE_t>()
{
    namespace QMC = QtMocConstants;
    QtMocHelpers::StringRefStorage qt_stringData {
        "GoToJailTile",
        "passedGo",
        "",
        "playerName",
        "reward"
    };

    QtMocHelpers::UintData qt_methods {
        // Signal 'passedGo'
        QtMocHelpers::SignalData<void(const QString &, int)>(1, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::QString, 3 }, { QMetaType::Int, 4 },
        }}),
    };
    QtMocHelpers::UintData qt_properties {
    };
    QtMocHelpers::UintData qt_enums {
    };
    return QtMocHelpers::metaObjectData<GoToJailTile, qt_meta_tag_ZN12GoToJailTileE_t>(QMC::MetaObjectFlag{}, qt_stringData,
            qt_methods, qt_properties, qt_enums);
}
Q_CONSTINIT const QMetaObject GoToJailTile::staticMetaObject = { {
    QMetaObject::SuperData::link<Tile::staticMetaObject>(),
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN12GoToJailTileE_t>.stringdata,
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN12GoToJailTileE_t>.data,
    qt_static_metacall,
    nullptr,
    qt_staticMetaObjectRelocatingContent<qt_meta_tag_ZN12GoToJailTileE_t>.metaTypes,
    nullptr
} };

void GoToJailTile::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    auto *_t = static_cast<GoToJailTile *>(_o);
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: _t->passedGo((*reinterpret_cast<std::add_pointer_t<QString>>(_a[1])),(*reinterpret_cast<std::add_pointer_t<int>>(_a[2]))); break;
        default: ;
        }
    }
    if (_c == QMetaObject::IndexOfMethod) {
        if (QtMocHelpers::indexOfMethod<void (GoToJailTile::*)(const QString & , int )>(_a, &GoToJailTile::passedGo, 0))
            return;
    }
}

const QMetaObject *GoToJailTile::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *GoToJailTile::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_staticMetaObjectStaticContent<qt_meta_tag_ZN12GoToJailTileE_t>.strings))
        return static_cast<void*>(this);
    return Tile::qt_metacast(_clname);
}

int GoToJailTile::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = Tile::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 1)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 1;
    }
    if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 1)
            *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType();
        _id -= 1;
    }
    return _id;
}

// SIGNAL 0
void GoToJailTile::passedGo(const QString & _t1, int _t2)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 0, nullptr, _t1, _t2);
}
namespace {
struct qt_meta_tag_ZN10ChanceTileE_t {};
} // unnamed namespace

template <> constexpr inline auto ChanceTile::qt_create_metaobjectdata<qt_meta_tag_ZN10ChanceTileE_t>()
{
    namespace QMC = QtMocConstants;
    QtMocHelpers::StringRefStorage qt_stringData {
        "ChanceTile",
        "cardDrawn",
        "",
        "cardMessage",
        "playerName"
    };

    QtMocHelpers::UintData qt_methods {
        // Signal 'cardDrawn'
        QtMocHelpers::SignalData<void(const QString &, const QString &)>(1, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::QString, 3 }, { QMetaType::QString, 4 },
        }}),
    };
    QtMocHelpers::UintData qt_properties {
    };
    QtMocHelpers::UintData qt_enums {
    };
    return QtMocHelpers::metaObjectData<ChanceTile, qt_meta_tag_ZN10ChanceTileE_t>(QMC::MetaObjectFlag{}, qt_stringData,
            qt_methods, qt_properties, qt_enums);
}
Q_CONSTINIT const QMetaObject ChanceTile::staticMetaObject = { {
    QMetaObject::SuperData::link<Tile::staticMetaObject>(),
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN10ChanceTileE_t>.stringdata,
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN10ChanceTileE_t>.data,
    qt_static_metacall,
    nullptr,
    qt_staticMetaObjectRelocatingContent<qt_meta_tag_ZN10ChanceTileE_t>.metaTypes,
    nullptr
} };

void ChanceTile::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    auto *_t = static_cast<ChanceTile *>(_o);
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: _t->cardDrawn((*reinterpret_cast<std::add_pointer_t<QString>>(_a[1])),(*reinterpret_cast<std::add_pointer_t<QString>>(_a[2]))); break;
        default: ;
        }
    }
    if (_c == QMetaObject::IndexOfMethod) {
        if (QtMocHelpers::indexOfMethod<void (ChanceTile::*)(const QString & , const QString & )>(_a, &ChanceTile::cardDrawn, 0))
            return;
    }
}

const QMetaObject *ChanceTile::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *ChanceTile::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_staticMetaObjectStaticContent<qt_meta_tag_ZN10ChanceTileE_t>.strings))
        return static_cast<void*>(this);
    return Tile::qt_metacast(_clname);
}

int ChanceTile::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = Tile::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 1)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 1;
    }
    if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 1)
            *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType();
        _id -= 1;
    }
    return _id;
}

// SIGNAL 0
void ChanceTile::cardDrawn(const QString & _t1, const QString & _t2)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 0, nullptr, _t1, _t2);
}
namespace {
struct qt_meta_tag_ZN18CommunityChestTileE_t {};
} // unnamed namespace

template <> constexpr inline auto CommunityChestTile::qt_create_metaobjectdata<qt_meta_tag_ZN18CommunityChestTileE_t>()
{
    namespace QMC = QtMocConstants;
    QtMocHelpers::StringRefStorage qt_stringData {
        "CommunityChestTile",
        "cardDrawn",
        "",
        "cardMessage",
        "playerName"
    };

    QtMocHelpers::UintData qt_methods {
        // Signal 'cardDrawn'
        QtMocHelpers::SignalData<void(const QString &, const QString &)>(1, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::QString, 3 }, { QMetaType::QString, 4 },
        }}),
    };
    QtMocHelpers::UintData qt_properties {
    };
    QtMocHelpers::UintData qt_enums {
    };
    return QtMocHelpers::metaObjectData<CommunityChestTile, qt_meta_tag_ZN18CommunityChestTileE_t>(QMC::MetaObjectFlag{}, qt_stringData,
            qt_methods, qt_properties, qt_enums);
}
Q_CONSTINIT const QMetaObject CommunityChestTile::staticMetaObject = { {
    QMetaObject::SuperData::link<Tile::staticMetaObject>(),
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN18CommunityChestTileE_t>.stringdata,
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN18CommunityChestTileE_t>.data,
    qt_static_metacall,
    nullptr,
    qt_staticMetaObjectRelocatingContent<qt_meta_tag_ZN18CommunityChestTileE_t>.metaTypes,
    nullptr
} };

void CommunityChestTile::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    auto *_t = static_cast<CommunityChestTile *>(_o);
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: _t->cardDrawn((*reinterpret_cast<std::add_pointer_t<QString>>(_a[1])),(*reinterpret_cast<std::add_pointer_t<QString>>(_a[2]))); break;
        default: ;
        }
    }
    if (_c == QMetaObject::IndexOfMethod) {
        if (QtMocHelpers::indexOfMethod<void (CommunityChestTile::*)(const QString & , const QString & )>(_a, &CommunityChestTile::cardDrawn, 0))
            return;
    }
}

const QMetaObject *CommunityChestTile::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *CommunityChestTile::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_staticMetaObjectStaticContent<qt_meta_tag_ZN18CommunityChestTileE_t>.strings))
        return static_cast<void*>(this);
    return Tile::qt_metacast(_clname);
}

int CommunityChestTile::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = Tile::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 1)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 1;
    }
    if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 1)
            *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType();
        _id -= 1;
    }
    return _id;
}

// SIGNAL 0
void CommunityChestTile::cardDrawn(const QString & _t1, const QString & _t2)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 0, nullptr, _t1, _t2);
}
QT_WARNING_POP
