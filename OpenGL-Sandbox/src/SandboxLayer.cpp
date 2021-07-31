#include "SandboxLayer.h"
#include "Platform/Windows/WindowsInput.h"

using namespace GLCore;
using namespace GLCore::Utils;

namespace ImGuiUtils
{
	static void HelpMarker(const char* desc, const ImVec4& color = {1.0f, 1.0f, 1.0f, 1.0f})
	{
		ImGui::TextDisabled("(?)");
		if (ImGui::IsItemHovered())
		{
			ImGui::BeginTooltip();
			ImGui::PushTextWrapPos(ImGui::GetFontSize() * 35.0f);
			ImGui::TextColored(color, desc);
			ImGui::PopTextWrapPos();
			ImGui::EndTooltip();
		}
	}
}

SandboxLayer::SandboxLayer()
	: m_CameraController(16.0f / 9.0f)
{
}

SandboxLayer::~SandboxLayer()
{
}

void SandboxLayer::OnAttach()
{
	EnableGLDebugging();

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	
	ImGuiIO& io = ImGui::GetIO();
	io.FontDefault = io.Fonts->AddFontFromFileTTF("assets/fonts/opensans/OpenSans-Regular.ttf", 18);
	io.Fonts->AddFontFromFileTTF("assets/fonts/arial/arial.ttf", 18);
	io.Fonts->AddFontFromFileTTF("assets/fonts/SourceCodePro/SourceCodePro-Regular.ttf", 18);

	SetDarkThemeColor();

	my_image_width = 0;
	my_image_height = 0;
	my_image_texture = 0;
	ret = LoadTextureFromFile("qrcode.png", &my_image_texture, &my_image_width, &my_image_height);
	// Init here
}

void SandboxLayer::OnDetach()
{
	// Shutdown here
}

void SandboxLayer::OnEvent(Event& event)
{
	// Events here
}

void SandboxLayer::OnUpdate(Timestep ts)
{
	// Render here

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void SandboxLayer::OnImGuiRender()
{
	// Note: Switch this to true to enable dockspace
	static bool dockspaceOpen = true;
	static bool opt_fullscreen_persistant = true;
	bool opt_fullscreen = opt_fullscreen_persistant;
	static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;

	// We are using the ImGuiWindowFlags_NoDocking flag to make the parent window not dockable into,
	// because it would be confusing to have two docking targets within each others.
	ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
	if (opt_fullscreen)
	{
		ImGuiViewport* viewport = ImGui::GetMainViewport();
		ImGui::SetNextWindowPos(viewport->Pos);
		ImGui::SetNextWindowSize(viewport->Size);
		ImGui::SetNextWindowViewport(viewport->ID);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
		window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
		window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
	}

	// When using ImGuiDockNodeFlags_PassthruCentralNode, DockSpace() will render our background and handle the pass-thru hole, so we ask Begin() to not render a background.
	if (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode)
		window_flags |= ImGuiWindowFlags_NoBackground;

	// Important: note that we proceed even if Begin() returns false (aka window is collapsed).
	// This is because we want to keep our DockSpace() active. If a DockSpace() is inactive, 
	// all active windows docked into it will lose their parent and become undocked.
	// We cannot preserve the docking relationship between an active window and an inactive docking, otherwise 
	// any change of dockspace/settings would lead to windows being stuck in limbo and never being visible.
	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
	ImGui::Begin("DockSpace Demo", &dockspaceOpen, window_flags);
	ImGui::PopStyleVar();

	if (opt_fullscreen)
		ImGui::PopStyleVar(2);

	// DockSpace
	ImGuiIO& io = ImGui::GetIO();
	ImGuiStyle& style = ImGui::GetStyle();
	float minWinSizeX = style.WindowMinSize.x;
	style.WindowMinSize.x = 370.0f;
	if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
	{
		ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
		ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
	}

	// To get a MenuItem with a checkbox you need to pass a bool to it 

	if (ImGui::BeginMenuBar())
	{
		if (ImGui::BeginMenu("File"))
		{
			// Disabling fullscreen would allow the window to be moved to the front of other windows, 
			// which we can't undo at the moment without finer window depth/z control.
			//ImGui::MenuItem("Fullscreen", NULL, &opt_fullscreen_persistant);1
			if (ImGui::MenuItem("New", "Ctrl+N"))
				LOG_INFO("New");

			if (ImGui::MenuItem("Open...", "Ctrl+O"))
				LOG_INFO("Open");

			if (ImGui::MenuItem("Save As...", "Ctrl+Shift+S"))
				LOG_INFO("Save as");

			if (ImGui::MenuItem("Exit")) 
				Application::Get().Close();

			ImGui::EndMenu();
		}

		if (ImGui::BeginMenu("Edit"))
		{
			if (ImGui::MenuItem("Demo Window"))
				m_showDemoWindow = m_showDemoWindow ? false : true;

			if (ImGui::MenuItem("Font"))
				m_showFontPopup = true;;

			if (ImGui::MenuItem("Write Image"))
				QRImageWriteTest();
				//ImageWriteTest(128, 128, 3);

			ImGui::EndMenu();
		}

		if (ImGui::BeginMenu("Help"))
		{
			if (ImGui::MenuItem("Computerinformation"))
				m_showComputerinfoPopup = true;

			// temporary
			ImGui::MenuItem("Check test", NULL, &test);

			ImGui::EndMenu();
		}

		ImGui::EndMenuBar();
	}

	ImGui::Begin("Test");
	ImGui::Checkbox("Show QR Code", &m_showWindow);

	if (m_showWindow)
	{
		QRWindow();
	}

	if (m_showDemoWindow)
		ImGui::ShowDemoWindow();
		

	ImGui::End();
	ImGui::End();

	ComputerInfoPopup();
	FontPopup();
}

void SandboxLayer::ComputerInfoPopup()
{
	if (m_showComputerinfoPopup)
	{
		ImGui::OpenPopup("Computerinformation###ComputerinfoPopup");
		m_showComputerinfoPopup = false;
	}

	ImVec2 center = ImGui::GetMainViewport()->GetCenter();
	ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));

	if (ImGui::BeginPopupModal("Computerinformation###ComputerinfoPopup", nullptr, ImGuiWindowFlags_AlwaysAutoResize))
	{
		ImGui::Text("GPU: %s", m_GraphicsCard);
		ImGui::Text("Vendor: %s", m_Vendor);

		if (ImGui::Button("Close"))
			ImGui::CloseCurrentPopup();

		ImGui::EndPopup();
	}
}

