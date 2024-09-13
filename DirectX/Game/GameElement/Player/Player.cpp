#include "Player.h"
#include "FrameInfo/FrameInfo.h"
#include "ImGuiManager/ImGuiManager.h"
void Player::Init()
{
#pragma region
	//ジャンプのテクスチャ
	jumpTexture.push_back("player1.png");
	jumpTexture.push_back("player2.png");
	jumpTexture.push_back("player3.png");
	jumpTexture.push_back("player4.png");
	for (std::string tex : jumpTexture) {
		TextureManager::GetInstance()->LoadTexture(tex);
	}

	//板ポリに画像を貼り付ける
	object_ = std::make_unique<Object>(jumpTexture[0]);

	//右と左のアフィン行列
	lookLeftMatrix = Matrix4x4::MakeAffinMatrix(Vector3{ 1.0f,1.0f,1.0 }, Vector3{ 0.0f,3.14f,0.0f }, Vector3{});
	lookRightMatrix = Matrix4x4::MakeAffinMatrix(Vector3{ 1.0f,1.0f,1.0 }, Vector3{ 0.0f,0.0f,0.0f }, Vector3{});

	jumpSE = std::make_unique<Audio>();
	jumpSE->Load("jump.mp3", "プレイヤーのジャンプの音");
	DropSE = std::make_unique<Audio>();
	DropSE->Load("finish.mp3", "プレイヤーの落下の音");

	jumpXmovement = jumpXCenter - object_->model->transform_.translate_.x;

	jumpXmovement = (std::max)(1.0f, std::abs(jumpXmovement));
#pragma endregion ジャンプ
	
	random = RandomGenerator::GetInstance();

	input_ = Input::GetInstance();

	global = std::make_unique<GlobalVariableUser>("Character", "Player");

	//当たり判定
	CreateCollider(ColliderShape::BOX2D, ColliderType::COLLIDER, ColliderMask::PLAYER);
	AddTargetMask(ColliderMask::FLOOR);
	AddTargetMask(ColliderMask::PAN);

	panTop = std::make_unique<Floor>("bread.png", Vector3{ 0.0f,topLimit ,0.0f }, panSize, this, ColliderMask::PAN);

	panBottom = std::make_unique<Floor>("bread.png", Vector3{ 0.0f,bottomLimit ,0.0f }, panSize, this);
	//予測線
	predictionLine = std::make_unique<Floor>("cheese.png", Vector3{ object_->model->transform_.translate_.x, object_->model->transform_.translate_.y - 2.0f, object_->model->transform_.translate_.z }, Vector3{ std::abs(jumpXmovement),0.1f,1.0f }, this, ColliderMask::PREDICTIONLINE);
	predictionLine->object_->model->color_ = { 1.0f,1.0f,1.0f,0.5f };

	//キャラクターのサイズ変更
	object_->model->transform_.scale_ = { 2.0f,2.0f,1.0f };
	object_->Update();
	SetGlobalVariables();

	//パーティクル
	jumpParticle_ = std::make_unique<JumpParticle>();
}

