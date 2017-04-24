#include <cmath>
#include <stdlib.h>
#include <stdio.h>
#include <cstring>
#include <iostream>
#include <fstream>
#include <sstream>
#include <chrono>

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

QString SimulationSession::snapshot()
{
	auto epsnev =  std::string("snapshot_");
	epsnev.append(_simulation->getName());
	auto date = static_cast<uint32_t>(std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count());
	auto date2 = std::to_string(date);
	epsnev.append(date2);
	epsnev.append(".eps");
	const char* c = epsnev.c_str();
	FILE* tf;
	auto Lb0 = 2 * _simulation->displayWidth();
	tf = fopen(c, "w+t");
	fprintf(tf, "%c!PS-Adobe-2.0\n", '%');
	fprintf(tf, "%c%cTitle: snapshot for Aco=1, Acr=0.5, Ase=-0.03, L=%d, K=%3.1f, t=%d MCS\n", '%', '%', _simulation->displayWidth(), 0.3, 500);
	fprintf(tf, "%c%cCreator: cosecrsx.c\n", '%', '%');
	fprintf(tf, "%c%cBoundingBox: 15 15 505 505\n", '%', '%');
	fprintf(tf, "%c%cEndComments\n", '%', '%');
	fprintf(tf, "/M {rmoveto} bind def\n");
	fprintf(tf, "/L {lineto} bind def\n");
	fprintf(tf, "/R {rlineto} bind def\n");
	fprintf(tf, "/S {moveto} bind def\n");
	fprintf(tf, "/nr {grestore 0 0.5 translate gsave}  def\n");
	for (uint8_t i = 0; i < _simulation->returnSpecies(); i++){
		std::string ret = "/c";
		ret.append(std::to_string(i));
		ret.append(" {");
		ret.append(std::to_string(static_cast<double>(_simulation->palette()[i].red()) / 255));
		ret.append(" ");
		ret.append(std::to_string(static_cast<double>(_simulation->palette()[i].green()) / 255));
		ret.append(" ");
		ret.append(std::to_string(static_cast<double>(_simulation->palette()[i].blue()) / 255));
		ret.append(" setrgbcolor} bind def \n");
		const char* end = ret.c_str();
		fprintf(tf, end);
	}
	for (uint8_t i = 0; i < _simulation->returnSpecies(); i++){
		std::string ret = "/pc";
		ret.append(std::to_string(i));
		ret.append(" {c");
		ret.append(std::to_string(i));
		ret.append(" ");
		ret.append("newpath 0 0 S 0.5 0 R 0 0.5 R -0.5 0 R closepath fill 0.5 0 translate} def \n");
		const char* end = ret.c_str();
		fprintf(tf, end);
	}
	fprintf(tf, "/aa {ca newpath 0 0 S 60 0 R 0 2 R -60 0 R closepath fill 60 0 translate} def\n");
	fprintf(tf, "/bb {cb newpath 0 0 S 60 0 R 0 2 R -60 0 R closepath fill 60 0 translate} def\n");
	fprintf(tf, "20 20 translate\n");
	fprintf(tf, "1 1 scale\n");
	fprintf(tf, "gsave\n");


	for (size_t i = 0; i < _simulation->width(); i++){
		for (size_t j = 0; j < _simulation->width(); j++){
			std::string res = " pc";
			res.append(std::to_string(_simulation->at(i,j)));
			const char* result = res.c_str();
			fprintf(tf, result);
		}
		fprintf(tf, "\n");
		fprintf(tf, "nr \n");
	}
	fprintf(tf,"0 %d translate\n",(-Lb0));
	fprintf(tf,"0.5 setlinewidth 0 setgray newpath\n");
	fprintf(tf,"0 0 S %d 0 R 0 %d R %d 0 R closepath stroke\n",Lb0,Lb0,(-Lb0));
	fprintf(tf,"showpage\n");
	fprintf(tf,"%c%cTrailer\n",'%','%');
	fflush(tf);
	fclose(tf);
	return QString::fromStdString(epsnev);
}

