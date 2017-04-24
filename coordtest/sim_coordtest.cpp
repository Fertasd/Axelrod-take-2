#include "coordtest/sim_coordtest.h"
#include <stdlib.h>
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

	palette().push_back(QColor(0, 0, 0, 255));		/* UC: defines the color table, colors can be given in many formats, see the documentation of QColor if the current one(hexadecimal) is not sufficient */
	palette().push_back(QColor(255, 255, 255, 255));

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
	 //altstep();

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

/*uint8_t Sim_coordtest::bisectStep(SimParameter param, double newValue)
{
	param = newValue;
	reset();
	for (size_t i = 0; i < 200; i++){
		if (res()[0] == width()*width()){
			return 0;
			break;
		}
		if (res()[1] == width()*width()){
			return 1;
			break;
		}
		#pragma omp parallel for
		 for(size_t ii=0; ii<width(); ii++)
		  {
		   size_t ix = rng.next() % width();
		   size_t jx = rng.next() % width();
		   uint8_t SX = at(ix, jx);
		   size_t ri = rng.next() % static_cast<uint32_t>(zz);
		   auto ip = nov[ix], jp = nov[jx];
		   auto im = csok[ix], jm = csok[jx];
		   auto iy = 0, jy = 0;

		  switch (ri) {
			 case 0:   iy = ip;   jy = jx;   break;
			 case 1:   iy = ix;   jy = jp;   break;
			 case 2:   iy = im;   jy = jx;   break;
			 case 3:   iy = ix;   jy = jm;   break;
							  }

		   uint8_t SY = at(iy, jy);
		   if (SX != SY) {

		   double PX=0.0;
		   PX+=pm[SX][at(ip, jx)];
		   PX+=pm[SX][at(ix, jp)];
		   PX+=pm[SX][at(im, jx)];
		   PX+=pm[SX][at(ix, jm)];

		   ip = nov[iy];      jp = nov[jy];
		   im = csok[iy];     jm = csok[jy];

		   double PY=0.0;
		   PY+=pm[SY][at(ip, jy)];
		   PY+=pm[SY][at(iy, jp)];
		   PY+=pm[SY][at(im, jy)];
		   PY+=pm[SY][at(iy, jm)];
		  double prd = 1.0 / (1 + exp(-(PY - PX)/Temp));
		  double r = rng.nextNormal();
		  if ( r<prd) {
			  res()[SX] -= 1;
			  res()[SY] += 1;
			  at(ix, jx) = SY;

		  }
	}
		 //}
	}
	if (res()[0] > res()[1])
		return 0;
	else
		return 1;
	throw std::runtime_error("no absorbing state found");
}

std::vector<QPair<double, double>> Sim_coordtest::bisectionAnalysis(SimParameter param1, SimParameter param2)
{
	std::vector<QPair<double, double>> retval;
	for (double i = range1Min; i < range1Max; i += biStep){
		param1 = i;
		retval.push_back( QPair<double, double>(i, bisect(param2)) );
		//qStdOut();
	}
	return retval;
}

double Sim_coordtest::bisect(SimParameter param)
{
	double minp = range2Min, maxp = range2Max, retek = 0;
	uint8_t minstrat = bisectStep(param,minp);
	while (maxp - minp > 0.001){
		double halfp = minp + ((abs(maxp - minp) / 2));
		retek = halfp;
		if (bisectStep(param, halfp) == minstrat){
			minp = halfp;
			}
		else maxp = halfp;
			}
	return retek;
}*/




