#include "basicauth.h"
#include "../server.h"

#include <iostream>
#include <algorithm>

#include <boost/archive/iterators/base64_from_binary.hpp>
#include <boost/archive/iterators/binary_from_base64.hpp>
#include <boost/archive/iterators/transform_width.hpp>
#include <boost/archive/iterators/insert_linebreaks.hpp>
#include <boost/archive/iterators/remove_whitespace.hpp>
#include <boost/algorithm/string/predicate.hpp>

namespace Develandoo 
{

namespace {

std::string decodeBase64(const std::string t) {
    std::string token = t;
    using namespace boost::archive::iterators;
    typedef transform_width<binary_from_base64<remove_whitespace
        <std::string::const_iterator> >, 8, 6> ItBinaryT;

    try {
        // If the input isn't a multiple of 4, pad with =
        size_t num_pad_chars((4 - token.size() % 4) % 4);
        token.append(num_pad_chars, '=');

        size_t pad_chars(std::count(token.begin(), token.end(), '='));
        std::replace(token.begin(), token.end(), '=', 'A');
        std::string output(ItBinaryT(token.begin()), ItBinaryT(token.end()));
        output.erase(output.end() - pad_chars, output.end());
        return output;
    } catch (std::exception const&) {
        std::cout << "Could not decode token";
        return "";
    }
}

} // unnamed namespace

BasicAuth::BasicAuth(Callback callback)
    : _callback(callback) {}

void BasicAuth::operator() (const httplib::Request& req, httplib::Response& res) {
    std::cout << "checking authorization..." << std::endl;
    if (isAuthorized(req)) {
        _callback(req, res);
    } else {
        res.status = 401;
        res.set_content("Unauthorized request", "text/plain");
    }
}

bool BasicAuth::isAuthorized(const httplib::Request& req) {
    std::string authHeader = req.headers.find("Authorization")->second;
    if(authHeader.empty() || !boost::starts_with(authHeader, "Basic ")) {
        return false;
    }

    std::string token = authHeader.substr(6 /* - Basic */);
    std::string decodedToken = decodeBase64(token);
    return validateUserPwd(decodedToken);
}

bool BasicAuth::validateUserPwd(const std::string& token) {
    const auto idx = token.find(":");
    if(idx != std::string::npos) {
        // tmp solution to check if username=user and pwd=password
        return token.substr(0, idx) == "user" && token.substr(idx + 1) == "password";
    } else {
        return false;
    }
}

} // namespace Develandoo