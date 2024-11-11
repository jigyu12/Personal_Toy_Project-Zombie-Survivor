#pragma once
#include "DataTable.h"

struct DataItem
{
	std::string textureId;
	float value = 0.f;
};

class ItemTable : public DataTable
{
public:
	static DataItem Undefined;
	std::string filePath = "tables/item_table.csv";

protected:
	std::unordered_map<std::string, DataItem> table;

public:
	ItemTable() : DataTable(DataTable::Types::Item) { };
	~ItemTable() = default;

	bool Load() override;
	void Release() override;

	const DataItem& Get(const std::string& id);
};