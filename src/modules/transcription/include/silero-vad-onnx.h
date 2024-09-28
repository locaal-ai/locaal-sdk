#ifndef SILERO_VAD_ONNX_H
#define SILERO_VAD_ONNX_H

#include <onnxruntime_cxx_api.h>
#include <vector>
#include <string>
#include <limits>

#ifdef _WIN32
typedef std::wstring SileroString;
#else
typedef std::string SileroString;
#endif

/**
 * @class timestamp_t
 * @brief Represents a timestamp with start and end times.
 *
 * The timestamp_t class encapsulates a time interval with a start and end time.
 * It provides constructors for initialization, an assignment operator, and an
 * equality comparison operator. Additionally, it includes methods for converting
 * the timestamp to a string representation.
 *
 * @var int timestamp_t::start
 * The start time of the timestamp.
 *
 * @var int timestamp_t::end
 * The end time of the timestamp.
 */
class timestamp_t {
public:
	int start;
	int end;

	// default + parameterized constructor
	timestamp_t(int start = -1, int end = -1);

	// assignment operator modifies object, therefore non-const
	timestamp_t &operator=(const timestamp_t &a);

	// equality comparison. doesn't modify object. therefore const.
	bool operator==(const timestamp_t &a) const;
	std::string string();

private:
	std::string format(const char *fmt, ...);
};

/**
 * @class VadIterator
 * @brief A class for Voice Activity Detection (VAD) using ONNX runtime.
 * 
 * This class provides methods to process audio data and detect speech segments
 * using a pre-trained ONNX model.
 * 
 * @note The class is designed to work with specific sample rates and window sizes.
 * 
 * @file silero-vad-onnx.h
 */
class VadIterator {
private:
	// OnnxRuntime resources
	Ort::Env env;
	Ort::SessionOptions session_options;
	std::shared_ptr<Ort::Session> session = nullptr;
	Ort::AllocatorWithDefaultOptions allocator;
	Ort::MemoryInfo memory_info = Ort::MemoryInfo::CreateCpu(OrtArenaAllocator, OrtMemTypeCPU);

private:
	/**
	 * @brief Initializes the engine threads.
	 * 
	 * @param inter_threads Number of inter threads.
	 * @param intra_threads Number of intra threads.
	 */
	void init_engine_threads(int inter_threads, int intra_threads);
	/**
	 * @brief Initializes the ONNX model.
	 * 
	 * @param model_path Path to the ONNX model file.
	 */
	void init_onnx_model(const SileroString &model_path);
	/**
	 * @brief Resets the internal states of the VAD.
	 * 
	 * @param reset_state Flag to indicate whether to reset the state.
	 */
	void reset_states(bool reset_state);
	/**
	 * @brief Predicts the VAD output for a single frame of audio data.
	 * 
	 * @param data A vector containing the audio data.
	 * @return The VAD prediction score.
	 */
	float predict_one(const std::vector<float> &data);
	/**
	 * @brief Predicts the VAD output for a sequence of audio data.
	 * 
	 * @param data A vector containing the audio data.
	 */
	void predict(const std::vector<float> &data);

public:
	/**
	 * @brief Processes the input audio data and updates the internal state.
	 * 
	 * @param input_wav A vector containing the input audio data.
	 * @param reset_state Flag to indicate whether to reset the state.
	 */
	void process(const std::vector<float> &input_wav, bool reset_state = true);
	/**
	 * @brief Processes the input audio data and outputs the processed audio data.
	 * 
	 * @param input_wav A vector containing the input audio data.
	 * @param output_wav A vector to store the output audio data.
	 */
	void process(const std::vector<float> &input_wav, std::vector<float> &output_wav);
	/**
	 * @brief Collects the speech chunks from the input audio data.
	 * 
	 * @param input_wav A vector containing the input audio data.
	 * @param output_wav A vector to store the collected speech chunks.
	 */
	void collect_chunks(const std::vector<float> &input_wav, std::vector<float> &output_wav);
	/**
	 * @brief Gets the timestamps of detected speech segments.
	 * 
	 * @return A vector of timestamps representing the speech segments.
	 */
	const std::vector<timestamp_t> get_speech_timestamps() const;
	/**
	 * @brief Drops the non-speech chunks from the input audio data.
	 * 
	 * @param input_wav A vector containing the input audio data.
	 * @param output_wav A vector to store the remaining audio data after dropping non-speech chunks.
	 */
	void drop_chunks(const std::vector<float> &input_wav, std::vector<float> &output_wav);
	/**
	 * @brief Sets the VAD threshold.
	 * 
	 * @param threshold_ The new threshold value.
	 */
	void set_threshold(float threshold_) { this->threshold = threshold_; }

	/**
	 * @brief Gets the window size in samples.
	 * 
	 * @return The window size in samples.
	 */
	int64_t get_window_size_samples() const { return window_size_samples; }

private:
	// model config
	int64_t window_size_samples; // Assign when init, support 256 512 768 for 8k; 512 1024 1536 for 16k.
	int sample_rate;             // Assign when init support 16000 or 8000
	int sr_per_ms;               // Assign when init, support 8 or 16
	float threshold;
	int min_silence_samples;               // sr_per_ms * #ms
	int min_silence_samples_at_max_speech; // sr_per_ms * #98
	int min_speech_samples;                // sr_per_ms * #ms
	float max_speech_samples;
	int speech_pad_samples; // usually a
	int audio_length_samples;

	// model states
	bool triggered = false;
	unsigned int temp_end = 0;
	unsigned int current_sample = 0;
	// MAX 4294967295 samples / 8sample per ms / 1000 / 60 = 8947 minutes
	int prev_end;
	int next_start = 0;

	//Output timestamp
	std::vector<timestamp_t> speeches;
	timestamp_t current_speech;

	// Onnx model
	// Inputs
	std::vector<Ort::Value> ort_inputs;

	std::vector<const char *> input_node_names = {"input", "state", "sr"};
	std::vector<float> input;
	unsigned int size_state = 2 * 1 * 128; // It's FIXED.
	std::vector<float> _state;
	std::vector<int64_t> sr;

	int64_t input_node_dims[2] = {};
	const int64_t state_node_dims[3] = {2, 1, 128};
	const int64_t sr_node_dims[1] = {1};

	// Outputs
	std::vector<Ort::Value> ort_outputs;
	std::vector<const char *> output_node_names = {"output", "stateN"};

public:
	/**
	 * @brief Constructs a VadIterator object.
	 * 
	 * @param ModelPath Path to the ONNX model file.
	 * @param Sample_rate Sample rate of the audio data (default is 16000).
	 * @param windows_frame_size Size of the window frame (default is 32).
	 * @param Threshold VAD threshold (default is 0.5).
	 * @param min_silence_duration_ms Minimum silence duration in milliseconds (default is 0).
	 * @param speech_pad_ms Speech padding in milliseconds (default is 32).
	 * @param min_speech_duration_ms Minimum speech duration in milliseconds (default is 32).
	 * @param max_speech_duration_s Maximum speech duration in seconds (default is infinity).
	 */
	VadIterator(const SileroString &ModelPath, int Sample_rate = 16000,
		    int windows_frame_size = 32, float Threshold = 0.5,
		    int min_silence_duration_ms = 0, int speech_pad_ms = 32,
		    int min_speech_duration_ms = 32,
		    float max_speech_duration_s = std::numeric_limits<float>::infinity());

	// Default constructor
	VadIterator() = default;
};

#endif // SILERO_VAD_ONNX_H
