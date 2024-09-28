#ifndef MODEL_DOWNLOADER_H
#define MODEL_DOWNLOADER_H

#include <string>

#include "model-downloader-types.h"

/**
 * @brief Finds the folder path for a given model.
 *
 * This function takes a ModelInfo object as input and returns the corresponding
 * folder path where the model is stored.
 *
 * @param model_info A reference to a ModelInfo object containing details about the model.
 * @return A string representing the path to the model's folder.
 */
std::string find_model_folder(const ModelInfo &model_info);

/**
 * @brief Finds a model file with the specified extension.
 * 
 * This function searches for a model file that matches the given extension
 * within the context of the provided model information.
 * 
 * @param model_info A reference to a ModelInfo object containing details about the model.
 * @param ext A string representing the file extension to search for.
 * @return A string containing the path to the model file with the specified extension.
 */
std::string find_model_ext_file(const ModelInfo &model_info, const std::string &ext);

/**
 * @brief Downloads a model based on the provided model information.
 * 
 * This function initiates the download of a model using the details specified
 * in the `model_info` parameter. It provides callbacks to handle the completion,
 * progress updates, and errors that may occur during the download process.
 * 
 * @param model_info A reference to a `ModelInfo` object containing the details of the model to be downloaded.
 * @param finished_callback A callback function to be called when the download is finished.
 * @param progress_callback A callback function to be called to report the progress of the download.
 * @param error_callback A callback function to be called if an error occurs during the download.
 */
void download_model(const ModelInfo &model_info, download_finished_callback_t finished_callback,
		    download_progress_callback_t progress_callback,
		    download_error_callback_t error_callback);

#endif // MODEL_DOWNLOADER_H
