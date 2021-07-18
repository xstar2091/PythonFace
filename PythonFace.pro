QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

RC_ICONS = icon.ico
#版本号
VERSION = 0.2.0
#中文
RC_LANG = 0x0004
# 公司名
QMAKE_TARGET_COMPANY = xstar的工具箱
# 产品名称
QMAKE_TARGET_PRODUCT = python script executor
# 详细描述
QMAKE_TARGET_DESCRIPTION = python script executor
# 版权
QMAKE_TARGET_COPYRIGHT = Copyright(C) 2021-2021 the xstar Ltd

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    command.cpp \
    commandrunner.cpp \
    main.cpp \
    maindialog.cpp \
    placeholderdialog.cpp \
    variable/impl/autovariable.cpp \
    variable/impl/fixvariable.cpp \
    variable/impl/variablebase.cpp \
    variable/impl/variablemanagerimpl.cpp \
    variable/variablemanager.cpp

HEADERS += \
    command.h \
    commandrunner.h \
    maindialog.h \
    placeholderdialog.h \
    variable/impl/autovariable.h \
    variable/impl/fixvariable.h \
    variable/impl/variablebase.h \
    variable/impl/variablemanagerimpl.h \
    variable/variablemanager.h

FORMS += \
    maindialog.ui \
    placeholderdialog.ui

TRANSLATIONS += \
    PythonFace_zh_CN.ts
CONFIG += lrelease
CONFIG += embed_translations

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
