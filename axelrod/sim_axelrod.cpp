#include "sim_axelrod.h"
#include <functional>
#include <random>
#include "utils.h"

/* UC stands for user-controlled, modify the values in these rows to tailor the simulation to your needs */

Sim_Axelrod::Sim_Axelrod(size_t width)
	: Simulation(width)
{	/* upon the creation of an object of this class, the following happens: */
	auto changedEvent = [=] (double) {
		reset();
	};

	F.onChanged(changedEvent);
	q.onChanged(changedEvent);
	numglobal.onChanged(changedEvent);

	palette().emplace_back(0xFFff0000);
	palette().emplace_back(0xFFe6d9cf);
	palette().emplace_back(0xFF26805c);
	palette().emplace_back(0xFFa3bfd9);
	palette().emplace_back(0xFFd982c7);
	palette().emplace_back(0xFFbf0000);
	palette().emplace_back(0xFFffdd00);
	palette().emplace_back(0xFF73ffc7);
	palette().emplace_back(0xFF4d5a66);
	palette().emplace_back(0xFFf2daed);
	palette().emplace_back(0xFFbf5656);
	palette().emplace_back(0xFF4c440b);
	palette().emplace_back(0xFFbfffe5);
	palette().emplace_back(0xFF3642b3);
	palette().emplace_back(0xFF7f002f);
	palette().emplace_back(0xFFf2b6b6);
	palette().emplace_back(0xFF26230b);
	palette().emplace_back(0xFF00c5cc);
	palette().emplace_back(0xFF4400cc);
	palette().emplace_back(0xFF330818);
	palette().emplace_back(0xFF807373);
	palette().emplace_back(0xFFb2a550);
	palette().emplace_back(0xFF006f73);
	palette().emplace_back(0xFF210b4d);
	palette().emplace_back(0xFFcc3d72);
	palette().emplace_back(0xFF000000);
	palette().emplace_back(0xFFb2ff00);
	palette().emplace_back(0xFF00191a);
	palette().emplace_back(0xFF140b26);
	palette().emplace_back(0xFF403036);
	palette().emplace_back(0xFF261100);
	palette().emplace_back(0xFFd5f291);
	palette().emplace_back(0xFF0088ff);
	palette().emplace_back(0xFF807399);
	palette().emplace_back(0xFFf27e24);
	palette().emplace_back(0xFF53664d);
	palette().emplace_back(0xFF0b2e4d);
	palette().emplace_back(0xFFa039bf);
	palette().emplace_back(0xFF7f5839);
	palette().emplace_back(0xFF00a61c);
	palette().emplace_back(0xFF73beff);
	palette().emplace_back(0xFFff00cc);
	palette().emplace_back(0xFFf2bb91);
	palette().emplace_back(0xFF0b4d16);
	palette().emplace_back(0xFF457299);
	palette().emplace_back(0xFF661f58);

	reset();		/* assigns the initial state to the simulation */
}

