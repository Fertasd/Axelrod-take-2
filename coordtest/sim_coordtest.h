#pragma once
#include "simulation.h"
#include "randgen.h"

/* UC stands for user-controlled, modify the values in these rows to tailor the simulation to your needs */

/*                                             2015.07.19.  */
/* Prisoner's Dilemma game with D, C, and Mixed strategies  */
/* z=4, stochastic imitation or best response               */
/* MC simulations                                           */

class Sim_coordtest: public Simulation
{
public:
	Sim_coordtest(size_t width = 400);							/*UC: declares a simulation with given width */
	inline constexpr static auto name() { return "coordtest"; } /*UC: returns the name of the simulation */
	std::vector<SimParameter> parameters() override;
	std::string getName() const override;
	uint32_t animationDelay() const override;
	uint32_t renderFrameSkip() const override;
	size_t displayWidth() const override;
	size_t resetType() const override;
	uint8_t connectionType() const override;
	void step() override;
	void altstep();
	void reset() override;
	size_t stepTargetNumber() const override;			// declarations of overrides to these functions,
	size_t returnSpecies() override;
private:
	RandomGenerator rng{1};								 /* initializes a random number generator */
	static constexpr uint8_t Ns = 2;					 /* UC: number of species/strategies               */
	SimParameter cop{"CoP", 1.0, "coordination payoff"}; /* USEFUL! This is how a parameter is defined: SimParameter parameter{"Parameter name", parameter value, "Parameter description"} */
	SimParameter crp{"CrP", 0.5, "cross-dependent payoff"};
	SimParameter sep{"SeP", -0.08, "self-dependent payoff"};
	const SimParameter Temp{"Temp", 0.3, "temperature"};
	SimParameter startselect{"StartSelect", 5, "Initial configuration. 1: random 2: interface 3: interfaces 4: island 5: island 6: empty 7: tilted interface 8: tilted interfaces"}; /*this is used to choose the initial state*/
	//const SimParameter Su{"Su", 2, "Sampling unit for the Monte Carlo step"};
	std::vector<uint8_t> data2;							/* declaration of a temporary structure that keeps track of changes in a step */
	double pm[Ns][Ns];									/* declaration of the payoff matrix */
};
