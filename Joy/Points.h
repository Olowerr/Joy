#pragma once
#include "Pickup.h"

class Points
{
public:
	Points(Pickup* pickups_in);
	void StartTimer();
	void CountTimer();
	void StopTimer();
private:
	Pickup* pickups;
	std::chrono::steady_clock::time_point startTime;
	std::chrono::steady_clock::time_point endTime;
};