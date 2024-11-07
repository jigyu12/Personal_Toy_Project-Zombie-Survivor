#pragma once

class SceneGame;

class Player : public GameObject
{
protected:
	sf::Sprite body;
	std::string textureId = "graphics/player.png";

	sf::Vector2f direction;
	sf::Vector2f look;

	float speed = 400.f;

	SceneGame* sceneGame;

	float shootDelay = 1.f;
	float shootTimer = 0.f;

	int ammoCountMax = 10;
	int ammoCountcurrent = 0;
	float reloadDelay = 2.f;
	float reloadTimer = 0.f;
	bool isReload = false;

public:
	Player(const std::string& name = "");
	~Player() = default;

	void SetPosition(const sf::Vector2f& pos) override;
	void SetRotation(float angle) override;
	void SetScale(const sf::Vector2f& scale) override;

	void SetOrigin(Origins preset) override;
	void SetOrigin(const sf::Vector2f& newOrigin) override;

	void Init() override;
	void Release() override;
	void Reset() override;
	void Update(float dt) override;
	void FixedUpdate(float dt) override;
	void Draw(sf::RenderWindow& window) override;

	void Shoot();

	sf::FloatRect GetGlobalBounds() const override { return body.getGlobalBounds(); }
	sf::FloatRect GetLocalBounds() const override { return body.getLocalBounds(); }
	int GetAmmoCountMax() const { return ammoCountMax; }
	int GetAmmoCountCurrent() const { return ammoCountcurrent; }

	void SetSubShootDelay(const float persent) { shootDelay *= persent; }
	void SetAddAmmoCountMax(const int value) { ammoCountMax += value; }
	void SetAddSpeed(const float value) { speed += value; }
	void SetAddAmmoCountCurrent(const int value);
};
