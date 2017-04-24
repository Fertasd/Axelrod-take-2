#include "sim_tox2xstoch.h"

/* UC stands for user-controlled, modify the values in these rows to tailor the simulation to your needs */

Sim_tox2xstoch::Sim_tox2xstoch(size_t width) : Simulation(width), data2(width * width)

{	/* upon the creation of an object of this class, the following happens: */

	auto changedEvent = [=] (double) {	/* UC: the payoff matrix is defined here, it must be defined in this fashion to work */
		pm[0][0] = 0; pm[0][1] = 4; pm[0][2] = 5; pm[0][3] = 1; pm[0][4] = 1; pm[0][5] = 3; pm[0][6] = 2; pm[0][7] = 3; pm[0][8] = 2; pm[0][9] = 6;
		pm[1][0] = 5; pm[1][1] = 0; pm[1][2] = 4; pm[1][3] = 3; pm[1][4] = 1; pm[1][5] = 1; pm[1][6] = 2; pm[1][7] = 2; pm[1][8] = 3; pm[1][9] = 6;
		pm[2][0] = 4; pm[2][1] = 5; pm[2][2] = 0; pm[2][3] = 1; pm[2][4] = 3; pm[2][5] = 1; pm[2][6] = 3; pm[2][7] = 2; pm[2][8] = 2; pm[2][9] = 6;
		pm[3][0] = 2; pm[3][1] = 3; pm[3][2] = 2; pm[3][3] = 0; pm[3][4] = 4; pm[3][5] = 5; pm[3][6] = 1; pm[3][7] = 1; pm[3][8] = 3; pm[3][9] = 6;
		pm[4][0] = 2; pm[4][1] = 2; pm[4][2] = 3; pm[4][3] = 5; pm[4][4] = 0; pm[4][5] = 4; pm[4][6] = 3; pm[4][7] = 1; pm[4][8] = 1; pm[4][9] = 6;
		pm[5][0] = 3; pm[5][1] = 2; pm[5][2] = 2; pm[5][3] = 4; pm[5][4] = 5; pm[5][5] = 0; pm[5][6] = 1; pm[5][7] = 3; pm[5][8] = 1; pm[5][9] = 6;
		pm[6][0] = 1; pm[6][1] = 1; pm[6][2] = 3; pm[6][3] = 2; pm[6][4] = 3; pm[6][5] = 2; pm[6][6] = 0; pm[6][7] = 4; pm[6][8] = 5; pm[6][9] = 6;
		pm[7][0] = 3; pm[7][1] = 1; pm[7][2] = 1; pm[7][3] = 2; pm[7][4] = 2; pm[7][5] = 3; pm[7][6] = 5; pm[7][7] = 0; pm[7][8] = 4; pm[7][9] = 6;
		pm[8][0] = 1; pm[8][1] = 3; pm[8][2] = 1; pm[8][3] = 3; pm[8][4] = 2; pm[8][5] = 2; pm[8][6] = 4; pm[8][7] = 5; pm[8][8] = 0; pm[8][9] = 6;
	};
	changedEvent(0);

	palette().push_back(0x00FFFFFF);		/* UC: defines the color table, colors can be given in many formats, see the documentation of QColor if the current one(hexadecimal) is not sufficient */
	palette().push_back(0x00FF0000);
	palette().push_back(0x0033CC00);
	palette().push_back(0x00FF6600);
	palette().push_back(0x00FFFF00);
	palette().push_back(0x000000FF);
	palette().push_back(0xFF000000);
	palette().push_back(0x0066FFFF);
	palette().push_back(0x009900CC);
	palette().push_back(0x007900CC);

	reset();		/* assigns the initial state to the simulation */
}

std::vector<SimParameter> Sim_tox2xstoch::parameters()
{
	return {startselect, Px, alfa, migrate/*, Su*/}; /* UC: adjust this list to only show the parameters you actually have in your simulation. */
}

