#pragma once
#include "Scene.h"

class Player;
class Zombie;
class Bullet;
class Map;
class TileMap;
class UiHud;
class UiUpgrade;
class UiGameOver;
class ItemGenerator;
enum class Upgrade;

class SceneGame : public Scene
{
protected:
	TileMap* map;
	Player* player;
	UiHud* uiHud;
	UiUpgrade* uiUpgrade;
	UiGameOver* uiGameOver;
	ItemGenerator* itemGenerator;

	std::list<Zombie*> zombies;
	ObjectPool<Zombie> zombiePool;
	sf::FloatRect zombieSpawnArea;

	std::list<Bullet*> bullets;
	ObjectPool<Bullet> bulletPool;

	sf::Sprite cursor;

	float waveTimeDelay = 5.f;
	float waveTimeTimer = 0.f;

	float spawnTimeDelay = 1.f;
	float spawnTimeTimer = 0.f;

	int wave = 1;
	int zombieSpawnNum = 1;
	int zombieCount = 0;
	int score = 0.f;
	int highScore = 0.f;

	bool isUpgrade = false;
	bool isGameOver = false;

public:
	SceneGame();
	virtual ~SceneGame() = default;

	void Init();
	void Release();
	void Enter();
	void Exit();
	void Update(float dt);
	void Draw(sf::RenderWindow& window) override;

	void SpawnZombies(int count);
	Bullet* TakeBullet();
	void ReturnBullet(Bullet* bullet);

	const std::list<Zombie*>& GetZombieList() const { return zombies; }

	void OnZombieDie(Zombie* zombie);

	void OnUpgrade(Upgrade up);

	UiHud* GetUiHud() const { return uiHud; }
	TileMap* GetTileMap() const { return map; }
	Player* GetPlayer() const { return player; }
	ItemGenerator* GetItemGenerator() const { return itemGenerator; }

	float GetWaveTime() const { return waveTimeDelay; }
	int GetScore() const { return score; }
	int GetHighScore() const { return highScore; }
	bool IsUpgrade() const { return isUpgrade; }

	void SetSubZombieCount(const int value); 
	void SetAddScore(const int value) { score += value; }
	void SetAddHiScore(const int value) { highScore += value; }

	void SetIsUpgrade(const bool isupgrade) { isUpgrade = isupgrade; }
	void SetIsGameOver(const bool isgameover) { isGameOver = isgameover; }
};

