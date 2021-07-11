#pragma once

#include <GLCore.h>
#include <GLCoreUtils.h>

enum class FontType
{
	Arial = 0, SourceCodePro, OpenSans
};

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
	bool m_showDemoWindow = false;
	bool m_showFontPopup = false;

	ImFont* m_currentFont;
	ImFont* m_ArialFont;
	ImFont* m_OpenSansFont;
	ImFont* m_SourceCodeProFont;

	const char* m_Vendor		= (const char*)glGetString(GL_VENDOR);
	const char* m_GraphicsCard	= (const char*)glGetString(GL_RENDERER);

	void ComputerInfoPopup();
	void FontPopup();

	void LoadFont(FontType type);

};