#include <memory>

namespace cv { 
    class Mat; 
}

namespace Develandoo {

class StreamStoreStrategy;
using StreamStoreStrategyPtr = std::unique_ptr<StreamStoreStrategy>;
using FramePtr = std::unique_ptr<cv::Mat>;

enum class StreamStoreType {
    VideoStreamStoreType = 0, 
    ImageStreamStoreType
};

class StreamStore {
public:
    StreamStore();
    ~StreamStore();

    void store(FramePtr&& framePtr);
    void setStrategy(StreamStoreType type);

private:
    StreamStoreStrategyPtr _strategyPtr;
};

}
