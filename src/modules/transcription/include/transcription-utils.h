#ifndef TRANSCRIPTION_UTILS_H
#define TRANSCRIPTION_UTILS_H

#include <string>
#include <vector>
#include <chrono>
#include <algorithm>
#include <cctype>

/**
 * @brief Fixes and validates a UTF-8 encoded string.
 *
 * This function takes a UTF-8 encoded string and ensures that it is properly
 * encoded. It attempts to fix any encoding errors and returns a valid UTF-8
 * string.
 *
 * @param str The input string that needs to be validated and fixed.
 * @return A valid UTF-8 encoded string.
 */
std::string fix_utf8(const std::string &str);

/**
 * @brief Removes leading and trailing non-alphabetic characters from a string.
 *
 * This function takes a string as input and returns a new string with all 
 * leading and trailing characters that are not alphabetic (a-z, A-Z) removed.
 *
 * @param str The input string from which to remove leading and trailing non-alphabetic characters.
 * @return A new string with leading and trailing non-alphabetic characters removed.
 */
std::string remove_leading_trailing_nonalpha(const std::string &str);

/**
 * @brief Splits a given string into a vector of substrings based on a specified delimiter.
 * 
 * This function takes a string and a delimiter character, and splits the string into 
 * substrings wherever the delimiter character is found. The resulting substrings are 
 * stored in a vector of strings.
 * 
 * @param string The input string to be split.
 * @param delimiter The character used to split the string.
 * @return std::vector<std::string> A vector containing the substrings obtained by splitting the input string.
 */
std::vector<std::string> split(const std::string &string, char delimiter);

/**
 * @brief Gets the current time in milliseconds.
 * 
 * This function returns the current time since the epoch (1970-01-01 00:00:00 UTC) in milliseconds.
 * It can be used for measuring time intervals or timestamps.
 * 
 * @return uint64_t The current time in milliseconds.
 */
inline uint64_t now_ms()
{
	return std::chrono::duration_cast<std::chrono::milliseconds>(
		       std::chrono::system_clock::now().time_since_epoch())
		.count();
}

/**
 * @brief Retrieves the current time in nanoseconds.
 * 
 * This function returns the current time since the epoch in nanoseconds.
 * It can be used for high-resolution time measurements.
 * 
 * @return uint64_t The current time in nanoseconds.
 */
inline uint64_t now_ns()
{
	return std::chrono::duration_cast<std::chrono::nanoseconds>(
		       std::chrono::system_clock::now().time_since_epoch())
		.count();
}

/**
 * @brief Splits a given string into a vector of words.
 *
 * This function takes a string as input and splits it into individual words
 * based on whitespace or other delimiters. The resulting words are stored
 * in a vector of strings.
 *
 * @param str_copy The input string to be split into words.
 * @return A vector of strings, each containing a word from the input string.
 */
std::vector<std::string> split_words(const std::string &str_copy);

/**
 * @brief Trims leading and trailing whitespace from a string-like object.
 *
 * This function removes all leading and trailing whitespace characters from
 * the given string-like object. The function works with any type that supports
 * the necessary operations, such as std::string or std::wstring.
 *
 * @tparam StringLike The type of the string-like object.
 * @param str The string-like object to be trimmed.
 * @return A copy of the input string-like object with leading and trailing
 *         whitespace removed.
 */
template<typename StringLike> StringLike trim(const StringLike &str)
{
	using CharType = typename StringLike::value_type;

	StringLike str_copy = str;
	str_copy.erase(str_copy.begin(),
		       std::find_if(str_copy.begin(), str_copy.end(), [](CharType ch) {
			       return !std::isspace(static_cast<unsigned char>(ch));
		       }));
	str_copy.erase(std::find_if(str_copy.rbegin(), str_copy.rend(),
				    [](CharType ch) {
					    return !std::isspace(static_cast<unsigned char>(ch));
				    })
			       .base(),
		       str_copy.end());
	return str_copy;
}

#endif // TRANSCRIPTION_UTILS_H
