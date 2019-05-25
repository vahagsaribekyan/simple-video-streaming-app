#define SERVER_CERT_FILE "./cert.pem"
#define SERVER_PRIVATE_KEY_FILE "./key.pem"
#define HOST "localhost"
#define PORT 8080

#include <iostream>
#include <chrono>
#include "../streamstore/streamstore.h"
#include "../streamparser/streamparser.h"

#include "app.h"

namespace Develandoo
{
    
App::App() 
    : _serverPtr(std::make_unique<httplib::SSLServer>(SERVER_CERT_FILE, SERVER_PRIVATE_KEY_FILE))
    , _streamStorePtr(std::make_unique<StreamStore>())
    , _streamParserPtr(std::make_unique<StreamParser>("http://qthttp.apple.com.edgesuite.net/1010qwoeiuryfg/sl.m3u8")) {
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

App::~App() = default;

void App::initRoutes() {
    // handle root to work fine with k8s
    _serverPtr->Get("/", [](const auto& req, auto& res) {
        res.set_content("Greeting from video streamer app", "text/plain");
    });

    _serverPtr->Post("/frames", [this](const auto& req, auto& res) {
        _streamStorePtr->setStrategy(StreamStoreType::ImageStreamStoreType);
        res.set_content("Ok", "text/plain");
    });

    _serverPtr->Post("/record", [this](const auto& req, auto& res) {
        _streamStorePtr->setStrategy(StreamStoreType::VideoStreamStoreType);
        res.set_content("Ok", "text/plain");
    });
}

int App::start() {
    return _serverPtr->is_valid() ? _serverPtr->listen(HOST, PORT) : -1;
}

}
