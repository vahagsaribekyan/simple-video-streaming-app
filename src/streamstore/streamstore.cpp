#include "streamstore.h"
#include <opencv2/opencv.hpp>

namespace Develandoo {

StreamStore::StreamStore() 
    : _strategyPtr(nullptr) {}

StreamStore::~StreamStore() = default;

void StreamStore::store(FramePtr&& framePtr) {
    if(_strategyPtr) {
        _strategyPtr->store(std::move(framePtr));
    }
}

void StreamStore::setStrategy(StreamStoreType type) {
    _strategyPtr = streamStoreFactory(type);
}

class StreamStoreStrategy {
public:
    virtual ~StreamStoreStrategy();

    virtual void store(FramePtr&& frame) = 0;
};

class VideoStreamStoreStrategy : public StreamStoreStrategy {
public:
    ~VideoStreamStoreStrategy();
    void store(FramePtr&& frame) override;
private:
    cv::VideoWriter _writer;
};

class ImageStreamStoreStrategy : public StreamStoreStrategy  {
public:
    void store(FramePtr&& frame) override;
};

std::unique_ptr<StreamStoreStrategy> streamStoreFactory(StreamStoreType type);

StreamStoreStrategy::~StreamStoreStrategy() = default;

void VideoStreamStoreStrategy::store(FramePtr&& frame) {
    if(!_writer.isOpened()) {
        _writer.open("output.mp4", 0x00000021, 30, frame->size());
    }

    _writer.write(frame);
}

void VideoStreamStoreStrategy::~VideoStreamStoreStrategy() {
    if(_writer.isOpened()) {
        _writer.release();
    }
}

void ImageStreamStoreStrategy::store(FramePtr&& frame) {
    static int i = 0;
    std::stringstream ss;
    ss << "frame"
       << i++
       << ".jpg";
    cv::imwrite(ss.str(), frame);
}

std::unique_ptr<StreamStoreStrategy> streamStoreFactory(StreamStoreType type) {
    switch (type)
    {
    case StreamStoreType::VideoStreamStoreType:
        return std::make_unique<VideoStreamStoreStrategy>();
    case StreamStoreType::ImageStreamStoreType:
        return std::make_unique<ImageStreamStoreStrategy>();
    }
} 

}
