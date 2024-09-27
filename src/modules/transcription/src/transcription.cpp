#include "transcription.h"
#include "logger.h"
#include "whisper-utils.h"

#include <iostream>
#include <model-downloader-types.h>
#include <model-downloader.h>

void set_text_callback(struct transcription_context *gf, const DetectionResultWithText &str)
{
	Logger::log(Logger::Level::INFO, "Transcription: %s", str.text.c_str());
}

void clear_current_caption(transcription_context *gf_) {};

// Callback sent when the VAD finds an audio chunk. Sample rate = WHISPER_SAMPLE_RATE, channels = 1
// The audio chunk is in 32-bit float format
void audio_chunk_callback(struct transcription_context *gf, const std::vector<float> pcm32f_data,
			  int vad_state, const DetectionResultWithText &result) {};

namespace locaal {

Transcription::Transcription()
{
	// Constructor implementation
	gf = new transcription_context();
}

Transcription::~Transcription()
{
	// Destructor implementation
	delete gf;
}

void Transcription::setTranscriptionParams(const std::string &language)
{
	language_ = language;
	// Add any additional logic for setting transcription parameters
}

void Transcription::setModelDownloadCallbacks(
	std::function<void(const std::string &, const std::string &)> onSuccess,
	std::function<void(const std::string &, const std::string &)> onFailure,
	std::function<void(const std::string &, const std::string &)> onProgress)
{
	onModelDownloadSuccess_ = onSuccess;
	onModelDownloadFailure_ = onFailure;
	onModelDownloadProgress_ = onProgress;
}

void Transcription::setTranscriptionCallback(
	std::function<void(const TranscriptionResult &)> callback)
{
	transcriptionCallback_ = callback;
}

void Transcription::startTranscription()
{
	Logger::log(Logger::Level::INFO, "Starting transcription...");
	// find the silero VAD model file
	const ModelInfo &silero_vad_model_info = models_info["Silero VAD v5"];
	const std::string silero_vad_model_file =
		find_model_ext_file(silero_vad_model_info, ".onnx");
	if (silero_vad_model_file.empty()) {
		Logger::log(Logger::Level::ERROR_LOG, "Cannot find Silero VAD model file");
		download_model(
			silero_vad_model_info,
			[this, silero_vad_model_file](int download_status,
						      const std::string &path) {
				if (download_status == DownloadStatus::DOWNLOAD_STATUS_OK) {
					Logger::log(Logger::Level::INFO, "Model download complete");
					this->onModelDownloadSuccess_("Silero VAD", path);
					this->startTranscription(); // start the transcription thread
				} else {
					Logger::log(Logger::Level::ERROR_LOG,
						    "Model download failed");
					this->onModelDownloadFailure_("Silero VAD",
								      "Model download failed");
				}
			},
			[this](int progress) {
				Logger::log(Logger::Level::INFO, "Download progress: %d%%",
					    progress);
				this->onModelDownloadProgress_("Silero VAD",
							       std::to_string(progress));
			},
			[this](int error_code, const std::string &error) {
				Logger::log(Logger::Level::ERROR_LOG, "Download error: %s",
					    error.c_str());
				this->onModelDownloadFailure_("Silero VAD", error);
			});
		return;
	}
	Logger::log(Logger::Level::INFO, "Silero VAD model file: %s",
		    silero_vad_model_file.c_str());

	// find the whisper model file
	const ModelInfo &whisper_model_info = models_info["Whisper Tiny English (74Mb)"];

	// check if the model exists, if not, download it
	std::string model_file_found = find_model_ext_file(whisper_model_info, ".bin");
	if (model_file_found == "") {
		Logger::log(Logger::Level::WARNING, "Whisper model does not exist");
		download_model(
			whisper_model_info,
			[this, silero_vad_model_file](int download_status,
						      const std::string &path) {
				if (download_status == DownloadStatus::DOWNLOAD_STATUS_OK) {
					Logger::log(Logger::Level::INFO, "Model download complete");
					this->onModelDownloadSuccess_("Whisper", path);
					this->startTranscription(); // start the transcription thread
				} else {
					Logger::log(Logger::Level::ERROR_LOG,
						    "Model download failed");
					this->onModelDownloadFailure_("Whisper",
								      "Model download failed");
				}
			},
			[this](int progress) {
				Logger::log(Logger::Level::INFO, "Download progress: %d%%",
					    progress);
				this->onModelDownloadProgress_("Whisper", std::to_string(progress));
			},
			[this](int error_code, const std::string &error) {
				Logger::log(Logger::Level::ERROR_LOG, "Download error: %s",
					    error.c_str());
				this->onModelDownloadFailure_("Whisper", error);
			});
		return;
	}

	// start the transcription thread
	start_whisper_thread_with_path(this->gf, "en", silero_vad_model_file.c_str());
}

void Transcription::stopTranscription()
{
	Logger::log(Logger::Level::INFO, "Stopping transcription...");
	// Implement the logic to stop the transcription process
	shutdown_whisper_thread(this->gf);
}

void Transcription::processAudio(const std::vector<float> &audioData)
{
	// Logger::log(Logger::Level::DEBUG, "Processing audio data...");
}

} // namespace locaal
