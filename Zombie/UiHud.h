#pragma once

class UiHud : public GameObject
{
protected:

	sf::Text textScore;
	sf::Text textHighScore;

	sf::Sprite iconAmmoIcon;
	sf::Text textAmmo;
	sf::RectangleShape gaugeHp;
	sf::Text textWave;
	sf::Text textZombieCount;

	std::string stringScoreId;

	float maxHp = 4000.f;
	float currentHp = 0.f;

	sf::Vector2f gaugeHpMaxSize = { maxHp / 10, 50.f };

public:
	UiHud(const std::string& name = "");
	~UiHud() = default;

	void SetPosition(const sf::Vector2f& pos) override;
	void SetRotation(float angle) override;
	void SetScale(const sf::Vector2f& scale) override;

	void SetOrigin(Origins preset) override;
	void SetOrigin(const sf::Vector2f& newOrigin) override;

	void Init() override;
	void Release() override;
	void Reset() override;
	void Update(float dt) override;
	void Draw(sf::RenderWindow& window) override;

	void SetScore(int s);
	void SetHiScore(int s);
	void SetAmmo(int current, int total);
	void SetHp(int hp, int max);
	void SetWave(int w);
	void SetZombieCount(int count);

	float GetMaxHp() const { return maxHp; }
	float GetCurrentHp() { return currentHp; }

	void SetStringScore(const std::string& scoreId);
	void OnLocalize(Languages lang) override;
};
