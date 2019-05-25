#include <memory>
#define CPPHTTPLIB_OPENSSL_SUPPORT
#include "../httpserver/server.h"

namespace Develandoo 
{

class StreamStore;
class StreamParser;

class App {
public:
    explicit App();
    ~App();
    int start();

private:
    void initRoutes();

private:
    std::unique_ptr<httplib::SSLServer> _serverPtr;
    std::unique_ptr<StreamStore> _streamStorePtr;
    std::unique_ptr<StreamParser> _streamParserPtr;
};

} // namespace Develandoo