std::vector<SimParameter> Sim_Axelrod::parameters()
{
	return {F, q, virint, _runs, virfor, physfor, remprob, addprob, numglobal, addnum};		/* UC: adjust this list to only show the parameters you actually have in your simulation. */
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
	return 200;			/* UC: returns the default display width of the simulation. */
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
		#pragma omp parallel for collapse(2)
		for (size_t i = 0; i < width(); i++) {
			for (size_t j = 0; j < width(); j++) {
				Datapoint* dp = & at(i,j);
				std::vector<Datapoint*> neighbors = findneighbors(i,j);
				if (realDist(rng) > virint)
				{
					std::uniform_int_distribution<uint64_t> sizeDist(0, neighbors.size()-1);
					Datapoint* neighbor = neighbors[sizeDist(rng)];
					auto overlap_between = dp->overlap(dp, neighbor);
					if ((overlap_between.first > 0 and overlap_between.first < F) or (realDist(rng) < physfor and overlap_between.first < F)){
						if (realDist(rng) > overlap_between.first/F) {
							std::uniform_int_distribution<uint64_t> ovlDist(0, overlap_between.second.size()-1);
							auto switchat = overlap_between.second[ovlDist(rng)];
							dp->attributes()[switchat] = neighbor->attributes()[switchat];
							if (_counter[dp->culture()] > 1){
								_counter[dp->culture()] -=1;
							}
							else{
								_counter.erase(dp->culture());
							}
							Datapoint::culture_t culture = 0;
							for (uint8_t j = 0; j < F; j++)
								culture += dp->attributes()[j] * static_cast<Datapoint::culture_t>(std::pow(q, F-1-j));
							dp->set_culture(culture);
							if (_counter.count(culture)) {
								_counter[culture] = _counter[culture] + 1;
							} else {
								_counter[culture] = 1;
							}
						}
					}


				}
				else {
					Datapoint* dp = & at(i,j);
					std::vector<Datapoint*> virneighborlist;
					for (const auto& element: dp->virneighbors()){
						virneighborlist.push_back(element);
					}
					std::uniform_int_distribution<uint64_t> sizeDist(0, dp->virneighbors().size()-1);
					Datapoint* neighbor = virneighborlist[sizeDist(rng)];
					auto overlap_between = dp->overlap(dp, neighbor);
					if ((overlap_between.first > 0 and overlap_between.first < F) or (realDist(rng) < virfor and overlap_between.first < F)){
						if (realDist(rng) > overlap_between.first/F) {
							std::uniform_int_distribution<uint64_t> ovlDist(0, overlap_between.second.size()-1);
							auto switchat = overlap_between.second[ovlDist(rng)];
							dp->attributes()[switchat] = neighbor->attributes()[switchat];
							Datapoint::culture_t culture = 0;
							for (uint8_t j = 0; j < F; j++)
								culture += dp->attributes()[j] * static_cast<Datapoint::culture_t>(std::pow(q, F-1-j));
							if (_counter[dp->culture()] > 1){
								_counter[dp->culture()] -=1;
							}
							else{
								_counter.erase(dp->culture());
							}
							dp->set_culture(culture);
							if (_counter.count(culture)) {
								_counter[culture] = _counter[culture] + 1;
							} else {
								_counter[culture] = 1;
							}
						}
					}
				}
			}
		}
	}
	update_clusters();
	update_virtuals();
	calculate_live();
	if (virfor > 0 and live() == 0) {
		if (_counter.size() > 1) {
			setlive(1);
		}
	}
}

void Sim_Axelrod::reset()		/* creates a specific strategy distribution across the grid */
{
	setlive(0);
	_counter.clear();
	thread_local std::mt19937_64 rng(std::random_device{}());
	std::uniform_int_distribution<uint64_t> dist(0, static_cast<uint64_t>(q.value()-1));
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
			if (_counter.count(culture)) {
				_counter[culture] = _counter[culture] + 1;
			} else {
				_counter[culture] = 1;
			}
		}
	}
	setup_virtuals2(checked_cast<size_t>(numglobal.value()));
	//setup_virtuals3(3);
	initialize_media();
	calculate_live();
}

size_t Sim_Axelrod::returnSpecies()
{
	return 0;
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
	clear_virtuals();
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
			at(i,j).virneighbors().insert(& virtuals(0));
			virtuals(0).influence().insert(& at(i,j));
		}
	}
}

void Sim_Axelrod::setup_virtuals2(size_t num)
{
	clear_virtuals();
	for (size_t iter = 0; iter < num; iter++){
		Datapoint dp;
		for (Datapoint::attribute_t count = 0; count < F; count++){
			dp.attributes().push_back(0);
		}
		Datapoint::culture_t culture = 0;
		for (uint8_t j = 0; j < F; j++)
			culture += dp.attributes()[j] * static_cast<Datapoint::culture_t>(std::pow(q, F-1-j));
		dp.set_culture(culture);
		add_virtual(dp);
	}
	std::mt19937_64 rng(std::random_device{}());
	std::uniform_int_distribution<size_t> listDist(0, _virtuals.size() - 1);
	for (size_t i = 0; i < width(); i++) {
		for (size_t j = 0; j < width(); j++) {
			at(i,j).virneighbors().clear();
			size_t const index = listDist(rng);
			at(i,j).virneighbors().insert(& virtuals(index));
			virtuals(index).influence().insert(& at(i,j));
		}
	}
}

