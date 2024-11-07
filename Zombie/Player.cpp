#include "stdafx.h"
#include "Player.h"
#include "SceneGame.h"
#include "Bullet.h"
#include "UiHud.h"
#include "TileMap.h"

Player::Player(const std::string& name)
	: GameObject(name)
{

}

void Player::SetPosition(const sf::Vector2f& pos)
{
	position = pos;
	body.setPosition(position);
}

void Player::SetRotation(float angle)
{
	rotation = angle;
	body.setRotation(angle);
}

void Player::SetScale(const sf::Vector2f& s)
{
	scale = s;
	body.setScale(s);
}

void Player::SetOrigin(Origins preset)
{
	originPreset = preset;
	if (originPreset != Origins::Custom)
	{
		origin = Utils::SetOrigin(body, originPreset);
	}
}

void Player::SetOrigin(const sf::Vector2f& newOrigin)
{
	originPreset = Origins::Custom;
	origin = newOrigin;
	body.setOrigin(origin);
}

void Player::Init()
{
	sortingLayer = SortingLayers::Foreground;
	sortingOrder = 0;
	SetOrigin(Origins::MC);
}

void Player::Release()
{
}

void Player::Reset()
{
	sceneGame = dynamic_cast<SceneGame*>(SCENE_MGR.GetCurrentScene());

	body.setTexture(TEXTURE_MGR.Get(textureId), true);
	SetOrigin(originPreset);
	SetPosition({ 0.f, 0.f });
	SetRotation(0.f);
	direction = { 1.f, 0.f };

	shootTimer = shootDelay;

	ammoCountcurrent = ammoCountMax;

	SceneGame* gameScene = dynamic_cast<SceneGame*>(SCENE_MGR.GetCurrentScene());
	if (gameScene)
		gameScene->GetUiHud()->SetAmmo(ammoCountcurrent, ammoCountMax);
}

void Player::Update(float dt)
{
	direction.x = InputMgr::GetAxis(Axis::Horizontal);
	direction.y = InputMgr::GetAxis(Axis::Vertical);
	float mag = Utils::Magnitude(direction);
	if (mag > 1.f)
	{
		 Utils::Normailize(direction);
	}

	sf::Vector2i mousePos = InputMgr::GetMousePosition();
	sf::Vector2f mouseWorldPos = SCENE_MGR.GetCurrentScene()->ScreenToWorld(mousePos);
	look = Utils::GetNormal(mouseWorldPos - position);

	SetRotation(Utils::Angle(look));

	sf::Vector2f newPosition = position + direction * speed * dt;
	SceneGame* gameScene = dynamic_cast<SceneGame*>(SCENE_MGR.GetCurrentScene());
	if (gameScene)
	{
		sf::FloatRect tileRect = gameScene->GetTileMap()->GetGlobalBounds();
		if (newPosition.x < tileRect.left + 50.f)
		{
			newPosition.x = tileRect.left + 50.f;
		}
		if (newPosition.x > tileRect.left + tileRect.width - 50.f)
		{
			newPosition.x = tileRect.left + tileRect.width - 50.f;
		}

		if (newPosition.y < tileRect.top + 50.f)
		{
			newPosition.y = tileRect.top + 50.f;
		}
		if (newPosition.y > tileRect.top + tileRect.height - 50.f)
		{
			newPosition.y = tileRect.top + tileRect.height - 50.f;
		}
	}
	
	SetPosition(newPosition);

	shootTimer += dt;

	if (ammoCountcurrent <= 0)
	{
		reloadTimer += dt;

		if (reloadTimer < reloadDelay)
			return;

		SceneGame* gameScene = dynamic_cast<SceneGame*>(SCENE_MGR.GetCurrentScene());
		ammoCountcurrent = ammoCountMax;
		gameScene->GetUiHud()->SetAmmo(ammoCountcurrent, ammoCountMax);
		reloadTimer = 0.f;
	}

	if (shootTimer > shootDelay && InputMgr::GetMouseButton(sf::Mouse::Left))
	{
		shootTimer = 0.f;

		Shoot();

		SceneGame* gameScene = dynamic_cast<SceneGame*>(SCENE_MGR.GetCurrentScene());
		if (gameScene)
		{
			ammoCountcurrent--;
			gameScene->GetUiHud()->SetAmmo(ammoCountcurrent, ammoCountMax);
		}
	}
}

void Player::Draw(sf::RenderWindow& window)
{
	window.draw(body);
}

void Player::Shoot()
{
	Bullet* bullet = sceneGame->TakeBullet();
	bullet->Fire(position, look, 1000.f, 10);
}
