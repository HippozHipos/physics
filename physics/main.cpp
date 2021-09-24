#define OLC_PGE_APPLICATION
#include "olcPixelGameEngine.h"
#include "Ui.h"
#include "Point.h"

void toggleSimulatingOnSpacePress(olc::PixelGameEngine* pge, bool& simulating)
{
	if (pge->GetKey(olc::SPACE).bReleased)
	{
		simulating = !simulating;
	}
}

class Application : public olc::PixelGameEngine
{
public:
	Application()
	{
		sAppName = "Physics";
	}

private:
	void ResetSimOnCPress()
	{
		if (GetKey(olc::C).bReleased)
		{
			connections.clear();
			points.clear();
			simulating = false;
		}
	}

public:
	bool OnUserCreate() override
	{
		return true;
	}
	bool OnUserUpdate(float elapsedTime) override
	{
		toggleSimulatingOnSpacePress(this, simulating);
		applyMouseForce(this, points);
		ResetSimOnCPress();
		addNodeOnClick(this, points);
		toggleFixedOnClick(this, points);
		freeDraw.freeDrawOnClick(this, points, connections);
		pointConnector.AddConnectionByDragging(this, points, connections);

		if (simulating)
		{
			for (auto& point : points)
			{
				updatePoint(this, *point);
				pointBounceOffBoundary(this, *point, (double)NodeDesignInfo::RADIUS);
			}

			for (auto& connection : connections)
			{
				connectTwoPoints(connection.p1, connection.p2, connection.length);
			}
		}

		Clear({35, 30, 33});
		drawTempConnectionLine(this, pointConnector);
		drawConnectionLines(this, connections);
		drawNodes(this, points);
		drawMouseForceArea(this);
		std::string simInfoText = simulating ? "Simulating" : "Editing";
		DrawString(5, 5, simInfoText, olc::WHITE);

		return true;
	}

private:
	std::vector<std::unique_ptr<Point>> points;
	std::vector<Connection> connections;
	PointConnectionAdder pointConnector;
	FreeDraw freeDraw;
	bool simulating = false;
};

int main()
{
	Application app;
	if (app.Construct(600, 320, 2, 2, false, true))
		app.Start();
	return 0;
}