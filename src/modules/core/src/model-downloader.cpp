#include "model-downloader.h"
#include "model-find-utils.h"
#include "logger.h"

#include <sago/platform_folders.h>

#include <filesystem>

#include <curl/curl.h>

std::filesystem::path get_models_folder()
{
	const std::string cache_folder = sago::getCacheDir();
	const std::filesystem::path absolute_cache_folder = std::filesystem::absolute(cache_folder);

	const std::filesystem::path models_folder =
		(absolute_cache_folder / "locaal" / "models").string();

	// Check if the data folder exists
	if (!std::filesystem::exists(models_folder)) {
		Logger::log(Logger::Level::INFO, "Creating models folder: %s",
			    models_folder.c_str());
		// Create the data folder
		if (!std::filesystem::create_directories(models_folder)) {
			Logger::log(Logger::Level::ERROR_LOG, "Failed to create models folder: %s",
				    models_folder.c_str());
			return "";
		}
	}

	return models_folder;
}

std::string find_model_folder(const ModelInfo &model_info)
{
	if (model_info.friendly_name.empty() || model_info.local_folder_name.empty() ||
	    model_info.files.empty()) {
		Logger::log(Logger::Level::ERROR_LOG, "Model info is invalid.");
		return "";
	}

	const std::filesystem::path data_folder_models = get_models_folder();

	const std::string model_local_data_path =
		(data_folder_models / model_info.local_folder_name).string();

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

std::string find_model_ext_file(const ModelInfo &model_info, const std::string &ext)
{
	const std::string model_local_folder_path = find_model_folder(model_info);
	if (model_local_folder_path.empty()) {
		return "";
	}

	return find_ext_file_in_folder(model_local_folder_path, ext);
}

size_t write_data(void *ptr, size_t size, size_t nmemb, FILE *stream)
{
	size_t written = fwrite(ptr, size, nmemb, stream);
	return written;
}

std::string get_filename_from_url(const std::string &url)
{
	auto lastSlashPos = url.find_last_of("/");
	auto queryPos = url.find("?", lastSlashPos);
	if (queryPos == std::string::npos) {
		return url.substr(lastSlashPos + 1);
	} else {
		return url.substr(lastSlashPos + 1, queryPos - lastSlashPos - 1);
	}
}

void download_model(const ModelInfo &model_info, download_finished_callback_t finished_callback,
		    download_progress_callback_t progress_callback,
		    download_error_callback_t error_callback)
{
	const std::filesystem::path module_config_models_folder = get_models_folder();

	const std::string model_local_config_path =
		(module_config_models_folder / model_info.local_folder_name).string();

	Logger::log(Logger::Level::INFO, "Model save path: %s", model_local_config_path.c_str());

	if (!std::filesystem::exists(model_local_config_path)) {
		// model folder does not exist, create it
		if (!std::filesystem::create_directories(model_local_config_path)) {
			Logger::log(Logger::Level::ERROR_LOG, "Failed to create model folder: %s",
				    model_local_config_path.c_str());
			error_callback(DownloadError::DOWNLOAD_ERROR_FILE,
				       "Failed to create model folder.");
			return;
		}
	}

	CURL *curl = curl_easy_init();
	if (curl) {
		for (auto &model_download_file : model_info.files) {
			Logger::log(Logger::Level::INFO, "Model URL: %s",
				    model_download_file.url.c_str());

			const std::string model_filename =
				get_filename_from_url(model_download_file.url);
			const std::string model_file_save_path =
				(std::filesystem::path(model_local_config_path) / model_filename)
					.string();
			if (std::filesystem::exists(model_file_save_path)) {
				Logger::log(Logger::Level::INFO, "Model file already exists: %s",
					    model_file_save_path.c_str());
				continue;
			}

			FILE *fp = fopen(model_file_save_path.c_str(), "wb");
			if (fp == nullptr) {
				Logger::log(Logger::Level::ERROR_LOG,
					    "Failed to open model file for writing %s.",
					    model_file_save_path.c_str());
				error_callback(DownloadError::DOWNLOAD_ERROR_FILE,
					       "Failed to open file.");
				return;
			}
			curl_easy_setopt(curl, CURLOPT_URL, model_download_file.url.c_str());
			curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_data);
			curl_easy_setopt(curl, CURLOPT_WRITEDATA, fp);
			curl_easy_setopt(curl, CURLOPT_NOPROGRESS, 0L);
			curl_easy_setopt(curl, CURLOPT_XFERINFOFUNCTION,
					 [progress_callback](void *clientp, curl_off_t dltotal,
							     curl_off_t dlnow, curl_off_t,
							     curl_off_t) {
						 if (dltotal == 0) {
							 return 0; // Unknown progress
						 }
						 int progress = (int)(dlnow * 100l / dltotal);
						 progress_callback(progress);
						 return 0;
					 });
			// curl_easy_setopt(curl, CURLOPT_XFERINFODATA, &progress_callback);
			// Follow redirects
			curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
			CURLcode res = curl_easy_perform(curl);
			if (res != CURLE_OK) {
				Logger::log(Logger::Level::ERROR_LOG,
					    "Failed to download model file %s.",
					    model_filename.c_str());
				error_callback(DownloadError::DOWNLOAD_ERROR_NETWORK,
					       "Failed to download model file.");
			}
			fclose(fp);
		}
		curl_easy_cleanup(curl);
		finished_callback(DownloadStatus::DOWNLOAD_STATUS_OK, model_local_config_path);
	} else {
		Logger::log(Logger::Level::ERROR_LOG, "Failed to initialize curl.");
		error_callback(DownloadError::DOWNLOAD_ERROR_NETWORK, "Failed to initialize curl.");
		finished_callback(DownloadStatus::DOWNLOAD_STATUS_ERROR, "");
	}
}