void Player::Update()
{
#ifdef _DEBUG
	ApplyGlobalVariables();
	panTop->Move(Vector3{ 0.0f,topLimit ,0.0f });
	panBottom->Move(Vector3{ 0.0f,bottomLimit ,0.0f });
#endif
	const float deltaTime = FrameInfo::GetInstance()->GetDeltaTime();

	Vector3 prePos = object_->model->transform_.translate_;

	//天井に当たった時の処理
	if (isHitCeiling) {
		HitCeiling();
		//プレイヤーがパンの位置に到達
		if (object_->model->transform_.translate_.y <= bottomLimit) {
			HitBottom();
			isPlayerBackFlag = true;
			panbottomMove = true;
		}
		if (panBottom->GetPos().y <= bottomDropLimit) {
			panbottomMove = false;

			isDrawbottomPanFlag = false;

		}

		//プレイヤーを特定の位置に戻す
		if (isPlayerBackFlag) {
			jumpForce = kJumpForce;
			jumpForceVec.y = jumpForce;
			object_->model->transform_.translate_ += jumpForceVec * deltaTime;

			if (object_->model->transform_.translate_.y > bottomPanReset) {
				isDrawbottomPanFlag = true;
				isPlayerBackFlag = false;
				isHitCeiling = false;
				panTop->Move(Vector3{ 0.0f,topLimit ,0.0f });
				panBottom->Move(Vector3{ 0.0f,bottomLimit ,0.0f });

				dropSEPlayFlag = true;
			}
		}
		object_->Update();
		panTop->Update();
		panBottom->Update();

		//床更新
		for (std::list<std::unique_ptr<Floor>>::iterator it = cheese_.begin(); it != cheese_.end(); it++) {
			(*it)->Update();
		}
		return;
	}

	jumpFlame += FrameInfo::GetInstance()->GetDeltaTime();

	jumpForceVec.y = jumpForce;
	//ジャンプインターバル
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
	//連続でジャンプしないように
	if (isFloorCollider == false) {
		floorColliderFrame += FrameInfo::GetInstance()->GetDeltaTime();

		if (floorColliderFrame > kFloorColliderFrame) {
			floorColliderFrame = 0;
			isFloorCollider = true;
		}
	}

	object_->model->transform_.translate_ += jumpForceVec * deltaTime;
	jumpForce -= gravity;

	//移動制御
	object_->SetTranslate({ std::clamp(object_->GetWorldTransform().translate_.x, -Xlimit, Xlimit) ,object_->GetWorldTransform().translate_.y,0.0f });

	//床更新
	for (std::list<std::unique_ptr<Floor>>::iterator it = cheese_.begin(); it != cheese_.end(); it++) {
		(*it)->Update();
	}
	//予測線更新

	jumpXmovement = jumpXCenter - object_->model->transform_.translate_.x;

	jumpXmovement = (std::max)(1.0f, std::abs(jumpXmovement));
	predictionLine->Move({ object_->model->transform_.translate_.x, object_->model->transform_.translate_.y - 2.0f, object_->model->transform_.translate_.z });
	predictionLine->SetSize({ std::abs(jumpXmovement),0.1f,1.0f });

	predictionLine->Update();

	object_->Update();

	prePos = object_->model->transform_.translate_ - prePos;

	ColliderUpdate(prePos);

	//パーティクル
	jumpParticle_->Update();

	//パンの更新
	panTop->Update();
	if (isDrawbottomPanFlag) {
		panBottom->Update();
	}
}

void Player::Draw(const Camera* camera)
{
	object_->Draw(*camera);

	//床描画
	for (std::list<std::unique_ptr<Floor>>::iterator it = cheese_.begin(); it != cheese_.end(); it++)
	{
		(*it)->Draw(camera);
	}
	//パンの描画
	if (isHitCeiling) {
		panTop->Draw(camera);
	}
#ifdef _DEBUG
	if (IsDraw) {
		panTop->Draw(camera);
	}
#endif
	if (isDrawbottomPanFlag) {
		panBottom->Draw(camera);
	}
	//パーティクル
	jumpParticle_->Draw();
	//予測線描画
	predictionLine->Draw(camera);
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
	//パーティクルを生成
	jumpParticle_->Create({ object_->model->transform_.translate_.x,object_->model->transform_.translate_.y - 2.0f ,object_->model->transform_.translate_.z });
}

void Player::Jump()
{
	jumpForceVec.x = 0.0f;
	jumpForceVec.y = jumpForce;

	if (input_->PressingKey(DIK_A)) {
		jumpForceVec.x = -kJumpForceX;
		jumpForce -= buttomGravity;
		object_->model->GetMaterialData()->uvTransform = lookLeftMatrix;
	}
	else if (input_->PressingKey(DIK_D)) {
		jumpForceVec.x = kJumpForceX;
		jumpForce -= buttomGravity;
		object_->model->GetMaterialData()->uvTransform = lookRightMatrix;
	}
	else if (input_->PressingKey(DIK_S)) {
		jumpForce -= buttomDown;
	}

	jumpForceVec.Length();

}

