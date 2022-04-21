#include "TimeManager.h"

TimeManager::TimeManager()
	: Clock::Clock(), previousTime(getElapsedTime()), currentTime(Time()), fps(0),
	m_deltaTime(0)
{

}

void TimeManager::showFps()
{
	Log::print(std::to_string((int)(fps)) + " fps");
}

void TimeManager::update()
{
	currentTime = getElapsedTime();

	m_deltaTime = currentTime.asSeconds() - previousTime.asSeconds();

	fps = 1.0f / m_deltaTime;

	previousTime = currentTime;
}

