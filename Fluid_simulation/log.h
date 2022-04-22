#ifndef LOG_H
#define LOG_H

#include <SFML/Graphics.hpp>

#include <iostream>
#include <string>

using namespace sf;

class Log
{
private:
	Log(){}
public:
	enum Level
	{
		Print, Warning, Error
	};

	template< typename T>
	static void print(const T& message);

	template< typename T>
	static void print(const Vector2<T>& vector);
	//static void warn(const std::string& message);
	//static void error(const std::string& message);
};

template< typename T>
void Log::print(const T& message)
{
	std::cout << "[Print] : " << message << std::endl;
}

template< typename T>
void Log::print(const Vector2<T>& vector)
{
	std::cout << "[Print] : " << "(" << vector.x << ", " << vector.y << ")" << std::endl;
}



#endif 
