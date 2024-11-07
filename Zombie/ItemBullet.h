#pragma once

class ItemBullet : public GameObject
{
protected:
	sf::Sprite body;
	std::string textureId = "graphics/ammo_pickup.png";

	int addAmmoVal;

public:
	ItemBullet(const std::string& name = "");
	~ItemBullet() = default;

	void SetPosition(const sf::Vector2f& pos) override;
	void SetRotation(float angle) override;
	void SetScale(const sf::Vector2f& scale) override;

	void SetOrigin(Origins preset) override;
	void SetOrigin(const sf::Vector2f& newOrigin) override;

	sf::FloatRect GetGlobalBounds() const override { return body.getGlobalBounds(); }
	sf::FloatRect GetLocalBounds() const override { return body.getLocalBounds(); }

	void Init() override;
	void Release() override;
	void Reset() override;
	void Update(float dt) override;
	void Draw(sf::RenderWindow& window) override;

	void SetAmmoVal(const int val);
};