void SandboxLayer::FontPopup()
{
	if (m_showFontPopup)
	{
		ImGui::OpenPopup("Fonts###Fontpopup");
		m_showFontPopup = false;
	}

	ImVec2 center = ImGui::GetMainViewport()->GetCenter();
	ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));

	if (ImGui::BeginPopupModal("Fonts###Fontpopup", nullptr, ImGuiWindowFlags_AlwaysAutoResize))
	{
		// From ImGui::ShowFontSelector
		// Maybe should take a better look at this
		ImGuiIO& io = ImGui::GetIO();
		ImFont* font_current = ImGui::GetFont();
		if (ImGui::BeginCombo("Font", font_current->GetDebugName()))
		{
			for (int n = 0; n < io.Fonts->Fonts.Size; n++)
			{
				ImFont* font = io.Fonts->Fonts[n];
				ImGui::PushID((void*)font);
				if (ImGui::Selectable(font->GetDebugName(), font == font_current))
					io.FontDefault = font;
				ImGui::PopID();
			}
			ImGui::EndCombo();
		}
		ImGui::SameLine();
		ImGuiUtils::HelpMarker("Warning: Some fonts may look weird!", { 1.0f, 0.0f, 0.0f, 1.0f });

		if (ImGui::Button("Close"))
			ImGui::CloseCurrentPopup();

		ImGui::EndPopup();
	}
}

void SandboxLayer::QRWindow()
{
	IM_ASSERT(ret);
	ImGui::Begin("OpenGL Texture Text");
	ImGui::Text("pointer = %p", my_image_texture);
	ImGui::Text("size = %d x %d", my_image_width, my_image_height);
	ImGui::Image((void*)(intptr_t)my_image_texture, ImVec2(my_image_width, my_image_height));
	ImGui::End();
}

