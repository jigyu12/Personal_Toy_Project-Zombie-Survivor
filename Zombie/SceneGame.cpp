#include "stdafx.h"
#include "SceneGame.h"
#include "Player.h"
#include "TileMap.h"
#include "Zombie.h"
#include "Bullet.h"
#include "UiHud.h"
#include "UiUpgrade.h"
#include "UiGameOver.h"
#include "ItemGenerator.h"
#include "ItemBullet.h"
#include "ItemHealth.h"
#include "ItemTable.h"

SceneGame::SceneGame()
	: Scene(SceneIds::Game)
{
}

void SceneGame::Init()
{
	map = AddGo(new TileMap("TileMap"));
	uiHud = AddGo(new UiHud("UiHud"));
	uiHud->SetStringScore("SCORE");

	uiUpgrade = AddGo(new UiUpgrade("UiUpgrade"));
	player = AddGo(new Player("Player"));
	uiGameOver = AddGo(new UiGameOver("UiGameOver"));
	itemGenerator = AddGo(new ItemGenerator("ItemGenerator"));

	itemBullets = new std::list<ItemBullet*>();
	itemBulletPool = new ObjectPool<ItemBullet>();
	itemHealths = new std::list<ItemHealth*>();
	itemHealthPool = new ObjectPool<ItemHealth>();

	std::fstream iFile("HighScore.txt", std::ios::in);

	if (!iFile)
	{
		std::cerr << "File IO Failed" << std::endl;
	}

	iFile >> highScore;

	iFile.close();
	
	Scene::Init();
}

void SceneGame::Release()
{
	Scene::Release();

	delete itemBullets;
	delete itemBulletPool;
	delete itemHealths;
	delete itemHealthPool;
}

void SceneGame::Enter()
{
	FRAMEWORK.GetWindow().setMouseCursorVisible(false);
	cursor.setTexture(TEXTURE_MGR.Get("graphics/crosshair.png"));
	Utils::SetOrigin(cursor, Origins::MC);

	sf::Vector2f size = FRAMEWORK.GetWindowSizeF();

	worldView.setSize(size);
	worldView.setCenter(0.f, 0.f);

	uiView.setSize(size);
	uiView.setCenter(size.x * 0.5f, size.y * 0.5f);

	wave = 1;
	score = 0.f;
	zombieSpawnNum = 1;
	zombieCount = 0;
	isUpgrade = false;
	isGameOver = false;
	waveTimeDelay = 10.f;
	waveTimeTimer = 0.f;
	spawnTimeDelay = 1.f;
	spawnTimeTimer = 0.f;
	addAmmoVal = ITEM_TABLE->Get("BULLET").value;
	addHealVal = ITEM_TABLE->Get("HEAL").value;

	Scene::Enter();

	zombieSpawnArea = map->GetMapBounds();

	uiUpgrade->SetActive(false);
	uiGameOver->SetActive(false);
	uiHud->SetWave(wave);
	uiHud->SetHiScore(highScore);
}

void SceneGame::Exit()
{
	FRAMEWORK.GetWindow().setMouseCursorVisible(true);

	for (auto zombie : zombies)
	{
		RemoveGo(zombie);
		zombiePool.Return(zombie);
	}
	zombies.clear();

	for (auto bullet : bullets)
	{
		RemoveGo(bullet);
		bulletPool.Return(bullet);
	}
	bullets.clear();

	Scene::Exit();
}

