#include "stdafx.h"
#include "ItemHealth.h"
#include "SceneGame.h"
#include "Player.h"
#include "UiHud.h"
#include "SceneGame.h"
#include "ItemGenerator.h"

ItemHealth::ItemHealth(const std::string& name)
	: GameObject(name)
{
}

void ItemHealth::SetPosition(const sf::Vector2f& pos)
{
	position = pos;
	body.setPosition(pos);
}

void ItemHealth::SetRotation(float angle)
{
	rotation = angle;
	body.setRotation(angle);
}

void ItemHealth::SetScale(const sf::Vector2f& s)
{
	scale = s;
	body.setScale(s);
}

void ItemHealth::SetOrigin(Origins preset)
{
	originPreset = preset;
	if (originPreset != Origins::Custom)
	{
		origin = Utils::SetOrigin(body, originPreset);
	}
}

void ItemHealth::SetOrigin(const sf::Vector2f& newOrigin)
{
	originPreset = Origins::Custom;
	origin = newOrigin;
	body.setOrigin(origin);
}

void ItemHealth::Init()
{
	sortingLayer = SortingLayers::Foreground;
	sortingOrder = 0;
}

void ItemHealth::Release()
{
}

void ItemHealth::Reset()
{
	body.setTexture(TEXTURE_MGR.Get(textureId));
	SetOrigin(Origins::MC);

	SetPosition({ 0.f, 0.f });
	SetRotation(0.f);
	SetScale({ 1.f, 1.f });

	addHealVal = dynamic_cast<SceneGame*>(SCENE_MGR.GetCurrentScene())->GetHealVal();
}

void ItemHealth::Update(float dt)
{
	Player* player = dynamic_cast<SceneGame*>(SCENE_MGR.GetCurrentScene())->GetPlayer();

	hitbox.UpdateTr(body, GetLocalBounds());

	if (player != nullptr && player->GetGlobalBounds().intersects(body.getGlobalBounds()))
	{
		HitBox& boxPlayer = player->GetHitBox();
		if (Utils::CheckCollision(hitbox, boxPlayer))
		{
			UiHud* uiHud = dynamic_cast<SceneGame*>(SCENE_MGR.GetCurrentScene())->GetUiHud();
			uiHud->SetHp(uiHud->GetCurrentHp() + addHealVal,uiHud->GetMaxHp());
			SetActive(false);
		}
	}
}

void ItemHealth::Draw(sf::RenderWindow& window)
{
	window.draw(body);
}

void ItemHealth::SetHealVal(const float val)
{
	addHealVal = val;
}
