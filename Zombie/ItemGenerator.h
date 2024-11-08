#pragma once

class ItemBullet;
class ItemHealth;

class ItemGenerator : public GameObject
{
protected:
	sf::Transformable body;

	float generateDelay = 1.0f;
	float generateTimer = 0.f;

public:
	ItemGenerator(const std::string& name = "");
	~ItemGenerator() = default;

	void SetRandomPositionInTileMap();

	void Init() override;
	void Release() override;
	void Reset() override;
	void Update(float dt) override;
	void Draw(sf::RenderWindow& window) override;
};
