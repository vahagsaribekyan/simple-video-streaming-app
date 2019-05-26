#include "streamparser.h"
#include <opencv2/opencv.hpp>

namespace Develandoo
{

StreamParser::StreamParser(const std::string& url) 
    : _url(url)
    , _stoped(true) {}

void StreamParser::start() {
    if(_stoped) {
        _stoped = false;
        _task = std::thread(&StreamParser::job, this);
    }
}

void StreamParser::stop() {
    if(!_stoped) {
        _stoped = true;
        _task.join();
    }
}

void StreamParser::setFrameCallback(FrameCallback callback) {
    _callback = callback;
}

bool StreamParser::isStoped() const {
    return _stoped;
}
void StreamParser::job() {
    cv::VideoCapture cap(_url);
    if (!cap.isOpened()) {
        std::cout << "Could not open the input video" << std::endl;
        return;
    }

    cv::Mat frame;
    while (cap.read(frame) && !_stoped && _callback) {
        _callback(std::make_unique<cv::Mat>(frame));
    }
    
    cap.release();
    _stoped = true;
}

} // namespace Develandoo