void Sim_Axelrod::setup_virtuals3(size_t num)
{
	for (size_t i = 0; i < width(); i++) {
		for (size_t j = 0; j < width(); j++) {
			at(i,j).virneighbors().clear();

			for (size_t index = 0; index < num; index++){
				std::mt19937_64 rng(std::random_device{}());
				std::uniform_int_distribution<uint64_t> sizeDist(0, width());
				uint64_t newi = sizeDist(rng);
				uint64_t newj = sizeDist(rng);
				at(i,j).virneighbors().insert(& at(newi, newj));
			}
		}
	}
}



void Sim_Axelrod::initialize_media()
{
	for (Datapoint& virt : _virtuals){
		for (Datapoint::attribute_t attr = 0; attr < F; attr++) {
			std::vector<Datapoint::culture_t> entries;
			Datapoint::culture_t mostfrequent;
			size_t freq;
			for (Datapoint* target : virt.influence()){
				entries.push_back(target->attributes()[attr]);
			}
			mostfrequent = entries[0];
			freq = as_unsigned(std::count(entries.begin(), entries.end(), entries[0]));
			for (size_t iter = 0; iter < entries.size(); ++iter) {
				size_t const count = as_unsigned(std::count(entries.begin(), entries.end(), entries[iter]));
				if (count > freq) {
					mostfrequent = entries[iter];
					freq = count;
				}
			}
			virt.attributes()[attr] = mostfrequent;
		}
		Datapoint::culture_t culture = 0;
		for (uint8_t j = 0; j < F; j++)
			culture += virt.attributes()[j] * checked_cast<Datapoint::culture_t>(std::pow(q, F-1-j));
		virt.set_culture(culture);
	}
}

void Sim_Axelrod::update_virtuals()
{
	for (Datapoint virt : virtuals()) {
		if (virt.influence().size() > 0) {
			for (Datapoint::attribute_t attr = 0; attr < F; attr++) {
				std::vector<Datapoint::culture_t> entries;
				Datapoint::culture_t mostfrequent;
				size_t freq;
				for (Datapoint* target : virt.influence()){
					entries.push_back(target->attributes()[attr]);
				}
				mostfrequent = entries[0];
				freq = as_unsigned(std::count(entries.begin(), entries.end(), entries[0]));
				for (size_t iter = 0; iter < entries.size(); iter++) {
					size_t const count = as_unsigned(std::count(entries.begin(), entries.end(), entries[iter]));
					if (count > freq) {
						mostfrequent = entries[iter];
						freq = count;
					}
				}
				virt.attributes()[attr] = mostfrequent;
			}
			Datapoint::culture_t culture = 0;
			for (uint8_t j = 0; j < F; j++)
				culture += virt.attributes()[j] * static_cast<Datapoint::culture_t>(std::pow(q, F-1-j));
			virt.set_culture(culture);
		}
	}
}

void Sim_Axelrod::update_virtuals3()
{
	for (size_t i = 0; i < width(); i++) {
		for (size_t j = 0; j < width(); j++) {
			std::mt19937_64 rng(std::random_device{}());
			std::uniform_real_distribution<double> realDist(0, 1);
			for ( const auto& virt : at(i,j).virneighbors()){
				Datapoint* dp = & at(i,j);
				auto overlap_between = dp->overlap(dp, virt);
				if (overlap_between.first == 0 and virt->influence().size() == 0){
					if (realDist(rng) < remprob){
						dp->virneighbors().erase(virt);

					}
					if (realDist(rng) < addprob){
						for (size_t i2 = 0; i2 < width(); i++) {
							for (size_t j2 = 0; j2 < width(); j++) {
								auto overlap_between2 = dp->overlap(dp,& at(i2,j2));
								if (dp->virneighbors().count(& at(i2,j2)) == 0) {
									if (overlap_between2.first > 0 and overlap_between2.first < F) {
										dp->virneighbors().insert(& at(i2, j2));
									}
								}
							}
						}
					}
				}
			}
		}
	}
}

void Sim_Axelrod::virtual_connections()
{
	thread_local std::mt19937_64 rng(std::random_device{}());
	std::uniform_int_distribution<uint64_t> sizeDist(0, width());
	for (size_t i = 0; i < width(); i++){
		for (size_t j = 0; j < width(); j++){
			size_t h = sizeDist(rng);
			size_t k = sizeDist(rng);
			at(i,j).virneighbors().insert(& at(h,k));
		}
	}
}

