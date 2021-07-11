#include "SandboxLayer.h"
#include "Platform/Windows/WindowsInput.h"

using namespace GLCore;
using namespace GLCore::Utils;

SandboxLayer::SandboxLayer()
	: m_CameraController(16.0f / 9.0f)
{
	printf("%s\n", m_Vendor);
	printf("%s\n", m_GraphicsCard);
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
	m_ArialFont			= io.Fonts->AddFontFromFileTTF("assets/fonts/arial/arial.ttf", 16);
	m_SourceCodeProFont	= io.Fonts->AddFontFromFileTTF("assets/fonts/SourceCodePro/SourceCodePro-Regular.ttf", 16);
	m_OpenSansFont		= io.Fonts->AddFontFromFileTTF("assets/fonts/opensans/OpenSans-Regular.ttf", 16);
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
				printf("Open\n");

			if (ImGui::MenuItem("Save As...", "Ctrl+Shift+S"))
				printf("Save as\n");

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

			ImGui::EndMenu();
		}

		if (ImGui::BeginMenu("Help"))
		{
			if (ImGui::MenuItem("Computerinformation"))
				m_showComputerinfoPopup = true;

			ImGui::EndMenu();
		}

		ImGui::EndMenuBar();
	}

	auto [x, y] = WindowsInput::GetMousePosition();
	// ImGui here
	ImGui::Begin("Test");

	ImGui::Text("Mouse Position: %f, %f", x, y);
	ImGui::SameLine();
	if (ImGui::Button("Test"))
		m_showWindow = m_showWindow ? false : true;

	if (m_showWindow)
	{
		ImGui::Begin("Button");

		ImGui::End();
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
		ImGui::Text("%s", m_GraphicsCard);

		if (ImGui::Button("OK"))
			ImGui::CloseCurrentPopup();

		ImGui::EndPopup();
	}
}

void SandboxLayer::FontPopup()
{
	if (m_showFontPopup)
	{
		ImGui::OpenPopup("Font###Fontpopup");
		m_showFontPopup = false;
	}

	ImVec2 center = ImGui::GetMainViewport()->GetCenter();
	ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));

	if (ImGui::BeginPopupModal("Font###Fontpopup", nullptr, ImGuiWindowFlags_AlwaysAutoResize))
	{
		ImGui::Text("Warning: A few fonts look like they are note meant for the program.");
		/*
		const char* items[3] = { "Arial", "SourceCodePro", "OpenSans" };
		static const char* item_current = items[0];

		if (ImGui::BeginCombo("Fonts", item_current))
		{
			for (int n = 0; n < IM_ARRAYSIZE(items); n++)
			{
				bool is_selected = (item_current == items[n]);

				if (ImGui::Selectable(items[n], is_selected))
				{
					item_current = items[n];
					//LoadFont(item_current);
					printf("%s: %d\n", item_current, *item_current);
				}

				if (is_selected)
					ImGui::SetItemDefaultFocus();   // Set the initial focus when opening the combo (scrolling + for keyboard navigation support in the upcoming navigation branch)
			}
			ImGui::EndCombo();
		}
		*/

		ImGui::ShowFontSelector("");

		if (ImGui::Button("OK"))
			ImGui::CloseCurrentPopup();

		ImGui::EndPopup();
	}
}

void SandboxLayer::LoadFont(FontType type)
{
	ImGuiIO& io = ImGui::GetIO();

	const char* fonts[3] = { "assets/fonts/arial/arial.ttf", "assets/fonts/opensans/OpenSans-Regular.ttf", "assets/fonts/SourceCodePro/SourceCodePro-Regular.ttf" };

	switch (type)
	{
	case FontType::Arial:
		m_currentFont = io.Fonts->AddFontFromFileTTF(fonts[0], 16);
		break;

	case FontType::OpenSans:
		m_currentFont = io.Fonts->AddFontFromFileTTF(fonts[1], 16);
		break;

	case FontType::SourceCodePro:
		m_currentFont = io.Fonts->AddFontFromFileTTF(fonts[2], 16);
		break;
	}
}