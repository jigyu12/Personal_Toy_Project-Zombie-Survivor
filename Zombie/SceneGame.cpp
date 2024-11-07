#include "stdafx.h"
#include "SceneGame.h"
#include "Player.h"
#include "TileMap.h"
#include "Zombie.h"
#include "Bullet.h"
#include "UiHud.h"
#include "UiUpgrade.h"
#include "UiGameOver.h"

SceneGame::SceneGame()
	: Scene(SceneIds::Game)
{
}

void SceneGame::Init()
{
	map = AddGo(new TileMap("TileMap"));
	player = AddGo(new Player("Player"));
	uiHud = AddGo(new UiHud("UiHud"));
	uiUpgrade = AddGo(new UiUpgrade("UiUpgrade"));
	uiGameOver = AddGo(new UiGameOver("UiGameOver"));
	Scene::Init();
}

void SceneGame::Release()
{
	Scene::Release();
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

	Scene::Enter();

	zombieSpawnArea = map->GetMapBounds();

	uiUpgrade->SetActive(false);
	uiGameOver->SetActive(false);
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

	Scene::Update(dt);

	if (InputMgr::GetKeyDown(sf::Keyboard::Escape))
	{
		SCENE_MGR.ChangeScene(SceneIds::Game);
	}

	if (InputMgr::GetKeyDown(sf::Keyboard::Space))
	{
		SpawnZombies(100);
	}

	if (InputMgr::GetKeyDown(sf::Keyboard::Num1))
	{
		uiUpgrade->SetActive(!uiUpgrade->IsActive());
	}

	if (InputMgr::GetKeyDown(sf::Keyboard::Num2))
	{
		uiGameOver->SetActive(!uiGameOver->IsActive());
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
	std::cout << (int)up << std::endl;
}
