#pragma once
#include "simulation.h"
#include "randgen.h"

/* UC stands for user-controlled, modify the values in these rows to tailor the simulation to your needs */

/*Axelrod model, new, shiny and fuck me*/

class Sim_Axelrod : public Simulation
{
public:
	Sim_Axelrod(size_t width = 100);								/*UC: declares a simulation with given width */
	inline constexpr static auto name() { return "Axelrod"; }	/*UC: returns the name of the simulation */
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
	static constexpr uint8_t Ns = 4;												/* UC: number of species               */
	const SimParameter Px{"Px", 0.1, "Exchange rate between neutral pairs"};		/* USEFUL! This is how a parameter is defined: SimParameter parameter{"Parameter name", parameter value, "Parameter description"} */
	const SimParameter F{"F", 3, "Number of features"};
	const SimParameter q{"q", 8, "Number of traits per feature"};
	const SimParameter virint{"virint", 0.36, "Probability of virtual interaction"};
	std::vector<std::vector<std::vector<uint8_t>>> data2;														/* declaration of a temporary structure that keeps track changes in a step */
};
