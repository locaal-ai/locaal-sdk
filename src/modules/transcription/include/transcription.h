#ifndef LOCAAL_TRANSCRIPTION_H
#define LOCAAL_TRANSCRIPTION_H

#include <string>
#include <functional>

namespace locaal {

struct TranscriptionResult {
	std::string text;
	uint64_t start_timestamp_ms;
	uint64_t end_timestamp_ms;
	std::string language;
    bool is_partial;
};

class Transcription {
public:
    Transcription();
    ~Transcription();

    void setTranscriptionParams(const std::string& language);

    void setModelDownloadCallbacks(
        std::function<void(const std::string&, const std::string&)> onSuccess,
        std::function<void(const std::string&, const std::string&)> onFailure,
        std::function<void(const std::string&, const std::string&)> onProgress
    );

    void setTranscriptionCallback(std::function<void(const TranscriptionResult&)> callback);

    void startTranscription();
    void stopTranscription();  // Added for completeness

    void processAudio(const std::vector<float>& audioData);

private:
    std::string language_;
    std::function<void(const std::string&, const std::string&)> onModelDownloadSuccess_;
    std::function<void(const std::string&, const std::string&)> onModelDownloadFailure_;
    std::function<void(const std::string&, const std::string&)> onModelDownloadProgress_;
    std::function<void(const TranscriptionResult&)> transcriptionCallback_;

    // Add any other necessary private members
};

} // namespace locaal

#endif // LOCAAL_TRANSCRIPTION_H
