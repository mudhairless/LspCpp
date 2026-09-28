#pragma once

#include <map>
#include <utility>

#include "LibLsp/JsonRpc/NotificationInMessage.h"
#include "LibLsp/JsonRpc/RequestInMessage.h"
#include "LibLsp/JsonRpc/lsResponseMessage.h"
#include "LibLsp/lsp/lsAny.h"
#include "LibLsp/lsp/lsCodeAction.h"
#include "LibLsp/lsp/lsCommand.h"
#include "LibLsp/lsp/lsFormattingOptions.h"
#include "LibLsp/lsp/lsRange.h"
#include "LibLsp/lsp/lsTextEdit.h"
#include "LibLsp/lsp/lsTextDocumentIdentifier.h"
#include "LibLsp/lsp/lsp_diagnostic.h"
#include "LibLsp/lsp/symbol.h"

struct RelativePattern
{
    lsp::Any baseUri;
    std::string pattern;

    MAKE_SWAP_METHOD(RelativePattern, baseUri, pattern)
};
MAKE_REFLECT_STRUCT(RelativePattern, baseUri, pattern)

struct NotebookDocumentFilter
{
    optional<std::string> notebookType;
    optional<std::string> scheme;
    optional<std::string> pattern;

    MAKE_SWAP_METHOD(NotebookDocumentFilter, notebookType, scheme, pattern)
};
MAKE_REFLECT_STRUCT(NotebookDocumentFilter, notebookType, scheme, pattern)

struct NotebookDocumentIdentifier
{
    lsDocumentUri uri;

    MAKE_SWAP_METHOD(NotebookDocumentIdentifier, uri)
};
MAKE_REFLECT_STRUCT(NotebookDocumentIdentifier, uri)

struct VersionedNotebookDocumentIdentifier : NotebookDocumentIdentifier
{
    optional<int> version;

    MAKE_SWAP_METHOD(VersionedNotebookDocumentIdentifier, uri, version)
};
MAKE_REFLECT_STRUCT(VersionedNotebookDocumentIdentifier, uri, version)

struct TextDocumentContentParams
{
    lsDocumentUri uri;

    MAKE_SWAP_METHOD(TextDocumentContentParams, uri)
};
MAKE_REFLECT_STRUCT(TextDocumentContentParams, uri)

struct TextDocumentContentResult
{
    std::string text;

    MAKE_SWAP_METHOD(TextDocumentContentResult, text)
};
MAKE_REFLECT_STRUCT(TextDocumentContentResult, text)

struct TextDocumentContentOptions
{
    std::vector<std::string> schemes;

    MAKE_SWAP_METHOD(TextDocumentContentOptions, schemes)
};
MAKE_REFLECT_STRUCT(TextDocumentContentOptions, schemes)

struct TextDocumentContentClientCapabilities
{
    optional<bool> dynamicRegistration;

    MAKE_SWAP_METHOD(TextDocumentContentClientCapabilities, dynamicRegistration)
};
MAKE_REFLECT_STRUCT(TextDocumentContentClientCapabilities, dynamicRegistration)

DEFINE_REQUEST_RESPONSE_TYPE(
    workspace_textDocumentContent, TextDocumentContentParams, TextDocumentContentResult, "workspace/textDocumentContent"
)
DEFINE_REQUEST_RESPONSE_TYPE(
    workspace_textDocumentContent_refresh, JsonNull, JsonNull, "workspace/textDocumentContent/refresh"
)

struct DocumentDiagnosticParams
{
    lsTextDocumentIdentifier textDocument;
    optional<std::string> identifier;
    optional<std::string> previousResultId;

    MAKE_SWAP_METHOD(DocumentDiagnosticParams, textDocument, identifier, previousResultId)
};
MAKE_REFLECT_STRUCT(DocumentDiagnosticParams, textDocument, identifier, previousResultId)

struct PreviousResultId
{
    lsDocumentUri uri;
    std::string value;

    MAKE_SWAP_METHOD(PreviousResultId, uri, value)
};
MAKE_REFLECT_STRUCT(PreviousResultId, uri, value)

struct WorkspaceDiagnosticParams
{
    optional<std::string> identifier;
    optional<std::vector<PreviousResultId>> previousResultIds;

