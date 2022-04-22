#include <SFML/Graphics.hpp>

#include <iostream>
#include <string>

#include "log.h"

using namespace sf;

class TimeManager : Clock
{
private:
    Time previousTime;
    Time currentTime;
    float fps;
    float m_deltaTime;

public:
    const float& deltaTime = m_deltaTime;

    TimeManager();
    void update();
    void showFps();
};


