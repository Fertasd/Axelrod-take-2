#include "saveManager.h"

#include "coordtest/sim_coordtest.h"

#include <iostream>
#include <fstream>
#include <cstring>

using namespace std;

/* UC: include the header(.h) file for any simulation you want to display here in a similar fashion
	example: #include "newsim/sim_newsim.h" */

/*template<typename SimT>
class SW : public SimulationWrapper
{
public:
	const char* name() const override
	{
		return SimT::name();
	}
	const std::shared_ptr<Simulation>& get() override
	{
		return _ptr ? _ptr : _ptr = std::make_shared<SimT>();
	}
private:
	std::shared_ptr<Simulation> _ptr;
};*/

SaveManager::SaveManager(QObject *parent)
	: QAbstractItemModel(parent), _data({})
{
	ifstream infile;

	infile.open("C:/Users/Matyas/Documents/simulationNew/simulation/Saves/master.txt");

	while (!infile.eof()) {
		std::string sLine;
		getline(infile, sLine);
		_data.push_back(sLine);
	}

	infile.close();
}

void SaveManager::init()
{

}


QModelIndex SaveManager::index(int row, int column, const QModelIndex &) const
{
	return createIndex(row, column, static_cast<quintptr>(row));		/* used to call the index of elements */
}

QModelIndex SaveManager::parent(const QModelIndex &) const
{
	return QModelIndex();						/* always returns an invalid index, has to exist but is unused */
}

int SaveManager::rowCount(const QModelIndex &) const
{
	return static_cast<int>(_data.size());						/* returns the length of the list of simulations */
}

int SaveManager::columnCount(const QModelIndex &) const
{
	return 1;									/* returns the number of columns in the list of simulations, always 1 */
}

QVariant SaveManager::data(const QModelIndex &index, int role) const
{
	if (role == Qt::DisplayRole)				/* if called to display, returns the name of the simulation with a given index,
												  otherwise returns invalid(default) */
		return QString::fromStdString(_data[static_cast<size_t>(index.row())]);
	return QVariant();
}

string SaveManager::getSimName(size_t input)
{
	return _data[input];
}

/*std::shared_ptr<Simulation> SaveManager::getSimulation(size_t index) const
{
	return _data[std::atof(index)];
}*/

