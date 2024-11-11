#include "stdafx.h"
#include "ItemBullet.h"
#include "SceneGame.h"
#include "Player.h"
#include "UiHud.h"
#include "ItemGenerator.h"
#include "ItemTable.h"

ItemBullet::ItemBullet(const std::string& name)
	: GameObject(name)
{
}

void ItemBullet::SetPosition(const sf::Vector2f& pos)
{
	position = pos;
	body.setPosition(pos);
}

void ItemBullet::SetRotation(float angle)
{
	rotation = angle;
	body.setRotation(angle);
}

void ItemBullet::SetScale(const sf::Vector2f& s)
{
	scale = s;
	body.setScale(s);
}

void ItemBullet::SetOrigin(Origins preset)
{
	originPreset = preset;
	if (originPreset != Origins::Custom)
	{
		origin = Utils::SetOrigin(body, originPreset);
	}
}

void ItemBullet::SetOrigin(const sf::Vector2f& newOrigin)
{
	originPreset = Origins::Custom;
	origin = newOrigin;
	body.setOrigin(origin);
}

void ItemBullet::Init()
{
	sortingLayer = SortingLayers::Foreground;
	sortingOrder = 0;
}

void ItemBullet::Release()
{
}

void ItemBullet::Reset()
{
	body.setTexture(TEXTURE_MGR.Get(ITEM_TABLE->Get("BULLET").textureId));
	SetOrigin(Origins::MC);

	SetPosition({ 0.f, 0.f });
	SetRotation(0.f);
	SetScale({ 1.f, 1.f });

	addAmmoVal = dynamic_cast<SceneGame*>(SCENE_MGR.GetCurrentScene())->GetAmmoVal();
}

void ItemBullet::Update(float dt)
{	
	Player* player = dynamic_cast<SceneGame*>(SCENE_MGR.GetCurrentScene())->GetPlayer();

	hitbox.UpdateTr(body, GetLocalBounds());

	if (player != nullptr && player->GetGlobalBounds().intersects(body.getGlobalBounds()))
	{
		HitBox& boxPlayer = player->GetHitBox();
		if (Utils::CheckCollision(hitbox, boxPlayer))
		{
			player->SetAddAmmoCountCurrent(addAmmoVal);
			std::cout << "Bullet: " << addAmmoVal << std::endl;
			dynamic_cast<SceneGame*>(SCENE_MGR.GetCurrentScene())->GetUiHud()->SetAmmo(player->GetAmmoCountCurrent(), player->GetAmmoCountMax());
			SetActive(false);
		}
	}
}

void ItemBullet::Draw(sf::RenderWindow& window)
{
	window.draw(body);
}

void ItemBullet::SetAmmoVal(const int val)
{
	addAmmoVal = val;
}
