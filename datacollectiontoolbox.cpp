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
		connect(parameterlist, &QListWidget::currentRowChanged, [=](double newValue){
			collector->setParameter(paramlist[newValue]);

		});

		windowLayout->addWidget(parameterlist);

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
