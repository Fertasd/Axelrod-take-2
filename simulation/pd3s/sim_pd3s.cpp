#include "sim_pd3s.h"

/* UC stands for user-controlled, modify the values in these rows to tailor the simulation to your needs */

Sim_pd3s::Sim_pd3s(size_t width) : Simulation(width), data2(width * width)

{	/* upon the creation of an object of this class, the following happens: */

	auto changedEvent = [=] (double) {	/* UC: the payoff matrix is defined here, it must be defined in this fashion to work */
		pm[0][0] = 0.0; pm[0][1] = tempt;   pm[0][2] = 0.0;
		pm[1][0] = 0.0; pm[1][1] = 1.0;     pm[1][2] = 1.0;
		pm[2][0] = -q;  pm[2][1] = 1.0 - q; pm[2][2] = 1.0 - q;
	};
	changedEvent(0);
	q.onChanged(changedEvent);
	tempt.onChanged(changedEvent);

	palette().push_back(0xFF000000);		/* UC: defines the color table, colors can be given in many formats, see the documentation of QColor if the current one(hexadecimal) is not sufficient */
	palette().push_back(0x00FFFFFF);
	palette().push_back(0x000000ED);

	reset();		/* assigns the initial state to the simulation */
}

std::vector<SimParameter> Sim_pd3s::parameters()      /* UC: change this to have your actual parameters in it*/
{
	return {startselect, tempt, Temp, q, xx, yy};		/* UC: adjust this list to only show the parameters you actually have in your simulation. */
}

uint32_t Sim_pd3s::renderFrameSkip() const
{
	return 1;		/* UC: returns the frame skip of the simulation(how many steps are done in the background) */
}

size_t Sim_pd3s::displayWidth() const
{
	return 120;   /* UC: return the default display with*/
}

uint8_t Sim_pd3s::connectionType() const
{
	return 0;    /* UC: returns the default connection system of the simulation. 0: square lattice 1: triangle lattice 2: random graph*/
}

size_t Sim_pd3s::resetType() const
{
	return 1;	/* UC: returns the default initial state of the simulation. 1: random 2: interface 3: interfaces 4: island 5: island 6: empty 7: tilted interface 8: tilted interfaces */
}

std::string Sim_pd3s::getName() const
{
	return "pd3s"; /* UC: returns the name of the simulation for internal uses*/
}

uint32_t Sim_pd3s::animationDelay() const
{
	return 200;		/* UC: returns the animation delay of the simulation(how long it waits before displaying the next state, in ms) */
}

void Sim_pd3s::step()		/* defines a simulation step */
{
	 //altstep();
	 #pragma omp parallel for
	 for(size_t ii = 0; ii < width() * width(); ii++)
	  {
	   size_t ixd = rng.next() % width();
	   size_t jxd = rng.next() % width();
	   uint8_t SXD = at(ixd, jxd);
	   size_t rid = rng.next() % static_cast<uint32_t>(_neighborList[player(ixd, jxd)].size());

	   size_t partner = _neighborList[player(ixd, jxd)][rid];
	   uint8_t SY = atNum(partner);
	   if (SXD != SY) {

		   double PX=0.0;
		   for (auto neighbor : _neighborList[player(ixd, jxd)]){
			   PX += pm[SXD][atNum(neighbor)];
		   }
		   double PY=0.0;
		   for (auto neighbor : _neighborList[partner]){
			   PY += pm[SY][atNum(neighbor)];
		   }
		  double prd = 1.0 / (1 + exp(-(PY - PX)/Temp));
		  double r = rng.nextNormal();
		  if ( r < prd) {
			   at(ixd, jxd) = SY;
		  }
	  }
}
}

void Sim_pd3s::reset()		/* creates a specific strategy distribution across the grid */
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
				if ((((ix - (width() / 2)) * (ix - (width() / 2))) + (jx - (width() / 2)) * (jx - (width() / 2))) < (width() / 4) * (width() / 4))
						 at(ix, jx) = 0;
			}
		}
		break;
	case 5:
		for (size_t ix = 0; ix < width(); ix++) { /* Island */
			for (size_t jx = 0; jx < width(); jx++) {
				at(ix, jx) = 0;
				if ((((ix - (width() / 2)) * (ix - (width() / 2))) + (jx - (width() / 2)) * (jx - (width() / 2))) < (width() / 4) * (width() / 4))
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

size_t Sim_pd3s::returnSpecies()
{
	return Ns;
}  /*MCS*/

size_t Sim_pd3s::stepTargetNumber() const
{
	return 500000;		/* returns the maximum number of steps, currently unused */
}
