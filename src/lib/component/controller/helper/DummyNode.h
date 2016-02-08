#ifndef DUMMY_NODE_H
#define DUMMY_NODE_H

#include "utility/math/Vector2.h"
#include "utility/types.h"
#include "utility/utility.h"

#include "data/graph/token_component/TokenComponentAccess.h"

class Node;

// temporary data structure for (visual) graph creation process
struct DummyNode
{
public:
	DummyNode()
		: visible(false)
		, childVisible(false)
		, topLevelAncestorId(0)
		, tokenId(0)
		, data(nullptr)
		, active(false)
		, connected(false)
		, expanded(false)
		, hasParent(true)
		, accessType(TokenComponentAccess::ACCESS_NONE)
		, invisibleSubNodeCount(0)
		, bundledNodeCount(0)
	{
	}

	bool isGraphNode() const
	{
		return data != nullptr;
	}

	bool isAccessNode() const
	{
		return accessType != TokenComponentAccess::ACCESS_NONE;
	}

	bool isExpandToggleNode() const
	{
		return !isGraphNode() && !isAccessNode() && !isBundleNode();
	}

	bool isBundleNode() const
	{
		return bundledNodes.size() > 0;
	}

	bool isExpanded() const
	{
		return expanded;
	}

	bool hasVisibleSubNode() const
	{
		for (const DummyNode& node : subNodes)
		{
			if (node.visible)
			{
				return true;
			}
		}

		return false;
	}

	bool hasActiveSubNode() const
	{
		if (active)
		{
			return true;
		}

		for (const DummyNode& node : subNodes)
		{
			if (node.hasActiveSubNode())
			{
				return true;
			}
		}

		return false;
	}

	size_t getActiveSubNodeCount() const
	{
		size_t count = 0;

		if (active)
		{
			count += 1;
		}

		for (const DummyNode& node : subNodes)
		{
			count += node.getActiveSubNodeCount();
		}

		return count;
	}

	bool hasConnectedSubNode() const
	{
		if (connected)
		{
			return true;
		}

		for (const DummyNode& node : subNodes)
		{
			if (node.hasConnectedSubNode())
			{
				return true;
			}
		}

		return false;
	}

	size_t getConnectedSubNodeCount() const
	{
		size_t count = 0;

		if (connected)
		{
			count += 1;
		}

		for (const DummyNode& node : subNodes)
		{
			count += node.getConnectedSubNodeCount();
		}

		return count;
	}

	std::vector<const DummyNode*> getConnectedSubNodes() const
	{
		std::vector<const DummyNode*> nodes;

		if (connected)
		{
			nodes.push_back(this);
		}

		for (const DummyNode& node : subNodes)
		{
			utility::append(nodes, node.getConnectedSubNodes());
		}

		return nodes;
	}

	std::vector<const DummyNode*> getAllBundledNodes() const
	{
		std::vector<const DummyNode*> nodes;
		for (const DummyNode& node : bundledNodes)
		{
			utility::append(nodes, node.getConnectedSubNodes());
		}
		return nodes;
	}

	size_t getBundledNodeCount() const
	{
		if (bundledNodeCount > 0)
		{
			return bundledNodeCount;
		}

		return bundledNodes.size();
	}

	Vec2i position;
	Vec2i size;

	bool visible;
	bool childVisible;

	Id topLevelAncestorId;
	Id tokenId;

	std::vector<DummyNode> subNodes;

	// GraphNode
	const Node* data;
	std::string name;

	bool active;
	bool connected;
	bool expanded;
	bool hasParent;

	// AccessNode
	TokenComponentAccess::AccessType accessType;

	// ExpandToggleNode
	size_t invisibleSubNodeCount;

	// BundleNode
	std::vector<DummyNode> bundledNodes;
	size_t bundledNodeCount;
};

#endif // DUMMY_NODE_H
