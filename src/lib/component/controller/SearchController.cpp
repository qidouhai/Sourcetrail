#include "component/controller/SearchController.h"

#include "component/view/SearchView.h"
#include "data/access/StorageAccess.h"

SearchController::SearchController(StorageAccess* storageAccess)
	: m_storageAccess(storageAccess)
{
}

SearchController::~SearchController()
{
}

void SearchController::handleMessage(MessageActivateAll* message)
{
	SearchMatch match = SearchMatch::createCommand(SearchMatch::COMMAND_ALL);
	getView()->setMatches(std::vector<SearchMatch>(1, match));
}

void SearchController::handleMessage(MessageActivateTokens* message)
{
	if (!message->keepContent() && !message->isFromSearch)
	{
		const std::vector<Id>& tokenIds =
			(message->originalTokenIds.size() > 0 ? message->originalTokenIds : message->tokenIds);

		getView()->setMatches(m_storageAccess->getSearchMatchesForTokenIds(tokenIds));
	}
}

void SearchController::handleMessage(MessageFind* message)
{
	getView()->setFocus();
}

void SearchController::handleMessage(MessageSearchAutocomplete* message)
{
	LOG_INFO("autocomplete string: \"" + message->query + "\"");
	getView()->setAutocompletionList(m_storageAccess->getAutocompletionMatches(message->query));
}

void SearchController::handleMessage(MessageShowErrors* message)
{
	SearchMatch match = SearchMatch::createCommand(SearchMatch::COMMAND_ERROR);
	getView()->setMatches(std::vector<SearchMatch>(1, match));
}

SearchView* SearchController::getView()
{
	return Controller::getView<SearchView>();
}
