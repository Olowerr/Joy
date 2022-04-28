#include "Points.h"

Points::Points()
	: pickups(nullptr)
{
}

bool Points::setTrackedPickups(Pickup* pickups_in)
{
	if (pickups = nullptr)
	{
		pickups = pickups_in;
		return true;
	}
	else
	{
		return false;
	}
}

void Points::StartTimer()
{
	startTime = std::chrono::steady_clock::now();
}

void Points::CountTimer()
{
}

void Points::StopTimer()
{
	endTime = std::chrono::steady_clock::now();

	std::chrono::duration<double> timerr = std::chrono::duration_cast<std::chrono::duration<double>>(endTime - startTime);
	int q = 0;

}
