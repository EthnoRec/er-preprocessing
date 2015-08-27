#ifndef CONFIG_H

#define CONFIG_H

#include <string>
#include "opencv2/opencv.hpp"
class Config {
    public:
    std::string original_dir, preprocessed_dir;
    cv::Size size;
};


#endif /* end of include guard: CONFIG_H */
