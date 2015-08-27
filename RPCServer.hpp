#include <jsonrpccpp/server.h>
#include <jsonrpccpp/server/connectors/httpserver.h>
#include <string>
#include "Config.hpp"

using namespace jsonrpc;

class RPCServer : public AbstractServer<RPCServer> {
    public:
        RPCServer(HttpServer &server, Config &config);
        void preprocess(const Json::Value& request, Json::Value& response);
        void notifyServer(const Json::Value& request);
        Config &config;
};
