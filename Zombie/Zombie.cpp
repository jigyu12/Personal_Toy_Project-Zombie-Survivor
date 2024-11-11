#include "stdafx.h"
#include "Zombie.h"
#include "Player.h"
#include "SceneGame.h"
#include "UiHud.h"
#include "ZombieTable.h"

Zombie::Zombie(const std::string& name) : GameObject(name)
{
}

void Zombie::SetPosition(const sf::Vector2f& pos)
{
	position = pos;
	body.setPosition(position);
}

void Zombie::SetRotation(float angle)
{
	rotation = angle;
	body.setRotation(rotation);
}

void Zombie::SetScale(const sf::Vector2f& s)
{
	scale = s;
	body.setScale(scale);
}

void Zombie::SetOrigin(Origins preset)
{
	originPreset = preset;
	if (originPreset != Origins::Custom)
	{
		origin = Utils::SetOrigin(body, originPreset);
	}
}

void Zombie::SetOrigin(const sf::Vector2f& newOrigin)
{
	originPreset = Origins::Custom;
	origin = newOrigin;
	body.setOrigin(origin);
}

sf::FloatRect Zombie::GetLocalBounds() const
{
	return body.getLocalBounds();
}

sf::FloatRect Zombie::GetGlobalBounds() const
{
	return body.getGlobalBounds();
}

void Zombie::Init()
{
	sortingLayer = SortingLayers::Foreground;
	sortingOrder = 0;

	SetType(type);
}

void Zombie::Release()
{
}

void Zombie::Reset()
{
	player = dynamic_cast<Player*>(SCENE_MGR.GetCurrentScene()->FindGo("Player"));
	sceneGame = dynamic_cast<SceneGame*>(SCENE_MGR.GetCurrentScene());

	body.setTexture(TEXTURE_MGR.Get(textureId));
	SetOrigin(Origins::MC);
	SetPosition({ 0.f, 0.f });
	SetRotation(0.f);
	SetScale({ 1.f, 1.f });

	isDead = false;
	deadDelay = 3.f;
	deadTimer = 0.f;
}

void Zombie::Update(float dt)
{
	if (isDead)
	{
		deadTimer += dt;
		if (deadTimer > deadDelay)
		{
			sceneGame->OnZombieDie(this);
		}
	}

	if (!isDead && player != nullptr && Utils::Distance(position, player->GetPosition()) > 1)
	{
		direction = Utils::GetNormal(player->GetPosition() - position);
		SetRotation(Utils::Angle(direction));
		SetPosition(position + direction * speed * dt);
	}

	hitbox.UpdateTr(body, GetLocalBounds());
}

void Zombie::Draw(sf::RenderWindow& window)
{
	window.draw(body);
	hitbox.Draw(window);
}

void Zombie::SetType(Types type)
{
	const auto& data = ZOMBIE_TABLE->Get(type);
	textureId = data.textureId;
	maxHp = data.maxHp;
	speed = data.speed;

	body.setTexture(TEXTURE_MGR.Get(textureId), true);
	hp = maxHp;
	deadTextureId = "graphics/blood.png";

	/*this->type = type;
	switch (this->type)
	{
	case Types::Bloater:
		textureId = "graphics/bloater.png";
		maxHp = 50;
		speed = 100.f;
		break;
	case Types::Chaser:
		textureId = "graphics/chaser.png";
		maxHp = 20;
		speed = 75.f;
		break;
	case Types::Crawler:
		textureId = "graphics/crawler.png";
		maxHp = 10;
		speed = 50.f;
		break;
	}
	body.setTexture(TEXTURE_MGR.Get(textureId), true);
	hp = maxHp;
	deadTextureId = "graphics/blood.png";*/
}

void Zombie::OnDamage(int d)
{
	hp -= d;
	if (hp <= 0 && sceneGame != nullptr)
	{
		isDead = true;
		body.setTexture(TEXTURE_MGR.Get(deadTextureId), true);
		sceneGame->SetSubZombieCount(1);
		sceneGame->SetAddScore(100);
		sceneGame->GetUiHud()->SetStringScore("SCORE");

		if (sceneGame->GetHighScore() < sceneGame->GetScore())
		{
			sceneGame->SetAddHiScore(100);
			sceneGame->GetUiHud()->SetHiScore(sceneGame->GetHighScore());
		}
	}
}