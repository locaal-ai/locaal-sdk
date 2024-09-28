#ifndef TRANSCRIPTION_CONTEXT_H
#define TRANSCRIPTION_CONTEXT_H

#include <whisper.h>

#include <thread>
#include <memory>
#include <mutex>
#include <condition_variable>
#include <functional>
#include <string>
#include <deque>
#include <optional>

#include "silero-vad-onnx.h"
#include "whisper-processing.h"
#include "token-buffer-thread.h"
#include "logger.h"
#include "audio-resampler.h"

#define MAX_PREPROC_CHANNELS 10

// Audio packet info
struct transcription_filter_audio_info {
	uint32_t frames;
	uint64_t timestamp_offset_ns; // offset (since start of processing) timestamp in ns
};

/**
 * @class transcription_context
 * @brief A class for managing the transcription context.
 * 
 * The transcription_context class provides methods to manage the transcription
 * context, including audio buffers, resampling, and the Whisper model.
 */
struct transcription_context {
	size_t channels;      // number of channels
	uint32_t sample_rate; // input sample rate
	// How many input frames (in input sample rate) are needed for the next whisper frame
	size_t frames;
	// How many frames were processed in the last whisper frame (this is dynamic)
	size_t last_num_frames;
	// Start begining timestamp in ms since epoch
	uint64_t start_timestamp_ms;
	// Sentence counter for srt
	size_t sentence_number;
	// Minimal subtitle duration in ms
	size_t min_sub_duration;
	// Maximal subtitle duration in ms
	size_t max_sub_duration;
	// Last time a subtitle was rendered
	uint64_t last_sub_render_time;
	bool cleared_last_sub;

	/* PCM buffers */
	std::vector<std::vector<float>> copy_buffers;
	std::deque<transcription_filter_audio_info> info_buffer;
	std::vector<std::deque<float>> input_buffers;
	std::deque<float> whisper_buffer;

	/* Resampler */
	AudioResampler resampler_to_whisper;
	std::deque<float> resampled_buffer;

	/* whisper */
	std::string whisper_model_path;
	struct whisper_context *whisper_context;
	whisper_full_params whisper_params;

	/* Silero VAD */
	std::unique_ptr<VadIterator> vad;

	float filler_p_threshold;
	float sentence_psum_accept_thresh;

	bool do_silence;
	int vad_mode;
	Logger::Level log_level = Logger::Level::DEBUG;
	bool log_words;
	bool caption_to_stream;
	bool active = false;
	bool save_to_file = false;
	bool save_srt = false;
	bool truncate_output_file = false;
	bool save_only_while_recording = false;
	bool process_while_muted = false;
	bool rename_file_to_match_recording = false;
	bool translate = false;
	std::string target_lang;
	std::string translation_output;
	bool enable_token_ts_dtw = false;
	std::vector<std::tuple<std::string, std::string>> filter_words_replace;
	bool fix_utf8 = true;
	bool enable_audio_chunks_callback = false;
	bool source_signals_set = false;
	bool initial_creation = true;
	bool partial_transcription = false;
	int partial_latency = 1000;
	float duration_filter_threshold = 2.25f;
	int segment_duration = 7000;

	// Last transcription result
	std::string last_text_for_translation;
	std::string last_text_translation;

	// Transcription context sentences
	int n_context_sentences;
	std::deque<std::string> last_transcription_sentence;

	// Text source to output the subtitles
	std::string text_source_name;
	// Callback to set the text in the output text source (subtitles)
	std::function<void(const DetectionResultWithText &result)> setTextCallback;
	// Output file path to write the subtitles
	std::string output_file_path;
	std::string whisper_model_file_currently_loaded;
	bool whisper_model_loaded_new;

	// Use std for thread and mutex
	std::thread whisper_thread;

	std::mutex whisper_buf_mutex;
	std::mutex whisper_ctx_mutex;
	std::condition_variable wshiper_thread_cv;
	std::optional<std::condition_variable> input_cv;

	bool buffered_output = false;
	TokenBufferThread captions_monitor;
	TokenBufferThread translation_monitor;
	int buffered_output_num_lines = 2;
	int buffered_output_num_chars = 30;
	TokenBufferSegmentation buffered_output_output_type =
		TokenBufferSegmentation::SEGMENTATION_TOKEN;

	/**
	 * @brief Default constructor.
	 */
	transcription_context() : whisper_buf_mutex(), whisper_ctx_mutex(), wshiper_thread_cv()
	{
		// initialize all pointers to nullptr
		for (size_t i = 0; i < copy_buffers.size(); i++) {
			copy_buffers[i].clear();
		}
		whisper_model_path = "";
		whisper_context = nullptr;
		output_file_path = "";
		whisper_model_file_currently_loaded = "";
	}
};

/**
 * @brief Sets the text callback for the transcription context.
 * 
 * This function assigns a callback function that will be invoked with the 
 * detection result containing text. The callback is associated with the 
 * provided transcription context.
 * 
 * @param gf A pointer to the transcription context structure.
 * @param str A reference to the DetectionResultWithText object containing 
 *            the detection result and associated text.
 */
void set_text_callback(struct transcription_context *gf, const DetectionResultWithText &str);

/**
 * @brief Clears the current caption in the transcription context.
 * 
 * This function resets or removes the current caption stored in the given
 * transcription context, effectively clearing any ongoing transcription data.
 * 
 * @param gf_ A pointer to the transcription context whose current caption
 *            needs to be cleared.
 */
void clear_current_caption(transcription_context *gf_);

/**
 * @brief Callback function to process audio chunks for transcription.
 *
 * This function is called to handle audio chunks and perform transcription.
 *
 * @param gf Pointer to the transcription context.
 * @param pcm32f_data Vector containing the audio data in 32-bit float PCM format.
 * @param vad_state Integer representing the Voice Activity Detection (VAD) state.
 * @param result Reference to a DetectionResultWithText object containing the detection result and associated text.
 */
void audio_chunk_callback(struct transcription_context *gf, const std::vector<float> pcm32f_data,
			  int vad_state, const DetectionResultWithText &result);

#endif /* TRANSCRIPTION_CONTEXT_H */
