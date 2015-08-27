#include <jsonrpccpp/server.h>
#include <jsonrpccpp/server/connectors/httpserver.h>
#include <string>
#include <vector>
#include "RPCServer.hpp"
#include "Image.hpp"

using namespace jsonrpc;

RPCServer::RPCServer(HttpServer &server, Config &config): 
    AbstractServer<RPCServer>(server),
    config(config) {
    this->bindAndAddMethod(jsonrpc::Procedure("preprocess", jsonrpc::PARAMS_BY_NAME, jsonrpc::JSON_STRING, "images",jsonrpc::JSON_ARRAY, "size", jsonrpc::JSON_ARRAY, NULL), &RPCServer::preprocess);
    this->bindAndAddNotification(Procedure("notifyServer", PARAMS_BY_NAME, NULL), &RPCServer::notifyServer);
}

void RPCServer::preprocess(const Json::Value& request, Json::Value& response)
{
    // [{"filename": "img1.jpg", eye_left: {origin: [0,0], extent: [10,10]...} }]
    Json::Value jimages = request["images"];
    Json::Value jsize = request["size"];
    config.size = cv::Size(jsize[0].asInt(), jsize[1].asInt());
    std::cout << "Size: " << config.size << std::endl;


    // Parse images
    for (Json::Value jimage: jimages) {
        Image image(jimage, config);
        image.preprocess();
    }
    response = "ok";
}

//notification
void RPCServer::notifyServer(const Json::Value& request)
{
    (void)request;
    std::cout << "server received some Notification" << std::endl;
}
