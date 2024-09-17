#ifndef AUDIO_RESAMPLER_H
#define AUDIO_RESAMPLER_H

#include <vector>

extern "C" {
#include <libavutil/channel_layout.h>
}

// Forward declarations
struct SwrContext;
struct AVChannelLayout;

class AudioResampler {
public:
	AudioResampler();
	~AudioResampler();

	void configure(int in_channels, int in_sample_rate, int out_channels, int out_sample_rate);
	std::vector<std::vector<float>> resample(const std::vector<std::vector<float>> &input);

private:
	SwrContext *swr_ctx;
	AVChannelLayout in_ch_layout;
	AVChannelLayout out_ch_layout;
	int in_sample_rate;
	int out_sample_rate;
	int in_channels;
	int out_channels;
};

#endif // AUDIO_RESAMPLER_H
