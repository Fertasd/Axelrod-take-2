#include "simparameterwidget.h"
#include <QtWidgets>
#include "overloadselector.h"

SimParameterWidget::SimParameterWidget(QWidget *parent) : QWidget(parent)
{
	_layout = new QFormLayout;
	setLayout(_layout);
	_layout->setMargin(0);
}

void SimParameterWidget::setSimulationParameters(std::vector<SimParameter> params)
{
	qDeleteAll(findChildren<QWidget*>(QString(), Qt::FindDirectChildrenOnly));
	_params = std::move(params);
	for (auto& param : _params)
	{
		auto* paramBox = new QDoubleSpinBox;
		paramBox->setToolTip(param.description());
		paramBox->setMinimum(-5);
		paramBox->setDecimals(4);
		paramBox->setValue(param);
		_layout->addRow(param.name(), paramBox);
		connect(paramBox, overload<void(double)>::get(&QDoubleSpinBox::valueChanged), [&](double newValue){ param = newValue; });
	}
}
