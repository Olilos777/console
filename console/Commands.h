#pragma once
#include <iostream>
#include <filesystem>
#include <vector>

namespace fs = std::filesystem;


class Commands
{
	static int n;
public:
	std::vector<std::string> listDir(std::string cwd, bool returnFlag = false);
	void changeDir(std::string newPath);

	std::string getCurrentDir();
	std::string getLine();
	std::string returnSize(const fs::path& path);
	void autoComplete(const std::string& input, const std::vector<std::string>& options);
	void eraseText(int numChars);
	void clear();
};

