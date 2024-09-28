#ifndef WHISPER_UTILS_H
#define WHISPER_UTILS_H

#include "transcription-context.h"

#include <string>

/**
 * @brief Shuts down the whisper thread associated with the given transcription context.
 *
 * This function is responsible for properly terminating the whisper thread
 * that is part of the transcription context. It ensures that all resources
 * are released and the thread is safely stopped.
 *
 * @param gf A pointer to the transcription context structure.
 */
void shutdown_whisper_thread(struct transcription_context *gf);
/**
 * @brief Starts a whisper thread with the specified path and VAD model file.
 *
 * This function initializes and starts a whisper thread using the provided
 * transcription context, path, and Silero VAD (Voice Activity Detection) model file.
 *
 * @param gf Pointer to the transcription context structure.
 * @param path The file path to be used by the whisper thread.
 * @param silero_vad_model_file The file path to the Silero VAD model file.
 */
void start_whisper_thread_with_path(struct transcription_context *gf, const std::string &path,
				    const char *silero_vad_model_file);

/**
 * @brief Finds the starting indices of the overlapping region between two sequences of whisper token data.
 *
 * This function compares two sequences of whisper token data and identifies the starting indices
 * where the sequences begin to overlap. It returns a pair of integers representing the starting
 * indices of the overlap in the first and second sequences, respectively.
 *
 * @param seq1 The first sequence of whisper token data.
 * @param seq2 The second sequence of whisper token data.
 * @return A pair of integers where the first element is the starting index of the overlap in seq1,
 *         and the second element is the starting index of the overlap in seq2.
 */
std::pair<int, int> findStartOfOverlap(const std::vector<whisper_token_data> &seq1,
				       const std::vector<whisper_token_data> &seq2);
/**
 * @brief Reconstructs a sentence from two sequences of whisper token data.
 *
 * This function takes two sequences of whisper token data and combines them
 * to reconstruct a sentence. The specific method of reconstruction depends
 * on the implementation details within the function.
 *
 * @param seq1 The first sequence of whisper token data.
 * @param seq2 The second sequence of whisper token data.
 * @return A vector of whisper token data representing the reconstructed sentence.
 */
std::vector<whisper_token_data> reconstructSentence(const std::vector<whisper_token_data> &seq1,
						    const std::vector<whisper_token_data> &seq2);

/**
 * @brief Convert a timestamp in milliseconds to a string in the format "MM:SS.sss" .
 * Taken from https://github.com/ggerganov/whisper.cpp/blob/master/examples/stream/stream.cpp
 * @param t_ms_offset Timestamp in milliseconds (offset from the beginning of the stream)
 * @return std::string Timestamp in the format "MM:SS.sss"
 */
std::string to_timestamp(uint64_t t_ms_offset);

#endif /* WHISPER_UTILS_H */
