#ifndef SWISS_H
#define SWISS_H

#include <iostream>
#include <string>
#include <vector>
#include <nlohmann/json.hpp>
#include <iomanip>

using namespace std;
using json = nlohmann::json;

json loadTurnamen();
void saveTurnamen(const json& data);
void sistemSwissRonde(vector<Tim>& tims, int rondeKe);
void sistemSwiss();
int runSistemSwiss();

#endif // SWISS_H