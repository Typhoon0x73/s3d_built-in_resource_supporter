#pragma once
#ifndef SIP_FILTER_TREE_VIEW_H_
#define SIP_FILTER_TREE_VIEW_H_

#include "XMLInfo/XMLParamDefine.h"
#include "ResourceInfo/ResourceDefine.h"
#include <Siv3D.hpp>

namespace sip
{
	struct FilterTreeItem;
	struct FilterTreeNode;
	using FilterTreeItemPtr = std::unique_ptr<FilterTreeItem>;
	using FilterTreeNodePtr = std::unique_ptr<FilterTreeNode>;

	struct FilterTreeItem
	{
		String name_;
	};

	struct FilterTreeNode
	{
		bool is_open_{ true };
		FilePath path_;
		std::vector<FilterTreeNodePtr> nodes_;
		std::vector<FilterTreeItemPtr> items_;
	};

	/// @brief 
	class FilterTreeView
	{
	public:

		/// @brief 
		explicit FilterTreeView() noexcept;

		/// @brief 
		~FilterTreeView() noexcept;

		/// @brief 
		void update() noexcept;

		/// @brief 
		void draw() noexcept;

		/// @brief 
		void applyTree() noexcept;

	private:

		/// @brief 
		/// @param root 
		void treeDraw(FilterTreeNode* root) noexcept;

		/// @brief 
		/// @param path 
		/// @param root 
		/// @return 
		FilterTreeNode* findNode(FilePathView path, FilterTreeNode* root) noexcept;

		/// @brief 
		FilterTreeNodePtr root_node_;

		/// @brief 
		double scroll_;

		/// @brief 
		RenderTexture render_target_;

		/// @brief 
		RectF area_;

		/// @brief 
		Vec2 draw_pos_;
	};
}

#endif // !SIP_FILTER_TREE_VIEW_H_
