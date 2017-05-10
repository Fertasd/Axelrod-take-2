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
	inline void clear_virtuals() {_virtuals.clear();}
	void setup_virtuals();
	void initialize_media();
	void update_virtuals();
	void virtual_connections();
	void update_virconnects();

private:
	SimParameter F{"F", 4, "Number of features"};
	SimParameter q{"q", 6, "Number of traits per feature"};
	SimParameter virint{"virint", 0.0, "Probability of virtual interaction"};
	SimParameter virfor{"virfor", 0.2, "Probability of forced virtual interaction"};
	SimParameter physfor{"physfor", 0.0, "Probability of forced physical interaction"};
	size_t _live = 0;
	std::vector<Datapoint> _virtuals;
};
