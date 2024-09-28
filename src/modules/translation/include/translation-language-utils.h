#ifndef TRANSLATION_LANGUAGE_UTILS_H
#define TRANSLATION_LANGUAGE_UTILS_H

#include <string>

/**
 * @brief Removes leading punctuation characters from the given text.
 *
 * This function takes a string and removes any punctuation characters
 * from the beginning of the string. It stops removing characters as soon
 * as it encounters a non-punctuation character.
 *
 * @param text The input string from which leading punctuation will be removed.
 * @return A new string with leading punctuation characters removed.
 */
std::string remove_start_punctuation(const std::string &text);

#endif // TRANSLATION_LANGUAGE_UTILS_H
