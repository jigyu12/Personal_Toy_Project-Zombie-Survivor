#pragma once

class ItemBullet;
class ItemHealth;

class ItemGenerator : public GameObject
{
protected:
	sf::Transformable body;

	float generateDelay = 1.0f;
	float generateTimer = 0.f;

	std::list<ItemBullet*> itemBullets;
	ObjectPool<ItemBullet> itemBulletPool;

	std::list<ItemHealth*> itemHealths;
	ObjectPool<ItemHealth> itemHealthPool;

	int addAmmoVal = 5; 
	float addHealVal = 500.f;

public:
	ItemGenerator(const std::string& name = "");
	~ItemGenerator() = default;

	void SetRandomPositionInTileMap();

	void Init() override;
	void Release() override;
	void Reset() override;
	void Update(float dt) override;
	void Draw(sf::RenderWindow& window) override;

	std::list<ItemBullet*> GetitemBulletList() const { return itemBullets; }
	std::list<ItemHealth*> GetitemHealthList() const { return itemHealths; }

	void SetAmmoVal(const int val) { addAmmoVal += val; }
	int GetAmmoVal() const { return addAmmoVal;  }
	void SetHealVal(const int val) { addHealVal += val; }
	float GetHealVal() const { return addHealVal; }
};
