#include "stdafx.h"
#include "ItemGenerator.h"
#include "SceneGame.h"
#include "TileMap.h"
#include "Scene.h"
#include "ItemBullet.h"
#include "ItemHealth.h"

ItemGenerator::ItemGenerator(const std::string& name)
	: GameObject(name)
{
}

void ItemGenerator::SetRandomPositionInTileMap()
{
	sf::Vector2f pos;
	float tileMapWidth = dynamic_cast<SceneGame*>(SCENE_MGR.GetCurrentScene())->GetTileMap()->GetGlobalBounds().width;
	float tileMapHeight = dynamic_cast<SceneGame*>(SCENE_MGR.GetCurrentScene())->GetTileMap()->GetGlobalBounds().height;
	pos.x = Utils::RandomRange(-tileMapWidth / 2 + 50.f , tileMapWidth / 2 - 50.f);
	pos.y = Utils::RandomRange(-tileMapHeight / 2 + 50.f, tileMapHeight / 2 - 50.f);

	position = pos;
	body.setPosition(pos);
}

void ItemGenerator::Init()
{
	
}

void ItemGenerator::Release()
{
}

void ItemGenerator::Reset()
{
	generateDelay = 1.0f;
	generateTimer = 0.f;
}

void ItemGenerator::Update(float dt)
{
	SetRandomPositionInTileMap();

	generateTimer += dt;
	if (generateTimer > generateDelay)
	{
		generateTimer = 0.f;

		if (Utils::RandomValue() < 0.5f)
		{
			ItemBullet* itemBullet = dynamic_cast<SceneGame*>(SCENE_MGR.GetCurrentScene())->GetItemBulletPool()->Take();
			dynamic_cast<SceneGame*>(SCENE_MGR.GetCurrentScene())->GetItemBulletList()->push_back(itemBullet);
			dynamic_cast<SceneGame*>(SCENE_MGR.GetCurrentScene())->AddGo(itemBullet);
			itemBullet->SetPosition(position);
		}
		else
		{
			ItemHealth* itemhealth = dynamic_cast<SceneGame*>(SCENE_MGR.GetCurrentScene())->GetItemHealthPool()->Take();
			dynamic_cast<SceneGame*>(SCENE_MGR.GetCurrentScene())->GetItemHealthList()->push_back(itemhealth);
			dynamic_cast<SceneGame*>(SCENE_MGR.GetCurrentScene())->AddGo(itemhealth);
			itemhealth->SetPosition(position);
		}
	}
}

void ItemGenerator::Draw(sf::RenderWindow& window)
{
}
