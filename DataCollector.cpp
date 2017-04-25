#include <fstream>
#include <chrono>
#include <iomanip>
#include <string>
#include <unordered_set>
#include <QMessageBox>
#include <QProgressBar>
#include <QtWidgets>
#include <QApplication>

#include "DataCollector.h"

DataCollector::DataCollector(QWidget* parent) : QWidget(parent)
{

}

void DataCollector::setSimulation(std::shared_ptr<Simulation>& sim)
{
	_simulation = sim;
}

void DataCollector::setStartpoint(double sp)
{
	_startpoint = sp;
}

void DataCollector::setEndpoint(double ep)
{
	_endpoint = ep;
}

void DataCollector::setParameter(SimParameter par)
{
	_param = par;
}

void DataCollector::setStep(double stp)
{
	_step = stp;
}

void DataCollector::setMaxSteps(int ms)
{
	_maxSteps = ms;
}

void DataCollector::setTherm(int th)
{
	_therm = th;
}

void DataCollector::collectData()
{
	std::string retString = "collect_";
	retString.append(_simulation->getName());
	auto date = static_cast<uint32_t>(std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count());
	auto date2 = std::to_string(date);
	retString.append(date2);
	retString.append(".dat");
	std::ofstream f(retString);
	f << "\n";

	std::unordered_set<Datapoint::culture_t> results;
	_simulation->reset();
	while(_simulation->live() > 0){
		QApplication::processEvents();
		_simulation->step();
	}
	for (size_t i = 0; i < _simulation->width(); i++)
		for (size_t j = 0; j < _simulation->width(); j++)
			results.insert(_simulation->at(i,j).culture());
	/*std::string stringy = static_cast<std::string>(results.size());
	chary = char(stringy);*/
	f << "\n";
	QApplication::processEvents();
}
