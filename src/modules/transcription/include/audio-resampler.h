#ifndef AUDIO_RESAMPLER_H
#define AUDIO_RESAMPLER_H

#include <vector>

extern "C" {
#include <libavutil/channel_layout.h>
}

// Forward declarations
struct SwrContext;
struct AVChannelLayout;

/**
 * @class AudioResampler
 * @brief A class for resampling audio data.
 *
 * The AudioResampler class provides functionality to configure and resample
 * audio data from one format to another. It supports setting input and output
 * channels and sample rates.
 *
 * @note This class uses the FFmpeg library for resampling.
 *
 * @example
 * @code
 * AudioResampler resampler;
 * resampler.configure(2, 44100, 2, 48000);
 * std::vector<std::vector<float>> input = ...; // Your input audio data
 * std::vector<std::vector<float>> output = resampler.resample(input);
 * @endcode
 */
class AudioResampler {
public:
	/**
	 * @brief Constructs a new AudioResampler object.
	 */
	AudioResampler();
	~AudioResampler();

	/**
	 * @brief Configures the resampler with input and output parameters.
	 *
	 * @param in_channels Number of input channels.
	 * @param in_sample_rate Input sample rate.
	 * @param out_channels Number of output channels.
	 * @param out_sample_rate Output sample rate.
	 */
	void configure(int in_channels, int in_sample_rate, int out_channels, int out_sample_rate);
	/**
	 * @brief Resamples the input audio data.
	 *
	 * @param input A 2D vector containing the input audio data.
	 * @return A 2D vector containing the resampled audio data.
	 */
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
