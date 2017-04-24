#pragma once
#include "simulation.h"
#include "randgen.h"

/* UC stands for user-controlled, modify the values in these rows to tailor the simulation to your needs */

/* pd24nmt.c                                 2016.06.21.  */
/* evolutionary PD game with bad and good moving teachers */
/* z=24, start from random initial state                  */
/* MC simulations                                         */

class Sim_pd3sl : public Simulation
{
public:
	Sim_pd3sl(size_t width = 500);								/*UC: declares a simulation with given width */
	inline constexpr static auto name() { return "pd3sl"; }		/*UC: returns the name of the simulation */
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
    size_t stepTargetNumber() const override;		/* declarations of overrides to these functions,
                                                          allowing them to return values specific to
														  this kind of simulation */
private:
	RandomGenerator rng;											/* initializes a random number generator */
	static constexpr uint8_t Ns = 3;								/* UC: number of species               */
	SimParameter T{"T", 1.1, "Temptation to choose defection"};		/* USEFUL! This is how a parameter is defined: SimParameter parameter{"Parameter name", parameter value, "Parameter description"} */
    SimParameter sigma{"sigma", 0.3, "loner's payoff"};
    SimParameter Temp{"Temp", 0.2, "Temperature (noise, possibility of trembles)"};
    SimParameter S{"S", 0.000, "Suckers payoff"};
    const SimParameter xx{"xx", 0.0, "Probability of interacting with far neighbors"};
	const SimParameter yy{"yy", 0.0, "Probability of spontaneously choosing a random strategy"};
	std::vector<uint8_t> data2;										/* declaration of a temporary structure that keeps track changes in a step */
	double pm[Ns][Ns];												/* declaration of the payoff matrix */
};
