#pragma once
#include "LibLsp/JsonRpc/message.h"
#include "InitializeParams.h"
#include "lsServerCapabilities.h"
#include "LibLsp/JsonRpc/lsResponseMessage.h"
#include "LibLsp/JsonRpc/RequestInMessage.h"

/**
 * Information about the server
 *
 * @since 3.15.0
 */
struct ServerInfo
{
    // The name of the server as defined by the server.
    std::string name;
    // The server's version as defined by the server.
    optional<std::string> version;

    MAKE_SWAP_METHOD(ServerInfo, name, version);
};

MAKE_REFLECT_STRUCT(ServerInfo, name, version);

/**
 * The capabilities the language server provides.
 */
struct InitializeResult
{
    lsServerCapabilities capabilities;

    /**
     * Information about the server.
     *
     * @since 3.15.0
     */
    optional<ServerInfo> serverInfo;

    MAKE_SWAP_METHOD(InitializeResult, capabilities, serverInfo);
};

MAKE_REFLECT_STRUCT(InitializeResult, capabilities, serverInfo);

/**
 * The initialize request is sent as the first request from the client to
 * the server.
 *
 * If the server receives request or notification before the initialize request it should act as follows:
 *      - for a request the respond should be errored with code: -32001. The message can be picked by the server.
 *  - notifications should be dropped, except for the exit notification. This will allow the exit a server without an initialize request.
 *
 * Until the server has responded to the initialize request with an InitializeResult
 * the client must not sent any additional requests or notifications to the server.
 *
 * During the initialize request the server is allowed to sent the notifications window/showMessage,
 * window/logMessage and telemetry/event as well as the window/showMessageRequest request to the client.
 */

DEFINE_REQUEST_RESPONSE_TYPE(td_initialize, lsInitializeParams, InitializeResult, "initialize");
