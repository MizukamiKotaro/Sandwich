#include "Player.h"
#include "FrameInfo/FrameInfo.h"

void Player::Init()
{
	//ジャンプのテクスチャ
	jumpTexture.push_back("player1.png");
	jumpTexture.push_back("player2.png");
	jumpTexture.push_back("player3.png");
	jumpTexture.push_back("player4.png");
	for (std::string tex : jumpTexture) {
		TextureManager::GetInstance()->LoadTexture(tex);
	}

	jumpSE = std::make_unique<Audio>();
	jumpSE->Load("jump.mp3", "プレイヤーのジャンプの音");

	random = RandomGenerator::GetInstance();

	//板ポリに画像を貼り付ける
	object_ = std::make_unique<Object>(jumpTexture[0]);
	//右と左のアフィン行列
	lookLeftMatrix = Matrix4x4::MakeAffinMatrix(Vector3{ 1.0f,1.0f,1.0 }, Vector3{ 0.0f,3.14f,0.0f }, Vector3{});
	lookRightMatrix = Matrix4x4::MakeAffinMatrix(Vector3{ 1.0f,1.0f,1.0 }, Vector3{ 0.0f,0.0f,0.0f }, Vector3{});

	//当たり判定
	CreateCollider(ColliderShape::BOX2D, ColliderType::COLLIDER, ColliderMask::PLAYER);
	AddTargetMask(ColliderMask::FLOOR);
	AddTargetMask(ColliderMask::PAN);

	input_ = Input::GetInstance();

	global = std::make_unique<GlobalVariableUser>("Character", "Player");
	SetGlobalVariables();
	object_->Update();

	panTop = std::make_unique<Floor>("bread.png", Vector3{ 0.0f,topLimit ,0.0f }, panSize, ColliderMask::PAN);

	panBottom = std::make_unique<Floor>("bread.png", Vector3{ 0.0f,bottomLimit ,0.0f }, panSize);

	//キャラクターのサイズ変更
	object_->model->transform_.scale_ = {2.0f,2.0f,1.0f};

}

void Player::Update()
{
#ifdef _DEBUG
	ApplyGlobalVariables();
#endif

	//天井に当たった時の処理
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
	object_->SetTranslate({ std::clamp(object_->GetWorldTransform().translate_.x, -Xlimit, Xlimit) ,object_->GetWorldTransform().translate_.y,0.0f });

	//床更新
	for (std::list<std::unique_ptr<Floor>>::iterator it = floor_.begin(); it != floor_.end(); it++) {
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
	jumpForce = kJumpForce;
	jumpForceVec.x = 0.0f;
	jumpForceVec.y = jumpForce;
	//テクスチャをランダムに変える
	currentTexture = randomTextureSelect(preTexture);
	preTexture = currentTexture;
	object_->model->SetTexture(TextureManager::GetInstance()->LoadTexture(jumpTexture[currentTexture]));
	//jumpSEを再生
	jumpSE->Play();
}

void Player::Jump()
{
	jumpForceVec.x = 0.0f;
	jumpForceVec.y = jumpForce;

	if (input_->PressingKey(DIK_A)) {
		jumpForceVec.x = -kJumpForceX;
		jumpForce -= gravity;
		object_->model->GetMaterialData()->uvTransform = lookLeftMatrix;
	}
	else if (input_->PressingKey(DIK_D)) {
		jumpForceVec.x = kJumpForceX;
		jumpForce -= gravity;
		object_->model->GetMaterialData()->uvTransform = lookRightMatrix;
	}

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

	floor_.push_back(std::unique_ptr<Floor>(new Floor("cheese.png", object_->model->transform_.translate_, { std::abs(jumpXmovement),0.1f,1.0f })));
}

void Player::HitCeiling()
{
	const float deltaTime = FrameInfo::GetInstance()->GetDeltaTime();
	dropSpeed_ = kDropSpeed * deltaTime;
	object_->model->transform_.translate_.y -= dropSpeed_;
	panTop->Move(Vector3{ 0.0f,object_->model->transform_.translate_.y,0.0f } - Vector3{ 0.0f,2.0f,0.0f });
}

void Player::HitBottom()
{
	panTop->Move({ 0.0f,topLimit ,0.0f });
	isHitCeiling = false;

}

void Player::ColliderUpdate()
{
	SetBox2D(object_->GetWorldTransform().translate_, object_->GetWorldTransform().scale_);
	SetCollider();
}

void Player::OnCollision(const Collider& collider)
{
	if (collider.GetMask() == ColliderMask::FLOOR) {
		if (isHitFlag) {
			return;
		}
		CommonJumpInit();
		isHitFlag = true;
	}
	if (collider.GetMask() == ColliderMask::PAN) {
		isHitCeiling = true;
	}
}

void Player::SetGlobalVariables()
{
	global->AddItem("落下速度", kDropSpeed, "落下関連");


	global->AddItem("ジャンプ力", kJumpForce, "ジャンプ");
	global->AddItem("横移動の大きさ", kJumpForceX, "ジャンプ");
	global->AddItem("ジャンプのインターバル", kJumpInterval, "ジャンプ");
	
	global->AddItem("左右の制限", Xlimit, "移動制御");

	global->AddItem("上のパンの位置", topLimit, "パン");
	global->AddItem("下のパンの位置", bottomLimit, "パン");

	ApplyGlobalVariables();
}

void Player::ApplyGlobalVariables()
{
	kDropSpeed = global->GetFloatValue("落下速度","落下関連");

	kJumpForce = global->GetFloatValue("ジャンプ力","ジャンプ");
	kJumpForceX = global->GetFloatValue("横移動の大きさ", "ジャンプ");
	kJumpInterval = global->GetFloatValue("ジャンプのインターバル", "ジャンプ");

	topLimit = global->GetFloatValue("上のパンの位置", "パン");
	bottomLimit = global->GetFloatValue("下のパンの位置", "パン");
}
