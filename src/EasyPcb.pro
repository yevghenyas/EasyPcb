#-------------------------------------------------
#
# Project created by QtCreator 2018-06-27T11:06:43
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = EasyPcb
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0
QMAKE_CXXFLAGS += -std=c++1y
QMAKE_CXXFLAGS += -static
QT += printsupport


SOURCES += \
    genpackwizard.cpp \
    genpackwizpinsdescpage.cpp \
    genpackwizstartpage.cpp \
    genwizdipgeompage.cpp \
        main.cpp \
        mainwindow.cpp \
    addneweltolibdlg.cpp \
    capgraphicalitem.cpp \
    custombutton.cpp \
    drawwrapper.cpp \
    filewriter.cpp \
    genericgraphicalitemscontainer.cpp \
    graphicalitem.cpp \
    graphicalitempropsdlg.cpp \
    graphicalitemscontainer.cpp \
    itemsfactory.cpp \
    linegraphicalitem.cpp \
    mywidget.cpp \
    packagegraphicalitem.cpp \
    previewlabel.cpp \
    printpreviewwidget.cpp \
    rectgraphicalitem.cpp \
    rectpackagegraphicalitem.cpp \
    roundpackagegraphicalitem.cpp \
    roundplategraphicalitem.cpp \
    ruler.cpp \
    setprintpropsdlg.cpp \
    filereader.cpp \
    additemcommand.cpp \
    deleteitemcommand.cpp \
    movebetweenlayerscommand.cpp \
    setpropscommand.cpp \
    ungroupcomand.cpp \
    moveitemcommand.cpp \
    movenodecommand.cpp \
    addnodecommand.cpp \
    groupcommand.cpp \
    deletenodecommand.cpp \
    rotateitemcommand.cpp \
    setboardprops.cpp \
    addvirtconitem.cpp \
    boardlayer.cpp \
    easypcb.cpp \
    pcboard.cpp \
    pcboardpropsdlg.cpp \
    textgraphicalitem.cpp \
    mousetrace.cpp \
    connectorgraphicalitem.cpp \
    connectorwrapper.cpp \
    boardlayerswrapper.cpp \
    autodlg.cpp \
    pcbautoconstructor.cpp \
    autoconstructprops.cpp \
    editmultiplate.cpp \
    layerscombo.cpp \
    multiplategraphicalitem.cpp \
    autocommanditem.cpp \
    componentstreemodel.cpp \
    componentstreeview.cpp \
    packagewrapper.cpp \
    gerbergenerator.cpp \
    editexporttogerberprops.cpp \
    excellongenerator.cpp

HEADERS += \
    genpackwizard.h \
    genpackwizpinsdescpage.h \
    genpackwizstartpage.h \
    genwizdipgeompage.h \
        mainwindow.h \
    addneweltolibdlg.h \
    capgraphicalitem.h \
    common.h \
    custombutton.h \
    drawwrapper.h \
    filereader.h \
    filewriter.h \
    genericgraphicalitemscontainer.h \
    graphicalitem.h \
    graphicalitempropsdlg.h \
    graphicalitemscontainer.h \
    itemsfactory.h \
    linegraphicalitem.h \
    mywidget.h \
    packagegraphicalitem.h \
    pagedata.h \
    previewlabel.h \
    printpreviewwidget.h \
    rectgraphicalitem.h \
    rectpackagegraphicalitem.h \
    roundpackagegraphicalitem.h \
    roundplategraphicalitem.h \
    ruler.h \
    setprintpropsdlg.h \
    additemcommand.h \
    deleteitemcommand.h \
    movebetweenlayerscommand.h \
    setpropscommand.h \
    ungroupcomand.h \
    moveitemcommand.h \
    movenodecommand.h \
    addnodecommand.h \
    groupcommand.h \
    deletenodecommand.h \
    rotateitemcommand.h \
    setboardprops.h \
    addvirtconitem.h \
    smartptr.h \
    boardlayer.h \
    easypcb.h \
    pcboard.h \
    pcboardpropsdlg.h \
    textgraphicalitem.h \
    mousetrace.h \
    connectorgraphicalitem.h \
    connectorwrapper.h \
    boardlayerswrapper.h \
    autodlg.h \
    pcbautoconstructor.h \
    autoconstructprops.h \
    editmultiplate.h \
    layerscombo.h \
    multiplategraphicalitem.h \
    autocommanditem.h \
    tree_data.h \
    componentstreemodel.h \
    componentstreeview.h \
    packagewrapper.h \
    gerbergenerator.h \
    editexporttogerberprops.h \
    excellongenerator.h

RESOURCES += \
    easypcb.qrc

