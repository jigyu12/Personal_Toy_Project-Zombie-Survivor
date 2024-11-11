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
class ItemBullet;
class ItemHealth;

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

	std::list<ItemBullet*>* itemBullets;
	ObjectPool<ItemBullet>* itemBulletPool;

	std::list<ItemHealth*>* itemHealths;
	ObjectPool<ItemHealth>* itemHealthPool;

	sf::Sprite cursor;

	float waveTimeDelay = 10.f;
	float waveTimeTimer = 0.f;

	float spawnTimeDelay = 1.f;
	float spawnTimeTimer = 0.f;

	int wave = 1;
	int zombieSpawnNum = 1;
	int zombieCount = 0;
	int score = 0;
	int highScore = 0;

	bool isUpgrade = false;
	bool isGameOver = false;

	int addAmmoVal;// = 5;
	float addHealVal;// = 500.f;

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
	void SetAddHiScore(const int value);

	void SetIsUpgrade(const bool isupgrade) { isUpgrade = isupgrade; }
	void SetIsGameOver(const bool isgameover) { isGameOver = isgameover; }

	std::list<ItemBullet*>* GetItemBulletList() const { return itemBullets; }
	ObjectPool<ItemBullet>* GetItemBulletPool() const { return itemBulletPool; }
	std::list<ItemHealth*>* GetItemHealthList() const { return itemHealths; }
	ObjectPool<ItemHealth>* GetItemHealthPool() const { return itemHealthPool; }

	void SetAmmoVal(const int val) { addAmmoVal += val; }
	int GetAmmoVal() const { return addAmmoVal; }
	void SetHealVal(const int val) { addHealVal += val; }
	float GetHealVal() const { return addHealVal; }
};

