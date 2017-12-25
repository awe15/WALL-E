/****************************************************************************
** Meta object code from reading C++ file 'monitor.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.5.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../monitor.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'monitor.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.5.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_monitor_t {
    QByteArrayData data[21];
    char stringdata0[270];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_monitor_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_monitor_t qt_meta_stringdata_monitor = {
    {
QT_MOC_LITERAL(0, 0, 7), // "monitor"
QT_MOC_LITERAL(1, 8, 10), // "on_openBtn"
QT_MOC_LITERAL(2, 19, 0), // ""
QT_MOC_LITERAL(3, 20, 11), // "on_closeBtn"
QT_MOC_LITERAL(4, 32, 10), // "on_linkBtn"
QT_MOC_LITERAL(5, 43, 9), // "timerDone"
QT_MOC_LITERAL(6, 53, 12), // "on_speed0Btn"
QT_MOC_LITERAL(7, 66, 12), // "on_speed1Btn"
QT_MOC_LITERAL(8, 79, 12), // "on_speed2Btn"
QT_MOC_LITERAL(9, 92, 12), // "on_speed3Btn"
QT_MOC_LITERAL(10, 105, 12), // "on_speed4Btn"
QT_MOC_LITERAL(11, 118, 12), // "on_speed5Btn"
QT_MOC_LITERAL(12, 131, 15), // "moveAngleChange"
QT_MOC_LITERAL(13, 147, 13), // "moveDirChange"
QT_MOC_LITERAL(14, 161, 12), // "on_getPWMBtn"
QT_MOC_LITERAL(15, 174, 12), // "on_setPWMBtn"
QT_MOC_LITERAL(16, 187, 17), // "on_camera1OpenBtn"
QT_MOC_LITERAL(17, 205, 17), // "on_camera2OpenBtn"
QT_MOC_LITERAL(18, 223, 16), // "on_cameraOpenBtn"
QT_MOC_LITERAL(19, 240, 17), // "on_cameraCloseBtn"
QT_MOC_LITERAL(20, 258, 11) // "on_port2Btn"

    },
    "monitor\0on_openBtn\0\0on_closeBtn\0"
    "on_linkBtn\0timerDone\0on_speed0Btn\0"
    "on_speed1Btn\0on_speed2Btn\0on_speed3Btn\0"
    "on_speed4Btn\0on_speed5Btn\0moveAngleChange\0"
    "moveDirChange\0on_getPWMBtn\0on_setPWMBtn\0"
    "on_camera1OpenBtn\0on_camera2OpenBtn\0"
    "on_cameraOpenBtn\0on_cameraCloseBtn\0"
    "on_port2Btn"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_monitor[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
      19,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    0,  109,    2, 0x0a /* Public */,
       3,    0,  110,    2, 0x0a /* Public */,
       4,    0,  111,    2, 0x0a /* Public */,
       5,    0,  112,    2, 0x0a /* Public */,
       6,    0,  113,    2, 0x0a /* Public */,
       7,    0,  114,    2, 0x0a /* Public */,
       8,    0,  115,    2, 0x0a /* Public */,
       9,    0,  116,    2, 0x0a /* Public */,
      10,    0,  117,    2, 0x0a /* Public */,
      11,    0,  118,    2, 0x0a /* Public */,
      12,    0,  119,    2, 0x0a /* Public */,
      13,    0,  120,    2, 0x0a /* Public */,
      14,    0,  121,    2, 0x0a /* Public */,
      15,    0,  122,    2, 0x0a /* Public */,
      16,    0,  123,    2, 0x0a /* Public */,
      17,    0,  124,    2, 0x0a /* Public */,
      18,    0,  125,    2, 0x0a /* Public */,
      19,    0,  126,    2, 0x0a /* Public */,
      20,    0,  127,    2, 0x0a /* Public */,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void monitor::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        monitor *_t = static_cast<monitor *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->on_openBtn(); break;
        case 1: _t->on_closeBtn(); break;
        case 2: _t->on_linkBtn(); break;
        case 3: _t->timerDone(); break;
        case 4: _t->on_speed0Btn(); break;
        case 5: _t->on_speed1Btn(); break;
        case 6: _t->on_speed2Btn(); break;
        case 7: _t->on_speed3Btn(); break;
        case 8: _t->on_speed4Btn(); break;
        case 9: _t->on_speed5Btn(); break;
        case 10: _t->moveAngleChange(); break;
        case 11: _t->moveDirChange(); break;
        case 12: _t->on_getPWMBtn(); break;
        case 13: _t->on_setPWMBtn(); break;
        case 14: _t->on_camera1OpenBtn(); break;
        case 15: _t->on_camera2OpenBtn(); break;
        case 16: _t->on_cameraOpenBtn(); break;
        case 17: _t->on_cameraCloseBtn(); break;
        case 18: _t->on_port2Btn(); break;
        default: ;
        }
    }
    Q_UNUSED(_a);
}

const QMetaObject monitor::staticMetaObject = {
    { &QMainWindow::staticMetaObject, qt_meta_stringdata_monitor.data,
      qt_meta_data_monitor,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *monitor::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *monitor::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_monitor.stringdata0))
        return static_cast<void*>(const_cast< monitor*>(this));
    return QMainWindow::qt_metacast(_clname);
}

int monitor::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QMainWindow::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 19)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 19;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 19)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 19;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
