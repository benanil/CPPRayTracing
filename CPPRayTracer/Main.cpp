
#include <iostream>
#include <gl/glew.h>
#include <GLFW/glfw3.h>
#include "Math.hpp" 
#include "RayTracer.hpp"

int main()
{
	std::cout << "Hello World!\n";
	Color32 result = Color(1.0, 1.0, 1.0).ConvertToColor32();
	
	std::cout << "r" << int(result.r) << "g" << int(result.g) << "b" << int(result.b);

	RayTracer::Initialize();
	RayTracer::RenderFrame();
}

