#include "audio-resampler.h"

#include <stdexcept>
#include <algorithm>

extern "C" {
#include <libavutil/opt.h>
#include <libavutil/channel_layout.h>
#include <libswresample/swresample.h>
}

AudioResampler::AudioResampler()
	: swr_ctx(nullptr),
	  in_sample_rate(0),
	  out_sample_rate(0),
	  in_channels(0),
	  out_channels(0)
{
	av_channel_layout_default(&in_ch_layout, 0);
	av_channel_layout_default(&out_ch_layout, 0);
}

AudioResampler::~AudioResampler()
{
	if (swr_ctx) {
		swr_free(&swr_ctx);
	}
	av_channel_layout_uninit(&in_ch_layout);
	av_channel_layout_uninit(&out_ch_layout);
}

void AudioResampler::configure(int in_channels, int in_sample_rate, int out_channels,
			       int out_sample_rate)
{
	if (swr_ctx) {
		swr_free(&swr_ctx);
	}

	av_channel_layout_uninit(&in_ch_layout);
	av_channel_layout_uninit(&out_ch_layout);
	av_channel_layout_default(&in_ch_layout, in_channels);
	av_channel_layout_default(&out_ch_layout, out_channels);

	this->in_sample_rate = in_sample_rate;
	this->out_sample_rate = out_sample_rate;
	this->in_channels = in_channels;
	this->out_channels = out_channels;

	swr_ctx = swr_alloc();
	if (!swr_ctx) {
		throw std::runtime_error("Could not allocate resampler context");
	}

	av_opt_set_chlayout(swr_ctx, "in_chlayout", &in_ch_layout, 0);
	av_opt_set_chlayout(swr_ctx, "out_chlayout", &out_ch_layout, 0);
	av_opt_set_int(swr_ctx, "in_sample_rate", in_sample_rate, 0);
	av_opt_set_int(swr_ctx, "out_sample_rate", out_sample_rate, 0);
	av_opt_set_sample_fmt(swr_ctx, "in_sample_fmt", AV_SAMPLE_FMT_FLTP, 0);
	av_opt_set_sample_fmt(swr_ctx, "out_sample_fmt", AV_SAMPLE_FMT_FLTP, 0);

	if (swr_init(swr_ctx) < 0) {
		throw std::runtime_error("Failed to initialize the resampling context");
	}
}

std::vector<std::vector<float>>
AudioResampler::resample(const std::vector<std::vector<float>> &input)
{
	if (!swr_ctx) {
		throw std::runtime_error("Resampler not configured");
	}

	if (input.size() != in_channels) {
		throw std::runtime_error("Input channel count doesn't match configuration");
	}

	int in_samples = (int)input[0].size();

	// Prepare input data
	std::vector<const float *> in_data(in_channels);
	for (int i = 0; i < in_channels; ++i) {
		in_data[i] = input[i].data();
	}

	// Calculate output size
	int64_t delay = swr_get_delay(swr_ctx, in_sample_rate);
	int out_samples = (int)av_rescale_rnd(delay + in_samples, out_sample_rate, in_sample_rate,
					      AV_ROUND_UP);

	// Prepare output buffer
	std::vector<std::vector<float>> output(out_channels, std::vector<float>(out_samples));
	std::vector<float *> out_data(out_channels);
	for (int i = 0; i < out_channels; ++i) {
		out_data[i] = output[i].data();
	}

	// Perform resampling
	int samples_out =
		swr_convert(swr_ctx, reinterpret_cast<uint8_t **>(out_data.data()), out_samples,
			    reinterpret_cast<const uint8_t **>(in_data.data()), in_samples);

	if (samples_out < 0) {
		throw std::runtime_error("Error while converting");
	}

	// Resize output to actual number of samples
	for (auto &channel : output) {
		channel.resize(samples_out);
	}

	return output;
}