void SandboxLayer::SetDarkThemeColor()
{
	// Taken from the Hazel-dev made by TheCherno
	auto& colors = ImGui::GetStyle().Colors;
	colors[ImGuiCol_WindowBg] = ImVec4{ 0.1f, 0.105f, 0.11f, 1.0f };

	// Headers
	colors[ImGuiCol_Header] = ImVec4{ 0.2f, 0.205f, 0.21f, 1.0f };
	colors[ImGuiCol_HeaderHovered] = ImVec4{ 0.3f, 0.305f, 0.31f, 1.0f };
	colors[ImGuiCol_HeaderActive] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };

	// Buttons
	colors[ImGuiCol_Button] = ImVec4{ 0.2f, 0.205f, 0.21f, 1.0f };
	colors[ImGuiCol_ButtonHovered] = ImVec4{ 0.3f, 0.305f, 0.31f, 1.0f };
	colors[ImGuiCol_ButtonActive] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };

	// Frame BG
	colors[ImGuiCol_FrameBg] = ImVec4{ 0.2f, 0.205f, 0.21f, 1.0f };
	colors[ImGuiCol_FrameBgHovered] = ImVec4{ 0.3f, 0.305f, 0.31f, 1.0f };
	colors[ImGuiCol_FrameBgActive] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };

	// Tabs
	colors[ImGuiCol_Tab] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };
	colors[ImGuiCol_TabHovered] = ImVec4{ 0.38f, 0.3805f, 0.381f, 1.0f };
	colors[ImGuiCol_TabActive] = ImVec4{ 0.28f, 0.2805f, 0.281f, 1.0f };
	colors[ImGuiCol_TabUnfocused] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };
	colors[ImGuiCol_TabUnfocusedActive] = ImVec4{ 0.2f, 0.205f, 0.21f, 1.0f };

	// Title
	colors[ImGuiCol_TitleBg] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };
	colors[ImGuiCol_TitleBgActive] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };
	colors[ImGuiCol_TitleBgCollapsed] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };

	// Resize Grip
	colors[ImGuiCol_ResizeGrip] = ImVec4(0.91f, 0.91f, 0.91f, 0.25f);
	colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.81f, 0.81f, 0.81f, 0.67f);
	colors[ImGuiCol_ResizeGripActive] = ImVec4(0.46f, 0.46f, 0.46f, 0.95f);

	// Scrollbar
	colors[ImGuiCol_ScrollbarBg] = ImVec4(0.02f, 0.02f, 0.02f, 0.53f);
	colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.31f, 0.31f, 0.31f, 1.0f);
	colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.41f, 0.41f, 0.41f, 1.0f);
	colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.51f, 0.51f, 0.51f, 1.0f);

	// Check Mark
	colors[ImGuiCol_CheckMark] = ImVec4(0.94f, 0.94f, 0.94f, 1.0f);

	// Slider
	colors[ImGuiCol_SliderGrab] = ImVec4(0.94f, 0.94f, 0.94f, 0.5f);
	colors[ImGuiCol_SliderGrabActive] = ImVec4(0.94f, 0.94f, 0.94f, 0.25f);
}


// Prints the given QrCode object to the console.
void printQr(const qrcodegen::QrCode& qr) {
	int border = 4;
	for (int y = -border; y < qr.getSize() + border; y++) {
		for (int x = -border; x < qr.getSize() + border; x++) {
			std::cout << (qr.getModule(x, y) ? "##" : "  ");
		}
		std::cout << std::endl;
	}
	std::cout << std::endl;
}

void SandboxLayer::BasicQRDemo()
{
	const char* text = "Hello, world!";
	const qrcodegen::QrCode::Ecc errCorLv1 = qrcodegen::QrCode::Ecc::MEDIUM;

	const qrcodegen::QrCode qr = qrcodegen::QrCode::encodeText(text, errCorLv1);
	printQr(qr);
}

void SandboxLayer::ImageWriteTest(unsigned char w, unsigned char h, unsigned char channels_num)
{
	unsigned char *data = new unsigned char[w * h * channels_num];

	int index = 0;
	for (int j = h - 1; j >= 0; --j)
	{
		for (int i = 0; i < w; ++i)
		{
			data[index++] = (unsigned char)(255.0 * i / w);
			data[index++] = (unsigned char)(255.0 * j / h);
			data[index++] = (unsigned char)(255.0 * 0.2);
		}
	}

	stbi_write_jpg("jpg_test.jpg", w, h, channels_num, data, w * channels_num);

	LOG_INFO("Wrote Image");
	delete[] data;
}

void SandboxLayer::QRImageWriteTest()
{
	std::string qrText = "Hello World!";
	std::string filename = "qrcode.png";

	int imgSize = 300;
	int minModulePixelSize = 3;
	QrToPng exampleQrPng1 = QrToPng(filename, imgSize, minModulePixelSize, qrText, true, qrcodegen::QrCode::Ecc::MEDIUM);

	std::cout << "Writing Example QR code 1 (normal) to " << filename << " with text: '" << qrText << "', size: " <<
		imgSize << "x" << imgSize << ", qr module pixel size: " << minModulePixelSize << ". " << std::endl;

	if (exampleQrPng1.writeToPNG())
		std::cout << "Success!" << std::endl;
	else
		std::cerr << "Failure..." << std::endl;
}

bool SandboxLayer::LoadTextureFromFile(const char* filename, GLuint* out_texture, int* out_width, int* out_height)
{
	m_Timer.Reset();
	// Load from file
	int image_width = 0;
	int image_height = 0;
	unsigned char* image_data = stbi_load(filename, &image_width, &image_height, NULL, 4);
	if (image_data == NULL)
		return false;

	// Create a OpenGL texture identifier
	GLuint image_texture;
	glGenTextures(1, &image_texture);
	glBindTexture(GL_TEXTURE_2D, image_texture);

	// Setup filtering parameters for display
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE); // This is required on WebGL for non power-of-two textures
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE); // Same

	// Upload pixels into texture
#if defined(GL_UNPACK_ROW_LENGTH) && !defined(__EMSCRIPTEN__)
	glPixelStorei(GL_UNPACK_ROW_LENGTH, 0);
#endif
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image_width, image_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image_data);
	stbi_image_free(image_data);

	*out_texture = image_texture;
	*out_width = image_width;
	*out_height = image_height;

	LOG_INFO("Needed {0} ms to load image", m_Timer.ElapsedTimeinMillis());

	return true;
}
