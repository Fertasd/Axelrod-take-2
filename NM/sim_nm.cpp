#include "sim_nm.h"

/* UC stands for user-controlled, modify the values in these rows to tailor the simulation to your needs */

Sim_NM::Sim_NM(size_t width) : Simulation(width), csok(width), nov(width), data2(width * width),pos(width*width)
{	/* upon the creation of an object of this class, the following happens: */

	auto changedEvent = [=] (double) {	/* UC: the payoff matrix is defined here, it must be defined in this fashion to work */
		pm[0][0] = 0.0; pm[0][1] = tempt;
		pm[1][0] = 0.0; pm[1][1] = 1.0;
	};

	changedEvent(0);
	tempt.onChanged(changedEvent);

	for (size_t i = 0; i < width; i++) { nov[i] = i + 1; csok[i] = i - 1; }
	nov[width - 1] = 0;   csok[0] = width - 1;		/* the index-resolving vectors get defined */

	palette().push_back(0xFF000000);		/* UC: defines the color table, colors can be given in many formats, see the documentation of QColor if the current one(hexadecimal) is not sufficient */
	palette().push_back(0x00FFFFFF);

	reset();		/* assigns the initial state to the simulation */
}

std::vector<SimParameter> Sim_NM::parameters()
{
	return {startselect, tempt};		/* UC: adjust this list to only show the parameters you actually have in your simulation. */
}

uint32_t Sim_NM::renderFrameSkip() const
{
	return 1;		/* UC: returns the frame skip of the simulation(how many steps are done in the background) */
}

size_t Sim_NM::displayWidth() const
{
	return 120;		/* UC: returns the default display width of the simulation. */
}

uint8_t Sim_NM::connectionType() const
{
	return 0;		/* UC: returns the default connection system of the simulation. 0: square lattice 1: triangle lattice 2: random graph*/
}

size_t Sim_NM::resetType() const
{
	return 1;		/* UC: returns the default initial state of the simulation. 1: random 2: interface 3: interfaces 4: island 5: island 6: empty 7: tilted interface 8: tilted interfaces */
}

std::string Sim_NM::getName() const
{
	return "NM";		/* UC: returns the name of the simulation for internal purposes. */
}

uint32_t Sim_NM::animationDelay() const
{
	return 1000;		/* UC: returns the animation delay of the simulation(how long it waits before displaying the next state, in ms) */
}
void Sim_NM::step()		/* defines a simulation step */
{
	#pragma omp parallel for collapse(2)
	for (size_t i = 0; i < width(); i++) {/* begins changes */
		for (size_t j = 0; j < width(); j++) {
			auto s0 = at(i,j);
			pos[i*width()+j] += pm[s0][s0];
			for (auto neighbor : _neighborList[player(i, j)]){
				pos[i*width()+j] += pm[s0][atNum(neighbor)];
				pos[neighbor] += pm[atNum(neighbor)][s0];
			}
		}
	}
	#pragma omp parallel for collapse(2)
	for (size_t i = 0; i < width(); i++) {
		for (size_t j = 0; j < width(); j++) {
			data2[i*width()+j] = at(i,j);
			uint32_t po1 = pos[i*width()+j], po2 = po1;
			auto in = i, jn = nov[j];
			po2 = pos[in*width()+jn];
			if (po2>po1) { po1 = po2; data2[i*width()+j] = at(in,jn); }
			in = nov[i];
			jn = nov[j];
			po2 = pos[in*width()+jn];
			if (po2>po1) { po1 = po2; data2[i*width()+j] = at(in,jn); }
			in = nov[i];
			jn = j;
			po2 = pos[in*width()+jn];
			if (po2>po1) { po1 = po2; data2[i*width()+j] = at(in,jn); }
			in = nov[i];
			jn = csok[j];
			po2 = pos[in*width()+jn];
			if (po2>po1) { po1 = po2; data2[i*width()+j] = at(in,jn); }
			in = i;
			jn = csok[j];
			po2 = pos[in*width()+jn];
			if (po2>po1) { po1 = po2; data2[i*width()+j] = at(in,jn); }
			in = csok[i];
			jn = csok[j];
			po2 = pos[in*width()+jn];
			if (po2>po1) { po1 = po2; data2[i*width()+j] = at(in,jn); }
			in = csok[i];
			jn = j;
			po2 = pos[in*width()+jn];
			if (po2>po1) { po1 = po2; data2[i*width()+j] = at(in,jn); }
			in = csok[i];
			jn = nov[j];
			po2 = pos[in*width()+jn];
			if (po2>po1) { po1 = po2; data2[i*width()+j] = at(in,jn); }
		}
	}/* done with changes */

	for (size_t i = 0; i < width(); i++) {
		for (size_t j = 0; j < width(); j++) {
			//auto s0 = 3*s2[i][j];
			at(i,j) = data2[i*width()+j];
			pos[i*width()+j] = 0.0;
		}
	}/* document changes */
}

void Sim_NM::reset()		/* creates a specific strategy distribution across the grid */
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

size_t Sim_NM::returnSpecies()
{
	return Ns;
}

size_t Sim_NM::stepTargetNumber() const
{
	return 50000;		/* returns the maximum number of steps, currently unused */
}
