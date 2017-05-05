#include <algorithm>
#include <random>
#include <QMessageBox>

#include "simulation.h"

Datapoint::Datapoint(std::vector<attribute_t> attributes, std::unordered_set<Datapoint*> virneighbors, culture_t culture)
	: _attributes(std::move(attributes)), _virneighbors(std::move(virneighbors)), _culture(culture)
{ }

std::pair<Datapoint::attribute_t, std::vector<Datapoint::attribute_t> > Datapoint::overlap(Datapoint* target, Datapoint* neighbor)
{
	attribute_t overlapy = 0;
	std::vector<Datapoint::attribute_t> locations;
	for (attribute_t index = 0; index < target->attributes().size(); index++){
		if (target->attributes()[index] = neighbor->attributes()[index]){
			overlapy += 1;
		}
		else {
			locations.push_back(index);
		}
	}
	std::pair<Datapoint::attribute_t, std::vector<Datapoint::attribute_t> > retpair;
	retpair.first = overlapy;
	retpair.second = locations;
	return retpair;
}

SimParameter::Data::Data(const QString &name, double value, const QString &description)
	: _name(name), _value(value), _description(description)
{ }

SimParameter::SimParameter(const QString& name, double value, const QString &description)
	: _data(std::make_shared<Data>(name, value, description))
{ }

SimParameter &SimParameter::operator=(double value)
{
	_data->_value = value;
	if(_data->onChanged)
		_data->onChanged(value);
	return *this;
}

void SimParameter::onChanged(std::function<void (double)> func)
{
	_data->onChanged = func;
}

Simulation::Simulation(size_t width) : _data(width * width), _width(width)
{ }

/*void Simulation::reconnect(uint8_t ctype)
{
	std::mt19937_64 rng(std::random_device{}());
	std::uniform_int_distribution<uint64_t> dist;

	std::vector<std::vector<size_t>> tempList;
	for (size_t in = 0; in < width() * width(); in++)
		tempList.push_back({});
	_neighborList = tempList;
	int nov[width()];
	int csok[width()];
	switch (ctype) {
		case 0:
			for (size_t in = 0; in < width() * width(); in++)
				_neighborList[in] = {0, 0, 0, 0};
			for (size_t i = 0; i < width(); i++) {
					nov[i] = i + 1;
					csok[i] = i - 1;
			}

			nov[width() - 1] = 0;
			csok[0] = width() - 1;
			for (size_t i = 0; i < width(); i++){
				for (size_t j = 0; j < width(); j++){
					_neighborList[player(i, j)][0] = player(i, nov[j]);
					_neighborList[player(i, j)][1] = player(i, csok[j]);
					_neighborList[player(i, j)][2] = player(nov[i], j);
					_neighborList[player(i, j)][3] = player(csok[i], j);
				}
			}
			break;
		case 1:
			size_t iu, ju;
			for (size_t in = 0; in < width() * width(); in++)
				_neighborList[in] = {0, 0, 0, 0, 0, 0};
			for(size_t i = 0; i < width(); i++) {
				for(size_t j = 0; j < width(); j++) {
					auto player1 = width() * i + j;
					iu = i;
					ju = j + 1;
					if (ju == width())
						ju = 0;
					_neighborList[player1][2] = player(iu, ju);

					ju = j - 1;
					if (j == 0)
						ju = width() - 1;
					_neighborList[player1][5] = player(iu, ju);
					iu = i + 1;
					if (i == width() - 1)
						iu = 0;
					if (i % 2 == 0){
						ju = j;
						if (ju == width())
							ju = 0;
						_neighborList[player1][1] = player(iu, ju);

						ju = j - 1;
						if (j == 0)
							ju = width() - 1;
						_neighborList[player1][0] = player(iu, ju);
					}
					if (i % 2 != 0){
						ju = j;
						if (ju == width())
							ju = 0;
						_neighborList[player1][0] = player(iu, ju);

						ju = j + 1;
						if (j == width() - 1)
							ju = 0;
						_neighborList[player1][1] = player(iu, ju);
					}

					iu = i - 1;
					if (i == 0)
						iu = width() - 1;
					if (i % 2 == 0){
						ju = j;
						//if (ju == width() - 1) ju = 0;
						_neighborList[player1][3] = player(iu, ju);

						ju = j - 1;
						if (j == 0)
							ju = width() - 1;
						_neighborList[player1][4] = player(iu, ju);
					}
					if (i % 2 != 0){
						ju = j;
						//if (ju == width() - 1) ju = 0;
						_neighborList[player1][4] = player(iu, ju);

						ju = j + 1;
						if (j == width() - 1)
							ju = 0;
						_neighborList[player1][3] = player(iu, ju);
					}
				}
			}
			break;
		case 2:
			std::vector<size_t> availableList;
			for (size_t i = 0; i < width(); i++){
				for (size_t j = 0; j < width(); j++){
					availableList.push_back(player(i, j));
				}
			}
			size_t connections = 0;
			while (connections < width() * width() * 2) {
				if (availableList.size() < 2) {
					break;
				}
				auto rand = dist(rng) % availableList.size();
				auto chosen = availableList[rand];
				auto rand2 = dist(rng) % availableList.size();
				while (rand2 == rand) {
					rand2 = dist(rng) % availableList.size();
				}
				auto chosen2 = availableList[rand2];
				//if (_neighborList[chosen].size() < 4)
				_neighborList[chosen].push_back(chosen2);
				if (_neighborList[chosen].size() == 4) {
					availableList.erase(std::remove(availableList.begin(), availableList.end(), chosen), availableList.end());
				}
				_neighborList[chosen2].push_back(chosen);
				if (_neighborList[chosen2].size() == 4) {
					availableList.erase(std::remove(availableList.begin(), availableList.end(), chosen2), availableList.end());
				}
				connections += 1;

			}			
			break;
		}
}*/
