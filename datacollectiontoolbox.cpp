#include <QtWidgets>
#include "datacollectiontoolbox.h"
#include "DataCollector.h"

#include "simparameterwidget.h"
#include "overloadselector.h"

DataCollectionToolbox::DataCollectionToolbox(QWidget *parent) : QWidget(parent)
{
	setup();
}

void DataCollectionToolbox::setSimulation(const std::shared_ptr<Simulation>& sim)
{
	_simulation = sim;
}

void DataCollectionToolbox::setup()
{
	if (_simulation){
		_window->setWindowTitle("Analysis parameters");
		_window->resize(400, 500);
		auto* wMainPanel = new QWidget;
		_window->setCentralWidget(wMainPanel);
		auto* windowLayout = new QVBoxLayout;
		wMainPanel->setLayout(windowLayout);
		auto* widget = new QWidget;
		windowLayout->addWidget(widget);
		auto* surface = new QFormLayout;
		widget->setLayout(surface);
		auto paramlist = _simulation->parameters();

		auto* parameterlist = new QListWidget;
		for (auto& param : paramlist){
			parameterlist->addItem(param.name());
		}


		auto* collector = new DataCollector;
		collector->setSimulation(_simulation);
		auto* bisector = new Bisector;
		bisector->setSimulation(_simulation);
		connect(parameterlist, &QListWidget::currentRowChanged, [=](double newValue){
			collector->setParameter(paramlist[newValue]);
			bisector->setParameter1(paramlist[newValue]);

		});

		windowLayout->addWidget(parameterlist);


		auto* startpoint = new QDoubleSpinBox;
		surface->addRow("Startpoint/Minimal point", startpoint);
		startpoint->setMinimum(-5);
		startpoint->setMaximum(5);
		connect(startpoint, overload<void(double)>::get(&QDoubleSpinBox::valueChanged), [=](double newValue){


			collector->setStartpoint(newValue);
			bisector->setStartpoint(newValue);
		});

		auto* endpoint = new QDoubleSpinBox;
		surface->addRow("Endpoint", endpoint);
		endpoint->setMinimum(-5);
		endpoint->setMaximum(5);
		connect(endpoint, overload<void(double)>::get(&QDoubleSpinBox::valueChanged), [=](double newValue){


			collector->setEndpoint(newValue);
			bisector->setEndpoint(newValue);
		});

		auto* step = new QDoubleSpinBox;
		surface->addRow("Step", step);
		step->setMinimum(0);

		connect(step, overload<void(double)>::get(&QDoubleSpinBox::valueChanged), [=](double newValue){
			collector->setStep(newValue);
			bisector->setStep(newValue);
		});

		auto* maxSteps = new QSpinBox;
		surface->addRow("Number of simulation steps for data collection", maxSteps);
		maxSteps->setMinimum(0);
		maxSteps->setMaximum(500000);
		connect(maxSteps, overload<void(int)>::get(&QSpinBox::valueChanged), [=](int newValue){


			collector->setMaxSteps(newValue); });

		auto* therm = new QSpinBox;
		therm->setMinimum(0);
		therm->setMaximum(5000);

		surface->addRow("Number of thermalization steps for data collection", therm);
		connect(therm, overload<void(int)>::get(&QSpinBox::valueChanged), [=](int newValue){ collector->setTherm(newValue); });


		startpoint->setValue(0.2);
		endpoint->setValue(0.3);
		step->setValue(0.2);


		auto* starterButton = new QPushButton("Begin data collection");
		surface->addWidget(starterButton);
		connect (starterButton, &QPushButton::clicked, [=]{
			starterButton->setEnabled(false);

			QApplication::processEvents();
			collector->collectData();
			QApplication::processEvents();
			starterButton->setEnabled(true);

		});

		_window->show();
	}

}

std::shared_ptr<QMainWindow> DataCollectionToolbox::getWindow()
{
	return _window;
}
