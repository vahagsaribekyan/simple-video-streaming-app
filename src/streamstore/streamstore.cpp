#include "streamstore.h"
#include <opencv2/opencv.hpp>

namespace Develandoo {

class StreamStoreStrategy {
public:
    virtual ~StreamStoreStrategy();

    virtual void store(FramePtr&& frame) = 0;
};

StreamStoreStrategy::~StreamStoreStrategy() = default;

class ImageStreamStoreStrategy : public StreamStoreStrategy  {
public:
    void store(FramePtr&& frame) override;
};

void ImageStreamStoreStrategy::store(FramePtr&& frame) {
    static int i = 0;
    std::stringstream ss;
    ss << "frame"
       << i++
       << ".jpg";
    cv::imwrite(ss.str(), *frame);
}

class VideoStreamStoreStrategy : public StreamStoreStrategy {
public:
    ~VideoStreamStoreStrategy();
    void store(FramePtr&& frame) override;
private:
    cv::VideoWriter _writer;
};

void VideoStreamStoreStrategy::store(FramePtr&& frame) {
    if(!_writer.isOpened()) {
        _writer.open("output.mp4", 0x00000021, 30, frame->size());
    }

    _writer.write(*frame);
}

VideoStreamStoreStrategy::~VideoStreamStoreStrategy() {
    if(_writer.isOpened()) {
        _writer.release();
    }
}

StreamStore::StreamStore() 
    : _strategyPtr(nullptr) {}

StreamStore::~StreamStore() = default;

void StreamStore::store(FramePtr&& framePtr) {
    if(_strategyPtr) {
        _strategyPtr->store(std::move(framePtr));
    }
}

void StreamStore::setStrategy(StreamStoreType type) {
    switch (type)
    {
    case StreamStoreType::VideoStreamStoreType:
        _strategyPtr = std::make_unique<VideoStreamStoreStrategy>();
        break;
    case StreamStoreType::ImageStreamStoreType:
        _strategyPtr = std::make_unique<ImageStreamStoreStrategy>();
        break;
    }
}

}
