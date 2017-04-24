#include "sim_socdil_mix_imit.h"

/* UC stands for user-controlled, modify the values in these rows to tailor the simulation to your needs */

Sim_socdil_mix_imit::Sim_socdil_mix_imit(size_t width) : Simulation(width), data2(width * width)

{	/* upon the creation of an object of this class, the following happens: */

	auto changedEvent = [=] (double) {	/* UC: the payoff matrix is defined here, it must be defined in this fashion to work */
		pm[0][0] = P;      pm[0][1] = T;				   pm[0][2] = w * T;
		pm[1][0] = S;      pm[1][1] = R;                   pm[1][2] = w * R + (1 - w) * S;
		pm[2][0] = w * S;  pm[2][1] = w * R + (1 - w) * T; pm[2][2] = w * w * R + w * (1 - w) * (T + S);
	};
	changedEvent(0);
	P.onChanged(changedEvent);
	S.onChanged(changedEvent);
	T.onChanged(changedEvent);
	R.onChanged(changedEvent);
	w.onChanged(changedEvent);

	palette().push_back(0x00FFFFFF);		/* UC: defines the color table, colors can be given in many formats, see the documentation of QColor if the current one(hexadecimal) is not sufficient */
	palette().push_back(0xFF00FFFF);
	palette().push_back(0xFFFFFFFF);

	reset();		/* assigns the initial state to the simulation */
}

std::vector<SimParameter> Sim_socdil_mix_imit::parameters()
{
	return {startselect, T, R, S, P, w, Temp};		/* UC: adjust this list to only show the parameters you actually have in your simulation. */
}

uint32_t Sim_socdil_mix_imit::animationDelay() const
{
	return 0;		/* UC: returns the animation delay of the simulation(how long it waits before displaying the next state, in ms) */
}

uint32_t Sim_socdil_mix_imit::renderFrameSkip() const
{
	return 50;		/* UC: returns the frame skip of the simulation(how many steps are done in the background) */
}

size_t Sim_socdil_mix_imit::displayWidth() const
{
	return 150;		/* UC: returns the default display width of the simulation. */
}

uint8_t Sim_socdil_mix_imit::connectionType() const
{
	return 0;		/* UC: returns the default connection system of the simulation. 0: square lattice 1: triangle lattice 2: random graph*/
}

size_t Sim_socdil_mix_imit::resetType() const
{
	return 1;		/* UC: returns the default initial state of the simulation. 1: random 2: interface 3: interfaces 4: island 5: island 6: empty 7: tilted interface 8: tilted interfaces */
}

std::string Sim_socdil_mix_imit::getName() const
{
	return "socdil_mix_imit";		/* UC: returns the name of the simulation for internal purposes. */
}

void Sim_socdil_mix_imit::step()		/* defines a simulation step */
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

void Sim_socdil_mix_imit::reset()		/* creates a specific strategy distribution across the grid */
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

size_t Sim_socdil_mix_imit::returnSpecies()
{
	return Ns;
}

size_t Sim_socdil_mix_imit::stepTargetNumber() const
{
	return 50000;		/* returns the maximum number of steps, currently unused */
}
