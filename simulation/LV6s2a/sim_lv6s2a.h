#pragma once
#include "simulation.h"
#include "randgen.h"

/* UC stands for user-controlled, modify the values in these rows to tailor the simulation to your needs */

/* lv6s1x.c                                          2003.04.18.  */
/* 6 species each having two predators and two preys              */
/* allowing site exchange between neutral pairs                   */
/* MC simulations                                                 */
/* pair correlations and fluctuations                             */

class Sim_LV6s2a : public Simulation
{
public:
	Sim_LV6s2a(size_t width = 500);								/*UC: declares a simulation with given width */
	inline constexpr static auto name() { return "LV6s2a"; }	/*UC: returns the name of the simulation */
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
	RandomGenerator rng;															/* initializes a random number generator */
	static constexpr uint8_t Ns = 6;												/* UC: number of species               */
	const SimParameter Px{"Px", 0.1, "Exchange rate between neutral pairs"};		/* USEFUL! This is how a parameter is defined: SimParameter parameter{"Parameter name", parameter value, "Parameter description"} */
	std::vector<uint8_t> data2;														/* declaration of a temporary structure that keeps track changes in a step */
	double pm[Ns][Ns];																/* declaration of the payoff matrix */
};
