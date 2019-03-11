CMAKE_MINIMUM_REQUIRED(VERSION 3.5)
PROJECT(GEM_QC_SW)

MESSAGE(${CMAKE_PROJECT_NAME})

find_package(ROOT)

SET(PROJECT_DIR ${PROJECT_SOURCE_DIR}/..)

ADD_COMPILE_OPTIONS(-std=c++11 -DUNIX)
INCLUDE_DIRECTORIES(${PROJECT_DIR}/Code/Include ${ROOT_INCLUDE_DIRS})
LINK_LIBRARIES(caenhvwrapper pthread ncurses dl m ${ROOT_LIBRARIES})

SET(SRC_DIR ${PROJECT_DIR}/Code/Src)

ADD_EXECUTABLE(Server ${SRC_DIR}/Main_Server.C ${SRC_DIR}/Server.C ${SRC_DIR}/HV_Controller.C)
ADD_EXECUTABLE(Preparation_QC_Long ${SRC_DIR}/Main_Preparation_QC_Long.C ${SRC_DIR}/Client.C ${SRC_DIR}/QC_Base.C ${SRC_DIR}/Preparation_QC_Long.C)
ADD_EXECUTABLE(QC_Long ${SRC_DIR}/Main_QC_Long.C ${SRC_DIR}/Client.C ${SRC_DIR}/QC_Base.C ${SRC_DIR}/QC_Long.C)
ADD_EXECUTABLE(Monitor ${SRC_DIR}/Main_Monitor.C ${SRC_DIR}/Monitor.C ${SRC_DIR}/Watcher.C ${SRC_DIR}/Painter.C ${SRC_DIR}/Submitter.C)

INSTALL(TARGETS Server Preparation_QC_Long QC_Long Monitor DESTINATION ${PROJECT_DIR}/Bin)
