#pragma once

#include <GLCore.h>
#include <GLCoreUtils.h>

#include "Util/Timer.h"

#include "../vendor/QRCode/QrCode.hpp"
#include "../vendor/stb/stb_image_write.h"
#include "../vendor/qrtopng/QrToPng.h"
#include "../stb_image/stb_image.h"

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
	Timer m_Timer;

	bool m_showWindow = false;
	bool m_showComputerinfoPopup = false;
	bool m_showDemoWindow = false;
	bool m_showFontPopup = false;

	bool test = false;

	ImFont* m_currentFont;

	const char* m_Vendor		= (const char*)glGetString(GL_VENDOR);
	const char* m_GraphicsCard	= (const char*)glGetString(GL_RENDERER);
	char m_QRWordBuffer[256] = "";
	char m_ImageSizeBuffer[5] = "300";

	int my_image_width = 0, my_image_height = 0;
	GLuint my_image_texture = 0;
	bool ret = false;

private:
	void ComputerInfoPopup();
	void FontPopup();
	void QRWindow();
	void SetDarkThemeColor();
	void GenerateQRCode();
};