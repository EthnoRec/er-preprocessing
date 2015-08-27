#include <iostream>
#include "Image.hpp"
#include <math.h>

Box::Box(Json::Value obj): cv::Rect(Point(obj["origin"]), Point(obj["extent"])) {}

void Box::reference(Box ref) {
    x = x - ref.x; 
    y = y - ref.y; 
    std::cout << tl() << " vs " << (tl()-ref.tl()) << std::endl;
}

Point::Point(Json::Value obj) {
    x = obj[0].asInt();
    y = obj[1].asInt();
}

Image::Image(Json::Value obj, Config &config): 
    eye_left(obj["eye_left"]), 
    eye_right(obj["eye_right"]), 
    bound(obj["bound"]),
    config(config) {

    filename = obj["filename"].asString();
}

void Image::load() {
    std::cout << config.original_dir << std::endl;
    cv::Mat full = cv::imread(config.original_dir+"/"+filename, 
            CV_LOAD_IMAGE_GRAYSCALE);

    // crop
    original = full(bound);
    eye_left.reference(bound);
    eye_right.reference(bound);
    bound.reference(bound);
}

double vector_cos(cv::Point a, cv::Point b) {
    return a.dot(b)/(cv::norm(a)*cv::norm(b));
}

void Image::preprocess() {
    std::cout << "Preprocess " << config.original_dir+"/"+filename << std::endl;
    std::cout << "eye_left: " << eye_left << " | eye_right: " << eye_right << std::endl;
    load();
    preprocessed = original;
    cv::Point avg_eye_left(config.size.width/2 - config.size.width/5, 
            config.size.height*0.3);
    cv::Point avg_eye_right(config.size.width/2 + config.size.width/5, 
            config.size.height*0.3);


    // Estimated eye point locations
    cv::Point eye_left_p(0.5*(eye_left.tl().x+eye_left.br().x), 
            0.5*(eye_left.tl().y+eye_left.br().y));
    cv::Point eye_right_p(0.5*(eye_right.tl().x+eye_right.br().x), 
            0.5*(eye_right.tl().y+eye_right.br().y));

    // Vectors
    cv::Point avg_eye_v = avg_eye_right - avg_eye_left;
    cv::Point eye_v = eye_right_p - eye_left_p;

    // Convert the return value of acos (radians) to degrees
    double angd = acos(vector_cos(avg_eye_v, eye_v))*180.0/M_PI;
    double scale_f = cv::norm(avg_eye_v)/cv::norm(eye_v);

    cv::Point eye_midpoint = 0.5*(eye_left_p+eye_right_p);
    cv::Point avg_eye_midpoint = 0.5*(avg_eye_left+avg_eye_right);

    std::cout << "scale: " << scale_f << " | angle: " << angd << " | about: " << eye_midpoint << std::endl;
    cv::Matx<double, 2, 3> rot = cv::getRotationMatrix2D(eye_midpoint, -angd, 
            scale_f);

    // Translate the image so that the eyes appear at avg_eye_* locations
    cv::Point2d translation = avg_eye_midpoint - eye_midpoint;
    rot.col(2) = rot.col(2) + cv::Vec2d(translation);

    cv::warpAffine(original, preprocessed, rot, config.size);
    save();
}

void Image::save() {
    cv::imwrite(config.preprocessed_dir+"/"+filename, preprocessed);
}
