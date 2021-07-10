#pragma once

#include <GLCore.h>
#include <GLCoreUtils.h>

class SandboxLayer : public GLCore::Layer
{
public:
	SandboxLayer();
	virtual ~SandboxLayer();

	virtual void OnAttach() override;
	virtual void OnDetach() override;
	virtual void OnEvent(GLCore::Event& event) override;
	virtual void OnUpdate(GLCore::Timestep ts) override;
	virtual void OnImGuiRender() override;
private:

	GLCore::Utils::OrthographicCameraController m_CameraController;

	bool m_showWindow = false;
	bool m_showComputerinfoPopup = false;

	const char* m_Vendor		= (const char*)glGetString(GL_VENDOR);
	const char* m_GraphicsCard	= (const char*)glGetString(GL_RENDERER);

	void ComputerInfoPopup();

};