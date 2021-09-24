#pragma once
#include "olcPixelGameEngine.h"
#include "pixelGameEngine.h"

class Point
{
public:
	Point(const olc::vd2d& position, const olc::vd2d& velocity, bool fixed);

public:
	olc::vd2d position;
	olc::vd2d previousPosition;
	bool fixed = false;
};

struct Connection
{
	Connection(Point& p1, Point& p2, double length);
	Point& p1;
	Point& p2;
	const double length;
};

void connectTwoPoints(Point& p1, Point& p2, double length);

void pointBounceOffBoundary(olc::PixelGameEngine* pge, Point& point, double radius);
void updatePoint(olc::PixelGameEngine* pge, Point& point);
bool connectionExists(const std::vector<Connection>& connections, const Point& p1, const Point& p2);
