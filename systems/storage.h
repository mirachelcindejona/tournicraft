#ifndef STORAGE_H
#define STORAGE_H

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <nlohmann/json.hpp>
#include <iomanip>
#include <algorithm>
#include <queue>

using namespace std;
using json = nlohmann::json;

json loadTurnamen();
void saveTurnamen(const json& data);

#endif