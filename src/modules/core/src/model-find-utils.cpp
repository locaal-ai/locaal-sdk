#include <filesystem>
#include <fstream>
#include <iostream>
#include <string>
#include <regex>

#include "model-find-utils.h"
#include "logger.h"

std::string find_file_in_folder_by_name(const std::string &folder_path,
					const std::string &file_name)
{
	for (const auto &entry : std::filesystem::directory_iterator(folder_path)) {
		if (entry.path().filename() == file_name) {
			return entry.path().string();
		}
	}
	return "";
}

// Find a file in a folder by expression
std::string find_file_in_folder_by_regex_expression(const std::string &folder_path,
						    const std::string &file_name_regex)
{
	for (const auto &entry : std::filesystem::directory_iterator(folder_path)) {
		if (std::regex_match(entry.path().filename().string(),
				     std::regex(file_name_regex))) {
			return entry.path().string();
		}
	}
	return "";
}

std::string find_ext_file_in_folder(const std::string &model_local_folder_path,
				    const std::string &ext)
{
	// find .bin file in folder
	for (const auto &entry : std::filesystem::directory_iterator(model_local_folder_path)) {
		if (entry.path().extension() == ext) {
			const std::string bin_file_path = entry.path().string();
			Logger::log(Logger::Level::INFO, "Model bin file found in folder: %s",
				    bin_file_path.c_str());
			return bin_file_path;
		}
	}
	Logger::log(Logger::Level::ERROR_LOG, "Model bin file not found in folder: %s",
		    model_local_folder_path.c_str());
	return "";
}
