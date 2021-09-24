#include "Point.h"
#include "utils.h"

Point::Point(const olc::vd2d& position, const olc::vd2d& velocity, bool fixed) :
	position{ position },
	previousPosition{ position - velocity }, 
	fixed{ fixed }
{
}

void connectTwoPoints(Point& p1, Point& p2, double length)
{
	for (int i = 0; i < 5; i++)
	{
		olc::vd2d centre = (p1.position + p2.position) * 0.5;
		olc::vd2d direction = (p1.position - p2.position).norm();
		if (!p1.fixed)
			p1.position = centre + direction * length / 2.0;
		if (!p2.fixed)
			p2.position = centre - direction * length / 2.0;
	}

}

void pointBounceOffBoundary(olc::PixelGameEngine* pge, Point& point, double radius)
{
	if (!point.fixed)
	{
		double scale = 1.8;
		if (point.position.x < radius)
		{
			point.position.x = radius;
			point.previousPosition.x += (point.position.x - point.previousPosition.x) * scale;
		}
		if (point.position.x > (double)pge->ScreenWidth() - radius)
		{
			point.position.x = (double)pge->ScreenWidth() - radius;
			point.previousPosition.x += (point.position.x - point.previousPosition.x) * scale;
		}
		if (point.position.y < radius)
		{
			point.position.y = radius;
			point.previousPosition.y += (point.position.y - point.previousPosition.y) * scale;
		}
		if (point.position.y > (double)pge->ScreenHeight() - radius)
		{
			point.position.y = (double)pge->ScreenHeight() - radius;
			point.previousPosition.y += (point.position.y - point.previousPosition.y) * scale;
		}
	}
}

void updatePoint(olc::PixelGameEngine* pge, Point& point)
{
	if (!point.fixed)
	{
		olc::vd2d  positionBeforeUpdate = point.position;
		point.position += point.position - point.previousPosition;
		point.position += olc::vd2d{ 0.0, 1.0 } * 100.0 * 
			(double)pge->GetElapsedTime() * (double)pge->GetElapsedTime();
		point.previousPosition = positionBeforeUpdate;
	}
}

bool connectionExists(const std::vector<Connection>& connections, const Point& p1, const Point& p2)
{
	bool exist = false;
	for (auto& connection : connections)
	{
		exist |= (&connection.p1) == (&p1) && (&connection.p2) == (&p2);
		exist |= (&connection.p2) == (&p1) && (&connection.p1) == (&p2);
	}
	return exist;
}

Connection::Connection(Point& p1, Point& p2, double length) :
	p1{ p1 },
	p2{ p2 },
	length{ length } 
{
}
