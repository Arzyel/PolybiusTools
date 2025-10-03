#pragma once
#ifndef KEYS
#define KEYS
#include <cstdint>
#include <string>

consteval uint32_t hash_const(const char* str) {
	uint32_t hash = 2166136261u;
	while (*str) {
		hash ^= static_cast<uint8_t>(*str++);
		hash *= 16777619u;
	}
	return hash;
}

inline uint32_t hash_runtime(std::string_view str) {
	uint32_t hash = 2166136261u;
	for (char c : str) {
		hash ^= static_cast<uint8_t>(c);
		hash *= 16777619u;
	}
	return hash;
}


constexpr const char* OWNER_CHAR = "owner";
constexpr const char* CONTROLLER_CHAR = "controller";
constexpr const char* ADD_CORE_CHAR = "add_core";
constexpr const char* CAPITAL_CHAR = "capital";
constexpr const char* IS_CITY_CHAR = "is_city";
constexpr const char* CULTURE_CHAR = "culture";
constexpr const char* RELIGION_CHAR = "religion";
constexpr const char* HRE_CHAR = "hre";
constexpr const char* BASE_TAX_CHAR = "base_taxe";
constexpr const char* BASE_PRODUCTION_CHAR = "base_production";
constexpr const char* BASE_MANPOWER_CHAR = "base_manpower";
constexpr const char* TRADE_GOODS_CHAR = "trade_goods";
constexpr const char* DISCOVERED_BY_CHAR = "discovered_by";
constexpr const char* CENTER_OF_TRADE_CHAR = "center_of_trade";
constexpr const char* ADD_PERMANENT_PROVINCE_MODIFIER_CHAR = "add_permanent_province_modifier";
constexpr const char* NAME_CHAR = "name";
constexpr const char* DURATION_CHAR= "duration";

//inside chat there is a convo with effects mapped to the nation effects , stopped at province effect in the modpage
constexpr uint32_t OWNER_HASH = hash_const(OWNER_CHAR);
constexpr uint32_t CONTROLLER_HASH = hash_const(CONTROLLER_CHAR);
constexpr uint32_t ADD_CORE_HASH = hash_const(ADD_CORE_CHAR);
constexpr uint32_t CAPITAL_HASH = hash_const(CAPITAL_CHAR);
constexpr uint32_t IS_CITY_HASH = hash_const(IS_CITY_CHAR);
constexpr uint32_t CULTURE_HASH = hash_const(CULTURE_CHAR);
constexpr uint32_t RELIGION_HASH = hash_const(RELIGION_CHAR);
constexpr uint32_t HRE_HASH = hash_const(HRE_CHAR);
constexpr uint32_t BASE_TAX_HASH = hash_const(BASE_TAX_CHAR);
constexpr uint32_t BASE_PRODUCTION_HASH = hash_const(BASE_PRODUCTION_CHAR);
constexpr uint32_t BASE_MANPOWER_HASH = hash_const(BASE_MANPOWER_CHAR);
constexpr uint32_t TRADE_GOODS_HASH = hash_const(TRADE_GOODS_CHAR);
constexpr uint32_t DISCOVERED_BY_HASH = hash_const(DISCOVERED_BY_CHAR);
constexpr uint32_t CENTER_OF_TRADE_HASH = hash_const(CENTER_OF_TRADE_CHAR);
constexpr uint32_t ADD_PERMANENT_PROVINCE_MODIFIER_HASH = hash_const(ADD_PERMANENT_PROVINCE_MODIFIER_CHAR);
constexpr uint32_t NAME_HASH = hash_const(NAME_CHAR);
constexpr uint32_t DURATION_HASH = hash_const(DURATION_CHAR);


std::unordered_map<uint32_t, const char*> locationKeys = {
	{OWNER_HASH,OWNER_CHAR},
	{CONTROLLER_HASH,CONTROLLER_CHAR}
};



#endif // !KEYS
