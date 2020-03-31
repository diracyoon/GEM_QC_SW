cmake_minimum_required(VERSION 3.5)

project(GEM_QC_SW)

MESSAGE(${CMAKE_PROJECT_NAME})

SET(PROJECT_DIR ${PROJECT_SOURCE_DIR}/..)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

find_package(ROOT)
find_package(Qt5 COMPONENTS Widgets REQUIRED)

ADD_COMPILE_OPTIONS(-std=c++11 -DUNIX)
INCLUDE_DIRECTORIES(${PROJECT_DIR}/Code/Include ${PROJECT_DIR}/Code/UI ${ROOT_INCLUDE_DIRS} ${QT_INCLUDES})
LINK_LIBRARIES(caenhvwrapper pthread ncurses dl m ${ROOT_LIBRARIES})

SET(SRC_DIR ${PROJECT_DIR}/Code/Src)
SET(UI_DIR ${PROJECT_DIR}/Code/UI)

ADD_EXECUTABLE(Server ${SRC_DIR}/Main_Server.C ${SRC_DIR}/Server.C ${SRC_DIR}/HV_Controller.C)
ADD_EXECUTABLE(Preparation_QC_Long ${SRC_DIR}/Main_Preparation_QC_Long.C ${SRC_DIR}/Client.C ${SRC_DIR}/QC_Base.C ${SRC_DIR}/Preparation_QC_Long.C)
ADD_EXECUTABLE(QC_Long ${SRC_DIR}/Main_QC_Long.C ${SRC_DIR}/Client.C ${SRC_DIR}/QC_Base.C ${SRC_DIR}/QC_Long.C)
ADD_EXECUTABLE(Monitor ${SRC_DIR}/Main_Monitor.C ${SRC_DIR}/Monitor.C ${SRC_DIR}/Watcher.C ${SRC_DIR}/Painter.C ${SRC_DIR}/Submitter.C)
add_executable(QC_UI ${UI_DIR}/MainWidget.ui ${UI_DIR}/QC2Widget.ui ${UI_DIR}/main.cpp ${UI_DIR}/MainWidget.cpp ${UI_DIR}/QC2Widget.cpp)

target_link_libraries(QC_UI Qt5::Widgets)

INSTALL(TARGETS Server Preparation_QC_Long QC_Long Monitor QC_UI DESTINATION ${PROJECT_DIR}/Bin)
