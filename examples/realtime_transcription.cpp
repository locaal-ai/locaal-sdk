#include <locaal.h>

#define SDL_MAIN_HANDLED
#include "audio_capture.h"

#include <iostream>

int main()
{
	// SDL_SetMainReady();

	// Initialize the library
	locaal::Transcription tt;

	// Set the transcription parameters (language, model, etc.)
	tt.setTranscriptionParams("en-US");

	tt.setModelDownloadCallbacks(
		[](const std::string &model_name, const std::string &model_path) {
			std::cout << "Model downloaded: " << model_name << " at " << model_path
				  << std::endl;
		},
		[](const std::string &model_name, const std::string &model_path) {
			std::cout << "Model download failed: " << model_name << " at " << model_path
				  << std::endl;
		},
		[](const std::string &model_name, const std::string &model_path) {
			std::cout << "Model download progress: " << model_name << " at "
				  << model_path << std::endl;
		});

	// Set the callbacks for the transcription
	tt.setTranscriptionCallback([](const locaal::TranscriptionResult &result) {
		// Print the transcription result
		std::cout << "Transcription:" << (result.is_partial ? " (partial) " : " ")
			  << result.text << std::endl;
	});

	// Start real-time transcription background thread
	tt.startTranscription();

	// Start capturing audio from the microphone
	AudioCapture audio_capture(1000);
	if (!audio_capture.initialize(0, 16000)) {
		std::cerr << "Failed to initialize audio capture" << std::endl;
		return 1;
	}
	if (!audio_capture.startCapture()) {
		std::cerr << "Failed to start audio capture" << std::endl;
		return 1;
	}

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