    MAKE_SWAP_METHOD(WorkspaceDiagnosticParams, identifier, previousResultIds)
};
MAKE_REFLECT_STRUCT(WorkspaceDiagnosticParams, identifier, previousResultIds)

// The value type of `relatedDocuments` (LSP 3.17): one related document's
// report, which is a full report computed fresh when the server answers (the
// closure a pull can surface is one level — a related report is never itself
// related — so the value type stays non-recursive; a value that recursed
// would need a pointer in the map's value slot).
struct RelatedDocumentDiagnosticReport
{
    std::string kind = "full";
    optional<std::string> resultId;
    std::vector<lsDiagnostic> items;

    MAKE_SWAP_METHOD(RelatedDocumentDiagnosticReport, kind, resultId, items)
};
MAKE_REFLECT_STRUCT(RelatedDocumentDiagnosticReport, kind, resultId, items)

struct FullDocumentDiagnosticReport
{
    std::string kind = "full";
    optional<std::string> resultId;
    std::vector<lsDiagnostic> items;

    // Diagnostics of related documents (LSP 3.17): a map of document URI to
    // that document's report. Present when the document's diagnostics depend
    // on other documents (an include graph), so a pull on `a.bas` can carry
    // `b.bi`'s problems alongside its own.
    optional<std::map<std::string, RelatedDocumentDiagnosticReport>>
        relatedDocuments;

    MAKE_SWAP_METHOD(FullDocumentDiagnosticReport, kind, resultId, items, relatedDocuments)
};
MAKE_REFLECT_STRUCT(
    FullDocumentDiagnosticReport, kind, resultId, items, relatedDocuments
)

// Report kind used when the client's `previousResultId` still names the
// current result: the report carries only the kind and the (unchanged)
// resultId, and the client keeps its cached diagnostic list.
struct UnchangedDocumentDiagnosticReport
{
    std::string kind = "unchanged";
    optional<std::string> resultId;

    MAKE_SWAP_METHOD(UnchangedDocumentDiagnosticReport, kind, resultId)
};
MAKE_REFLECT_STRUCT(UnchangedDocumentDiagnosticReport, kind, resultId)

// The `DocumentDiagnosticReport` union (`full` | `unchanged`), discriminated
// on the wire by the `kind` member. The protocol reflects it into the
// response of `textDocument/diagnostic` and the per-document report carries in
// the `workspace/diagnostic` answer.
struct DocumentDiagnosticReport
{
    typedef std::pair<optional<FullDocumentDiagnosticReport>,
                      optional<UnchangedDocumentDiagnosticReport>>
        Either;
};

extern void Reflect(Reader& visitor, DocumentDiagnosticReport::Either& value);
extern void Reflect(Writer& visitor, DocumentDiagnosticReport::Either& value);

struct WorkspaceDocumentDiagnosticReport
{
    lsDocumentUri uri;
    optional<int> version;
    DocumentDiagnosticReport::Either report;

    MAKE_SWAP_METHOD(WorkspaceDocumentDiagnosticReport, uri, version, report)
};
MAKE_REFLECT_STRUCT(WorkspaceDocumentDiagnosticReport, uri, version, report)

struct WorkspaceDiagnosticReport
{
    std::vector<WorkspaceDocumentDiagnosticReport> items;

    MAKE_SWAP_METHOD(WorkspaceDiagnosticReport, items)
};
MAKE_REFLECT_STRUCT(WorkspaceDiagnosticReport, items)

struct DiagnosticOptions
{
    optional<std::string> identifier;
    optional<bool> interFileDependencies;
    optional<bool> workspaceDiagnostics;

    MAKE_SWAP_METHOD(DiagnosticOptions, identifier, interFileDependencies, workspaceDiagnostics)
};
MAKE_REFLECT_STRUCT(DiagnosticOptions, identifier, interFileDependencies, workspaceDiagnostics)

struct DiagnosticClientCapabilities
{
    optional<bool> dynamicRegistration;
    optional<bool> relatedDocumentSupport;

    MAKE_SWAP_METHOD(DiagnosticClientCapabilities, dynamicRegistration, relatedDocumentSupport)
};
MAKE_REFLECT_STRUCT(DiagnosticClientCapabilities, dynamicRegistration, relatedDocumentSupport)

