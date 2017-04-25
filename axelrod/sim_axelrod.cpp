#include "sim_axelrod.h"
#include <random>

/* UC stands for user-controlled, modify the values in these rows to tailor the simulation to your needs */

Sim_Axelrod::Sim_Axelrod(size_t width)
	: Simulation(width)
{	/* upon the creation of an object of this class, the following happens: */

	palette().push_back(0xFF730000);		/* UC: defines the color table, colors can be given in many formats, see the documentation of QColor if the current one(hexadecimal) is not sufficient */
	palette().push_back(0xFF333300);
	palette().push_back(0xFF000066);
	palette().push_back(0xFF000000);

	reset();		/* assigns the initial state to the simulation */
}

std::vector<SimParameter> Sim_Axelrod::parameters()
{
	return {startselect, Px, F, q, virint};		/* UC: adjust this list to only show the parameters you actually have in your simulation. */
}

uint32_t Sim_Axelrod::animationDelay() const
{
	return 0;		/* UC: returns the animation delay of the simulation(how long it waits before displaying the next state, in ms) */
}

uint32_t Sim_Axelrod::renderFrameSkip() const
{
	return 1;		/* UC: returns the frame skip of the simulation(how many steps are done in the background) */
}

size_t Sim_Axelrod::displayWidth() const
{
	return 100;			/* UC: returns the default display width of the simulation. */
}

uint8_t Sim_Axelrod::connectionType() const
{
	return 0;			/* UC: returns the default connection system of the simulation. 0: square lattice 1: triangle lattice 2: random graph*/
}

size_t Sim_Axelrod::resetType() const
{
	return 1;			/* UC: returns the default initial state of the simulation. 1: random 2: interface 3: interfaces 4: island 5: island 6: empty 7: tilted interface 8: tilted interfaces */
}

std::string Sim_Axelrod::getName() const
{
	return "Axelrod";		/* UC: returns the name of the simulation for internal purposes. */
}

void Sim_Axelrod::step()		/* defines a simulation step */
{
	#pragma omp parallel default(none)
	{
		thread_local std::mt19937_64 rng(std::random_device{}());
		std::uniform_real_distribution<double> realDist(0, 1);
		#pragma omp parallel for
		for (size_t i = 0; i < width(); i++)
		{
			for (size_t j = 0; j < width(); j++)
			{
				if (realDist(rng) < 1)
				{
					if (realDist(rng) < virint)
					{
						std::uniform_int_distribution<uint64_t> sizeDist(0, _neighborList[player(i, j)].size());
						auto neighbor = _neighborList[player(i, j)][sizeDist(rng)];
					}
				}
			}
		}
	}
}

void Sim_Axelrod::reset()		/* creates a specific strategy distribution across the grid */
{
	thread_local std::mt19937_64 rng(std::random_device{}());
	std::uniform_int_distribution<uint64_t> dist(0, static_cast<uint64_t>(q.value()));
	for (size_t ix = 0; ix < width(); ix++) /* random strategy distribution */
		for (size_t jx = 0; jx < width(); jx++)
		{
			Datapoint &dp = at(ix, jx);
			for (uint8_t index = 0; index < F; index++)
				dp.push_attribute(dist(rng));

			Datapoint::culture_t culture = 0;
			for (uint8_t j = 0; j < F; j++)
			{
				Datapoint::culture_t hmm = dp.attributes()[j];
				culture += hmm * static_cast<Datapoint::culture_t>(std::pow(q, F-j) * q);
			}
			dp.set_culture(culture);
		}
}

size_t Sim_Axelrod::returnSpecies()
{
	return Ns;
}

size_t Sim_Axelrod::stepTargetNumber() const
{
	return 5000;		/* returns the maximum number of steps, currently unused */
}