int Player::randomTextureSelect(int PreTextture)
{
	currentTexture = random->RandInt(0, 4);
	if (currentTexture == PreTextture) {
		currentTexture = randomTextureSelect(PreTextture);
	}
	return currentTexture;
}

void Player::CreateFloor()
{
	jumpXmovement = jumpXCenter - object_->model->transform_.translate_.x;

	jumpXmovement = (std::max)(1.0f, std::abs(jumpXmovement));

	cheese_.push_back(std::unique_ptr<Floor>(new Floor("cheese.png", { object_->model->transform_.translate_.x,object_->model->transform_.translate_.y - 2.0f,object_->model->transform_.translate_.z }, { std::abs(jumpXmovement),sizeY,1.0f }, this)));
}

void Player::HitCeiling()
{
	const float deltaTime = FrameInfo::GetInstance()->GetDeltaTime();
	dropSpeed_ = kDropSpeed * deltaTime;

	if (dropSEPlayFlag) {
		dropSEPlayFlag = false;
		DropSE->Play();
	}

	if (isPlayerBackFlag == false) {
		object_->model->transform_.translate_.y -= dropSpeed_;
		prePanPos = object_->model->transform_.translate_.y;

	}
	else if (isPlayerBackFlag) {
		prePanPos -= dropSpeed_;
	}

	panTop->Move(Vector3{ 0.0f,prePanPos,0.0f } - Vector3{ 0.0f,2.0f,0.0f });
	if (panbottomMove) {
		panBottom->Move(Vector3{ 0.0f,prePanPos,0.0f } - Vector3{ 0.0f,4.0f,0.0f });

	}
}

void Player::HitBottom()
{
	cheese_.clear();


}

void Player::ColliderUpdate(const Vector3& move)
{
	SetBox2D(object_->GetWorldTransform().translate_, object_->GetWorldTransform().scale_, move);
	SetCollider();
}

void Player::OnCollision(const Collider& collider)
{
	if (collider.GetMask() == ColliderMask::FLOOR) {
		if (isFloorCollider) {
			CommonJumpInit();
			isFloorCollider = false;
		}
	}
	if (collider.GetMask() == ColliderMask::PAN) {
		isHitCeiling = true;
	}
}

void Player::SetGlobalVariables()
{
	global->AddItem("落下速度", kDropSpeed, "落下関連");

	global->AddItem("ジャンプ力", kJumpForce, "ジャンプ");
	global->AddItem("重力", gravity, "ジャンプ");
	global->AddItem("ボタンを押した時にかかる力", buttomGravity, "ジャンプ");
	global->AddItem("下ボタンを押した時にかかる力", buttomDown, "ジャンプ");
	global->AddItem("横移動の大きさ", kJumpForceX, "ジャンプ");
	global->AddItem("ジャンプのインターバル", kJumpInterval, "ジャンプ");

	global->AddItem("左右の制限", Xlimit, "移動制御");

	global->AddItem("上のパンの位置", topLimit, "パン");
	global->AddItem("下のパンの位置", bottomLimit, "パン");
	global->AddItem("下のパン再出現位置", bottomPanReset, "パン");

	global->AddItem("Yのサイズ", sizeY, "チーズ");

	ApplyGlobalVariables();
}

void Player::ApplyGlobalVariables()
{
	kDropSpeed = global->GetFloatValue("落下速度", "落下関連");

	kJumpForce = global->GetFloatValue("ジャンプ力", "ジャンプ");
	gravity = global->GetFloatValue("重力", "ジャンプ");
	buttomGravity = global->GetFloatValue("ボタンを押した時にかかる力", "ジャンプ");
	buttomDown = global->GetFloatValue("下ボタンを押した時にかかる力", "ジャンプ");
	kJumpForceX = global->GetFloatValue("横移動の大きさ", "ジャンプ");
	kJumpInterval = global->GetFloatValue("ジャンプのインターバル", "ジャンプ");

	topLimit = global->GetFloatValue("上のパンの位置", "パン");
	bottomLimit = global->GetFloatValue("下のパンの位置", "パン");
	bottomPanReset = global->GetFloatValue("下のパン再出現位置", "パン");

	sizeY = global->GetFloatValue("Yのサイズ", "チーズ");
}
