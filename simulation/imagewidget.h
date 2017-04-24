#pragma once

#include <memory>
#include <QWidget>
#include "simulation.h"

class ImageWidget : public QWidget
{
	Q_OBJECT
public:
	explicit ImageWidget(QWidget *parent = 0);              /*creates an interface*/
	void setSimulation(const std::shared_ptr<Simulation> &sim); /* declares a function that takes in a pointer and adds the*/
	void setDisplayWidth(size_t dpw);	/*corresponding simulation to the layout */
	void setConnectionType(uint8_t ctype);

protected:
	void paintEvent(QPaintEvent *event) override;			/*declares the paint event of the interface */
private:
	std::shared_ptr<Simulation> _simulation;				/* declares a pointer to a simulation */
	size_t _dpwidth;
	uint8_t _connectionType;
};
