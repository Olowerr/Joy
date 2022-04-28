#pragma once
#include "Pickup.h"
using namespace std::chrono;

class Points
{
public:
	Points();
	void setTrackedPickups(Pickup* pickups_in);

	// == Time ==
	void StartTimer();
	float getTotalTimeTaken();
	// == Points ==
	UINT getFinalScore();


private:
	Pickup* pickups;
	
	// == Time ==
	steady_clock::time_point startTime;
	steady_clock::time_point endTime;
	duration<float> timer;
	// == Points ==
	UINT totalScore;

private:
	// == Time ==
	void StopTimer();
};