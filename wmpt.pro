QT       += core gui network sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    admin_ban.cpp \
    admin_delete_comment.cpp \
    admin_send_ad.cpp \
    commentwindow.cpp \
    customer_main.cpp \
    edit_menu.cpp \
    edit_password.cpp \
    edit_rest_message.cpp \
    main.cpp \
    login.cpp \
    paymentwindow.cpp \
    reg.cpp \
    restaurant_main.cpp \
    select_dish.cpp \
    select_dish_2.cpp \
    select_rest.cpp \
    select_rest_2.cpp \
    superuser_main.cpp \
    view_income.cpp \
    view_order.cpp \
    view_proc_order.cpp

HEADERS += \
    admin_ban.h \
    admin_delete_comment.h \
    admin_send_ad.h \
    commentwindow.h \
    customer_main.h \
    edit_menu.h \
    edit_password.h \
    edit_rest_message.h \
    login.h \
    paymentwindow.h \
    reg.h \
    restaurant_main.h \
    select_dish.h \
    select_dish_2.h \
    select_rest.h \
    select_rest_2.h \
    superuser_main.h \
    view_income.h \
    view_order.h \
    view_proc_order.h

FORMS += \
    admin_ban.ui \
    admin_delete_comment.ui \
    admin_send_ad.ui \
    commentwindow.ui \
    customer_main.ui \
    edit_menu.ui \
    edit_password.ui \
    edit_rest_message.ui \
    login.ui \
    paymentwindow.ui \
    reg.ui \
    restaurant_main.ui \
    select_dish.ui \
    select_dish_2.ui \
    select_rest.ui \
    select_rest_2.ui \
    superuser_main.ui \
    view_income.ui \
    view_order.ui \
    view_proc_order.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    Icon.qrc
