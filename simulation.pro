QT += core gui widgets
CONFIG += c++14

TARGET = simulation
TEMPLATE = app

msvc:QMAKE_CXXFLAGS_RELEASE += /openmp
gcc:QMAKE_CXXFLAGS_RELEASE += -O3 -march=native -fopenmp -D_GLIBCXX_PARALLEL
gcc:QMAKE_LFLAGS_RELEASE += -fopenmp

SOURCES += main.cpp\
    simulation.cpp \
    mainwindow.cpp \
    imagewidget.cpp \
    simulationmanager.cpp \
    simulationsession.cpp \
    simparameterwidget.cpp \
    LV4s1a/sim_lv4s1a.cpp \
    coordtest/sim_coordtest.cpp \
    datacollectiontoolbox.cpp \
    DataCollector.cpp \
    saveManager.cpp \
    axelrod/sim_axelrod.cpp

HEADERS  += \
    utils.h \
    simulation.h \
    mainwindow.h \
    imagewidget.h \
    simulationmanager.h \
    simulationsession.h \
    simparameterwidget.h \
    overloadselector.h \
    LV4s1a/sim_lv4s1a.h \
    coordtest/sim_coordtest.h \
    datacollectiontoolbox.h \
    DataCollector.h \
    saveManager.h \
    axelrod/sim_axelrod.h