void Sim_Axelrod::calculate_live()
{
	setlive(0);
	for (size_t i = 0; i < width(); i++) {
		for (size_t j = 0; j < width(); j++) {
			Datapoint* dp = & at(i,j);
			std::vector<Datapoint*> neighbors = findneighbors(i,j);
			for (Datapoint * const neighbor : neighbors) {
				std::pair<Datapoint::attribute_t, std::vector<Datapoint::attribute_t>> overlap_between = dp->overlap(dp, neighbor);
				if (overlap_between.first > 0 and overlap_between.first < F){
					setlive(live()+1);
				}
			}
			if (virint > 0){
				for (const auto& neighbor: dp->virneighbors()) {
					std::pair<Datapoint::attribute_t, std::vector<Datapoint::attribute_t>> overlap_between = dp->overlap(dp, neighbor);
					if (overlap_between.first > 0 and overlap_between.first < F){
						setlive(live()+1);
					}
				}
			}
		}
	}
}

void Sim_Axelrod::update_clusters()
{
	for (size_t i = 0; i < width(); i++) {
		for (size_t j = 0; j < width(); j++) {
			std::mt19937_64 rng(std::random_device{}());
			std::uniform_real_distribution<double> realDist(0, 1);
			Datapoint* dp = & at(i,j);
			std::unordered_set<Datapoint*> deathrow;
			for ( Datapoint* virt : dp->virneighbors()){
				auto overlap_between = dp->overlap(dp, virt);
				if (overlap_between.first == 0 and virt->influence().size() > 0){
					deathrow.insert(virt);
					virt->influence().erase(dp);
				}
			}
			for (Datapoint* row : deathrow){
				dp->virneighbors().erase(row);
			}
			size_t maxoverlap = 0;
			std::unordered_set<Datapoint*> maxglobals;
			for ( Datapoint& virt2 : _virtuals) {
				if (dp->overlap(dp, & virt2).first >= maxoverlap and dp->overlap(dp, & virt2).first < F) {
					if (dp->overlap(dp, & virt2).first == maxoverlap) {
						maxglobals.insert(& virt2);
					}
					else{
						maxoverlap = dp->overlap(dp, & virt2).first;
						maxglobals.clear();
						maxglobals.insert(& virt2);
					}
				}
			}
			for (Datapoint* global : maxglobals){
				dp->virneighbors().insert(global);
				global->influence().insert(dp);
			}
		}
	}
}

std::vector<Datapoint*> Sim_Axelrod::findneighbors(size_t i, size_t j)
{
	std::vector<Datapoint*> neighbors;
	if (i != 0 and j != 0 and i != width()-1 and j != width()-1){
		neighbors.push_back(& at(i, j+1));
		neighbors.push_back(& at(i, j-1));
		neighbors.push_back(& at(i+1, j));
		neighbors.push_back(& at(i-1, j));
	}
	if (i == 0 and j != 0 and j != width()-1){
		neighbors.push_back(& at(i, j+1));
		neighbors.push_back(& at(i, j-1));
		neighbors.push_back(& at(i+1, j));
	}
	if (j == 0 and i != 0 and i != width()-1){
		neighbors.push_back(& at(i, j+1));
		neighbors.push_back(& at(i+1, j));
		neighbors.push_back(& at(i-1, j));
	}
	if (j == width()-1 and i != 0 and i != width()-1){
		neighbors.push_back(& at(i, j-1));
		neighbors.push_back(& at(i+1, j));
		neighbors.push_back(& at(i-1, j));
	}
	if (i == width()-1 and j != 0 and j != width()-1){
		neighbors.push_back(& at(i, j+1));
		neighbors.push_back(& at(i, j-1));
		neighbors.push_back(& at(i-1, j));

	}
	if (i == 0 and j == 0){
		neighbors.push_back(& at(i, j+1));
		neighbors.push_back(& at(i+1, j));
	}
	if (i == 0 and j == width()-1){
		neighbors.push_back(& at(i, j-1));
		neighbors.push_back(& at(i+1, j));
	}
	if (i == width()-1 and j == 0){
		neighbors.push_back(& at(i, j+1));
		neighbors.push_back(& at(i-1, j));
	}
	if (i == width()-1 and j == width()-1){
		neighbors.push_back(& at(i, j-1));
		neighbors.push_back(& at(i-1, j));
	}
	return neighbors;
}
