#include "OgreApp.hpp"

int main(int argc, char *argv[])
{
	// Create application object
	ant::OgreApp * app = new(std::nothrow) ant::OgreApp();

	app->m_settings.m_level = "world\\physics_test.xml";

	if (!app->init())
	{
		std::cout << "Could not initialize app!" << std::endl;
	}

	int exitCode = app->run();

	delete app;

	app = NULL;

	return exitCode;
}