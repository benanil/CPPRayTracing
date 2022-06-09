
#include <iostream>
#include <gl/glew.h>
#include <GLFW/glfw3.h>
#include "Math.hpp" 
#include "RayTracer.hpp"
#include <Windows.h>

int main()
{
	std::cout << "Hello World!\n";
	AMath::Quaternion quternion;
	RayTracer::Initialize();
	RayTracer::RenderFrame();
	ShellExecute(nullptr, nullptr, L"export.jpg", nullptr, nullptr, 0);
}

