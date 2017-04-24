#pragma once

#include <QMainWindow>
#include "simulationmanager.h"
#include "simulationsession.h"
#include "saveManager.h"

class MainWindow : public QMainWindow
{
	Q_OBJECT
public:
	explicit MainWindow(QWidget *parent = 0);  /* creates the display window */
private:
	SimulationManager simulationManager;		/* adds a simulation manager object to the window */
	SaveManager saveManager;
	SimulationSession session;
	bool shouldSimulate;
};
