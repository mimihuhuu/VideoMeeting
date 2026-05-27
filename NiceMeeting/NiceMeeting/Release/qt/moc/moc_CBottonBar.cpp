/****************************************************************************
** Meta object code from reading C++ file 'CBottonBar.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.14.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "../../../../CBottonBar.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'CBottonBar.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.14.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_CBottonBar_t {
    QByteArrayData data[13];
    char stringdata0[166];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_CBottonBar_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_CBottonBar_t qt_meta_stringdata_CBottonBar = {
    {
QT_MOC_LITERAL(0, 0, 10), // "CBottonBar"
QT_MOC_LITERAL(1, 11, 14), // "sig_EndMeeting"
QT_MOC_LITERAL(2, 26, 0), // ""
QT_MOC_LITERAL(3, 27, 15), // "sig_enableVideo"
QT_MOC_LITERAL(4, 43, 7), // "enabled"
QT_MOC_LITERAL(5, 51, 15), // "sig_enableAudio"
QT_MOC_LITERAL(6, 67, 15), // "sig_shareScreen"
QT_MOC_LITERAL(7, 83, 14), // "sig_connectMic"
QT_MOC_LITERAL(8, 98, 10), // "sig_invite"
QT_MOC_LITERAL(9, 109, 17), // "sig_manageMembers"
QT_MOC_LITERAL(10, 127, 8), // "sig_chat"
QT_MOC_LITERAL(11, 136, 16), // "sig_recordScreen"
QT_MOC_LITERAL(12, 153, 12) // "sig_settings"

    },
    "CBottonBar\0sig_EndMeeting\0\0sig_enableVideo\0"
    "enabled\0sig_enableAudio\0sig_shareScreen\0"
    "sig_connectMic\0sig_invite\0sig_manageMembers\0"
    "sig_chat\0sig_recordScreen\0sig_settings"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_CBottonBar[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
      10,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
      10,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    0,   64,    2, 0x06 /* Public */,
       3,    1,   65,    2, 0x06 /* Public */,
       5,    1,   68,    2, 0x06 /* Public */,
       6,    0,   71,    2, 0x06 /* Public */,
       7,    1,   72,    2, 0x06 /* Public */,
       8,    0,   75,    2, 0x06 /* Public */,
       9,    0,   76,    2, 0x06 /* Public */,
      10,    0,   77,    2, 0x06 /* Public */,
      11,    0,   78,    2, 0x06 /* Public */,
      12,    0,   79,    2, 0x06 /* Public */,

 // signals: parameters
    QMetaType::Void,
    QMetaType::Void, QMetaType::Bool,    4,
    QMetaType::Void, QMetaType::Bool,    4,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Bool,    4,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void CBottonBar::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<CBottonBar *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->sig_EndMeeting(); break;
        case 1: _t->sig_enableVideo((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 2: _t->sig_enableAudio((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 3: _t->sig_shareScreen(); break;
        case 4: _t->sig_connectMic((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 5: _t->sig_invite(); break;
        case 6: _t->sig_manageMembers(); break;
        case 7: _t->sig_chat(); break;
        case 8: _t->sig_recordScreen(); break;
        case 9: _t->sig_settings(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (CBottonBar::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&CBottonBar::sig_EndMeeting)) {
                *result = 0;
                return;
            }
        }
        {
            using _t = void (CBottonBar::*)(bool );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&CBottonBar::sig_enableVideo)) {
                *result = 1;
                return;
            }
        }
        {
            using _t = void (CBottonBar::*)(bool );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&CBottonBar::sig_enableAudio)) {
                *result = 2;
                return;
            }
        }
        {
            using _t = void (CBottonBar::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&CBottonBar::sig_shareScreen)) {
                *result = 3;
                return;
            }
        }
        {
            using _t = void (CBottonBar::*)(bool );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&CBottonBar::sig_connectMic)) {
                *result = 4;
                return;
            }
        }
        {
            using _t = void (CBottonBar::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&CBottonBar::sig_invite)) {
                *result = 5;
                return;
            }
        }
        {
            using _t = void (CBottonBar::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&CBottonBar::sig_manageMembers)) {
                *result = 6;
                return;
            }
        }
        {
            using _t = void (CBottonBar::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&CBottonBar::sig_chat)) {
                *result = 7;
                return;
            }
        }
        {
            using _t = void (CBottonBar::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&CBottonBar::sig_recordScreen)) {
                *result = 8;
                return;
            }
        }
        {
            using _t = void (CBottonBar::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&CBottonBar::sig_settings)) {
                *result = 9;
                return;
            }
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject CBottonBar::staticMetaObject = { {
    QMetaObject::SuperData::link<QWidget::staticMetaObject>(),
    qt_meta_stringdata_CBottonBar.data,
    qt_meta_data_CBottonBar,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *CBottonBar::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *CBottonBar::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_CBottonBar.stringdata0))
        return static_cast<void*>(this);
    return QWidget::qt_metacast(_clname);
}

int CBottonBar::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 10)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 10;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 10)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 10;
    }
    return _id;
}

// SIGNAL 0
void CBottonBar::sig_EndMeeting()
{
    QMetaObject::activate(this, &staticMetaObject, 0, nullptr);
}

// SIGNAL 1
void CBottonBar::sig_enableVideo(bool _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void CBottonBar::sig_enableAudio(bool _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}

// SIGNAL 3
void CBottonBar::sig_shareScreen()
{
    QMetaObject::activate(this, &staticMetaObject, 3, nullptr);
}

// SIGNAL 4
void CBottonBar::sig_connectMic(bool _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 4, _a);
}

// SIGNAL 5
void CBottonBar::sig_invite()
{
    QMetaObject::activate(this, &staticMetaObject, 5, nullptr);
}

// SIGNAL 6
void CBottonBar::sig_manageMembers()
{
    QMetaObject::activate(this, &staticMetaObject, 6, nullptr);
}

// SIGNAL 7
void CBottonBar::sig_chat()
{
    QMetaObject::activate(this, &staticMetaObject, 7, nullptr);
}

// SIGNAL 8
void CBottonBar::sig_recordScreen()
{
    QMetaObject::activate(this, &staticMetaObject, 8, nullptr);
}

// SIGNAL 9
void CBottonBar::sig_settings()
{
    QMetaObject::activate(this, &staticMetaObject, 9, nullptr);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
