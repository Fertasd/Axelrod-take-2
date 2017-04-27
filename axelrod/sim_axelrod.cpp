#include "sim_axelrod.h"
#include <functional>
#include <random>

/* UC stands for user-controlled, modify the values in these rows to tailor the simulation to your needs */

Sim_Axelrod::Sim_Axelrod(size_t width)
	: Simulation(width)
{	/* upon the creation of an object of this class, the following happens: */

	palette().push_back(0xFFff0000);
	palette().push_back(0xFFe6d9cf);
	palette().push_back(0xFF26805c);
	palette().push_back(0xFFa3bfd9);
	palette().push_back(0xFFd982c7);
	palette().push_back(0xFFbf0000);
	palette().push_back(0xFFffdd00);
	palette().push_back(0xFF73ffc7);
	palette().push_back(0xFF4d5a66);
	palette().push_back(0xFFf2daed);
	palette().push_back(0xFFbf5656);
	palette().push_back(0xFF4c440b);
	palette().push_back(0xFFbfffe5);
	palette().push_back(0xFF3642b3);
	palette().push_back(0xFF7f002f);
	palette().push_back(0xFFf2b6b6);
	palette().push_back(0xFF26230b);
	palette().push_back(0xFF00c5cc);
	palette().push_back(0xFF4400cc);
	palette().push_back(0xFF330818);
	palette().push_back(0xFF807373);
	palette().push_back(0xFFb2a550);
	palette().push_back(0xFF006f73);
	palette().push_back(0xFF210b4d);
	palette().push_back(0xFFcc3d72);
	palette().push_back(0xFF000000);
	palette().push_back(0xFFb2ff00);
	palette().push_back(0xFF00191a);
	palette().push_back(0xFF140b26);
	palette().push_back(0xFF403036);
	palette().push_back(0xFF261100);
	palette().push_back(0xFFd5f291);
	palette().push_back(0xFF0088ff);
	palette().push_back(0xFF807399);
	palette().push_back(0xFFf27e24);
	palette().push_back(0xFF53664d);
	palette().push_back(0xFF0b2e4d);
	palette().push_back(0xFFa039bf);
	palette().push_back(0xFF7f5839);
	palette().push_back(0xFF00a61c);
	palette().push_back(0xFF73beff);
	palette().push_back(0xFFff00cc);
	palette().push_back(0xFFf2bb91);
	palette().push_back(0xFF0b4d16);
	palette().push_back(0xFF457299);
	palette().push_back(0xFF661f58);


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
				if (realDist(rng) > 0.8)
				{
					Datapoint* dp = & at(i,j);
					std::vector<Datapoint*> neighbors;
					if (i != 0 and j != 0 and i != width()-1 and j != width()-1){
						neighbors.push_back(& at(i, j+1));
						neighbors.push_back(& at(i, j-1));
						neighbors.push_back(& at(i+1, j));
						neighbors.push_back(& at(i-1, j));
						neighbors.push_back(& at(i+1, j+1));
						neighbors.push_back(& at(i-1, j+1));
						neighbors.push_back(& at(i+1, j-1));
						neighbors.push_back(& at(i-1, j-1));
					}
					if (i == 0 and j != 0 and j != width()-1){
						neighbors.push_back(& at(i, j+1));
						neighbors.push_back(& at(i, j-1));
						neighbors.push_back(& at(i+1, j));
						neighbors.push_back(& at(i+1, j+1));
						neighbors.push_back(& at(i+1, j-1));
					}
					if (j == 0 and i != 0 and i != width()-1){
						neighbors.push_back(& at(i, j+1));
						neighbors.push_back(& at(i+1, j));
						neighbors.push_back(& at(i-1, j));
						neighbors.push_back(& at(i+1, j+1));
						neighbors.push_back(& at(i-1, j+1));
					}
					if (j == width()-1 and i != 0 and i != width()-1){
						neighbors.push_back(& at(i, j-1));
						neighbors.push_back(& at(i+1, j));
						neighbors.push_back(& at(i-1, j));
						neighbors.push_back(& at(i+1, j-1));
						neighbors.push_back(& at(i-1, j-1));
					}
					if (i == width()-1 and j != 0 and j != width()-1){
						neighbors.push_back(& at(i, j+1));
						neighbors.push_back(& at(i, j-1));
						neighbors.push_back(& at(i-1, j));
						neighbors.push_back(& at(i-1, j+1));
						neighbors.push_back(& at(i-1, j-1));

					}
					if (i == 0 and j == 0){
						neighbors.push_back(& at(i, j+1));
						neighbors.push_back(& at(i+1, j));
						neighbors.push_back(& at(i+1, j+1));
					}
					if (i == 0 and j == width()-1){
						neighbors.push_back(& at(i, j-1));
						neighbors.push_back(& at(i+1, j));
						neighbors.push_back(& at(i+1, j-1));
					}
					if (i == width()-1 and j == 0){
						neighbors.push_back(& at(i, j+1));
						neighbors.push_back(& at(i-1, j));
						neighbors.push_back(& at(i-1, j+1));
					}
					if (i == width()-1 and j == width()-1){
						neighbors.push_back(& at(i, j-1));
						neighbors.push_back(& at(i-1, j));
						neighbors.push_back(& at(i-1, j+1));
						neighbors.push_back(& at(i-1, j-1));
					}
					if (realDist(rng) > virint)
					{
						std::uniform_int_distribution<uint64_t> sizeDist(0, neighbors.size()-1);
						Datapoint* neighbor = neighbors[sizeDist(rng)];
						auto overlap_between = dp->overlap(dp, neighbor);
						if (overlap_between.first > 0 and overlap_between.first < F){
							if (realDist(rng) > overlap_between.first/F) {
								Datapoint::attribute_t inoverlap = 0;
								for (Datapoint::attribute_t count = 0; count < neighbors.size(); count ++) {
									Datapoint* neighbor = neighbors[count];
									auto overlap_between = dp->overlap(dp, neighbor);
									if (overlap_between.first > 0 and overlap_between.first < F){
										inoverlap +=1;
									}
								}
								for (Datapoint::attribute_t count = 0; count < dp->virneighbors().size(); count ++) {
									Datapoint* neighbor = dp->virneighbors()[count];
									auto overlap_between = dp->overlap(dp, neighbor);
									if (overlap_between.first > 0 and overlap_between.first < F){
										inoverlap += 1;
									}
								}
								std::uniform_int_distribution<uint64_t> ovlDist(0, overlap_between.second.size()-1);
								auto switchat = overlap_between.second[ovlDist(rng)];
								dp->attributes()[switchat] = neighbor->attributes()[switchat];
								Datapoint::culture_t culture = 0;
								for (uint8_t j = 0; j < F; j++)
									culture += dp->attributes()[j] * static_cast<Datapoint::culture_t>(std::pow(q, F-1-j));
								dp->set_culture(culture);
								Datapoint::attribute_t outoverlap = 0;
								for (Datapoint::attribute_t count = 0; count < neighbors.size(); count ++) {
									Datapoint* neighbor = neighbors[count];
									auto overlap_between = dp->overlap(dp, neighbor);
									if (overlap_between.first > 0 and overlap_between.first < F){
										outoverlap += 1;
									}
								}
								for (Datapoint::attribute_t count = 0; count < dp->virneighbors().size(); count ++) {
									Datapoint* neighbor = dp->virneighbors()[count];
									auto overlap_between = dp->overlap(dp, neighbor);
									if (overlap_between.first > 0 and overlap_between.first < F){
										outoverlap += 1;
									}
								}
								setlive(live()-inoverlap+outoverlap);
							}
						}


					}
					else {
						Datapoint* dp = & at(i,j);
						std::uniform_int_distribution<uint64_t> sizeDist(0, dp->virneighbors().size()-1);
						Datapoint* neighbor = dp->virneighbors()[sizeDist(rng)];
						auto overlap_between = dp->overlap(dp, neighbor);
						if (overlap_between.first > 0 and overlap_between.first < F){
							if (realDist(rng) > overlap_between.first/F) {
								Datapoint::attribute_t inoverlap = 0;
								for (Datapoint::attribute_t count = 0; count < neighbors.size(); count ++) {
									Datapoint* neighbor = neighbors[count];
									auto overlap_between = dp->overlap(dp, neighbor);
									if (overlap_between.first > 0 and overlap_between.first < F){
										inoverlap +=2;
									}
								}
								for (Datapoint::attribute_t count = 0; count < dp->virneighbors().size(); count ++) {
									Datapoint* neighbor = dp->virneighbors()[count];
									auto overlap_between = dp->overlap(dp, neighbor);
									if (overlap_between.first > 0 and overlap_between.first < F){
										inoverlap += 2;
									}
								}
								std::uniform_int_distribution<uint64_t> ovlDist(0, overlap_between.second.size()-1);
								auto switchat = overlap_between.second[ovlDist(rng)];
								dp->attributes()[switchat] = neighbor->attributes()[switchat];
								Datapoint::culture_t culture = 0;
								for (uint8_t j = 0; j < F; j++)
									culture += dp->attributes()[j] * static_cast<Datapoint::culture_t>(std::pow(q, F-1-j));
								dp->set_culture(culture);
								Datapoint::attribute_t outoverlap = 0;
								for (Datapoint::attribute_t count = 0; count < neighbors.size(); count ++) {
									Datapoint* neighbor = neighbors[count];
									auto overlap_between = dp->overlap(dp, neighbor);
									if (overlap_between.first > 0 and overlap_between.first < F){
										outoverlap += 2;
									}
								}
								for (Datapoint::attribute_t count = 0; count < dp->virneighbors().size(); count ++) {
									Datapoint* neighbor = dp->virneighbors()[count];
									auto overlap_between = dp->overlap(dp, neighbor);
									if (overlap_between.first > 0 and overlap_between.first < F){
										outoverlap += 2;
									}
								}
								setlive(live()-inoverlap+outoverlap);
							}
						}

					}
				}
			}
		}
	}
}

