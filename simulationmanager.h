#pragma once

#include <memory>
#include <QAbstractItemModel>
#include "simulation.h"

class SimulationWrapper
{
public:
	virtual const char* name() const = 0;
	virtual const std::shared_ptr<Simulation>& get() = 0;
};

class SimulationManager : public QAbstractItemModel			/* class for managing simulations */
{
	Q_OBJECT
public:
	explicit SimulationManager(QObject *parent = 0);		/* initializes a SimulationManager object */
	QModelIndex index(int row, int column, const QModelIndex &parent) const override; /* declares a function that returns the index of an element */
	QModelIndex parent(const QModelIndex &child) const override;/* declares a function that returns the parent of an element, currently unused */
	int rowCount(const QModelIndex &parent) const override;		/* declares a function that returns the row count of the table of simulations */
	int columnCount(const QModelIndex &parent) const override;	/* declares a function that returns the column count of the table of simulations */
	QVariant data(const QModelIndex &index, int role) const override; /* declares a structure that keeps track of the simulations */
	std::shared_ptr<Simulation> getSimulation(size_t index) const; /* declares a pointer to a simulation with a given index */
private:
	std::vector<std::shared_ptr<SimulationWrapper>> _data;					/* initializes a structure that keeps track of the simulations
																		using their pointers*/
};
