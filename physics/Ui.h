#pragma once
#include "olcPixelGameEngine.h"
#include "Point.h"

struct NodeDesignInfo
{
	static constexpr int RADIUS = 6;
	static constexpr unsigned int TEMP_CONNECTION_LINE_BIT_PATTERN = 0x0F0F0F0F0;
	static const olc::Pixel COLOR;
	static const olc::Pixel TEMP_CONNECTION_LINE_COLOR;
	static const olc::Pixel CONNECTION_LINE_COLOR;
	static const olc::Pixel FIXED_COLOR;
};

void addPoint(std::vector<std::unique_ptr<Point>>& points, const olc::vf2d& position, const olc::vf2d& velocity,
	bool fixed);

void drawNode(olc::PixelGameEngine* pge, const Point& point);
void drawNodes(olc::PixelGameEngine* pge, const std::vector<std::unique_ptr<Point>>& points);
void drawConnectionLines(olc::PixelGameEngine* pge, const std::vector<Connection>& connections);

Point* pointUnderMouse(olc::PixelGameEngine* pge, const std::vector<std::unique_ptr<Point>>& points, 
	double tolerance = 1.0);
void addNodeOnClick(olc::PixelGameEngine* pge, std::vector<std::unique_ptr<Point>>& points);
void toggleFixedOnClick(olc::PixelGameEngine* pge, std::vector<std::unique_ptr<Point>>& points);

class PointConnectionAdder
{
public:
	PointConnectionAdder() = default;
	void AddConnectionByDragging(olc::PixelGameEngine* pge, 
		std::vector<std::unique_ptr<Point>>& points, std::vector<Connection>& connections);

	bool Busy() const noexcept;
	Point* From() const noexcept;

private:
	Point* from = nullptr;
	bool busy = false;
};

void drawTempConnectionLine(olc::PixelGameEngine* pge, const PointConnectionAdder& connector);
void applyMouseForce(olc::PixelGameEngine* pge, std::vector<std::unique_ptr<Point>>& points);
void drawMouseForceArea(olc::PixelGameEngine* pge);

class FreeDraw
{
public:
	FreeDraw() = default;

public:
	void freeDrawOnClick(olc::PixelGameEngine* pge, std::vector<std::unique_ptr<Point>>& points,
		std::vector<Connection>& connections);
	Point* From() const noexcept;
	bool Busy() const noexcept;

private:
	Point* from = nullptr;
	bool busy = false;
};