struct WorkspaceDiagnosticClientCapabilities
{
    optional<bool> refreshSupport;

    MAKE_SWAP_METHOD(WorkspaceDiagnosticClientCapabilities, refreshSupport)
};
MAKE_REFLECT_STRUCT(WorkspaceDiagnosticClientCapabilities, refreshSupport)

DEFINE_REQUEST_RESPONSE_TYPE(
    td_diagnostic, DocumentDiagnosticParams, DocumentDiagnosticReport::Either,
    "textDocument/diagnostic"
)
DEFINE_REQUEST_RESPONSE_TYPE(
    workspace_diagnostic, WorkspaceDiagnosticParams, WorkspaceDiagnosticReport, "workspace/diagnostic"
)

struct InlineValueContext
{
    int frameId = 0;
    lsRange stoppedLocation;

    MAKE_SWAP_METHOD(InlineValueContext, frameId, stoppedLocation)
};
MAKE_REFLECT_STRUCT(InlineValueContext, frameId, stoppedLocation)

struct InlineValueParams
{
    lsTextDocumentIdentifier textDocument;
    lsRange range;
    InlineValueContext context;

    MAKE_SWAP_METHOD(InlineValueParams, textDocument, range, context)
};
MAKE_REFLECT_STRUCT(InlineValueParams, textDocument, range, context)

struct InlineValue
{
    lsRange range;
    optional<std::string> text;
    optional<std::string> variableName;
    optional<bool> caseSensitiveLookup;
    optional<std::string> expression;

    MAKE_SWAP_METHOD(InlineValue, range, text, variableName, caseSensitiveLookup, expression)
};
MAKE_REFLECT_STRUCT(InlineValue, range, text, variableName, caseSensitiveLookup, expression)

struct InlineValueOptions
{
    optional<bool> workDoneProgress;

    MAKE_SWAP_METHOD(InlineValueOptions, workDoneProgress)
};
MAKE_REFLECT_STRUCT(InlineValueOptions, workDoneProgress)

struct InlineValueClientCapabilities
{
    optional<bool> dynamicRegistration;

    MAKE_SWAP_METHOD(InlineValueClientCapabilities, dynamicRegistration)
};
MAKE_REFLECT_STRUCT(InlineValueClientCapabilities, dynamicRegistration)

DEFINE_REQUEST_RESPONSE_TYPE(td_inlineValue, InlineValueParams, std::vector<InlineValue>, "textDocument/inlineValue")
DEFINE_REQUEST_RESPONSE_TYPE(workspace_inlineValue_refresh, JsonNull, JsonNull, "workspace/inlineValue/refresh")

struct SelectedCompletionInfo
{
    lsRange range;
    std::string text;

    MAKE_SWAP_METHOD(SelectedCompletionInfo, range, text)
};
MAKE_REFLECT_STRUCT(SelectedCompletionInfo, range, text)

struct InlineCompletionContext
{
    int triggerKind = 1;
    optional<SelectedCompletionInfo> selectedCompletionInfo;

    MAKE_SWAP_METHOD(InlineCompletionContext, triggerKind, selectedCompletionInfo)
};
MAKE_REFLECT_STRUCT(InlineCompletionContext, triggerKind, selectedCompletionInfo)

struct InlineCompletionParams
{
    lsTextDocumentIdentifier textDocument;
    lsPosition position;
    InlineCompletionContext context;

    MAKE_SWAP_METHOD(InlineCompletionParams, textDocument, position, context)
};
MAKE_REFLECT_STRUCT(InlineCompletionParams, textDocument, position, context)

struct InlineCompletionItem
{
    std::string insertText;
    optional<lsRange> range;
    optional<lsCommandWithAny> command;

    MAKE_SWAP_METHOD(InlineCompletionItem, insertText, range, command)
};
MAKE_REFLECT_STRUCT(InlineCompletionItem, insertText, range, command)

struct InlineCompletionList
{
    std::vector<InlineCompletionItem> items;

    MAKE_SWAP_METHOD(InlineCompletionList, items)
};
MAKE_REFLECT_STRUCT(InlineCompletionList, items)

struct InlineCompletionOptions
{
    optional<bool> workDoneProgress;

