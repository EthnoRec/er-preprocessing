#ifndef IMAGE_H

#define IMAGE_H
#include <json/value.h>
#include "opencv2/highgui/highgui.hpp"
#include "Config.hpp"
class Point : public cv::Point2i {
    public:
        Point(Json::Value obj);
};

class Box : public cv::Rect {
    public:
        Box(Json::Value obj);
        void reference(Box ref);
};

class Image {
    public:
        std::string filename;
        Box eye_left, eye_right, bound;
        Image(Json::Value, Config&);
        Config &config;


        void preprocess();

    private:
        cv::Mat original, preprocessed;

        void load();
        void save();

};


#endif /* end of include guard: IMAGE_H */
