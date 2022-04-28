#include "Points.h"

Points::Points()
	: pickups(nullptr), totalScore(0)
{
}

void Points::setTrackedPickups(Pickup* pickups_in)
{
	pickups = pickups_in;
}

void Points::StartTimer()
{
	startTime = steady_clock::now();
}

float Points::getTotalTimeTaken()
{
	return timer.count();
}

UINT Points::getFinalScore()
{
	StopTimer();
	return totalScore;
}

void Points::StopTimer()
{
	endTime = steady_clock::now();
	timer = duration_cast<duration<float>>(endTime - startTime);

	totalScore = (int)timer.count() + (int)( pickups->getAmountHitPickups() * pickups->getPoints() ); // TODO : Decide on a system...?
}
