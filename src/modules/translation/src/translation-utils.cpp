

#include "translation-includes.h"
#include "translation.h"
#include "translation-utils.h"
#include "logger.h"
#include "model-downloader.h"

void start_translation(struct translation_context *ctx, const std::string &translation_model_index)
{
	Logger::log(Logger::Level::INFO, "Starting translation...");

	const ModelInfo &translation_model_info = models_info[translation_model_index];
	std::string model_file_found = find_model_folder(translation_model_info);
	if (model_file_found == "") {
		Logger::log(Logger::Level::INFO,
			    "Translation CT2 model does not exist. Downloading...");
		download_model(
			translation_model_info,
			[ctx, model_file_found](int download_status, const std::string &path) {
				if (download_status == 0) {
					Logger::log(Logger::Level::INFO,
						    "CT2 model download complete");
					build_and_enable_translation(ctx, path);
				} else {
					Logger::log(Logger::Level::ERROR_LOG,
						    "Model download failed");
					ctx->model_loaded = false;
				}
			},
			[](int progress) {
				Logger::log(Logger::Level::INFO, "Download progress: %d", progress);
			},
			[](int error, const std::string &message) {
				Logger::log(Logger::Level::ERROR_LOG, "Download error: %s",
					    message.c_str());
			});
	} else {
		// Model exists, just load it
		build_and_enable_translation(ctx, model_file_found);
	}
}
