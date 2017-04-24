#pragma once

#include <memory>
#include <QWidget>
#include "simulation.h"

class DataCollector : public QWidget
{
	Q_OBJECT
public:
	explicit DataCollector(QWidget *parent = 0);
	void setSimulation(std::shared_ptr<Simulation> &sim);
	void setStartpoint(double sp);
	void setEndpoint(double ep);
	void setParameter(SimParameter par);
	void setStep(double stp);
	void setMaxSteps(int ms);
	void setTherm(int th);
	void collectData();
	double _startpoint = 0;
private:
	std::shared_ptr<Simulation> _simulation;
	SimParameter _param{"default", 0, "what am I even doing"};
	double _endpoint = 0;
	double _step = 0;
	int _maxSteps = 0;
	int _therm = 0;
};
