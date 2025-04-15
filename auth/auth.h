#ifndef AUTH_H
#define AUTH_H

#include <string>
#include "nlohmann/json.hpp"

using namespace std;
using json = nlohmann::json;

// Fungsi-fungsi autentikasi
void registerOperator();
bool loginOperator();
json loadOperators();
void saveOperators(const json& data);
bool usernameExists(const json& users, const string& username);
int findUserIndex(const json& users, const string& username);

// Konstanta
extern const string USERS_FILE;

#endif // AUTH_H
