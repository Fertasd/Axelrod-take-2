#pragma once
#include "simulation.h"
#include "randgen.h"

/* UC stands for user-controlled, modify the values in these rows to tailor the simulation to your needs */

/* RPS game on square lattice with stochastic CA rule      */
/* z=4, start from random initial state                    */
/* MC simulations with logit rule                          */

class Sim_RPS_CA : public Simulation
{
public:
	Sim_RPS_CA(size_t width = 500);							/*UC: declares a simulation with given width */
	//inline QString name() override { return "RPS_CA"; }	/*UC: returns the name of the simulation */
	std::vector<SimParameter> parameters() override;
	uint32_t animationDelay() const override;
	uint32_t renderFrameSkip() const override;
	size_t displayWidth() const override;
	uint8_t connectionType() const override;
	size_t resetType() const override;
	std::string getName() const override;
	void step() override;
	void reset() override;
	inline constexpr static auto name() { return "RPS_CA"; }
	size_t returnSpecies() override;
	size_t stepTargetNumber() const override;	/* declarations of overrides to these functions,
														  allowing them to return values specific to
														  this kind of simulation */
private:
	RandomGenerator rng;								/* initializes a random number generator */
	static constexpr uint8_t Ns = 3;					/* UC: number of species               */
	const SimParameter K{"K", 0.5, "temperature"};		/* This is how a parameter is defined: SimParameter parameter{"Parameter name", parameter value, "Parameter description"} */
	std::vector<uint8_t> data2;							/* declaration of a temporary structure that keeps track changes in a step */
	double pm[Ns][Ns];									/* declaration of the payoff matrix */
};
