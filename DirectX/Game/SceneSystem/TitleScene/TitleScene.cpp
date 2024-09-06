#include "TitleScene.h"
#include "Kyoko.h"
#include "ImGuiManager/ImGuiManager.h"
#include "ImGuiManager/DopeSheet.h"

TitleScene::TitleScene()
{
	FirstInit();

	camera_->transform_.translate_ = { 0.0f,6.0f,-70.0f };
	camera_->Update();

	dome_ = std::make_unique<Dome>();
	stage_ = std::make_unique<Stage>();
	screen_ = std::make_unique<Screen>("TitleScreen", "Screen");

	title_ = std::make_unique<Sprite>("title.png");
	title_->pos_ = { 630.0f,360.0f };
	title_->size_ *= 2;
	title_->Update();

	space_ = std::make_unique<Sprite>("space.png");
	space_->pos_ = { 640.0f,560.0f };
	space_->Update();

	rainbow_ = std::make_unique<Sprite>("rainbow.png");
	highLumi_ = std::make_unique<HighLumi>();
	post_ = std::make_unique<PostEffect>();
	texcoodY_ = 0.0f;

	scanNoise_ = std::make_unique<ScanNoise>();
	negaposi_ = std::make_unique<NegaPosiInverse>();
	mosaic_ = std::make_unique<Mosaic>();
	rgbShift_ = std::make_unique<RGBShift>();
	rgbShift_->rgbShiftData_->shift = 0.016f;

	spotLightBox_ = std::make_unique<SpotLightBoxAnimation>("TitleSpotLight1");
	spotLightBox2_ = std::make_unique<SpotLightBoxAnimation>("TitleSpotLight2");

	vignette_ = std::make_unique<Vignette>();
	vignette_->color_ = { 0.0f,1.0f,0.2f,1.0f };
	noise_ = std::make_unique<Noise>();
	bloom_ = std::make_unique<Bloom>();
	radial_ = std::make_unique<RadialBlur>();
	radial_->radialBlurData_->width = 0.03f;
	radial_->radialBlurData_->numSamples = 8;

	hsvFilter_ = std::make_unique<HSVFilter>();

	se_.Load("SE/select.mp3","決定音");

	gp_ = std::make_unique<GPUParticle>("hoge", "circle.png");
	gp_->Update(0.0f);

	spotlightAndOutline_ = std::make_unique<SpotLightAndOutline>();
}

void TitleScene::Initialize()
{

}

void TitleScene::Update()
{
	texcoodY_ += 10.0f;
	if (texcoodY_ >= 720.0f) {
		texcoodY_ -= 720.0f;
	}
	rainbow_->SetTextureTopLeft({ 0.0f,texcoodY_ });

	if (input_->PressedGamePadButton(Input::GamePadButton::A)) {
		// シーン切り替え
		ChangeScene(SELECT);
		se_.Play();
	}

#ifdef _DEBUG
	ImGuiCommon::DopeSheet();
	if (input_->PressedKey(DIK_Q)) {
		ChangeScene(STAGE);
		se_.Play();
	}
	else if (input_->PressedKey(DIK_SPACE)) {
		ChangeScene(SELECT);
		se_.Play();
	}
	ImGui::Begin("カメラ");
	ImGui::DragFloat3("位置", &camera_->transform_.translate_.x, 0.01f);
	ImGui::DragFloat3("角度", &camera_->transform_.rotate_.x, 0.01f);
	ImGui::End();

	ImGui::Begin("hsv");
	ImGui::SliderFloat("hue", &hsvFilter_->hsvData_->hue, -1.0f, 1.0f);
	ImGui::SliderFloat("saturate", &hsvFilter_->hsvData_->saturate, -1.0f, 1.0f);
	ImGui::SliderFloat("value", &hsvFilter_->hsvData_->value, -1.0f, 1.0f);
	ImGui::End();

	camera_->Update();
#endif // _DEBUG
	screen_->Update();
	spotLightBox_->Update(0.01f);
	spotLightBox2_->Update(-0.01f);
	noise_->Update(0.01f);
	bloom_->Update();
	gp_->Update(0.0f);
}

void TitleScene::Draw()
{
	WrightPostEffect();

	Kyoko::Engine::PreDraw();
	bloom_->Draw();
	BlackDraw();
	Kyoko::Engine::PostDraw();
}

void TitleScene::WrightPostEffect()
{
	highLumi_->PreDrawScene();
	title_->Draw();
	highLumi_->PostDrawScene();

	post_->PreDrawScene();
	rainbow_->Draw();
	highLumi_->Draw(BlendMode::kBlendModeMultiply);
	post_->PostDrawScene();

	mosaic_->PreDrawScene();
	title_->Draw();
	post_->Draw();
	mosaic_->PostDrawScene();

	rgbShift_->PreDrawScene();
	mosaic_->Draw();
	rgbShift_->PostDrawScene();

	vignette_->PreDrawScene();
	noise_->Draw();
	vignette_->PostDrawScene();

	radial_->PreDrawScene();
	rgbShift_->Draw();
	vignette_->Draw();
	radial_->PostDrawScene();

	screen_->PreDrawScene();
	radial_->Draw();
	screen_->PostDrawScene();

	spotlightAndOutline_->PreDrawOutline();
	spotlightAndOutline_->PostDrawOutline();

	spotlightAndOutline_->PreDrawObject();
	dome_->Draw(camera_.get());
	stage_->Draw(camera_.get());
	screen_->Draw(camera_.get());

	spotLightBox_->Draw(camera_.get());
	spotLightBox2_->Draw(camera_.get());

	space_->Draw();

	BlackDraw();
	spotlightAndOutline_->PostDrawObject();

	spotlightAndOutline_->PreDrawLight();
	spotLightBox_->DrawLight(*camera_.get());
	spotLightBox2_->DrawLight(*camera_.get());
	spotlightAndOutline_->PostDrawLight();

	bloom_->PreDrawScene();
	spotlightAndOutline_->Draw(*camera_.get());
	bloom_->PostDrawScene();
}

