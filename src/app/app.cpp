#include "app.h"

#include <iostream>
#include <chrono>
#include "../streamstore/streamstore.h"
#include "../streamparser/streamparser.h"
#include "../httpserver/basicauth/basicauth.h"

namespace Develandoo
{

namespace
{

const char* const SERVER_CERT_FILE        = "./cert.pem";
const char* const SERVER_PRIVATE_KEY_FILE = "./key.pem";
const char* const HOST                    = "localhost";
const int         PORT                    = 443;

}

App::App() 
    : _serverPtr(std::make_unique<httplib::SSLServer>(SERVER_CERT_FILE, SERVER_PRIVATE_KEY_FILE))
    , _streamStorePtr(std::make_unique<StreamStore>())
    , _streamParserPtr(std::make_unique<StreamParser>("./public/index.m3u8")) {
    if (_serverPtr->is_valid()) {
        initRoutes();

        _serverPtr->set_error_handler([](const auto& /*req*/, auto& res) {
            const char *fmt = "<p>Error Status: <span style='color:red;'>%d</span></p>";
            char buf[BUFSIZ];
            snprintf(buf, sizeof(buf), fmt, res.status);
            res.set_content(buf, "text/html");
        });

        _streamParserPtr->setFrameCallback([this](FramePtr&& frame) {
            _streamStorePtr->store(std::move(frame));
        });
    }
};

App::~App() {
    if(_streamParserPtr && !_streamParserPtr->isStoped()) {
        _streamParserPtr->stop();
    }
};

void App::initRoutes() {
    // handle root to work fine with k8s
    _serverPtr->Get("/", [](const auto& req, auto& res) {
        res.set_content("Greeting from video streamer app", "text/plain");
    });

    _serverPtr->Post("/frames", BasicAuth([this](const auto& req, auto& res) {
        std::cout << "Request received to path /frames" << std::endl;
        _streamParserPtr->stop();
        _streamStorePtr->setStrategy(StreamStoreType::ImageStreamStoreType);
        _streamParserPtr->start();

        res.set_content("Ok", "text/plain");
    }));

    _serverPtr->Post("/record", BasicAuth([this](const auto& req, auto& res) {
        std::cout << "Request received to path /record" << std::endl;
        _streamParserPtr->stop();
        _streamStorePtr->setStrategy(StreamStoreType::VideoStreamStoreType);
        _streamParserPtr->start();

        res.set_content("Ok", "text/plain");
    }));
}

int App::start() {
    if (_serverPtr->is_valid()) {
        std::cout << "Server is running on port: " << PORT << std::endl;
        return _serverPtr->listen(HOST, PORT);
    } else {
        std::cout << "Could not start the server" << std::endl;
        return -1;
    }
}

} // namespace Develandoo
