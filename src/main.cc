#include "SpellCorrectServer.h"
#include "MyConfig.h"


int main(void)
{
	SM::MyConfig * conf = SM::MyConfig::getInstance();
	SM::SpellCorrectServer * server = SM::SpellCorrectServer::getInstance();

	server->start();
	while(1);
	server->stop();

	return 0;
}
