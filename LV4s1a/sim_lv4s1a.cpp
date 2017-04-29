#include "sim_lv4s1a.h"

/* UC stands for user-controlled, modify the values in these rows to tailor the simulation to your needs */

Sim_LV4s1a::Sim_LV4s1a(size_t width) : Simulation(width), data2(width * width)

{	/* upon the creation of an object of this class, the following happens: */

	auto changedEvent = [=] (double) {
		pm[0][0] = 0; pm[0][1] = 3; pm[0][2] = 1; pm[0][3] = 2;    /* UC: the payoff matrix is defined here, it must be defined in this fashion to work */
		pm[1][0] = 1; pm[1][1] = 0; pm[1][2] = 1; pm[1][3] = 1;
		pm[2][0] = 3; pm[2][1] = 3; pm[2][2] = 0; pm[2][3] = 1;
		pm[3][0] = 2; pm[3][1] = 3; pm[3][2] = 3; pm[3][3] = 0;
	};
	changedEvent(0);

	palette().emplace_back(0x00FFFFFF);		/* UC: defines the color table, colors can be given in many formats, see the documentation of QColor if the current one(hexadecimal) is not sufficient */
	palette().emplace_back(0x00FF0000);
	palette().emplace_back(0x0033CC00);
	palette().emplace_back(0xFF000000);

	reset();		/* assigns the initial state to the simulation */
}

std::vector<SimParameter> Sim_LV4s1a::parameters()
{
	return {startselect, Px};		/* UC: adjust this list to only show the parameters you actually have in your simulation. */
}

uint32_t Sim_LV4s1a::animationDelay() const
{
	return 0;		/* UC: returns the animation delay of the simulation(how long it waits before displaying the next state, in ms) */
}

uint32_t Sim_LV4s1a::renderFrameSkip() const
{
	return 1200;		/* UC: returns the frame skip of the simulation(how many steps are done in the background) */
}

size_t Sim_LV4s1a::displayWidth() const
{
	return 120;			/* UC: returns the default display width of the simulation. */
}

uint8_t Sim_LV4s1a::connectionType() const
{
	return 0;			/* UC: returns the default connection system of the simulation. 0: square lattice 1: triangle lattice 2: random graph*/
}

size_t Sim_LV4s1a::resetType() const
{
	return 1;			/* UC: returns the default initial state of the simulation. 1: random 2: interface 3: interfaces 4: island 5: island 6: empty 7: tilted interface 8: tilted interfaces */
}

std::string Sim_LV4s1a::getName() const
{
	return "LV4s1a";		/* UC: returns the name of the simulation for internal purposes. */
}

void Sim_LV4s1a::step()		/* defines a simulation step */
{

}

void Sim_LV4s1a::reset()		/* creates a specific strategy distribution across the grid */
{

}

size_t Sim_LV4s1a::returnSpecies()
{
	return Ns;
}

size_t Sim_LV4s1a::live()
{
  return 0;
}

size_t Sim_LV4s1a::stepTargetNumber() const
{
	return 5000;		/* returns the maximum number of steps, currently unused */
}
