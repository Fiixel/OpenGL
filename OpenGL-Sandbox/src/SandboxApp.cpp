#include "GLCore.h"
#include "SandboxLayer.h"

using namespace GLCore;

class Sandbox : public Application
{
public:
	Sandbox()
		: Application("Test")
	{
		PushLayer(new SandboxLayer());
	}
};

int main()
{
	std::unique_ptr<Sandbox> app = std::make_unique<Sandbox>();
	app->Run();
}