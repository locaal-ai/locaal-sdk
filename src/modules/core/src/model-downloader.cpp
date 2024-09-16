#include "model-downloader.h"
#include "model-find-utils.h"
#include "Logger.h"

#include <sago/platform_folders.h>

#include <filesystem>

std::string find_model_folder(const ModelInfo &model_info)
{
	if (model_info.friendly_name.empty() || model_info.local_folder_name.empty() ||
	    model_info.files.empty()) {
		Logger::log(Logger::Level::ERROR, "Model info is invalid.");
		return "";
	}

	const std::string data_folder_models = sago::getCacheDir();
	const std::filesystem::path module_data_models_folder =
		std::filesystem::absolute(data_folder_models);

	const std::string model_local_data_path =
		(module_data_models_folder / "locaal" / "models" / model_info.local_folder_name)
			.string();

	Logger::log(Logger::Level::INFO, "Checking if model '%s' exists in cache...",
		    model_info.friendly_name.c_str());

	if (std::filesystem::exists(model_local_data_path)) {
		Logger::log(Logger::Level::INFO, "Model folder found in data: %s",
			    model_local_data_path.c_str());
		return model_local_data_path;
	}

	Logger::log(Logger::Level::INFO, "Model '%s' not found.", model_info.friendly_name.c_str());
	return "";
}

std::string find_model_bin_file(const ModelInfo &model_info)
{
	const std::string model_local_folder_path = find_model_folder(model_info);
	if (model_local_folder_path.empty()) {
		return "";
	}

	return find_bin_file_in_folder(model_local_folder_path);
}