void Sim_Axelrod::reset()		/* creates a specific strategy distribution across the grid */
{
	setlive(0);
	setup_virtuals();
	thread_local std::mt19937_64 rng(std::random_device{}());
	std::uniform_int_distribution<uint64_t> dist(0, static_cast<uint64_t>(q.value()));
	for (size_t ix = 0; ix < width(); ix++) {/* random strategy distribution */
		for (size_t jx = 0; jx < width(); jx++)
		{
			Datapoint &dp = at(ix, jx);
			dp.attributes().clear();
			for (uint8_t index = 0; index < F; index++)
				dp.push_attribute(dist(rng));

			Datapoint::culture_t culture = 0;
			for (uint8_t j = 0; j < F; j++)
				culture += dp.attributes()[j] * static_cast<Datapoint::culture_t>(std::pow(q, F-1-j));
			dp.set_culture(culture);
			}
	}
	update_virtuals();
	for (size_t i = 0; i < width(); i++) {/* random strategy distribution */
		for (size_t j = 0; j < width(); j++) {
			Datapoint* dp = & at(i,j);
			std::vector<Datapoint*> neighbors;
			if (i != 0 and j != 0 and i != width()-1 and j != width()-1){
				neighbors.push_back(& at(i, j+1));
				neighbors.push_back(& at(i, j-1));
				neighbors.push_back(& at(i+1, j));
				neighbors.push_back(& at(i-1, j));
				neighbors.push_back(& at(i+1, j+1));
				neighbors.push_back(& at(i-1, j+1));
				neighbors.push_back(& at(i+1, j-1));
				neighbors.push_back(& at(i-1, j-1));
			}
			if (i == 0 and j != 0 and j != width()-1){
				neighbors.push_back(& at(i, j+1));
				neighbors.push_back(& at(i, j-1));
				neighbors.push_back(& at(i+1, j));
				neighbors.push_back(& at(i+1, j+1));
				neighbors.push_back(& at(i+1, j-1));
			}
			if (j == 0 and i != 0 and i != width()-1){
				neighbors.push_back(& at(i, j+1));
				neighbors.push_back(& at(i+1, j));
				neighbors.push_back(& at(i-1, j));
				neighbors.push_back(& at(i+1, j+1));
				neighbors.push_back(& at(i-1, j+1));
			}
			if (j == width()-1 and i != 0 and i != width()-1){
				neighbors.push_back(& at(i, j-1));
				neighbors.push_back(& at(i+1, j));
				neighbors.push_back(& at(i-1, j));
				neighbors.push_back(& at(i+1, j-1));
				neighbors.push_back(& at(i-1, j-1));
			}
			if (i == width()-1 and j != 0 and j != width()-1){
				neighbors.push_back(& at(i, j+1));
				neighbors.push_back(& at(i, j-1));
				neighbors.push_back(& at(i-1, j));
				neighbors.push_back(& at(i-1, j+1));
				neighbors.push_back(& at(i-1, j-1));

			}
			if (i == 0 and j == 0){
				neighbors.push_back(& at(i, j+1));
				neighbors.push_back(& at(i+1, j));
				neighbors.push_back(& at(i+1, j+1));
			}
			if (i == 0 and j == width()-1){
				neighbors.push_back(& at(i, j-1));
				neighbors.push_back(& at(i+1, j));
				neighbors.push_back(& at(i+1, j-1));
			}
			if (i == width()-1 and j == 0){
				neighbors.push_back(& at(i, j+1));
				neighbors.push_back(& at(i-1, j));
				neighbors.push_back(& at(i-1, j+1));
			}
			if (i == width()-1 and j == width()-1){
				neighbors.push_back(& at(i, j-1));
				neighbors.push_back(& at(i-1, j));
				neighbors.push_back(& at(i-1, j+1));
				neighbors.push_back(& at(i-1, j-1));
			}
			for (Datapoint::attribute_t count = 0; count < neighbors.size(); count ++) {
				Datapoint* neighbor = neighbors[count];
				auto overlap_between = dp->overlap(dp, neighbor);
				if (overlap_between.first > 0 and overlap_between.first < F){
					setlive(live()+1);
				}
			}
			for (Datapoint::attribute_t count = 0; count < dp->virneighbors().size(); count ++) {
				Datapoint* neighbor = dp->virneighbors()[count];
				auto overlap_between = dp->overlap(dp, neighbor);
				if (overlap_between.first > 0 and overlap_between.first < F){
					setlive(live()+1);
				}
			}
		}
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

size_t Sim_Axelrod::live()
{
	{ return _live; }
}

void Sim_Axelrod::setup_virtuals()
{
	Datapoint dp;
	for (Datapoint::attribute_t count = 0; count < F; count++){
		dp.attributes().push_back(0);
	}
	Datapoint::culture_t culture = 0;
	for (uint8_t j = 0; j < F; j++)
		culture += dp.attributes()[j] * static_cast<Datapoint::culture_t>(std::pow(q, F-1-j));
	dp.set_culture(culture);
	add_virtual(dp);
	for (size_t i = 0; i < width(); i++) {
		for (size_t j = 0; j < width(); j++) {
			at(i,j).virneighbors().clear();
			at(i,j).virneighbors().push_back(& virtuals(0));
		}
	}
}

void Sim_Axelrod::update_virtuals()
{
	for (Datapoint::attribute_t attr = 0; attr < F; attr++) {
		std::vector<Datapoint::culture_t> entries;
		Datapoint::culture_t mostfrequent;
		size_t freq;
		for (size_t i = 0; i < width(); i++){
			for (size_t j = 0; j < width(); j++){
				entries.push_back(at(i,j).attributes()[attr]);
			}
		}
		mostfrequent = entries[0];
		freq = std::count(entries.begin(), entries.end(), entries[0]);
		for (size_t iter = 0; iter < entries.size(); iter++) {
			if (std::count(entries.begin(), entries.end(), entries[iter]) > freq) {
				mostfrequent = entries[iter];
				freq = std::count(entries.begin(), entries.end(), entries[iter]);
			}
		}
		virtuals(0).attributes()[attr] = mostfrequent;
	}
	Datapoint::culture_t culture = 0;
	for (uint8_t j = 0; j < F; j++)
		culture += virtuals(0).attributes()[j] * static_cast<Datapoint::culture_t>(std::pow(q, F-1-j));
	virtuals(0).set_culture(culture);
}

