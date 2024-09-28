#ifndef WHISPER_MODEL_UTILS_H
#define WHISPER_MODEL_UTILS_H

#include "transcription-filter-data.h"

/**
 * @brief Updates the whisper model within the given transcription context.
 *
 * This function takes a pointer to a transcription_context structure and updates
 * the whisper model associated with it. The specific details of the update process
 * are determined by the implementation of this function.
 *
 * @param gf A pointer to the transcription_context structure that contains the
 *           whisper model to be updated.
 */
void update_whisper_model(struct transcription_context *gf);

#endif // WHISPER_MODEL_UTILS_H
