#include <fstream>
#include <chrono>
#include <iomanip>
#include <string>
#include <QMessageBox>
#include <QProgressBar>
#include <QtWidgets>
#include <QApplication>

#include "Bisector.h"

Bisector::Bisector(QWidget* parent) : QWidget(parent)
{

}

void Bisector::setSimulation(std::shared_ptr<Simulation>& sim)
{
	_simulation = sim;
}

void Bisector::setStartpoint(double sp)
{
	_startpoint = sp;
}

void Bisector::setEndpoint(double ep)
{
	_endpoint = ep;
}

void Bisector::setStep(double stp)
{
	_step = stp;
}

void Bisector::setMinpoint(double sp)
{
	_minpoint = sp;
}

void Bisector::setMaxpoint(double ep)
{
	_maxpoint = ep;
}

void Bisector::setParameter1(SimParameter par)
{
	_param1 = par;
}

void Bisector::setParameter2(SimParameter par2)
{
	_param2 = par2;
}

void Bisector::setAccuracy(double stp)
{
	_accuracy = stp;
}

void Bisector::setMaxSteps(int ms)
{
	_maxSteps = ms;
}

void Bisector::setTherm(int th)
{
	_therm = th;
}

void Bisector::bisect()
{
	std::string retString = "bisect_";
	retString.append(_simulation->getName());
	auto date = static_cast<uint32_t>(std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count());
	auto date2 = std::to_string(date);
	retString.append(date2);
	retString.append(".dat");
	std::ofstream f(retString);
	f << _param1.name().toStdString();
	f << std::setw(12) << _param2.name().toStdString();
	for (size_t i = 0; i < _simulation->returnSpecies(); i++) {
		auto string = QString("Strategy");
		string.append(QString::number(i + 1));
		f << std::setw(12) << string.toStdString();
	}
	for (size_t i = 0; i < _simulation->returnSpecies(); i++) {
		auto string = QString("DeltaStrategy");
		string.append(QString::number(i + 1));
		f << std::setw(12) << string.toStdString();
	}
	f << "\n";
	auto* changeTracker = new QProgressBar;
	changeTracker->setMinimum(0);
	changeTracker->setMaximum(abs(_endpoint - _startpoint) / _step + 1);
	//for (int i = _startpoint; i <= _endpoint; i += _step){

		_param1 = _startpoint;
		_param2 = _minpoint;
		_minstrat = bisectionAnalysis();
		_param2 = _maxpoint;
		_maxstrat = bisectionAnalysis();
		if (_minstrat != _maxstrat){

			QApplication::processEvents();
			changeTracker->setValue(changeTracker->value() + 1);
			changeTracker->show();
			QApplication::processEvents();
			while (abs(_maxpoint - _minpoint) >= _accuracy) {
				_halfpoint = _minpoint + (abs(_maxpoint - _minpoint) / 2);
				_param2 = _halfpoint;
				uint8_t halfStrat = bisectionAnalysis();
				if (halfStrat == _minstrat)
					_minpoint = _halfpoint;
				else
					_maxpoint = _halfpoint;
			QString string = QString::number(_param1.value());
			f << string.toStdString() << std::setw(12) << QString::number(_halfpoint).toStdString() << "\n";



			}
		}
		else

			throw std::runtime_error("minpoint strategy matches maxpoint strategy");
}
//}

uint8_t Bisector::bisectionAnalysis()
{
	auto* stepTracker = new QProgressBar;
	stepTracker->setMinimum(0);
	stepTracker->setMaximum(15000);
	std::vector<int> results(_simulation->returnSpecies() * 2 + 1);
	std::vector<int> resultsTemp(_simulation->returnSpecies() * 2 + 1);
	uint8_t flip = 0;
	size_t flops = 0;
	_simulation->reset();
	for (size_t ix = 0; ix < _simulation-> width(); ix++)
		 for (size_t jx = 0; jx <_simulation->width(); jx++)
			 results[_simulation->at(ix, jx)]++;

	for (size_t i = 0; i < 150; i++){
		_simulation->step();
		QApplication::processEvents();
		stepTracker->setValue(stepTracker->value() + 1);
		stepTracker->show();
		QApplication::processEvents();
		resultsTemp[0] = 0;
		resultsTemp[1] = 0;
		resultsTemp[2] = 0;
		resultsTemp[3] = 0;
		for (size_t ix = 0; ix < _simulation-> width(); ix++)
			 for (size_t jx = 0; jx <_simulation->width(); jx++)
				 resultsTemp[_simulation->at(ix, jx)]++;
		resultsTemp[2] = resultsTemp[0] - results[0];
		resultsTemp[3] = resultsTemp[1] - results[1];
		flops += 1;
		if (resultsTemp[2] < 0){
			if (results[4] >= 0){
				flip = 1 - flip;
				flops = 0;
				results[4] = -1;
			}
		}
		if (resultsTemp[2] > 0){
			if (results[4] <= 0){
				flip = 1 - flip;
				flops = 0;
				results[4] = 1;
			}
		}

		if (resultsTemp[0] == static_cast<int>(_simulation->width() * _simulation->width())){
			return 0;
			break;
		}
		if (resultsTemp[1] == static_cast<int>(_simulation->width() * _simulation->width())){
			return 1;
			break;
		}
		if (resultsTemp[3] == 0){
			if (resultsTemp[0] != resultsTemp[1]){
				if (resultsTemp[0] > resultsTemp[1]){
				return 0;
				break;
				}
				else {
					return 1;
					break;
				}
			}
		}

		if (flops > 100){
			if (resultsTemp[0] != resultsTemp[1]){
				if (resultsTemp[2] > 0){
				return 0;
				break;
				}
				else {
					return 1;
					break;
				}
			}

		}
	}
	return 0;
	throw std::runtime_error("no absorbing state found");
}

