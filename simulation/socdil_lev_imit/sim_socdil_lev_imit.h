#pragma once
#include "simulation.h"
#include "randgen.h"

/* UC stands for user-controlled, modify the values in these rows to tailor the simulation to your needs */

/*                                             2015.07.19.  */
/* Prisoner's Dilemma game with D, C, and Mixed strategies  */
/* z=4, stochastic imitation or best response               */
/* MC simulations                                           */

class Sim_socdil_lev_imit : public Simulation
{
public:
	Sim_socdil_lev_imit(size_t width = 500);							/*UC: declares a simulation with given width */
	inline constexpr static auto name() { return "socdil_lev_imit"; }	/*UC: returns the name of the simulation */
	std::vector<SimParameter> parameters() override;
	uint32_t animationDelay() const override;
	uint32_t renderFrameSkip() const override;
	size_t displayWidth() const override;
	uint8_t connectionType() const override;
	size_t resetType() const override;
	std::string getName() const override;
	void step() override;
	void reset() override;
	size_t returnSpecies() override;
	size_t stepTargetNumber() const override;			/* declarations of overrides to these functions,
														  allowing them to return values specific to
														  this kind of simulation */
private:
	RandomGenerator rng;											/* initializes a random number generator */
	static constexpr uint8_t Ns = 2;								/* UC: number of species/strategies               */
	SimParameter d{"delta", 15, "temptation to choose defection"};	/* USEFUL! This is how a parameter is defined: SimParameter parameter{"Parameter name", parameter value, "Parameter description"} */
	SimParameter e{"epsilon", 50, "reward for cooperation"};
	const SimParameter zz{"zz", 4, "number of neighbors"};
	const SimParameter Temp{"Temp", 0.3, "temperature"};
	//const SimParameter Su{"Su", 2, "Sampling unit for the Monte Carlo step"};
	std::vector<uint8_t> data2;										/* declaration of a temporary structure that keeps track changes in a step */
	double pm[Ns][Ns];												/* declaration of the payoff matrix */
};
