#ifndef MODEL_FIND_UTILS_H
#define MODEL_FIND_UTILS_H

#include <string>

#include "model-downloader-types.h"

/**
 * @brief Searches for a file by name within a specified folder.
 *
 * This function traverses the given folder path and attempts to locate a file
 * that matches the specified file name. If the file is found, the function
 * returns the full path to the file. If the file is not found, an empty string
 * is returned.
 *
 * @param folder_path The path to the folder where the search will be conducted.
 * @param file_name The name of the file to search for within the folder.
 * @return A string containing the full path to the found file, or an empty string if the file is not found.
 */
std::string find_file_in_folder_by_name(const std::string &folder_path,
					const std::string &file_name);
/**
 * @brief Finds a file with a specific extension in a given folder.
 *
 * This function searches for a file with the specified extension within the
 * provided folder path. If a file with the given extension is found, the
 * function returns the file name.
 *
 * @param model_local_folder_path The path to the folder where the search will be conducted.
 * @param ext The file extension to search for (e.g., ".txt", ".jpg").
 * @return A string containing the name of the file with the specified extension if found,
 *         otherwise an empty string.
 */
std::string find_ext_file_in_folder(const std::string &model_local_folder_path,
				    const std::string &ext);
/**
 * @brief Finds a file in a specified folder that matches a given regular expression.
 *
 * This function searches through the files in the provided folder path and returns the name
 * of the first file that matches the specified regular expression pattern.
 *
 * @param folder_path The path to the folder where the search will be conducted.
 * @param file_name_regex The regular expression pattern to match the file names against.
 * @return A string containing the name of the first file that matches the regular expression.
 *         If no file matches, an empty string is returned.
 */
std::string find_file_in_folder_by_regex_expression(const std::string &folder_path,
						    const std::string &file_name_regex);

#endif // MODEL_FIND_UTILS_H
