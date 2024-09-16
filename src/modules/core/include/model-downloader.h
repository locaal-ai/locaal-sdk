#ifndef MODEL_DOWNLOADER_H
#define MODEL_DOWNLOADER_H

#include <string>

#include "model-downloader-types.h"

std::string find_model_folder(const ModelInfo &model_info);
std::string find_model_bin_file(const ModelInfo &model_info);

#endif // MODEL_DOWNLOADER_H
