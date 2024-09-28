#ifndef TRANSLATION_UTILS_H
#define TRANSLATION_UTILS_H

#include "translation.h"

/**
 * @brief Starts the translation process using the specified translation model.
 * 
 * @param ctx A pointer to the translation context structure.
 * @param translation_model_index A reference to a string representing the index of the translation model to be used.
 */
void start_translation(struct translation_context *ctx, const std::string &translation_model_index);

#endif // TRANSLATION_UTILS_H
