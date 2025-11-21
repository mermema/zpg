#include "App.h"

int main() {
	App* app = new App();
	app->init();
	app->createScenes();
	app->run();
	delete app;
	return 0;
}