uint32_t Sim_tox2xstoch::animationDelay() const
{
	return 0;		/* UC: returns the default animation delay of the simulation(how long it waits before displaying the next state, in ms) */
}

uint32_t Sim_tox2xstoch::renderFrameSkip() const
{
	return 1200;		/* UC: returns the default frame skip of the simulation(how many steps are done in the background) */
}

size_t Sim_tox2xstoch::displayWidth() const
{
	return 300;			/* UC: returns the default display width of the simulation. */
}

uint8_t Sim_tox2xstoch::connectionType() const
{
	return 0;			/* UC: returns the default connection system of the simulation. 0: square lattice 1: triangle lattice 2: random graph*/
}

size_t Sim_tox2xstoch::resetType() const
{
	return 1;			/* UC: returns the default initial state of the simulation. 1: random 2: interface 3: interfaces 4: island 5: island 6: empty 7: tilted interface 8: tilted interfaces */
}

std::string Sim_tox2xstoch::getName() const
{
	return "tox2xstoch";		/* UC: returns the name of the simulation for internal purposes. */
}

void Sim_tox2xstoch::step()		/* defines a simulation step */
{
	#pragma omp parallel for
	for (size_t eMCS = 0; eMCS < width(); eMCS++) {/* begins changes */
		auto chose = rng.next() % data2.size();
		auto i1 = data2[chose].first;
		auto j1 = data2[chose].second;
		auto neighbor = _neighborList[player(i1, j1)][rng.next() % _neighborList[player(i1, j1)].size()];
		auto s1 = at(i1, j1);
		auto s2 = atNum(neighbor);
		std::pair<size_t, size_t> neighcor = {(neighbor-neighbor % width())/width(), neighbor % width()};
		std::vector<double> birthrates = {1,1,1,1,1,1,1,1,1};
		std::vector<double> deathrates = {1,1,1,1,1,1,1,1,1};
		uint8_t sk = pm[s1][s2];
		double r = 0;
		r = rng.nextNormal();
		if (r < deathrates[s1]){
			at(i1, j1) = 0;
			data2.erase(data2.begin()+chose);
		}
		else{
		switch (sk) {
		case 0: break;
		case 1: atNum(neighbor) = 9;
				data2.erase(std::remove(data2.begin(), data2.end(), neighcor), data2.end());
			break;
		case 2: at(i1, j1) = 9;
				data2.erase(data2.begin()+chose);
			break;
		case 3: r = rng.nextNormal();
			if (r<Px) {
				at(i1, j1) = s2;
				atNum(neighbor) = s1;
			}
			break;
		case 4: r = rng.nextNormal();
			if (r<alfa) {
				atNum(neighbor) = s1;
			}
			break;
		case 5: r = rng.nextNormal();
			if (r<alfa) {
				at(i1, j1) = s2;
			}
			break;
		case 6: r = rng.nextNormal();
			if (r < birthrates[s1]){
				atNum(neighbor) = s1;
				data2.push_back({(neighbor-neighbor % width())/width(), neighbor % width()});
			} else{
				at(i1,j1) = 9;
				atNum(neighbor) = s1;
				data2.erase(data2.begin()+chose);
				data2.push_back({(neighbor-neighbor % width())/width(), neighbor % width()});
			}
			break;
		}
		}/*case sk */
	} /* done with changes */
}

void Sim_tox2xstoch::reset()		/* creates a specific strategy distribution across the grid */
{
	switch (static_cast<uint8_t>(startselect.value())){
	case 1:
		for (size_t ix = 0; ix < width(); ix++) /* random strategy distribution */
			for (size_t jx = 0; jx < width(); jx++){
				 at(ix, jx) = rng.next() % Ns;
				 if (at(ix,jx)!=9){
					 data2.push_back({ix,jx});
				 }
			}
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

size_t Sim_tox2xstoch::returnSpecies()
{
	return Ns;			/* Returns the number of strategies. */
}

size_t Sim_tox2xstoch::stepTargetNumber() const
{
	return 50000;		/* returns the maximum number of steps, currently unused */
}

