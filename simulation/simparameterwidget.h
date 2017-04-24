#pragma once

#include <QFormLayout>
#include <QWidget>
#include "simulation.h"

class SimParameterWidget : public QWidget
{
	Q_OBJECT
public:
	explicit SimParameterWidget(QWidget *parent = 0);
	void setSimulationParameters(std::vector<SimParameter> params);
private:
	std::vector<SimParameter> _params;
	QFormLayout* _layout;
};
