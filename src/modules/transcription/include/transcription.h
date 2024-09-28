#ifndef LOCAAL_TRANSCRIPTION_H
#define LOCAAL_TRANSCRIPTION_H

#include <string>
#include <functional>

struct transcription_context;

namespace locaal {

/**
 * @struct TranscriptionResult
 * @brief Represents the result of a transcription process.
 *
 * This structure holds the details of a transcription result, including the
 * transcribed text, timestamps, language, and whether the result is partial.
 *
 * @var TranscriptionResult::text
 * The transcribed text.
 *
 * @var TranscriptionResult::start_timestamp_ms
 * The start timestamp of the transcription in milliseconds.
 *
 * @var TranscriptionResult::end_timestamp_ms
 * The end timestamp of the transcription in milliseconds.
 *
 * @var TranscriptionResult::language
 * The language of the transcribed text.
 *
 * @var TranscriptionResult::is_partial
 * Indicates whether the transcription result is partial.
 */
struct TranscriptionResult {
	std::string text;
	uint64_t start_timestamp_ms;
	uint64_t end_timestamp_ms;
	std::string language;
	bool is_partial;
};

/**
 * @class Transcription
 * @brief Handles the transcription process, including setting parameters, managing callbacks, and processing audio data.
 * 
 * The Transcription class provides methods to configure transcription parameters, set callbacks for model download events and transcription results, and process audio data for transcription.
 */
class Transcription {
public:
	/**
	 * @brief Constructs a new Transcription object.
	 */
	Transcription();

	/**
	 * @brief Destroys the Transcription object.
	 */
	~Transcription();

	/**
	 * @brief Sets the transcription parameters.
	 * 
	 * @param language The language to be used for transcription.
	 */
	void setTranscriptionParams(const std::string &language);

	/**
	 * @brief Sets the callbacks for model download events.
	 * 
	 * @param onSuccess Callback function to be called on successful model download.
	 * @param onFailure Callback function to be called on model download failure.
	 * @param onProgress Callback function to be called to report model download progress.
	 */
	void setModelDownloadCallbacks(
		std::function<void(const std::string &, const std::string &)> onSuccess,
		std::function<void(const std::string &, const std::string &)> onFailure,
		std::function<void(const std::string &, const std::string &)> onProgress);

	/**
	 * @brief Sets the callback for transcription results.
	 * 
	 * @param callback Callback function to be called with the transcription result.
	 */
	void setTranscriptionCallback(std::function<void(const TranscriptionResult &)> callback);

	/**
	 * @brief Starts the transcription process.
	 */
	void startTranscription();

	/**
	 * @brief Stops the transcription process.
	 */
	void stopTranscription();

	/**
	 * @brief Processes the provided audio data for transcription.
	 * 
	 * @param audioData The audio data to be transcribed.
	 */
	void processAudio(const std::vector<float> &audioData);

private:
	std::string language_;
	std::function<void(const std::string &, const std::string &)> onModelDownloadSuccess_;
	std::function<void(const std::string &, const std::string &)> onModelDownloadFailure_;
	std::function<void(const std::string &, const std::string &)> onModelDownloadProgress_;
	std::function<void(const TranscriptionResult &)> transcriptionCallback_;

	// Add any other necessary private members
	transcription_context *gf;
};

} // namespace locaal

#endif // LOCAAL_TRANSCRIPTION_H
