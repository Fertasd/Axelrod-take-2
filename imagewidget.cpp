#include "imagewidget.h"
#include <QPainter>
#include <algorithm>
#include "utils.h"

ImageWidget::ImageWidget(QWidget *parent)
	: QWidget(parent), _dpwidth(std::numeric_limits<size_t>::max()), _connectionType(255)
{ }

void ImageWidget::setSimulation(const std::shared_ptr<Simulation> &sim)
{
	_simulation = sim;			/* links a simulation pointer to the display */
}

void ImageWidget::setDisplayWidth(size_t dpw)
{
	_dpwidth = dpw;
}

void ImageWidget::setConnectionType(uint8_t ctype)
{
	_connectionType = ctype;
}

void ImageWidget::paintEvent(QPaintEvent*)		/* defines what happens when the display gets painted */
{
	if (!_simulation)			/* if there is no simulation, no painting occurs */
		return;

	SIM_ASSERT_M(_dpwidth != std::numeric_limits<size_t>::max(), "_dpwidth must be initialized.");
	SIM_ASSERT_M(_connectionType != 255, "_connectionType must be initialized.");

	const int minside = std::min(width(), height());
	QRect square(0, 0, minside, minside);
	int step = 0;
	switch(_connectionType) /* This is where the size of a little square is defined */
	{
	case 0:
		step = minside/_simulation->width();
		break;
	case 1:
		step = minside/static_cast<int>(_dpwidth);
		step += step % 2;
		break;
	case 2:
		step = minside/static_cast<int>(_dpwidth);
		break;
	default:
		SIM_FAIL_M("Unknown connection type " + std::to_string(_connectionType));
	}

	QPainter painter(this); /* creates a painter object(QPainter) that fills every square with the color corresponding to its strategy */
	//for (size_t i = 0; i < _simulation->displayWidth(); ++i)

	switch(_connectionType) // this is where the system-specific drawing algorithms are used
	{
	case 0:
		for (int i = 0; i < static_cast<int>(std::min(_simulation->width(), _simulation->displayWidth())); ++i)
		{
			//for (size_t j = 0; j < _simulation->displayWidth(); ++j)
			for (int j = 0; j < static_cast<int>(std::min(_simulation->width(), _simulation->displayWidth())); ++j)
			{
				painter.fillRect(j*step + square.x(), i*step + square.y(), step, step,_simulation->palette()[_simulation->at(i,j).culture() % _simulation->palette().size()]);
			}
		}
		break;
	case 1:
		for (int i = 0; i < static_cast<int>(std::min(_simulation->width(), _simulation->displayWidth())); ++i)
		{
			for (int j = 0; j < static_cast<int>(std::min(_simulation->width(), _simulation->displayWidth())); ++j)
			{
				if (i % 2 == 0) {
					painter.fillRect(j*step + square.x(), i*step + square.y(), step, step,_simulation->palette()[_simulation->at(i,j).culture() % _simulation->palette().size()]);
				}
				else {
					painter.fillRect(j*step + square.x() + step / 2, i*step + square.y(), step, step,_simulation->palette()[_simulation->at(i,j).culture() % _simulation->palette().size()]);
				}
			}
		}
		break;
	case 2:
		for (int i = 0; i < static_cast<int>(std::min(_simulation->width(), _simulation->displayWidth())); ++i)
		{
			//for (size_t j = 0; j < _simulation->displayWidth(); ++j)
			for (int j = 0; j < static_cast<int>(std::min(_simulation->width(), _simulation->displayWidth())); ++j)
			{
				painter.fillRect(j*step + square.x(), i*step + square.y(), step, step,_simulation->palette()[_simulation->at(i,j).culture() % _simulation->palette().size()]);
			}
		}
		break;
	default:
		SIM_FAIL_M("Unknown connection type " + std::to_string(_connectionType));
	}
}
