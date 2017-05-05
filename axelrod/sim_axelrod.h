#pragma once
#include "simulation.h"

/* UC stands for user-controlled, modify the values in these rows to tailor the simulation to your needs */

/*Axelrod model, new, shiny and fuck me*/

class Sim_Axelrod final : public Simulation
{
public:
	Sim_Axelrod(size_t width = 5);								/*UC: declares a simulation with given width */
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
	size_t stepTargetNumber() const override;
	size_t live() override;
	inline void setlive(size_t input) { _live = input; }
	inline const Datapoint& virtuals(size_t i) const { return _virtuals[i]; }
	inline Datapoint& virtuals(size_t i) { return _virtuals[i]; }
	inline void add_virtual(Datapoint input) {_virtuals.push_back(input);}
	void setup_virtuals();
	void update_virtuals();
	void virtual_connections();
	void update_virconnects();

private:
	static constexpr uint8_t Ns = 4;												/* UC: number of species               */
	const SimParameter Px{"Px", 0.1, "Exchange rate between neutral pairs"};		/* USEFUL! This is how a parameter is defined: SimParameter parameter{"Parameter name", parameter value, "Parameter description"} */
	SimParameter F{"F", 2, "Number of features"};
	SimParameter q{"q", 3, "Number of traits per feature"};
	SimParameter virint{"virint", 0.0, "Probability of virtual interaction"};
	size_t _live = 0;
	std::vector<Datapoint> _virtuals;
};
