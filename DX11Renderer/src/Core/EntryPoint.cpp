#include "Core/Application.h"

using namespace Yassin;

int WINAPI WinMain(
	HINSTANCE hInstance,
	HINSTANCE,
	LPSTR,
	int nCmdShow)
{
	Application app({});

	return app.Run();
}