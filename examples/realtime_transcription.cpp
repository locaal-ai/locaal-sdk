#include <locaal.h>
#include <logger.h>

#define SDL_MAIN_HANDLED
#include "audio_capture.h"

#include <iostream>

int main()
{
	// set the log level to DEBUG
	Logger::setLogLevel(Logger::Level::DEBUG);

	// Initialize the library
	locaal::Transcription tt;

	// Set the transcription parameters (language, model, etc.)
	tt.setTranscriptionParams("en-US");

	tt.setModelDownloadCallbacks(
		[](const std::string &model_name, const std::string &model_path) {
			Logger::stream(Logger::Level::INFO)
				<< "Model downloaded: " << model_name << " at " << model_path;
		},
		[](const std::string &model_name, const std::string &model_path) {
			Logger::stream(Logger::Level::INFO)
				<< "Model download failed: " << model_name << " at " << model_path;
		},
		[](const std::string &model_name, const std::string &model_path) {
			Logger::stream(Logger::Level::INFO)
				<< "Model download progress: " << model_name << " at "
				<< model_path;
		});

	// Set the callbacks for the transcription
	tt.setTranscriptionCallback([](const locaal::TranscriptionResult &result) {
		// Print the transcription result
		Logger::stream(Logger::Level::INFO)
			<< "Transcription:" << (result.is_partial ? " (partial) " : " ")
			<< result.text;
	});

	// Start real-time transcription background thread
	tt.startTranscription();

	// Start capturing audio from the microphone
	AudioCapture audio_capture(1000);
	if (!audio_capture.initialize(0, 16000)) {
		Logger::stream(Logger::Level::ERROR_LOG) << "Failed to initialize audio capture";
		return 1;
	}
	if (!audio_capture.startCapture()) {
		Logger::stream(Logger::Level::ERROR_LOG) << "Failed to start audio capture";
		return 1;
	}

	Logger::log(Logger::Level::INFO, "Starting real-time transcription...");

	// Main loop
	while (true) {
		// Handle SDL events
		if (!handleSDLEvents()) {
			break;
		}

		// Get audio data from the audio capture
		std::vector<float> audio_data;
		audio_capture.getAudioData(1000, audio_data);

		// Process the audio data for transcription
		tt.processAudio(audio_data);
	}

	return 0;
}
