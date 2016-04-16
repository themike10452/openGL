#include "Game.h"
#include <Windows.h>

int main(int argc, void** argv)
{
	try
	{
		openGL::Window window;
		openGL::Game game(window);
		game.Start();
		return 0;
	}
	catch (std::exception e)
	{
		MessageBox(nullptr, e.what(), "Exception", MB_OK);
		return -1;
	}
}