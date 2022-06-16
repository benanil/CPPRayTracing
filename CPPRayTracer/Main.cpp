
#include <iostream>
#include <gl/glew.h>
#include <GLFW/glfw3.h>
#include "RayTracer.hpp"
#include <Windows.h>
#include "Math/All.hpp"

using namespace mat;

int main()
{
	std::cout << "Hello World!\n";

	float radian = 90.0f * DegToRad;
    Matrix4 mResult = 
		Matrix4::Identity() *
		Matrix4::FromPosition(20, 20, 0) *
		Matrix4::FromQuaternion(Quaternion::FromEuler(0, radian, radian));

	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			std::cout << mResult.m[3-i][j] << " ";
		}
		std::cout << std::endl;
	}
	// RayTracer::Initialize();
	// RayTracer::RenderFrame();


	// ShellExecute(nullptr, nullptr, L"export.jpg", nullptr, nullptr, 0);
}

