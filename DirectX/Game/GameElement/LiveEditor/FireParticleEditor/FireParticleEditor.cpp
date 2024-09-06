#include "FireParticleEditor.h"
#include "Camera.h"
#include "TextureManager.h"
#include "RandomGenerator/RandomGenerator.h"

FireParticleEditor::FireParticleEditor(const std::string& mainName, const std::string& name, const uint32_t& no)
{
	CreateStageEditor(mainName, name, no);
	
	tex_ = ParticleManager::GetInstance()->GetDrawData("circle.png", BlendMode::kBlendModeAdd);
	
	SetGlobalVariable();
}

void FireParticleEditor::Update(const float& time)
{
#ifdef _DEBUG
	UpdateGlobalVariable();
#endif // _DEBUG

	RandomGenerator* rand = RandomGenerator::GetInstance();
	hoge ho{};
	ho.pos = pos_;
	ho.vel = { rand->RandFloat(-1.5f,1.5f), rand->RandFloat(3.0f,5.0f),0.0f };
	ho.color = { rand->RandFloat(0.5f,1.0f),rand->RandFloat(0.1f,0.9f),0.1f, 1.0f };
	ho.time = 0.0f;

	aho_.push_back(ho);
	time_ = std::fmodf(time_, 0.1f);

	for (std::list<hoge>::iterator it = aho_.begin(); it != aho_.end();) {
		(*it).time += time;

		(*it).pos += (*it).vel * time;
		(*it).vel.y -= 1.0f * time;

		if ((*it).time >= 4.0f) {
			it = aho_.erase(it);
		}
		else {
			it++;
		}
	}
}

void FireParticleEditor::Draw(const Camera& camera)
{
	if(&camera){}
	for (hoge& ho : aho_) {
		Matrix4x4 matrix = Matrix4x4::MakeAffinMatrix({1.5f,1.5f,1.0f}, Vector3{0.0f,0.0f,0.0f}, ho.pos);
		ParticleManager::GetInstance()->AddParticle(ParticleData{ matrix, Matrix4x4::MakeIdentity4x4(), ho.color }, tex_);
	}
}

void FireParticleEditor::SetGlobalVariable()
{
	stageEditor_->AddItem("座標", pos_);
	ApplyGlobalVariable();
}

void FireParticleEditor::ApplyGlobalVariable()
{
	pos_ = stageEditor_->GetVector3Value("座標");
}
