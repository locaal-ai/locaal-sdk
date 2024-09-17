#ifndef MODEL_DOWNLOADER_H
#define MODEL_DOWNLOADER_H

#include <string>

#include "model-downloader-types.h"

std::string find_model_folder(const ModelInfo &model_info);
std::string find_model_ext_file(const ModelInfo &model_info, const std::string &ext);

void download_model(const ModelInfo &model_info, download_finished_callback_t finished_callback,
		    download_progress_callback_t progress_callback,
		    download_error_callback_t error_callback);

#endif // MODEL_DOWNLOADER_H
