#include "mainwindow.h"
#include <chrono>
#include <fstream>
#include <iostream>
#include <unordered_set>
#include <QtWidgets>
#include <utils.h>
#include "imagewidget.h"
#include "simparameterwidget.h"
#include "overloadselector.h"
#include "datacollectiontoolbox.h"
#include "simulationmanager.h"

MainWindow::MainWindow(QWidget *parent)
	: QMainWindow(parent), shouldSimulate(false)
{   
	setWindowTitle("Simulation");			/* initializes the main window with a timer built in,
											  UC: sets initial size and window title */
	resize(800, 600);

	auto* timer = new QTimer(this);

	auto* mainPanel = new QWidget;
	setCentralWidget(mainPanel);
	auto* layout = new QHBoxLayout;
	mainPanel->setLayout(layout);			/* creates a main panel(QWidget), adds a horizontal box layout
											  (QHBoxLayout) as the main interface */

	auto* image = new ImageWidget;
	layout->addWidget(image, 1);			/* creates a surface(ImageWidget) for displaying images,
											  adds it to the layout with weighing 1(helps with resizing issues) */
	connect(&session, &SimulationSession::simulationChanged, [=](const std::shared_ptr<Simulation>& sim){
		shouldSimulate = false;
		timer->stop();
		image->setSimulation(sim);
		image->update();
	});

	connect(&session, &SimulationSession::displayWidthChanged, [=](size_t dpw){
		shouldSimulate = false;
		timer->stop();
		image->setDisplayWidth(checked_cast<int>(dpw));
		image->update();
	});

	connect(&session, &SimulationSession::connectionTypeChanged, [=](uint8_t ctype){
		shouldSimulate = false;
		timer->stop();
		//session.simulation()->reconnect(ctype);
		image->setConnectionType(ctype);
		image->update();
	});

	auto* buttonLayout = new QVBoxLayout;
	layout->addLayout(buttonLayout, 0);		/* creates a vertical box layout(QVBoxLayout) as a surface for
											  displaying buttons and the simulation list, then adds it to the
												main layout with weighing 0(helps with resizing issues) */

	auto* startButton = new QPushButton("Start/Continue");	/* creates a button with a given name */
	buttonLayout->addWidget(startButton);					/* adds it to the vertical layout */
	auto* pauseButton = new QPushButton("Pause");
	buttonLayout->addWidget(pauseButton);
	auto* resetButton = new QPushButton("Reset");
	buttonLayout->addWidget(resetButton);

	auto* analButton = new QPushButton("Data collection");
	analButton->setCheckable(true);
	buttonLayout->addWidget(analButton);

	//auto* simList = new QListView;							/* creates an object(QListView) for displaying the list */
	//buttonLayout->addWidget(simList);						/* adds it to the vertical layout */
	//simList->setResizeMode(QListView::Adjust);				/* allows the list to be resized if necessary*/
	//simList->setModel(&simulationManager);					/* specifies the Simulation Manager as the source of the list */
	session.simulation(simulationManager.getSimulation(0));

	auto* formLayout = new QFormLayout;
	buttonLayout->addLayout(formLayout);

	auto* dper = new QSpinBox;

	formLayout->addRow("Display width", dper);
	dper->setMinimum(60);
	dper->setMaximum(500);
	dper->setWrapping(true);

	connect(dper, overload<void(int)>::get(&QSpinBox::valueChanged), [=](int newValue){
		session.displayWidth(static_cast<size_t>(newValue));
	});
	connect(&session, &SimulationSession::displayWidthChanged, [=](size_t newValue){ dper->setValue(static_cast<int>(newValue)); });

	auto* delayer = new QSpinBox;
	formLayout->addRow("Animation delay", delayer);
	delayer->setMinimum(0);
	delayer->setMaximum(3600000);
	delayer->setWrapping(true);

	connect(delayer, overload<void(int)>::get(&QSpinBox::valueChanged), [=](int newValue){ session.animationDelay(static_cast<uint32_t>(newValue)); });
	connect(&session, &SimulationSession::animationDelayChanged, [=](uint32_t newValue){ delayer->setValue(static_cast<int>(newValue)); });

	auto* skipper = new QSpinBox;
	formLayout->addRow("Frame skip", skipper);
	skipper->setMinimum(1);
	skipper->setMaximum(50000);
	skipper->setWrapping(true);

	connect(skipper, overload<void(int)>::get(&QSpinBox::valueChanged), [=](int newValue){ session.renderFrameSkip(static_cast<uint32_t>(newValue)); });
	connect(&session, &SimulationSession::renderFrameSkipChanged, [=](uint32_t newValue){ skipper->setValue(static_cast<int>(newValue)); });

	auto* connecter = new QSpinBox;
	formLayout->addRow("Connection type", connecter);
	connecter->setMinimum(0);
	connecter->setMaximum(2);
	connecter->setWrapping(true);
	connecter->setToolTip("Connection system. 0: square lattice 1: triangle lattice 2: random graph");

	connect(connecter, overload<void(int)>::get(&QSpinBox::valueChanged), [=](int newValue){ session.connectionType(static_cast<uint8_t>(newValue)); });
	connect(&session, &SimulationSession::connectionTypeChanged, [=](uint8_t newValue){ connecter->setValue(static_cast<int>(newValue)); });

	auto* params = new SimParameterWidget;
	buttonLayout->addWidget(params);	
	params->setSimulationParameters(session.simulation()->parameters());
	connect(&session, &SimulationSession::simulationChanged, [=]{
		params->setSimulationParameters(session.simulation()->parameters());
		session.simulation()->startselect = session.simulation()->resetType();
	});

	connect(timer, &QTimer::timeout, [=]{
		timer->stop();
		for (size_t i = 0; i < session.renderFrameSkip() && shouldSimulate; ++i)
		{
			session.simulation()->step(); /* do a certain number of simulation steps */			
			QApplication::processEvents();
		}		
		image->update();		/* display the current state of the simulation */
		if (shouldSimulate)
			timer->start();
	});
	connect(&session, &SimulationSession::animationDelayChanged, [=](uint32_t delay){ timer->setInterval(static_cast<int>(delay)); });
	connect(startButton, &QPushButton::clicked, [=]{ if (session.simulation()) { shouldSimulate = true; timer->start(); } });		/* on clicking the start and stop buttons, the timer starts and stops */
	connect(pauseButton, &QPushButton::clicked, [=]{ shouldSimulate = false; timer->stop(); });
	connect(resetButton, &QPushButton::clicked, [=]{
		if (session.simulation())
		{
			shouldSimulate = false;
			timer->stop();
			session.simulation()->reset();
			image->update();
		}
	}); /* connects the clicked() event of the reset button with a series of events: the timer stops, the simulation is reset and displayed */

	connect(analButton, &QPushButton::clicked, [=]{
		/*std::string retString = "collect_";
		retString.append(session.simulation()->getName());
		auto date = static_cast<uint32_t>(std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count());
		auto date2 = std::to_string(date);
		retString.append(date2);
		retString.append(".dat");*/
		for (uint8_t iter = 0; iter < session.simulation()->_runs; iter++){
			session.simulation()->reset();
			std::ofstream ofs;
			ofs.open("numberofcultures.dat", std::ofstream::out | std::ofstream::trunc);
			SIM_USERASSERT_M(ofs.is_open(), "Cannot open numberofcultures.dat");
			ofs.close();
			std::ofstream of;
			of.open("largestculture.dat", std::ofstream::out | std::ofstream::trunc);
			SIM_USERASSERT_M(of.is_open(), "Cannot open largestculture.dat");
			of.close();
			while(session.simulation()->live() > 0){
				QApplication::processEvents();
				session.simulation()->step();
				std::ofstream f;
				std::ofstream g;
				g.open ("largestculture.dat", std::ofstream::out | std::ofstream::app);
				f.open ("numberofcultures.dat", std::ofstream::out | std::ofstream::app);
				SIM_USERASSERT_M(f.is_open(), "Cannot open numberofcultures.dat");
				SIM_USERASSERT_M(g.is_open(), "Cannot open largestculture.dat");
				size_t cultsize = session.simulation()->_counter.size();
				f << cultsize << "\n";
				size_t maxsize = 0;
				for (auto cult : session.simulation()->_counter){
					if (cult.second > maxsize)
						maxsize = cult.second;
				}
				g << maxsize << "\n";
				f.close();
				g.close();
			}
		}
		QApplication::processEvents();
		QMessageBox::information(this, "Notice", "Data collection finished");
		QApplication::processEvents();
		});

}
