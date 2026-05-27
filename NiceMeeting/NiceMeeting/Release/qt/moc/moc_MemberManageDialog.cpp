/****************************************************************************
** Meta object code from reading C++ file 'MemberManageDialog.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.14.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "../../../../MemberManageDialog.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'MemberManageDialog.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.14.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_MemberManageDialog_t {
    QByteArrayData data[11];
    char stringdata0[159];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_MemberManageDialog_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_MemberManageDialog_t qt_meta_stringdata_MemberManageDialog = {
    {
QT_MOC_LITERAL(0, 0, 18), // "MemberManageDialog"
QT_MOC_LITERAL(1, 19, 12), // "sig_kickUser"
QT_MOC_LITERAL(2, 32, 0), // ""
QT_MOC_LITERAL(3, 33, 3), // "uid"
QT_MOC_LITERAL(4, 37, 20), // "sig_blockRemoteVideo"
QT_MOC_LITERAL(5, 58, 20), // "sig_blockRemoteAudio"
QT_MOC_LITERAL(6, 79, 17), // "sig_muteUserAudio"
QT_MOC_LITERAL(7, 97, 14), // "onKickSelected"
QT_MOC_LITERAL(8, 112, 18), // "onBlockRemoteVideo"
QT_MOC_LITERAL(9, 131, 11), // "onMuteAudio"
QT_MOC_LITERAL(10, 143, 15) // "onMuteRemoteMic"

    },
    "MemberManageDialog\0sig_kickUser\0\0uid\0"
    "sig_blockRemoteVideo\0sig_blockRemoteAudio\0"
    "sig_muteUserAudio\0onKickSelected\0"
    "onBlockRemoteVideo\0onMuteAudio\0"
    "onMuteRemoteMic"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_MemberManageDialog[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
       8,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       4,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,   54,    2, 0x06 /* Public */,
       4,    1,   57,    2, 0x06 /* Public */,
       5,    1,   60,    2, 0x06 /* Public */,
       6,    1,   63,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       7,    0,   66,    2, 0x08 /* Private */,
       8,    0,   67,    2, 0x08 /* Private */,
       9,    0,   68,    2, 0x08 /* Private */,
      10,    0,   69,    2, 0x08 /* Private */,

 // signals: parameters
    QMetaType::Void, QMetaType::UInt,    3,
    QMetaType::Void, QMetaType::UInt,    3,
    QMetaType::Void, QMetaType::UInt,    3,
    QMetaType::Void, QMetaType::UInt,    3,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void MemberManageDialog::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<MemberManageDialog *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->sig_kickUser((*reinterpret_cast< uint(*)>(_a[1]))); break;
        case 1: _t->sig_blockRemoteVideo((*reinterpret_cast< uint(*)>(_a[1]))); break;
        case 2: _t->sig_blockRemoteAudio((*reinterpret_cast< uint(*)>(_a[1]))); break;
        case 3: _t->sig_muteUserAudio((*reinterpret_cast< uint(*)>(_a[1]))); break;
        case 4: _t->onKickSelected(); break;
        case 5: _t->onBlockRemoteVideo(); break;
        case 6: _t->onMuteAudio(); break;
        case 7: _t->onMuteRemoteMic(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (MemberManageDialog::*)(unsigned int );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&MemberManageDialog::sig_kickUser)) {
                *result = 0;
                return;
            }
        }
        {
            using _t = void (MemberManageDialog::*)(unsigned int );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&MemberManageDialog::sig_blockRemoteVideo)) {
                *result = 1;
                return;
            }
        }
        {
            using _t = void (MemberManageDialog::*)(unsigned int );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&MemberManageDialog::sig_blockRemoteAudio)) {
                *result = 2;
                return;
            }
        }
        {
            using _t = void (MemberManageDialog::*)(unsigned int );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&MemberManageDialog::sig_muteUserAudio)) {
                *result = 3;
                return;
            }
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject MemberManageDialog::staticMetaObject = { {
    QMetaObject::SuperData::link<MeetingFramelessDialog::staticMetaObject>(),
    qt_meta_stringdata_MemberManageDialog.data,
    qt_meta_data_MemberManageDialog,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *MemberManageDialog::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *MemberManageDialog::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_MemberManageDialog.stringdata0))
        return static_cast<void*>(this);
    return MeetingFramelessDialog::qt_metacast(_clname);
}

int MemberManageDialog::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = MeetingFramelessDialog::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 8)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 8;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 8)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 8;
    }
    return _id;
}

// SIGNAL 0
void MemberManageDialog::sig_kickUser(unsigned int _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void MemberManageDialog::sig_blockRemoteVideo(unsigned int _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void MemberManageDialog::sig_blockRemoteAudio(unsigned int _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}

// SIGNAL 3
void MemberManageDialog::sig_muteUserAudio(unsigned int _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 3, _a);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
