#pragma once
#include <string>
#include <vector>

#include "configuration.h"

constexpr short RUN_TYPES = 0;
constexpr short RUN_WEAPONS = 1;
constexpr short RUN_VARIANT = 2;

using std::string;
using std::vector;

namespace Parser
{
	bool ReadConfig();
	bool LoadOffsets();
	bool GetConfigPath(string* path, string file_name);
	short ParseKey(string key);
	void SetValue(Configuration* config, string name, string value);
	void Distribute(vector<string> line, short run);
	bool String2Bool(const string& s);
};