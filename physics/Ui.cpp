#include "Ui.h"
#include "utils.h"

const olc::Pixel NodeDesignInfo::COLOR = olc::BLUE;
const olc::Pixel NodeDesignInfo::TEMP_CONNECTION_LINE_COLOR = { 150, 150, 150 };
const olc::Pixel NodeDesignInfo::CONNECTION_LINE_COLOR = { 230, 230, 230 };
const olc::Pixel NodeDesignInfo::FIXED_COLOR = { 255, 0, 0 };

void addPoint(std::vector<std::unique_ptr<Point>>& points, const olc::vf2d& position, const olc::vf2d& velocity, bool fixed)
{
	points.push_back(std::make_unique<Point>(position, velocity, fixed));
}

void drawNode(olc::PixelGameEngine* pge, const Point& point)
{
	pge->FillCircle(point.position, NodeDesignInfo::RADIUS,
		point.fixed ? NodeDesignInfo::FIXED_COLOR : NodeDesignInfo::COLOR);
}

void drawNodes(olc::PixelGameEngine* pge, const std::vector<std::unique_ptr<Point>>& points)
{
	for (auto& point : points)
	{
		Point* p = point.get();
		drawNode(pge, *point.get());
	}
}

void drawConnectionLines(olc::PixelGameEngine* pge, const std::vector<Connection>& connections)
{
	for (auto& connection : connections)
	{
		pge->DrawLine(connection.p1.position, connection.p2.position, NodeDesignInfo::CONNECTION_LINE_COLOR);
	}
}

Point* pointUnderMouse(olc::PixelGameEngine* pge, const std::vector<std::unique_ptr<Point>>& points,
	double tolerance)
{
	for (auto& point : points)
	{
		olc::vd2d mouse = (olc::vd2d)pge->GetMousePos();
		if (utils::distance(point->position, mouse) < NodeDesignInfo::RADIUS * tolerance)
		{
			return point.get();
		}
	}
	return nullptr;
}

void addNodeOnClick(olc::PixelGameEngine* pge, std::vector<std::unique_ptr<Point>>& points)
{
	if (pointUnderMouse(pge, points, 2.5) == nullptr)
	{
		if (pge->GetMouse(0).bReleased && !pge->GetKey(olc::D).bHeld)
		{
			olc::vd2d mouse = (olc::vd2d)pge->GetMousePos();
			addPoint(points, mouse, { 0.0, 0.0 }, false);
		}
	}
}

void toggleFixedOnClick(olc::PixelGameEngine* pge, std::vector<std::unique_ptr<Point>>& points)
{
	if (pge->GetKey(olc::CTRL).bHeld)
	{
		if (pge->GetMouse(0).bReleased)
		{
			Point* p = pointUnderMouse(pge, points);
			if (p != nullptr)
			{
				p->fixed = !p->fixed;
			}
		}
	}
}

void PointConnectionAdder::AddConnectionByDragging(olc::PixelGameEngine* pge, std::vector<std::unique_ptr<Point>>& points, std::vector<Connection>& connections)
{
	auto reset = [&]
	{
		from = nullptr;
		busy = false;
	};

	if (pge->GetKey(olc::D).bHeld && !pge->GetKey(olc::A).bHeld)
	{
		Point* p = pointUnderMouse(pge, points, 1.5);
		if (pge->GetMouse(0).bPressed)
		{
			if (p != nullptr)
			{
				from = p;
				busy = true;
			}
		}

		if (pge->GetMouse(0).bReleased && pge->GetKey(olc::D).bHeld && !pge->GetKey(olc::A).bHeld)
		{
			if (p == from)
			{
				reset();
			}
			else if (from != nullptr && p != nullptr)
			{
				if (!connectionExists(connections, *p, *from))
				{
					connections.push_back(Connection{ *p, *from, 
						utils::distance(p->position, from->position)});
				}
				reset();
			}
			else if (p == nullptr)
			{
				reset();
			}
		}
	}
	else
	{
		reset();
	}
}

Point* PointConnectionAdder::From() const noexcept
{
	return from;
}

bool PointConnectionAdder::Busy() const noexcept
{
	return busy;
}

void drawTempConnectionLine(olc::PixelGameEngine* pge, const PointConnectionAdder& connector)
{
	if (connector.Busy())
	{
		pge->DrawLine(connector.From()->position, pge->GetMousePos(), NodeDesignInfo::TEMP_CONNECTION_LINE_COLOR, 
		  NodeDesignInfo::TEMP_CONNECTION_LINE_BIT_PATTERN);
	}
}

void applyMouseForce(olc::PixelGameEngine* pge, std::vector<std::unique_ptr<Point>>& points)
{
	if (pge->GetKey(olc::F).bHeld)
	{
		for (auto& point : points)
		{
			olc::vd2d mouse = (olc::vd2d)pge->GetMousePos();
			if (utils::distance(point->position, mouse) < 16.0)
			{
				point->previousPosition -= (point->position - mouse) * 0.5;
			}
		}
	}
}

void drawMouseForceArea(olc::PixelGameEngine* pge)
{
	if (pge->GetKey(olc::F).bHeld)
	{
		pge->FillCircle(pge->GetMousePos(), 10, olc::GREEN);
	}
}

void FreeDraw::freeDrawOnClick(olc::PixelGameEngine* pge, std::vector<std::unique_ptr<Point>>& points,
	std::vector<Connection>& connections)
{
	auto reset = [&]
	{
		from = nullptr;
		busy = false;
	};

	if (pge->GetMouse(0).bHeld && !pge->GetMouse(0).bPressed && pge->GetKey(olc::A).bHeld)
	{
		Point* p = pointUnderMouse(pge, points, 2.0);
		if (p == nullptr)
		{
			addPoint(points, pge->GetMousePos(), { 0.0f, 0.0f }, false);
			if (from != nullptr && !connectionExists(connections, *points.back(), *from))
			{
				if (points.back().get() == from)
				{
					return;
				}

				connections.push_back
				(
					Connection
					{
						*from,
						*points.back(),
						utils::distance(from->position, points.back()->position)
					}
				);
			}
			from = points.back().get();
		}
		else
		{
			if (from != nullptr && !connectionExists(connections, *p, *from))
			{
				if (p == from)
				{
					return;
				}
				connections.push_back
				(
					Connection
					{
						*from,
						*p,
						utils::distance(from->position, p->position)
					}
				);
			}
			from = p;
		}
	}

	if (pge->GetMouse(0).bReleased)
	{
		reset();
	}
}

Point* FreeDraw::From() const noexcept
{
	return from;
}

bool FreeDraw::Busy() const noexcept
{
	return busy;
}