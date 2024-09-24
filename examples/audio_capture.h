#pragma once

#define SDL_MAIN_HANDLED
#include <SDL.h>

#include <atomic>
#include <cstdint>
#include <vector>
#include <mutex>

class AudioCapture {
public:
	AudioCapture(int buffer_duration_ms);
	~AudioCapture();

	bool initialize(int device_index, int sample_rate);
	bool startCapture();
	bool stopCapture();
	bool resetBuffer();

	void processAudio(uint8_t *stream, int length);
	void getAudioData(int duration_ms, std::vector<float> &output);

private:
	SDL_AudioDeviceID device_id = 0;
	int buffer_duration_ms = 0;
	int sample_rate = 0;
	std::atomic_bool is_capturing;
	std::mutex buffer_mutex;
	std::vector<float> audio_buffer;
	size_t write_position = 0;
	size_t buffer_size = 0;
};

bool handleSDLEvents();
