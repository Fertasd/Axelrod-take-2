#include <cmath>
#include <cstdio>
#include <cstring>
#include <iostream>
#include <fstream>
#include <sstream>
#include <chrono>
#include "utils.h"
#include "simulationsession.h"

using namespace std;

SimulationSession::SimulationSession(QObject *parent) : QObject(parent), _animationDelay(200), _renderFrameSkip(0)
{
	nameList.insert(pair<QString, int>(QString("coordtest"), 10));
	nameList.insert(pair<QString, int>(QString("LV4s1a"), 1));
	nameList.insert(pair<QString, int>(QString("Axelrod"), 1));
}



void SimulationSession::simulation(const std::shared_ptr<Simulation> &simulation)
{
	_simulation = simulation;
	emit simulationChanged(_simulation);
	animationDelay(_simulation->animationDelay());
	renderFrameSkip(_simulation->renderFrameSkip());
	displayWidth(_simulation->displayWidth());
	connectionType(_simulation->connectionType());
}

void SimulationSession::animationDelay(uint32_t delay)
{
	_animationDelay = delay;
	emit animationDelayChanged(_animationDelay);
}

void SimulationSession::renderFrameSkip(uint32_t frames)
{
	_renderFrameSkip = frames;
	emit renderFrameSkipChanged(_renderFrameSkip);
}

void SimulationSession::displayWidth(size_t dpwidth)
{
	_displayWidth = dpwidth;
	emit displayWidthChanged(_displayWidth);
}

void SimulationSession::connectionType(uint8_t ctype)
{
	_connectionType = ctype;
	emit connectionTypeChanged(_connectionType);
}

size_t SimulationSession::getIndex(std::string const &fileName)
{
	std::string baseString = "C:/Users/Matyas/Documents/simulationNew/simulation/Saves/";
	baseString.append(fileName);
	ifstream infile(baseString);
	SIM_USERASSERT_M(infile.is_open(), "Cannot open " + baseString);
	std::vector<double> numbers;
	//while (!infile.eof()) {
		std::string sLine;
		getline(infile, sLine);
		numbers.push_back(std::stod(sLine));
	//}

	infile.close();
	return static_cast<size_t>(numbers[0]);
}