QString SimulationSession::save()
{
	auto filename =  std::string("save_");
	filename.append(_simulation->getName());
	for (auto param : _simulation->parameters()){
		std::ostringstream strs4;
		strs4 << param.value();
		std::string str4 = strs4.str();
		filename.append(str4);
		filename.append(param.name().toStdString());

		std::ostringstream strs3;
		strs3 << param.value();
		std::string str3 = strs3.str();
		filename.append(str3);
	}
	filename.append("ID");
	auto date = static_cast<uint32_t>(std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count());
	auto date2 = std::to_string(date);
	filename.append(date2);
	filename.append(".txt");
	FILE* tf;
	std::string path = "C:/Users/Matyas/Documents/simulationNew/simulation/Saves/";
	path.append(filename);
	const char* c = path.c_str();
	tf = fopen(c, "w+t");
	std::ostringstream strs0;
	strs0 << nameList.find(QString::fromStdString(simulation()->getName()))->second;
	strs0 << "\n";
	std::string str0 = strs0.str();
	const char* c0 = str0.c_str();
	fprintf(tf, c0);
	std::ostringstream strs;
	strs << animationDelay();
	strs << "\n";
	std::string str = strs.str();	
	const char* c1 = str.c_str();
	fprintf(tf, c1);
	std::ostringstream strs2;
	strs2 << renderFrameSkip();
	strs2 << "\n";
	str = strs2.str();
	const char* c2 = str.c_str();
	fprintf(tf, c2);
	std::ostringstream strs3;
	strs3 << connectionType() << "\n";
	str = strs3.str();
	const char* c3 = str.c_str();
	fprintf(tf, c3);
	std::ostringstream strs4;
	strs4 << displayWidth() << "\n";
	str = strs4.str();
	const char* c4 = str.c_str();
	fprintf(tf, c4);
	for (auto param : _simulation->parameters()){
		std::ostringstream strs;
		strs << param.value();
		std::string str = strs.str();
		str.append("\n");
		const char* cp = str.c_str();
		fprintf(tf, cp);
	}
	for (size_t ik = 0; ik < _simulation->width() * _simulation->width(); ik++){
		std::string str2 = QString::number(_simulation->atNum(ik)).toStdString();
		str2.append("\n");
		const char* cx = str2.c_str();
		fprintf(tf, cx);
	}
	fflush(tf);
	fclose(tf);
	/*FILE* tf2;
	tf2 = fopen("C:/Users/Matyas/Documents/simulationNew/simulation/Saves/master.txt", "w+t");
	const char* filend = filename.c_str();
	fprintf(tf2, filend);
	fprintf(tf2, "\n");
	fflush(tf2);
	fclose(tf2);*/

	ofstream infile;
	infile.open("C:/Users/Matyas/Documents/simulationNew/simulation/Saves/master.txt", std::ios_base::app);
	const char* filend = filename.c_str();
	infile << filend;
	infile << "\n";
	infile.close();
	/*infile.open("C:/Users/Matyas/Documents/simulationNew/simulation/Saves/master.txt", std::ios_base::app);
	filend = filename.c_str();
	infile << filend;
	infile << "\n";
	infile << filend;
	infile.close();*/
	return QString::fromStdString(filename);
}

void SimulationSession::load(std::string fileName)
{
	ifstream infile;
	std::string baseString = "C:/Users/Matyas/Documents/simulationNew/simulation/Saves/";
	baseString.append(fileName);
	//baseString.append("\"");
	infile.open(baseString);
	std::vector<double> numbers;
	while (!infile.eof()) {
		std::string sLine;
		getline(infile, sLine);
		if (sLine != ""){
			numbers.push_back(std::stod(sLine, nullptr));
		}
	}

	infile.close();
	animationDelay(numbers[1]);
	renderFrameSkip(numbers[2]);
	connectionType(numbers[3]);
	displayWidth(numbers[4]);
	for (size_t i = 0; i < _simulation->parameters().size(); i++){
		_simulation->parameters()[i] = numbers[i + 5];
	for (size_t j = 0; j < _simulation->width() * _simulation->width(); j++){
		_simulation->atNum(j) = numbers[j +  _simulation->parameters().size() + 5];
	}
	}


}

size_t SimulationSession::getIndex(std::string fileName)
{
	ifstream infile;
	std::string baseString = "C:/Users/Matyas/Documents/simulationNew/simulation/Saves/";
	baseString.append(fileName);
	//baseString.append("\"");
	infile.open(baseString);
	std::vector<double> numbers;
	//while (!infile.eof()) {
		std::string sLine;
		getline(infile, sLine);
		numbers.push_back(std::stod(sLine));
	//}

	infile.close();
	return static_cast<size_t>(numbers[0]);
}
