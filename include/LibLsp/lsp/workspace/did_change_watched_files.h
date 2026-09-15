#pragma once

#include "LibLsp/JsonRpc/NotificationInMessage.h"
#include "LibLsp/lsp/lsDocumentUri.h"
enum class lsFileChangeType
{
    Created = 1,
    Changed = 2,
    Deleted = 3,
};

#ifdef _WIN32
MAKE_REFLECT_TYPE_PROXY(lsFileChangeType);
#else
//#pragma clang diagnostic push
//#pragma clang diagnostic ignored "-Wunused-function"
MAKE_REFLECT_TYPE_PROXY(lsFileChangeType);
//#pragma clang diagnostic pop
#endif

/**
 * A file the server wants change events for. `kind` is a WatchKind bitmask
 * (Create = 1 | Change = 2 | Delete = 4); omitted defaults to 7 (all three).
 */
struct lsFileSystemWatcher
{
    std::string globPattern;
    optional<int> kind;

    MAKE_SWAP_METHOD(lsFileSystemWatcher, globPattern, kind);
};
MAKE_REFLECT_STRUCT(lsFileSystemWatcher, globPattern, kind);

/**
 * Watchers for `workspace/didChangeWatchedFiles`: carried both as the static
 * initialize capability (workspace.didChangeWatchedFiles) and inside a
 * `client/registerCapability` registration's registerOptions.
 */
struct lsDidChangeWatchedFilesOptions
{
    std::vector<lsFileSystemWatcher> watchers;

    MAKE_SWAP_METHOD(lsDidChangeWatchedFilesOptions, watchers);
};
MAKE_REFLECT_STRUCT(lsDidChangeWatchedFilesOptions, watchers);

/**
 * An event describing a file change.
 */
struct lsFileEvent
{
    lsDocumentUri uri;
    lsFileChangeType type;

    MAKE_SWAP_METHOD(lsFileEvent, uri, type)
};
MAKE_REFLECT_STRUCT(lsFileEvent, uri, type);

struct lsDidChangeWatchedFilesParams
{
    std::vector<lsFileEvent> changes;
    MAKE_SWAP_METHOD(lsDidChangeWatchedFilesParams, changes);
};
MAKE_REFLECT_STRUCT(lsDidChangeWatchedFilesParams, changes);

/**
  * The workspace/didChangeWorkspaceFolders notification is sent from the client
  * to the server to inform the server about workspace folder configuration changes.
  * The notification is sent by default if both ServerCapabilities/workspaceFolders
  * and ClientCapabilities/workspace/workspaceFolders are true; or if the server has
  * registered to receive this notification it first.
  */

DEFINE_NOTIFICATION_TYPE(
    Notify_WorkspaceDidChangeWatchedFiles, lsDidChangeWatchedFilesParams, "workspace/didChangeWatchedFiles"
);
