/**
 * @file language_codes.h
 * @brief Header file for language code mappings used in translation modules.
 *
 * This file contains declarations for several maps that provide mappings
 * between language codes and their corresponding language names. These
 * mappings are used for various translation functionalities.
 *
 * @var language_codes
 * A map that provides a mapping from language codes to language names.
 *
 * @var language_codes_reverse
 * A map that provides a reverse mapping from language names to language codes.
 *
 * @var language_codes_from_whisper
 * A map that provides a mapping from Whisper language codes to standard language codes.
 *
 * @var language_codes_to_whisper
 * A map that provides a mapping from standard language codes to Whisper language codes.
 */
#ifndef LANGUAGE_CODES_H
#define LANGUAGE_CODES_H

#include <map>
#include <string>

extern std::map<std::string, std::string> language_codes;
extern std::map<std::string, std::string> language_codes_reverse;
extern std::map<std::string, std::string> language_codes_from_whisper;
extern std::map<std::string, std::string> language_codes_to_whisper;

#endif // LANGUAGE_CODES_H
