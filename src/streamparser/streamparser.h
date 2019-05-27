#include <string>
#include <thread>
#include <mutex>
#include <functional>

namespace cv { 
    class Mat; 
}

namespace Develandoo
{

using FramePtr = std::unique_ptr<cv::Mat>;
using FrameCallback = std::function<void(FramePtr&&)>;

class StreamParser {
public:
    StreamParser(const std::string& url);

    void start();
    void stop();
    void setFrameCallback(FrameCallback callback);

    bool isStoped() const;
private:
    void job();

private:
    FrameCallback _callback;
    std::thread _task;
    std::string _url;
    bool _stoped;

    std::mutex _mutex;
};

} // namespace Develandoo
