#ifndef MODEL_DOWNLOADER_TYPES_H
#define MODEL_DOWNLOADER_TYPES_H

#include <functional>
#include <map>
#include <string>
#include <vector>

typedef std::function<void(int download_status, const std::string &path)>
	download_finished_callback_t;
typedef std::function<void(int download_progress_percent)> download_progress_callback_t;

enum DownloadStatus { DOWNLOAD_STATUS_OK, DOWNLOAD_STATUS_ERROR };
enum DownloadError { DOWNLOAD_ERROR_OK, DOWNLOAD_ERROR_NETWORK, DOWNLOAD_ERROR_FILE };

typedef std::function<void(int download_error_code, const std::string &error_message)>
	download_error_callback_t;

struct ModelFileDownloadInfo {
	std::string url;
	std::string sha256;
};

enum ModelType { MODEL_TYPE_TRANSCRIPTION, MODEL_TYPE_TRANSLATION, MODEL_TYPE_VAD };

struct ModelInfo {
	std::string friendly_name;
	std::string local_folder_name;
	ModelType type;
	std::vector<ModelFileDownloadInfo> files;
};

extern std::map<std::string, ModelInfo> models_info;

#endif /* MODEL_DOWNLOADER_TYPES_H */
