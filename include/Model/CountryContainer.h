#pragma once
#ifndef COUNTRY_CONTAINER_H
#define COUNTRY_CONTAINER_H

#include <unordered_map>
#include <vector>
#include <string>
#include <cstdint>
#include <filesystem>
#include <iostream>
#include "SimpleParser.h"

class CountryContainer {
public:
    CountryContainer() = default;
    ~CountryContainer() = default;
    void initializeData();
    std::unordered_map<std::string, std::string> tagToName;
protected:

private:
};

#endif // COUNTRY_CONTAINER_H
