#pragma once

#include <memory>
#include <iostream>
#include <fstream>
#include <cstring>
#include <QAbstractItemModel>

/*class SimulationWrapper
{
public:
	virtual const char* name() const = 0;
	virtual const std::shared_ptr<Simulation>& get() = 0;
};*/

class SaveManager : public QAbstractItemModel			// class for managing simulations
{
	Q_OBJECT
public:
	explicit SaveManager(QObject *parent = 0);		/* initializes a SimulationManager object */
	QModelIndex index(int row, int column, const QModelIndex &parent) const override; /* declares a function that returns the index of an element */
	QModelIndex parent(const QModelIndex &child) const override;/* declares a function that returns the parent of an element, currently unused */
	int rowCount(const QModelIndex &parent) const override;		/* declares a function that returns the row count of the table of simulations */
	int columnCount(const QModelIndex &parent) const override;	/* declares a function that returns the column count of the table of simulations */
	QVariant data(const QModelIndex &index, int role) const override; /* declares a structure that keeps track of the simulations */
	//std::shared_ptr<Simulation> getSimulation(size_t index) const; /* declares a pointer to a simulation with a given index */
	std::string getSimName(size_t input);
	void init();
private:
	std::vector<std::string> _data;					/* initializes a structure that keeps track of the simulations
																		using their pointers*/
};
