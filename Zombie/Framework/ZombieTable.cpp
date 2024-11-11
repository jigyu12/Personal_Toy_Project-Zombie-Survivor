#include "stdafx.h"
#include "ZombieTable.h"
#include "rapidcsv.h"


DataZombie ZombieTable::Undefined;

bool ZombieTable::Load()
{
	Release();

	rapidcsv::Document doc(filePath, rapidcsv::LabelParams(0, -1));
	for (int i = 0; i < doc.GetRowCount(); ++i)
	{
		auto row = doc.GetRow<std::string>(i);
		Zombie::Types id = (Zombie::Types)std::stoi(row[0]);
		if (table.find(id) != table.end())
		{
			std::cout << "좀비 오류!" << std::endl;
			return false;
		}

		table[id].id = id;
		table[id].textureId = row[1];
		table[id].maxHp = std::stoi(row[2]);
		table[id].speed = std::stof(row[3]);
	}

	return true;
}

void ZombieTable::Release()
{
	table.clear();
}

const DataZombie& ZombieTable::Get(Zombie::Types id)
{
	auto find = table.find(id);
	if (find == table.end())
	{
		return Undefined;
	}
	return find->second;
}
