/**
 * @file vad-processing.h
 * @brief Header file for Voice Activity Detection (VAD) processing.
 * 
 * This file contains the declarations for VAD state and mode enums, 
 * the vad_state structure, and functions for VAD-based segmentation.
 * 
 * Enums:
 * - VadState: Represents the state of the VAD.
 *   - VAD_STATE_WAS_ON: VAD was previously on.
 *   - VAD_STATE_WAS_OFF: VAD was previously off.
 *   - VAD_STATE_IS_OFF: VAD is currently off.
 *   - VAD_STATE_PARTIAL: VAD is in a partial state.
 * 
 * - VadMode: Represents the mode of the VAD.
 *   - VAD_MODE_ACTIVE: VAD is active.
 *   - VAD_MODE_HYBRID: VAD is in hybrid mode.
 *   - VAD_MODE_DISABLED: VAD is disabled.
 * 
 * Structs:
 * - vad_state: Structure to hold the state of the VAD.
 *   - bool vad_on: Indicates if VAD is on.
 *   - uint64_t start_ts_offest_ms: Start timestamp offset in milliseconds.
 *   - uint64_t end_ts_offset_ms: End timestamp offset in milliseconds.
 *   - uint64_t last_partial_segment_end_ts: Timestamp of the last partial segment end.
 * 
 * Functions:
 * - vad_state vad_based_segmentation(transcription_context *gf, vad_state last_vad_state):
 *   Performs VAD-based segmentation.
 * 
 * - vad_state hybrid_vad_segmentation(transcription_context *gf, vad_state last_vad_state):
 *   Performs hybrid VAD segmentation.
 * 
 * - void initialize_vad(transcription_context *gf, const char *silero_vad_model_file):
 *   Initializes the VAD with the given model file.
 */
#ifndef VAD_PROCESSING_H
#define VAD_PROCESSING_H

enum VadState { VAD_STATE_WAS_ON = 0, VAD_STATE_WAS_OFF, VAD_STATE_IS_OFF, VAD_STATE_PARTIAL };
enum VadMode { VAD_MODE_ACTIVE = 0, VAD_MODE_HYBRID, VAD_MODE_DISABLED };

/**
 * @struct vad_state
 * @brief Represents the state of the Voice Activity Detection (VAD) process.
 * 
 * This structure holds information about the VAD status and various timestamps
 * related to the start and end of detected voice segments.
 * 
 * @var vad_state::vad_on
 * Indicates whether VAD is currently active.
 * 
 * @var vad_state::start_ts_offest_ms
 * The timestamp offset in milliseconds when VAD started.
 * 
 * @var vad_state::end_ts_offset_ms
 * The timestamp offset in milliseconds when VAD ended.
 * 
 * @var vad_state::last_partial_segment_end_ts
 * The timestamp of the end of the last partial voice segment detected.
 */
struct vad_state {
	bool vad_on;
	uint64_t start_ts_offest_ms;
	uint64_t end_ts_offset_ms;
	uint64_t last_partial_segment_end_ts;
};

/**
 * @brief Segments audio data based on Voice Activity Detection (VAD).
 *
 * This function processes the given transcription context to determine
 * segments of audio that contain speech, using the provided last VAD state
 * as a reference.
 *
 * @param gf Pointer to the transcription context containing audio data and
 *           other relevant information for VAD processing.
 * @param last_vad_state The last known state of the VAD, used to maintain
 *                       continuity in segmentation.
 * @return The updated VAD state after processing the current audio data.
 */
vad_state vad_based_segmentation(transcription_context *gf, vad_state last_vad_state);
/**
 * @brief Segments audio data using a hybrid Voice Activity Detection (VAD) algorithm.
 *
 * This function processes the given transcription context to determine the current
 * VAD state based on the provided last VAD state. It is used to segment audio data
 * into speech and non-speech segments.
 *
 * @param gf Pointer to the transcription context containing audio data and other relevant information.
 * @param last_vad_state The last known VAD state, used as a reference for the current segmentation.
 * @return The current VAD state after processing the audio data.
 */
vad_state hybrid_vad_segmentation(transcription_context *gf, vad_state last_vad_state);
/**
 * @brief Initializes the Voice Activity Detection (VAD) system.
 *
 * This function sets up the VAD system using the provided transcription context
 * and the specified Silero VAD model file. It prepares the system to detect
 * voice activity in audio streams.
 *
 * @param gf Pointer to the transcription context structure.
 * @param silero_vad_model_file Path to the Silero VAD model file.
 */
void initialize_vad(transcription_context *gf, const char *silero_vad_model_file);

#endif // VAD_PROCESSING_H
