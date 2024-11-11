#include "stdafx.h"
#include "ItemTable.h"

DataItem ItemTable::Undefined;

bool ItemTable::Load()
{
	Release();

	rapidcsv::Document doc(filePath, rapidcsv::LabelParams(0, -1));
	for (int i = 0; i < doc.GetRowCount(); ++i)
	{
		std::vector<std::string> strings = doc.GetRow<std::string>(i);
		auto it = table.find(strings[0]);
		if (it != table.end())
		{
			std::cout << "스트링 테이블 키 중복!" << std::endl;
			return false;
		}
		
		auto& dataItem = table[strings[0]];
		dataItem.textureId = strings[1];
		dataItem.value = std::stof(strings[2]);
	}

	return true;
}

void ItemTable::Release()
{
	table.clear();
}

const DataItem& ItemTable::Get(const std::string& id)
{
	auto find = table.find(id);
	if (find == table.end())
	{
		return Undefined;
	}
	return find->second;
}