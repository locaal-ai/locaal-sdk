#include "audio_capture.h"
#include <algorithm>
#include <cstring>

AudioCapture::AudioCapture(int buffer_duration_ms)
    : buffer_duration_ms(buffer_duration_ms), is_capturing(false) {}

AudioCapture::~AudioCapture() {
    if (device_id != 0) {
        SDL_CloseAudioDevice(device_id);
    }
}

bool AudioCapture::initialize(int device_index, int requested_sample_rate) {
    SDL_AudioSpec desired_spec, obtained_spec;

    SDL_zero(desired_spec);
    desired_spec.freq = requested_sample_rate;
    desired_spec.format = AUDIO_F32;
    desired_spec.channels = 1;
    desired_spec.samples = 1024;
    desired_spec.callback = [](void* userdata, uint8_t* stream, int len) {
        static_cast<AudioCapture*>(userdata)->processAudio(stream, len);
    };
    desired_spec.userdata = this;

    device_id = SDL_OpenAudioDevice(SDL_GetAudioDeviceName(device_index, 1), 1, &desired_spec, &obtained_spec, 0);
    if (device_id == 0) {
        SDL_Log("Failed to open audio device: %s", SDL_GetError());
        return false;
    }

    sample_rate = obtained_spec.freq;
    audio_buffer.resize(sample_rate * buffer_duration_ms / 1000);
    write_position = 0;
    buffer_size = 0;

    return true;
}

bool AudioCapture::startCapture() {
    if (device_id == 0) return false;
    is_capturing = true;
    SDL_PauseAudioDevice(device_id, 0);
    return true;
}

bool AudioCapture::stopCapture() {
    if (device_id == 0) return false;
    is_capturing = false;
    SDL_PauseAudioDevice(device_id, 1);
    return true;
}

bool AudioCapture::resetBuffer() {
    std::lock_guard<std::mutex> lock(buffer_mutex);
    write_position = 0;
    buffer_size = 0;
    std::fill(audio_buffer.begin(), audio_buffer.end(), 0.0f);
    return true;
}

void AudioCapture::processAudio(uint8_t* stream, int length) {
    if (!is_capturing) return;

    std::lock_guard<std::mutex> lock(buffer_mutex);
    int sample_count = length / sizeof(float);
    const float* input = reinterpret_cast<const float*>(stream);

    for (int i = 0; i < sample_count; ++i) {
        audio_buffer[write_position] = input[i];
        write_position = (write_position + 1) % audio_buffer.size();
        if (buffer_size < audio_buffer.size()) {
            ++buffer_size;
        }
    }
}

void AudioCapture::getAudioData(int duration_ms, std::vector<float>& output) {
    std::lock_guard<std::mutex> lock(buffer_mutex);
    int requested_samples = (duration_ms * sample_rate) / 1000;
    requested_samples = std::min(requested_samples, static_cast<int>(buffer_size));

    output.resize(requested_samples);
    size_t start_pos = (write_position - requested_samples + audio_buffer.size()) % audio_buffer.size();

    for (int i = 0; i < requested_samples; ++i) {
        output[i] = audio_buffer[(start_pos + i) % audio_buffer.size()];
    }
}

bool handleSDLEvents() {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT) {
            return false;
        }
    }
    return true;
}
