#pragma once

#include <memory>
#include <QWidget>
#include "simulation.h"

class Bisector : public QWidget
{
	Q_OBJECT
public:
	explicit Bisector(QWidget *parent = 0);
	void setSimulation(std::shared_ptr<Simulation> &sim);
	void setStartpoint(double sp);
	void setEndpoint(double ep);
	void setStep(double stp);
	void setMinpoint(double sp);
	void setMaxpoint(double ep);
	void setParameter1(SimParameter par);
	void setParameter2(SimParameter par2);
	void setAccuracy(double stp);
	void setMaxSteps(int ms);
	void setTherm(int th);
	void bisect();
	uint8_t bisectionAnalysis();
	double _minpoint = 0;
	double _startpoint = 0;
private:
	std::shared_ptr<Simulation> _simulation;
	SimParameter _param1{"default", 0, "what am I even doing"};
	SimParameter _param2{"default", 0, "sameeeee"};
	double _maxpoint = 0;
	double _halfpoint = 0;
	double _endpoint = 0;
	double _step = 0;
	double _accuracy = 0;
	int _maxSteps = 0;
	int _therm = 0;
	uint8_t _minstrat = 0;
	uint8_t _maxstrat = 0;
};
