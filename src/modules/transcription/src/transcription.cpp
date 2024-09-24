#include "transcription.h"
#include "logger.h"
#include "whisper-utils.h"

#include <iostream>

void set_text_callback(struct transcription_context *gf, const DetectionResultWithText &str)
{
	Logger::log(Logger::Level::INFO, "Transcription: %s", str.text.c_str());
}

void clear_current_caption(transcription_context *gf_){};

// Callback sent when the VAD finds an audio chunk. Sample rate = WHISPER_SAMPLE_RATE, channels = 1
// The audio chunk is in 32-bit float format
void audio_chunk_callback(struct transcription_context *gf, const std::vector<float> pcm32f_data,
			  int vad_state, const DetectionResultWithText &result){};

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
	// Add any additional logic for setting model download callbacks
}

void Transcription::setTranscriptionCallback(
	std::function<void(const TranscriptionResult &)> callback)
{
	transcriptionCallback_ = callback;
	// Add any additional logic for setting transcription callback
}

void Transcription::startTranscription()
{
	Logger::log(Logger::Level::INFO, "Starting transcription...");
	// start the transcription thread
	start_whisper_thread_with_path(this->gf, "en", "silero_vad_model_file");
}

void Transcription::stopTranscription()
{
	Logger::log(Logger::Level::INFO, "Stopping transcription...");
	// Implement the logic to stop the transcription process
	shutdown_whisper_thread(this->gf);
}

void Transcription::processAudio(const std::vector<float> &audioData)
{
	Logger::log(Logger::Level::INFO, "Processing audio data...");
}

} // namespace locaal
