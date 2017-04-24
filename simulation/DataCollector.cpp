#include <fstream>
#include <chrono>
#include <iomanip>
#include <string>
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
	f << _param.name().toStdString();
	for (size_t i = 0; i < _simulation->returnSpecies(); i++) {
		auto string = QString("Strategy");
		string.append(QString::number(i + 1));
		f << std::setw(12) << string.toStdString();
	}
	f << "\n";

	std::vector<size_t> results(_simulation->returnSpecies());
	auto* changeTracker = new QProgressBar;
	changeTracker->setMinimum(0);
	changeTracker->setMaximum((abs(_endpoint - _startpoint) / _step) + 1);

	auto* stepTracker = new QProgressBar;
	stepTracker->setMinimum(1);
	stepTracker->setMaximum(_maxSteps + _therm - 1);
	for (double i = _startpoint; i < _endpoint + _step; i += _step){

		_param = i;		//your code goes here hopefully

		for (size_t i = 0; i < results.size(); i++)
				results[i] = 0;
		_simulation->reset();
		stepTracker->reset();
		for (size_t step = 1; step < static_cast<size_t>(_maxSteps + _therm); step++){

			stepTracker->setValue(stepTracker->value() + 1);
			stepTracker->show();
			QApplication::processEvents();
			_simulation->step();
			if (step > static_cast<size_t>(_therm)){

			}
		}
		for (size_t i = 0; i < _simulation->width(); i++)
			for (size_t j = 0; j < _simulation->width(); j++)
				results[_simulation->at(i,j)] += 1;

		f << _param;
		for(size_t i = 0; i < _simulation->returnSpecies(); i++)
		{
			f << std::setw(12) << static_cast<double>(results[i]) / (_simulation->width() * _simulation->width());
		}

		f << "\n";
		QApplication::processEvents();
		changeTracker->setValue(changeTracker->value() + 1);
		changeTracker->show();

	 }
}
