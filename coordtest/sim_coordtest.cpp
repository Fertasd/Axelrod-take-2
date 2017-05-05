#include "coordtest/sim_coordtest.h"
#include <exception>
#include "QTextStream"
#include <QMessageBox>
#include <QProgressBar>

/* UC stands for user-controlled, modify the values in these rows to tailor the simulation to your needs */

Sim_coordtest::Sim_coordtest(size_t width) : Simulation(width), data2(width * width)
{
	/* upon the creation of an object of this class, the following happens: */
	auto changedEvent = [=] (double) {
		pm[0][0] = cop+sep+crp; pm[0][1] = -cop+sep-crp;
		pm[1][0] = -cop-sep+crp; pm[1][1] = cop-sep-crp;
	};

	cop.onChanged(changedEvent);
	crp.onChanged(changedEvent);
	sep.onChanged(changedEvent);
	changedEvent(0);

	palette().emplace_back(0, 0, 0, 255);		/* UC: defines the color table, colors can be given in many formats, see the documentation of QColor if the current one(hexadecimal) is not sufficient */
	palette().emplace_back(255, 255, 255, 255);

	reset();		/* assigns the initial state to the simulation */
}

std::vector<SimParameter> Sim_coordtest::parameters()
{
	return {cop, sep, crp, startselect, Temp};		/* UC: adjust this list to only show the parameters you actually have in your simulation. */
}

std::string Sim_coordtest::getName() const
{
	return "coordtest";		/* UC: returns the name of the simulation for internal purposes. */
}

uint32_t Sim_coordtest::animationDelay() const
{
	return 0;		/* UC: returns the animation delay of the simulation(how long it waits before displaying the next state, in ms) */
}

uint32_t Sim_coordtest::renderFrameSkip() const
{
	return 1;		/* UC: returns the frame skip of the simulation(how many steps are done in the background) */
}

size_t Sim_coordtest::displayWidth() const
{
	return 400;		/* UC: returns the default display width of the simulation. */
}

size_t Sim_coordtest::resetType() const
{
	return 5;		/* UC: returns the default initial state of the simulation. 1: random 2: interface 3: interfaces 4: island 5: island 6: empty 7: tilted interface 8: tilted interfaces */
}

uint8_t Sim_coordtest::connectionType() const
{
	return 0;		/* UC: returns the default connection system of the simulation. 0: square lattice 1: triangle lattice 2: random graph*/
}

void Sim_coordtest::step()		/* defines a simulation step */
{
}



void Sim_coordtest::reset()		/* creates a specific strategy distribution across the grid */
{

}

size_t Sim_coordtest::stepTargetNumber() const
{
	return 50000;		/* returns the maximum number of steps, currently unused */
}

size_t Sim_coordtest::returnSpecies()
{
	return Ns;
}

size_t Sim_coordtest::live()
{
  return 0;
}
