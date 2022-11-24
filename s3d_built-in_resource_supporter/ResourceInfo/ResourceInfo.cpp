#include "ResourceInfo.h"
#include "ResourceSection.h"
#include "ResourceParser.h"
#include "ResourceFormat.h"

namespace sip
{
	ResourceInfo::ResourceInfo() noexcept
		: sections_{}
		, footer_comment_{ U"" }
	{
	}

	ResourceInfo::~ResourceInfo() noexcept
	{
	}

	void ResourceInfo::clear() noexcept
	{
		sections_.clear();
		footer_comment_ = U"";
	}

	bool ResourceInfo::open(FilePathView path) noexcept
	{
		TextReader reader{ path };
		if (not reader)
		{
			Logger << path << U" : ファイルを開けませんでした。\n";
			return false;
		}
		clear();
		return ResourceParser::parse(reader.readAll(), this);
	}

	bool ResourceInfo::save(FilePathView path) noexcept
	{
		TextWriter writer{ path, TextEncoding::UTF8_NO_BOM };
		if (not writer)
		{
			Logger << path << U" : ファイルへの書き込み失敗。\n";
			return false;
		}
		writer.write(U"# include <Siv3D/Windows/Resource.hpp>\n\n");
		writer.write(ResourceFormat::format(this));
		return true;
	}

	bool ResourceInfo::registSection(StringView name) noexcept
	{
		ResourceSectionPtr section(new ResourceSection());
		section->setName(name);
		sections_.emplace_back(std::move(section));
		return true;
	}

	std::size_t ResourceInfo::getSectionSize() const noexcept
	{
		return sections_.size();
	}

	ResourceSection* ResourceInfo::getSection(std::size_t index) noexcept
	{
		if (index >= sections_.size())
		{
			Logger << index << U" : 配列の境界外を指定しました。\n";
			return nullptr;
		}
		return sections_[index].get();
	}

	const ResourceSections& ResourceInfo::getSections() const noexcept
	{
		return sections_;
	}

	const String& ResourceInfo::getFooterComment() const noexcept
	{
		return footer_comment_;
	}

	void ResourceInfo::setFooterComment(const String& comment) noexcept
	{
		footer_comment_ = comment;
	}
}
