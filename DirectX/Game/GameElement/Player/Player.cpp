#include "Player.h"
#include "FrameInfo/FrameInfo.h"

void Player::Init()
{

	jumpTexture.push_back("player1.png");
	jumpTexture.push_back("player2.png");
	jumpTexture.push_back("player3.png");
	jumpTexture.push_back("player4.png");
	for (std::string tex : jumpTexture) {
		TextureManager::GetInstance()->LoadTexture(tex);
	}
	random = RandomGenerator::GetInstance();

	//板ポリに画像を貼り付ける
	object_ = std::make_unique<Object>(jumpTexture[0]);
	//右と左のアフィン行列
	lookLeftMatrix = Matrix4x4::MakeAffinMatrix(Vector3{1.0f,1.0f,1.0},Vector3{0.0f,3.14f,0.0f},Vector3{});
	lookRightMatrix = Matrix4x4::MakeAffinMatrix(Vector3{1.0f,1.0f,1.0},Vector3{0.0f,0.0f,0.0f},Vector3{});

	//当たり判定
	CreateCollider(ColliderShape::BOX2D,ColliderType::COLLIDER,ColliderMask::PLAYER);
	AddTargetMask(ColliderMask::FLOOR);
	AddTargetMask(ColliderMask::PAN);

	input_ = Input::GetInstance();

	global = std::make_unique<GlobalVariableUser>("Character","Player");
	SetGlobalVariables();
	object_->Update();

	panTop = std::make_unique<Floor>("bread.png", Vector3{ 0.0f,topLimit ,0.0f }, panSize,ColliderMask::PAN);

	panBottom = std::make_unique<Floor>("bread.png", Vector3{ 0.0f,bottomLimit ,0.0f },panSize);

}

void Player::Update()
{
#ifdef _DEBUG
	//ApplyGlobalVariables();
#endif

#pragma region ImGui
	ImGui::Begin("Play");
	if (ImGui::Button("clear")) {
		floor_.clear();
	}
	ImGui::End();
#pragma endregion
	//天井の判定の代わり
	if (isHitCeiling) {
		HitCeiling();
		floor_.clear();
		if (object_->model->transform_.translate_.y < bottomLimit) {

			HitBottom();
		}
		object_->Update();
		panTop->Update();
		return;
	}

	jumpFlame += FrameInfo::GetInstance()->GetDeltaTime();
	//ジャンプをしない

	jumpForceVec.y = jumpForce;

	if (jumpFlame > kJumpInterval) {
		//スペースでジャンプ初期化
		if (input_->PressedKey(DIK_SPACE)) {
			CreateFloor();
			JumpInit();
		}
	}
	//ジャンプの処理
	if (jumpFlag) {
		Jump();
	}

	const float deltaTime = FrameInfo::GetInstance()->GetDeltaTime();
	object_->model->transform_.translate_ += jumpForceVec * deltaTime;
	jumpForce -= 0.1f;

	//移動制御
	object_->SetTranslate({ std::clamp(object_->GetWorldTransform().translate_.x, -18.0f, 18.0f) ,object_->GetWorldTransform().translate_.y,0.0f });

	//床更新
	for (std::list<std::unique_ptr<Floor>>::iterator it = floor_.begin(); it != floor_.end(); it++){
		(*it)->Update();
	}

	object_->Update();

	ColliderUpdate();

	//パンの更新
	panTop->Update();
	panBottom->Update();
}

void Player::Draw(const Camera* camera)
{
	object_->Draw(*camera);

	//床描画
	for (std::list<std::unique_ptr<Floor>>::iterator it = floor_.begin(); it != floor_.end(); it++)
	{
		(*it)->Draw(camera);
	}
	//パンの描画
	if (isHitCeiling) {
		panTop->Draw(camera);
	}
	panBottom->Draw(camera);
}

void Player::JumpInit()
{
	jumpFlame = 0;
	CommonJumpInit();
}

void Player::CommonJumpInit()
{
	jumpFlag = true;
	jumpXCenter = object_->model->transform_.translate_.x;
	jumpForce = 10.0f;
	jumpForceVec.x = 0.0f;
	jumpForceVec.y = jumpForce;

	currentTexture = randomTextureSelect(preTexture);
	preTexture = currentTexture;
	object_->model->SetTexture(TextureManager::GetInstance()->LoadTexture(jumpTexture[currentTexture]));
}

void Player::Jump()
{
	jumpForceVec.x = 0.0f;
	jumpForceVec.y = jumpForce;

	if (input_->PressingKey(DIK_A)) {
		jumpForceVec.x = -5.0f;
		jumpForce -= 0.05f;
		object_->model->GetMaterialData()->uvTransform = lookLeftMatrix;
	}
	else if (input_->PressingKey(DIK_D)) {
		jumpForceVec.x = 5.0f;
		jumpForce -= 0.05f;
		object_->model->GetMaterialData()->uvTransform = lookRightMatrix;
	}

	ImGui::Begin("Player");
	ImGui::Text("%f", object_->GetWorldTransform().translate_.x);
	ImGui::End();

	jumpForceVec.Length();

}

int Player::randomTextureSelect(int PreTextture)
{
	currentTexture = random->RandInt(0, 3);
	if (currentTexture == PreTextture) {
		currentTexture = randomTextureSelect(PreTextture);
	}
	return currentTexture;
}

void Player::CreateFloor()
{
	jumpXmovement = jumpXCenter - object_->model->transform_.translate_.x;

	jumpXmovement = (std::max)(1.0f, std::abs(jumpXmovement));

	floor_.push_back(std::unique_ptr<Floor>(new Floor("cheese.png",object_->model->transform_.translate_, {std::abs(jumpXmovement),0.1f,1.0f})));
}

void Player::HitCeiling()
{
	const float deltaTime = FrameInfo::GetInstance()->GetDeltaTime();
	dropSpeed_ = 50.0f * deltaTime;
	object_->model->transform_.translate_.y -= dropSpeed_;
	panTop->Move(object_->model->transform_.translate_ - Vector3{0.0f,2.0f,0.0f});
}

void Player::HitBottom()
{
	panTop->Move({ 0.0f,topLimit ,0.0f });
	isHitCeiling = false;

}

void Player::ColliderUpdate()
{	
	SetBox2D(object_->GetWorldTransform().translate_,object_->GetWorldTransform().scale_);
	SetCollider();
}

void Player::OnCollision(const Collider& collider)
{
	if (collider.GetMask() == ColliderMask::FLOOR) {
		CommonJumpInit();
	}
	if (collider.GetMask() == ColliderMask::PAN) {
		isHitCeiling = true;
	}
}

void Player::SetGlobalVariables()
{
	global->AddItem("拡縮",object_->GetWorldTransform().scale_,"トランスフォーム");
	global->AddItem("回転",object_->GetWorldTransform().rotate_,"トランスフォーム");
	global->AddItem("座標",object_->GetWorldTransform().translate_,"トランスフォーム");

	ApplyGlobalVariables();
}

void Player::ApplyGlobalVariables()
{
	object_->SetTranslate(global->GetVector3Value("座標","トランスフォーム"));
}
