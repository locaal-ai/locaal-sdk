#ifndef WHISPER_PROCESSING_H
#define WHISPER_PROCESSING_H

#include <whisper.h>

// buffer size in msec
#define DEFAULT_BUFFER_SIZE_MSEC 3000
// overlap in msec
#define DEFAULT_OVERLAP_SIZE_MSEC 125
#define MAX_OVERLAP_SIZE_MSEC 1000
#define MIN_OVERLAP_SIZE_MSEC 125
#define MAX_MS_WORK_BUFFER 11000

enum DetectionResult {
	DETECTION_RESULT_UNKNOWN = 0,
	DETECTION_RESULT_SILENCE = 1,
	DETECTION_RESULT_SPEECH = 2,
	DETECTION_RESULT_SUPPRESSED = 3,
	DETECTION_RESULT_NO_INFERENCE = 4,
	DETECTION_RESULT_PARTIAL = 5,
};

/**
 * @struct DetectionResultWithText
 * @brief Represents the result of a detection process along with associated text and metadata.
 * 
 * This structure holds the detection result, the detected text, timestamps, tokens, and language information.
 * 
 * @var DetectionResultWithText::result
 * The result of the detection process.
 * 
 * @var DetectionResultWithText::text
 * The text associated with the detection result.
 * 
 * @var DetectionResultWithText::start_timestamp_ms
 * The start timestamp of the detection in milliseconds.
 * 
 * @var DetectionResultWithText::end_timestamp_ms
 * The end timestamp of the detection in milliseconds.
 * 
 * @var DetectionResultWithText::tokens
 * A vector of tokens associated with the detected text.
 * 
 * @var DetectionResultWithText::language
 * The language of the detected text.
 */
struct DetectionResultWithText {
	DetectionResult result;
	std::string text;
	uint64_t start_timestamp_ms;
	uint64_t end_timestamp_ms;
	std::vector<whisper_token_data> tokens;
	std::string language;
};

struct transcription_context;

/**
 * @brief Processes transcription in a loop.
 *
 * This function continuously processes transcription tasks within the given
 * transcription context. It is designed to handle transcription operations
 * in a loop, ensuring that all tasks are processed as expected.
 *
 * @param gf A pointer to the transcription context structure that holds
 *           the necessary data and state for the transcription process.
 */
void whisper_loop(struct transcription_context *gf);
/**
 * @brief Initializes the whisper context with the given model path and transcription context.
 *
 * This function sets up the whisper context required for processing transcriptions.
 *
 * @param model_path The path to the model file used for whisper processing.
 * @param gf A pointer to the transcription context structure.
 * @return A pointer to the initialized whisper context structure.
 */
struct whisper_context *init_whisper_context(const std::string &model_path,
					     struct transcription_context *gf);
/**
 * @brief Runs inference and executes callbacks based on the transcription context and VAD state.
 *
 * This function processes the transcription context within the specified time offsets and 
 * performs necessary actions based on the Voice Activity Detection (VAD) state.
 *
 * @param gf Pointer to the transcription context structure.
 * @param start_offset_ms The start offset in milliseconds for the inference.
 * @param end_offset_ms The end offset in milliseconds for the inference.
 * @param vad_state The current state of the Voice Activity Detection.
 */
void run_inference_and_callbacks(transcription_context *gf, uint64_t start_offset_ms,
				 uint64_t end_offset_ms, int vad_state);

#endif // WHISPER_PROCESSING_H