    MAKE_SWAP_METHOD(InlineCompletionOptions, workDoneProgress)
};
MAKE_REFLECT_STRUCT(InlineCompletionOptions, workDoneProgress)

struct InlineCompletionClientCapabilities
{
    optional<bool> dynamicRegistration;

    MAKE_SWAP_METHOD(InlineCompletionClientCapabilities, dynamicRegistration)
};
MAKE_REFLECT_STRUCT(InlineCompletionClientCapabilities, dynamicRegistration)

DEFINE_REQUEST_RESPONSE_TYPE(
    td_inlineCompletion, InlineCompletionParams, optional<InlineCompletionList>, "textDocument/inlineCompletion"
)

struct DocumentRangesFormattingParams
{
    lsTextDocumentIdentifier textDocument;
    std::vector<lsRange> ranges;
    lsFormattingOptions options;

    MAKE_SWAP_METHOD(DocumentRangesFormattingParams, textDocument, ranges, options)
};
MAKE_REFLECT_STRUCT(DocumentRangesFormattingParams, textDocument, ranges, options)

DEFINE_REQUEST_RESPONSE_TYPE(
    td_rangesFormatting, DocumentRangesFormattingParams, std::vector<lsTextEdit>, "textDocument/rangesFormatting"
)
DEFINE_REQUEST_RESPONSE_TYPE(workspace_foldingRange_refresh, JsonNull, JsonNull, "workspace/foldingRange/refresh")
DEFINE_REQUEST_RESPONSE_TYPE(
    workspace_semanticTokens_refresh, JsonNull, JsonNull, "workspace/semanticTokens/refresh"
)
DEFINE_REQUEST_RESPONSE_TYPE(workspace_inlayHint_refresh, JsonNull, JsonNull, "workspace/inlayHint/refresh")
DEFINE_REQUEST_RESPONSE_TYPE(workspace_diagnostic_refresh, JsonNull, JsonNull, "workspace/diagnostic/refresh")

struct DocumentRangeFormattingClientCapabilities
{
    optional<bool> dynamicRegistration;
    optional<bool> rangesSupport;

    MAKE_SWAP_METHOD(DocumentRangeFormattingClientCapabilities, dynamicRegistration, rangesSupport)
};
MAKE_REFLECT_STRUCT(DocumentRangeFormattingClientCapabilities, dynamicRegistration, rangesSupport)

struct DocumentRangeFormattingOptions
{
    optional<bool> workDoneProgress;
    optional<bool> rangesSupport;

    MAKE_SWAP_METHOD(DocumentRangeFormattingOptions, workDoneProgress, rangesSupport)
};
MAKE_REFLECT_STRUCT(DocumentRangeFormattingOptions, workDoneProgress, rangesSupport)

struct SnippetTextEdit
{
    lsRange range;
    std::string snippet;
    optional<lsChangeAnnotationIdentifier> annotationId;

    MAKE_SWAP_METHOD(SnippetTextEdit, range, snippet, annotationId)
};
MAKE_REFLECT_STRUCT(SnippetTextEdit, range, snippet, annotationId)

struct CodeLensResolveSupport
{
    std::vector<std::string> properties;

    MAKE_SWAP_METHOD(CodeLensResolveSupport, properties)
};
MAKE_REFLECT_STRUCT(CodeLensResolveSupport, properties)

struct CodeLensClientCapabilities
{
    optional<bool> dynamicRegistration;
    optional<CodeLensResolveSupport> resolveSupport;

    MAKE_SWAP_METHOD(CodeLensClientCapabilities, dynamicRegistration, resolveSupport)
};
MAKE_REFLECT_STRUCT(CodeLensClientCapabilities, dynamicRegistration, resolveSupport)

struct CodeActionKindDocumentation
{
    std::string kind;
    lsCommandWithAny command;

    MAKE_SWAP_METHOD(CodeActionKindDocumentation, kind, command)
};
MAKE_REFLECT_STRUCT(CodeActionKindDocumentation, kind, command)

DEFINE_REQUEST_RESPONSE_TYPE(codeAction_resolve, CodeAction, CodeAction, "codeAction/resolve")
DEFINE_REQUEST_RESPONSE_TYPE(workspaceSymbol_resolve, lsSymbolInformation, lsSymbolInformation, "workspaceSymbol/resolve")
