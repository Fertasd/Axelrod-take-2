#pragma once
#include <functional>
#include <memory>
#include <vector>
#include <QColor>

class Datapoint
{
public:
	using attribute_t = uint64_t;
	using culture_t = uint64_t;
	Datapoint() = default;
	Datapoint(std::vector<attribute_t> attributes, std::vector<Datapoint*> virneighbors, culture_t culture);
	inline std::vector<attribute_t> & attributes() { return _attributes; }
	inline std::vector<Datapoint*> & virneighbors() { return _virneighbors; }
	inline culture_t culture() const { return _culture; }
	inline culture_t & culture() { return _culture; }
private:
	std::vector<attribute_t> _attributes;
	std::vector<Datapoint*> _virneighbors;
	culture_t _culture;
};

class SimParameter
{
	struct Data
	{
		Data(const QString &name, double value, const QString& description);
		const QString _name;
		double _value;
		const QString _description;
		std::function<void(double)> onChanged;
	};
public:
	SimParameter(const QString &name, double value, const QString& description = QString());
	inline void linkTo(const SimParameter& target) { _data = target._data; }
	inline const QString& name() const { return _data->_name; }
	inline const QString& description() const { return _data->_description; }
	inline double value() const { return _data->_value; }
	inline operator double() const { return _data->_value; }
	SimParameter& operator=(double value);
	void onChanged(std::function<void(double)>);
	inline SimParameter& operator+=(double rhs){ _data->_value += rhs; return *this; }
private:
	std::shared_ptr<Data> _data;
};

class Simulation						/* the parent class for all simulations */
{
public:
	explicit Simulation(size_t width);					/* initializes a simulation */
	virtual ~Simulation() = default;
	virtual std::vector<SimParameter> parameters() { return {}; }
														  /*virtual declarations of the functions that give us the name,
														  animation delay(time between picture updates) and
														  render frame skip(how many steps the simulation runs before
														  updating the picture) of the simulation */
	virtual uint32_t animationDelay() const = 0;
	virtual uint32_t renderFrameSkip() const = 0;
	virtual size_t displayWidth() const = 0;
	virtual uint8_t connectionType() const = 0;
	virtual size_t resetType() const = 0;
	virtual std::string getName() const = 0;
	virtual void step() = 0;							/* virtual declarations of the functions that describe a simulation step or a reset */
	virtual void reset() = 0;
	virtual size_t stepTargetNumber() const = 0;		/* virtually declares maximum number of steps run, currently unused */
	virtual size_t returnSpecies() = 0;
	inline const std::vector<QColor>& palette() const { return _palette; }
	inline std::vector<QColor>& palette() { return _palette; }				/* declaration of the function that calls the color table */
	inline const std::vector<Datapoint>& data() const { return _data; }
	inline std::vector<Datapoint>& data() { return _data; }					/* declaration of a function that calls data from the simulation */
	inline size_t width() const { return _width; }							/* declares a function that returns the side length of a simulation */
	inline const Datapoint& at(size_t i, size_t j) const { return _data[i * _width + j]; }
	inline Datapoint& atNum(size_t num) { return _data[num]; }
	inline const Datapoint& atNum(size_t num) const { return _data[num]; }
	inline size_t player(size_t i, size_t j) { return i * _width + j; }
	SimParameter startselect{"StartSelect", 1, "Initial configuration. 1: random 2: interface 3: interfaces 4: island 5: island 6: empty 7: tilted interface 8: tilted interfaces"}; /*this is used to choose the initial state*/
	void reconnect(uint8_t ctype);
	std::vector<std::vector<size_t>> _neighborList;
private:
	std::vector<Datapoint> _data; /*declaration of the vector that records the data of the simulation*/
	std::vector<QColor> _palette; /* declaration of the color table */
	size_t _width;				/* declaration of size length of the simulation */
};