void SceneGame::Update(float dt)
{
	cursor.setPosition(ScreenToUi(InputMgr::GetMousePosition()));

	if (InputMgr::GetKeyDown(sf::Keyboard::Num1))
	{
		Variables::currentLang = Languages::korean;
		OnLocalize(Variables::currentLang);
	}

	if (InputMgr::GetKeyDown(sf::Keyboard::Num2))
	{
		Variables::currentLang = Languages::English;
		OnLocalize(Variables::currentLang);
	}

	if (InputMgr::GetKeyDown(sf::Keyboard::Num3))
	{
		Variables::currentLang = Languages::Japanese;
		OnLocalize(Variables::currentLang);
	}

	if (isGameOver)
	{
		uiGameOver->SetActive(true);

		if (InputMgr::GetKeyDown(sf::Keyboard::Enter))
		{
			SCENE_MGR.ChangeScene(SceneIds::Game);
		}

		return;
	}

	if (isUpgrade)
		return;

	Scene::Update(dt);

	waveTimeTimer += dt;
	
	if (waveTimeTimer > waveTimeDelay)
	{
		wave++;
		uiHud->SetWave(wave);
		zombieSpawnNum += 1;
		waveTimeTimer = 0.f;

		uiUpgrade->SetActive(!uiUpgrade->IsActive());
		isUpgrade = true;
	}

	spawnTimeTimer += dt;
	if (spawnTimeTimer > spawnTimeDelay)
	{
		SpawnZombies(zombieSpawnNum);
		zombieCount += zombieSpawnNum;
		uiHud->SetZombieCount(zombieCount);
		spawnTimeTimer = 0.f;
	}

	if (player != nullptr)
	{
		worldView.setCenter(player->GetPosition());
	}
}

void SceneGame::Draw(sf::RenderWindow& window)
{
	Scene::Draw(window);

	const sf::View& saveView = window.getView();
	window.setView(uiView);
	window.draw(cursor);
	window.setView(saveView);
}

void SceneGame::SpawnZombies(int count)
{
	for (int i = 0; i < count; ++i)
	{
		Zombie* zombie = zombiePool.Take();
		zombies.push_back(zombie);

		Zombie::Types zombieType = (Zombie::Types)Utils::RandomRange(0, Zombie::TotalTypes - 1);
		zombie->SetType(zombieType);

		sf::Vector2f pos = Utils::RandomPointInRect(zombieSpawnArea);
		zombie->SetPosition(pos);
		
		AddGo(zombie);
	}
}

Bullet* SceneGame::TakeBullet()
{
	Bullet* bullet = bulletPool.Take();
	bullets.push_back(bullet);
	AddGo(bullet);
	return bullet;
}

void SceneGame::ReturnBullet(Bullet* bullet)
{
	RemoveGo(bullet);
	bulletPool.Return(bullet);
	bullets.remove(bullet);
}

void SceneGame::OnZombieDie(Zombie* zombie)
{
	RemoveGo(zombie);
	zombiePool.Return(zombie);
	zombies.remove(zombie);
}

void SceneGame::OnUpgrade(Upgrade up)
{
	uiUpgrade->SetActive(false);
	
	switch (up)
	{
	case Upgrade::RateOfFire:
		player->SetSubShootDelay(0.8f);
		break;
	case Upgrade::ClipSize:
		player->SetAddAmmoCountMax(10);
		uiHud->SetAmmo(player->GetAmmoCountCurrent(), player->GetAmmoCountMax());
		break;
	case Upgrade::MaxHealth:
		uiHud->SetHp(uiHud->GetCurrentHp() + 1000.f, uiHud->GetMaxHp() + 1000.f);
		break;
	case Upgrade::RunSpeed:
		player->SetAddSpeed(100.f);
		break;
	case Upgrade::HealthPickups:
	{
		addHealVal += 500.f;
		for (auto heals : *itemHealths)
		{
			heals->SetHealVal(addHealVal);
		}
	}
		break;
	case Upgrade::AmmoPickups:
	{
		addAmmoVal += 5;
		for (auto bullets : *itemBullets)
		{
			bullets->SetAmmoVal(addAmmoVal);
		}
	}
		break;
	}

	isUpgrade = false;
}

void SceneGame::SetSubZombieCount(const int value)
{
	zombieCount -= value;
	uiHud->SetZombieCount(zombieCount);
}

void SceneGame::SetAddHiScore(const int value)
{
	highScore += value;

	std::fstream oFile("HighScore.txt", std::ios::out);

	if (!oFile)
	{
		std::cerr << "File IO Failed" << std::endl;
	}

	oFile << highScore;

	oFile.close();
	
}
