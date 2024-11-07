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
enum class Upgrade;

class SceneGame : public Scene
{
protected:
	TileMap* map;
	Player* player;
	UiHud* uiHud;
	UiUpgrade* uiUpgrade;
	UiGameOver* uiGameOver;

	std::list<Zombie*> zombies;
	ObjectPool<Zombie> zombiePool;
	sf::FloatRect zombieSpawnArea;

	std::list<Bullet*> bullets;
	ObjectPool<Bullet> bulletPool;

	sf::Sprite cursor;

	float waveTimeDelay = 3.f;
	float waveTimeTimer = 0.f;

	float spawnTimeDelay = 1.f;
	float spawnTimeTimer = 0.f;

	int wave = 1;
	int zombieSpawnNum = 1;
	int zombieCount = 0;
	int score = 0.f;
	int highScore = 0.f;

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
	float GetWaveTime() const { return waveTimeDelay; }
	int GetScore() const { return score; }
	int GetHighScore() const { return highScore; }

	void SetSubZombieCount(const int value); 
	void SetAddScore(const int value) { score += value; }
	void SetAddHiScore(const int value) { highScore += value; }
};

