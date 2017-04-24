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

	palette().push_back(0x00FFFFFF);		/* UC: defines the color table, colors can be given in many formats, see the documentation of QColor if the current one(hexadecimal) is not sufficient */
	palette().push_back(0x00FF0000);
	palette().push_back(0x0033CC00);
	palette().push_back(0xFF000000);

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
	#pragma omp parallel for
	for (size_t eMCS = 0; eMCS < width(); eMCS++) {/* begins changes */
		auto i1 = rng.next() % width();
		auto j1 = rng.next() % width();
		auto neighbor = _neighborList[player(i1, j1)][rng.next() % _neighborList[player(i1, j1)].size()];

		auto s1 = at(i1, j1);
		auto s2 = atNum(neighbor);
		double r = 0;
		uint8_t sk = pm[s1][s2];

		switch (sk) {
		case 0: break;
		case 1: atNum(neighbor) = s1;
			//Nst[s1]++;
			//Nst[s2]--;
			break;
		case 2: r = rng.nextNormal();
			if (r<Px) {
				at(i1, j1) = s2;
				atNum(neighbor)=s1;
			}
			break;
		case 3: at(i1, j1) = s2;
			//Nst[s1]--;
			//Nst[s2]++;
			break;
		} /*case sk */
	} /* done with changes */
}

void Sim_LV4s1a::reset()		/* creates a specific strategy distribution across the grid */
{
	switch (static_cast<uint8_t>(startselect.value())){
	case 1:
		for (size_t ix = 0; ix < width(); ix++) /* random strategy distribution */
			for (size_t jx = 0; jx < width(); jx++)
				 at(ix, jx) = rng.next() % Ns;
		break;
	case 2:
		for (size_t ix = 0; ix < width() / 3; ix++) /* two domains */
			for (size_t jx = 0; jx < width(); jx++)
				 at(ix, jx) = 0;

		for (size_t ix = width() / 3; ix < width(); ix++)
			for (size_t jx = 0; jx < width(); jx++)
				 at(ix, jx) = 1;
		break;
	case 3:
		for (size_t ix = 0; ix < width(); ix++) { /* stripes */
			for (size_t jx=0; jx < width(); jx++) {
				int strat1 = 0;
				if (ix < (3 * (width() / 4.0))) { strat1 = 1;}
				if (ix < (2 * (width() / 4.0))) { strat1 = 0;}
				if (ix < width() / 4.0) { strat1 = 1;}
				at(ix, jx) = strat1;
			}
		}
		break;
	case 4:
		for (size_t ix = 0; ix < width(); ix++) { /* Island */
			for (size_t jx = 0; jx < width(); jx++) {
				at(ix, jx) = 1;
				if (((ix - (width() / 2) * (ix - width() / 2)) + (jx - width() / 2) * (jx - width() / 2)) < (width() / 4) * (width() / 4))
						 at(ix, jx) = 0;
			}
		}
		break;
	case 5:
		for (size_t ix = 0; ix < width(); ix++) { /* Island */
			for (size_t jx = 0; jx < width(); jx++) {
				at(ix, jx) = 0;
				if (((ix - (width() / 2) * (ix  -width() / 2)) + (jx - width() / 2) * (jx - width() / 2)) < (width() / 4) * (width() / 4))
						 at(ix, jx) = 1;
			}
		}
		break;
	case 6:
		//createDomains();
		break;
	case 7:
		for (size_t ix = 0; ix < width(); ix++){ /* tilted domains */
			for (size_t jx = 0; jx < ix; jx++)
				at(ix, jx) = 0;
			for (size_t kx = ix; kx < width(); kx++)
				at(ix, kx) = 1;
		}
		break;
	case 8:
		uint8_t strat1 = 0; /* tilted stripes */
		uint8_t idist = width() / 4;
		for (size_t ix = 0; ix < width(); ix++) {
		   for (size_t jx = 0; jx < width(); jx++) {
				   strat1 = 0;
				   auto xys = ix + jx;
				   if (xys < (7 * idist))
					   strat1 = 1;
				   if (xys < (6 * idist))
					   strat1 = 0;
				   if (xys < (5 * idist))
					   strat1 = 1;
				   if (xys < (4 * idist))
					   strat1 = 0;
				   if (xys < (3 * idist))
					   strat1 = 1;
				   if (xys < (2 * idist))
					   strat1 = 0;
				   if (xys < idist)
					   strat1 = 1;
				   at(ix, jx) = strat1;
		   }
		}

	}
}

size_t Sim_LV4s1a::returnSpecies()
{
	return Ns;
}

size_t Sim_LV4s1a::stepTargetNumber() const
{
	return 5000;		/* returns the maximum number of steps, currently unused */
}
