#include "FilterTreeView.h"
#include "stdafx.h"
#include "XMLInfo/EditFilters.h"
#include "XMLInfo/XMLData.h"
#include "XMLInfo/XMLElement.h"
#include "XMLInfo/XMLAttribute.h"

namespace sip
{
	FilterTreeView::FilterTreeView() noexcept
		: root_node_{ new FilterTreeNode() }
		, scroll_{ 0.0 }
		, render_target_{ 1280, 720, Palette::White }
		, area_{ 0, 0, 200, 200 }
		, draw_pos_{ 0, 300 }
	{
	}

	FilterTreeView::~FilterTreeView() noexcept
	{
	}

	void FilterTreeView::update() noexcept
	{
		if (area_.movedBy(draw_pos_).mouseOver())
		{
			scroll_ += Mouse::Wheel() * 8;
		}
	}

	void FilterTreeView::draw() noexcept
	{
		render_target_.clear(Palette::Gray);
		{
			ScopedRenderTarget2D target{ render_target_ };
			treeDraw(root_node_.get());
		}
		render_target_(area_).draw(draw_pos_);
	}

	void FilterTreeView::applyTree() noexcept
	{
		auto edit_filters = g_pGetBlackboard(EditFilters* const)->get("edit_filters");
		root_node_.reset(new FilterTreeNode());
		for (const auto& guid : edit_filters->getGUIDs())
		{
			FilterTreeNodePtr tree_node(new FilterTreeNode());
			auto tmp_path = guid.first.replaced(U'\\', U'/');
			tree_node->path_ = tmp_path;
			auto node = findNode(tmp_path.substr(0, tmp_path.lastIndexOf(U'/')), root_node_.get());
			if (node == nullptr)
			{
				node = root_node_.get();
			}
			node->nodes_.emplace_back(std::move(tree_node));
		}
		for (const auto& item_group : edit_filters->getItemGroups())
		{
			for (const auto& item : item_group.second->getChildren())
			{
				auto filter = item.second->findData(U"Filter");
				if (filter)
				{
					auto tmp_path = filter->getElement()->getValue().replaced(U'\\', U'/');
					auto node = findNode(tmp_path, root_node_.get());
					if (node)
					{
						FilterTreeItemPtr tree_item(new FilterTreeItem());
						tree_item->name_ = item.second->getElement()->findAttribute(U"Include")->getValue();
						node->items_.emplace_back(std::move(tree_item));
					}
				}
				else
				{
					FilterTreeItemPtr tree_item(new FilterTreeItem());
					tree_item->name_ = item.second->getElement()->findAttribute(U"Include")->getValue();
					root_node_->items_.emplace_back(std::move(tree_item));
				}
			}
		}
	}

	void FilterTreeView::treeDraw(FilterTreeNode* root) noexcept
	{
		static double tree_draw_offset_y = 0;
		static double tree_draw_offset_x = 0;
		FilterTreeNode* draw_tree = root;
		bool is_root = (root->path_.compare(U"")==0);
		if (is_root)
		{
			draw_tree = root_node_.get();
			root_node_->is_open_ = true;
			tree_draw_offset_y = -scroll_;
		}
		if (!is_root)
		{
			auto filter_name = FileSystem::BaseName(FileSystem::FullPath(draw_tree->path_));
			SimpleGUI::GetFont()(filter_name).draw(10 + tree_draw_offset_x, 10 + tree_draw_offset_y);
			tree_draw_offset_y += 30;
		}
		if (draw_tree->is_open_)
		{
			tree_draw_offset_x += 40;
			for (const auto& child : draw_tree->nodes_)
			{
				treeDraw(child.get());
			}
			for (const auto& item : draw_tree->items_)
			{
				auto file_name = FileSystem::FileName(FileSystem::FullPath(item->name_));
				SimpleGUI::GetFont()(file_name).draw(10 + tree_draw_offset_x, 10 + tree_draw_offset_y);
				tree_draw_offset_y += 30;
			}
			tree_draw_offset_x -= 40;
		}
	}

	FilterTreeNode* FilterTreeView::findNode(FilePathView path, FilterTreeNode* root) noexcept
	{
		if (path.compare(U"") == 0)
		{
			return nullptr;
		}
		if (root->path_.compare(path) == 0)
		{
			return root;
		}
		for (const auto& it : root->nodes_)
		{
			auto node = findNode(path, it.get());
			if (node == nullptr)
			{
				continue;
			}
			return node;
		}
		return nullptr;
	}
}
