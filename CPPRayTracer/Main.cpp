
#include <iostream>
#include <gl/glew.h>
#include <GLFW/glfw3.h>
#include "Math.hpp" 
#include "RayTracer.hpp"

int main()
{
	std::cout << "Hello World!\n";
	AMath::Quaternion quternion;
	RayTracer::Initialize();
	RayTracer::RenderFrame();
}

