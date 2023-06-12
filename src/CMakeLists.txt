cmake_minimum_required(VERSION 2.6)
project(EasyPcb)
find_package(Qt4 REQUIRED)

include_directories(${QT_INCLUDES} ${CMAKE_CURRENT_BINARY_DIR})

set(EasyPcb_SRCS EasyPcb.cpp main.cpp mywidget.cpp testboard.cpp custombutton.cpp graphicalitem.cpp roundplategraphicalitem.cpp linegraphicalitem.cpp ruler.cpp statehandler.cpp dipchipgraphicalitem.cpp graphicalitemscontainer.cpp rectgraphicalitem.cpp mainwindow.cpp itemsfactory.cpp packagegraphicalitem.cpp filereader.cpp drawwrapper.cpp genericgraphicalitemscontainer.cpp rectpackagegraphicalitem.cpp roundpackagegraphicalitem.cpp capgraphicalitem.cpp filewriter.cpp addneweltolibdlg.cpp graphicalitempropsdlg.cpp testboardpropsdlg.cpp setprintpropsdlg.cpp printpreviewwidget.cpp)

qt4_automoc(${EasyPcb_SRCS})
add_executable(EasyPcb ${EasyPcb_SRCS})
target_link_libraries(EasyPcb ${QT_QTCORE_LIBRARY} ${QT_QTGUI_LIBRARY})

install(TARGETS EasyPcb RUNTIME DESTINATION bin)
