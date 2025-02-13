#include "ImGuiManager.h"

#ifdef _DEBUG
#pragma comment(lib, "winmm.lib")
#include "Engine/Base/DirectXBase/DirectXBase.h"
#include "Engine/Base/WindowsInfo/WindowsInfo.h"
#include "Engine/Base/DescriptorHeapManager/DescriptorHeapManager.h"
#include "DescriptorHeapManager/DescriptorHeap/DescriptorHeap.h"
#include "Externals/imgui/imgui_impl_dx12.h"
#include "Externals/imgui/imgui_impl_win32.h"
#include "Externals/imgui/ImGuizmo.h"
bool isDrawMonitor = false;
#endif // _DEBUG

void Kyoko::ImGuiManager::Initialize()
{
#ifdef _DEBUG
	DescriptorHeap* srvHeap = DescriptorHeapManager::GetInstance()->GetSRVDescriptorHeap();

	const DescriptorHandles* handles = srvHeap->GetNewDescriptorHandles();

	//ImGuiの初期化
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGui::StyleColorsDark();
	ImGui_ImplWin32_Init(WindowsInfo::GetInstance()->GetHwnd());
	ImGui_ImplDX12_Init(DirectXBase::GetInstance()->GetDevice(),
		2,
		DXGI_FORMAT_R8G8B8A8_UNORM_SRGB,
		srvHeap->GetHeap(),
		handles->cpuHandle,
		handles->gpuHandle);

	ImGuiIO& io = ImGui::GetIO();
	io.Fonts->AddFontFromFileTTF("Resources/Font/GenShinGothic-Heavy.ttf", 16.0f, nullptr, ImGui::GetIO().Fonts->GetGlyphRangesJapanese());
	io.ConfigFlags = ImGuiConfigFlags_DockingEnable;
	ImGuizmo::SetImGuiContext(ImGui::GetCurrentContext());
	Vector2 size = WindowsInfo::GetInstance()->GetWindowSize();
	ImGuizmo::SetOrthographic(false);
	ImGuizmo::SetRect(0, 0, size.x, size.y);
#endif // _DEBUG
}

void Kyoko::ImGuiManager::Finalize()
{
#ifdef _DEBUG
	//解放処理
	ImGui_ImplDX12_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();
#endif // _DEBUG
}

void Kyoko::ImGuiManager::Begin()
{
#ifdef _DEBUG
	ImGui_ImplDX12_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();
	ImGuizmo::BeginFrame();

	/*ImGui::Begin("ImGuiManager");
	ImGui::Checkbox("ImGuiにはめ込み描画するか", &isDrawMonitor);
	ImGui::End();*/
#endif // _DEBUG
}

void Kyoko::ImGuiManager::End()
{
#ifdef _DEBUG
	/*if (isDrawMonitor) {
		ImTextureID my_tex_id = (ImTextureID)DirectXBase::GetInstance()->GetCurrentDescriptorHandle().ptr;
		ImVec2 tex_size = ImVec2(512, 512);
		ImGui::Image(my_tex_id, tex_size);
		ImGui::Begin("My Texture Window");
		ImGui::Image(my_tex_id, tex_size);
		ImGui::End();
	}*/
	//ImGuiの内部コマンドを生成する
	ImGui::Render();
#endif // _DEBUG
}

void Kyoko::ImGuiManager::Draw()
{
#ifdef _DEBUG
	//実際のcommandListのImGuiの描画コマンドを積む。描画処理の終わったタイミング
	ImGui_ImplDX12_RenderDrawData(ImGui::GetDrawData(), DirectXBase::GetInstance()->GetCommandList());
#endif // _DEBUG
}