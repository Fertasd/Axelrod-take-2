#pragma once

#include <QMainWindow>
#include <QFormLayout>
#include <QWidget>
#include "simulation.h"
#include "DataCollector.h"
#include "DataCollector.h"
#include "Bisector.h"

class DataCollectionToolbox : public QWidget
{
    Q_OBJECT
public:
    explicit DataCollectionToolbox(QWidget *parent = 0);
	void setSimulation(const std::shared_ptr<Simulation> &sim);
	void setup();
	std::shared_ptr<QMainWindow> getWindow();
private:
	std::shared_ptr<Simulation> _simulation;
	std::shared_ptr<QMainWindow> _window = std::make_shared<QMainWindow>();
	DataCollector* collector;
	Bisector* bisector;
};
