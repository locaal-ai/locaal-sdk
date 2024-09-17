#ifdef _WIN32
#define NOMINMAX
#endif

#include "whisper-utils.h"
#include "whisper-processing.h"
#include "logger.h"
#include "model-downloader.h"

void update_whisper_model(struct transcription_context *gf, const std::string new_model_path)
{
	// update the whisper model path

	const ModelInfo &silero_vad_model_info = models_info["Silero VAD v5"];
	const std::string silero_vad_model_file =
		find_model_ext_file(silero_vad_model_info, ".onnx");
	if (silero_vad_model_file.empty()) {
		Logger::log(Logger::Level::ERROR_LOG, "Cannot find Silero VAD model file");
		return;
	}
	Logger::log(gf->log_level, "Silero VAD model file: %s", silero_vad_model_file);

	if (gf->whisper_model_path.empty() || gf->whisper_model_path != new_model_path) {

		if (gf->whisper_model_path != new_model_path) {
			// model path changed
			Logger::log(gf->log_level, "model path changed from %s to %s",
				    gf->whisper_model_path.c_str(), new_model_path.c_str());

			// check if this is loading the initial model or a switch
			gf->whisper_model_loaded_new = !gf->whisper_model_path.empty();
		}

		shutdown_whisper_thread(gf);

		if (models_info.count(new_model_path) == 0) {
			Logger::log(Logger::Level::WARNING, "Model '%s' does not exist",
				    new_model_path.c_str());
			return;
		}

		const ModelInfo &model_info = models_info[new_model_path];

		// check if the model exists, if not, download it
		std::string model_file_found = find_model_ext_file(model_info, ".bin");
		if (model_file_found == "") {
			Logger::log(Logger::Level::WARNING, "Whisper model does not exist");
			download_model(
				model_info,
				[gf, new_model_path, silero_vad_model_file](
					int download_status, const std::string &path) {
					if (download_status == DownloadStatus::DOWNLOAD_STATUS_OK) {
						Logger::log(Logger::Level::INFO,
							    "Model download complete");
						gf->whisper_model_path = new_model_path;
						start_whisper_thread_with_path(
							gf, path, silero_vad_model_file.c_str());
					} else {
						Logger::log(Logger::Level::ERROR_LOG,
							    "Model download failed");
					}
				},
				[](int progress) {
					Logger::log(Logger::Level::INFO, "Download progress: %d%%",
						    progress);
				},
				[](int error_code, const std::string &error) {
					Logger::log(Logger::Level::ERROR_LOG, "Download error: %s",
						    error.c_str());
				});
		} else {
			// Model exists, just load it
			gf->whisper_model_path = new_model_path;
			start_whisper_thread_with_path(gf, model_file_found,
						       silero_vad_model_file.c_str());
		}
	} else {
		// model path did not change
		Logger::log(gf->log_level, "Model path did not change: %s == %s",
			    gf->whisper_model_path.c_str(), new_model_path.c_str());
	}
}
