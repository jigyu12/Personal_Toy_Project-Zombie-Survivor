#pragma once
#include "DataTable.h"
#include "Zombie.h"

struct DataZombie
{
	Zombie::Types id;
	std::string textureId;
	int maxHp = 0;
	float speed = 0.f;
};

class Zombie;

class ZombieTable : public DataTable
{
public:
	static DataZombie Undefined;

protected:
	std::unordered_map<Zombie::Types, DataZombie> table;
	std::string filePath = "tables/zombie_table.csv";

public:
	ZombieTable() : DataTable(DataTable::Types::String) { };
	~ZombieTable() = default;

	bool Load() override;
	void Release() override;


	const DataZombie& Get(Zombie::Types id);
};
