#pragma once
#include "simulation.h"

/* UC stands for user-controlled, modify the values in these rows to tailor the simulation to your needs */

/*Axelrod model, new, shiny and fuck me*/

class Sim_Axelrod final : public Simulation
{
public:
	Sim_Axelrod(size_t width = 10);								/*UC: declares a simulation with given width */
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
	inline std::vector<Datapoint> virtuals() { return _virtuals; }
	inline void add_virtual(Datapoint input) {_virtuals.push_back(input);}
	inline void clear_virtuals() {_virtuals.clear();}
	void setup_virtuals();
	void setup_virtuals2(size_t num);
	void setup_virtuals3(size_t num);
	void initialize_media();
	void update_virtuals();
	void update_virtuals3();
	void virtual_connections();
	void update_virconnects();
	void calculate_live();
	void update_clusters();
	std::vector<Datapoint*> findneighbors(size_t i, size_t j);

private:
	SimParameter F{"F", 5, "Number of features"};
	SimParameter q{"q", 15, "Number of traits per feature"};
	SimParameter virint{"virint", 0.2, "Probability of virtual interaction"};
	SimParameter virfor{"virfor", 0.0, "Probability of forced virtual interaction"};
	SimParameter physfor{"physfor", 0.0, "Probability of forced physical interaction"};
	SimParameter remprob{"remprob", 0.0, "Probability of removing differing virtual neighbors"};
	SimParameter addprob{"addprob", 0.0, "Probability of adding similar virtual neighbors"};
	SimParameter addnum{"addnum",1,"number of global agents connected in a single update"};
	SimParameter numglobal{"numglobal", 1, "number of global agents"};
	size_t _live = 0;
	std::vector<Datapoint> _virtuals;
};
