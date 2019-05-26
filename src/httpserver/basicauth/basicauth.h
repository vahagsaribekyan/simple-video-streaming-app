#include <functional>
#include <string>

namespace httplib {
    struct Request; 
    struct Response; 
}

namespace Develandoo
{

using Callback = std::function<void(const httplib::Request&, httplib::Response&)>;

class BasicAuth {
public:
    BasicAuth(Callback callback);
    void operator() (const httplib::Request& req, httplib::Response& res);

private:
    bool isAuthorized(const httplib::Request& req);
    bool validateUserPwd(const std::string&);

private:
    Callback _callback;
};

} // namespace Develandoo
