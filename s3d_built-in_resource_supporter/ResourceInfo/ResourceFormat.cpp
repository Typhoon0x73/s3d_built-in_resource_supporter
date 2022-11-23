#include "ResourceFormat.h"
#include "ResourceInfo.h"
#include "ResourceSection.h"
#include "ResourceTag.h"
#include "ResourceData.h"
#include "DefineResourceData.h"

namespace
{
	static std::int64_t s_identifier_num = 1;
}

namespace sip
{
	namespace detail
	{
		String erasePreEnter(const String& str)
		{
			if (str[0] == '\n')
			{
				return str.substr(1);
			}
			return str;
		}

		String formatBlockComment(const String& comment)
		{
			String fmt = U"";
			fmt += prefix_rc_tag_strs[static_cast<int>(RcTag::block_comment)];
			fmt += comment;
			fmt += suffix_rc_tag_strs[static_cast<int>(RcTag::block_comment)];
			return erasePreEnter(fmt);
		}

		String formatSection(const ResourceSection* section)
		{
			String fmt = U"";
			if (section == nullptr)
			{
				return fmt;
			}
			fmt += prefix_rc_tag_strs[static_cast<int>(RcTag::section)];
			fmt += section->getName();
			fmt += suffix_rc_tag_strs[static_cast<int>(RcTag::section)];
			return erasePreEnter(fmt);
		}

		String formatTag(const ResourceTag* tag)
		{
			String fmt = U"";
			if (tag == nullptr)
			{
				return fmt;
			}
			String name = tag->getName();
			if (name.compare(U"") == 0)
			{
				return fmt;
			}
			fmt += prefix_rc_tag_strs[static_cast<int>(RcTag::tag)];
			fmt += name;
			fmt += suffix_rc_tag_strs[static_cast<int>(RcTag::tag)];
			return erasePreEnter(fmt);
		}

		String formatResource(const ResourceData* rc)
		{
			String fmt = U"";
			if (rc == nullptr)
			{
				return fmt;
			}

			if (rc->getPath().indexOf(U' ') != String::npos)
			{
				fmt += prefix_rc_tag_strs[static_cast<int>(RcTag::identifier)];
				fmt += U"{} FILE \"{}"_fmt(s_identifier_num++, rc->format());
				fmt += suffix_rc_tag_strs[static_cast<int>(RcTag::identifier)];
				return (rc->isEnable() ? U"" : U"//") + erasePreEnter(fmt);
			}

			RcTag tag;
			switch (rc->getResourceDataType())
			{
			case ResourceDataType::Resource:
				tag = RcTag::resource;
				break;
			case ResourceDataType::DefineResource:
				tag = RcTag::def_resource;
				break;
			default:
				return fmt;
			}
			fmt += prefix_rc_tag_strs[static_cast<int>(tag)];
			fmt += rc->format();
			fmt += suffix_rc_tag_strs[static_cast<int>(tag)];
			return (rc->isEnable() ? U"" : U"//") + erasePreEnter(fmt);
		}
	}

	String sip::ResourceFormat::format(ResourceInfo* pInfo) noexcept
	{
		if (pInfo == nullptr)
		{
			return U"";
		}
		s_identifier_num = 1;
		String fmt = U"";
		const auto& sections = pInfo->getSections();
		const auto& footer = pInfo->getFooterComment();

		for (const auto& section : sections)
		{
			fmt += detail::formatSection(section.get());
			for (const auto& rc : section->getTag()->getResources())
			{
				fmt += detail::formatResource(rc.get());
			}
			fmt += U"\n";
			for (const auto& tag : section->getTags())
			{
				fmt += detail::formatTag(tag.get());
				for (const auto& rc : tag->getResources())
				{
					fmt += detail::formatResource(rc.get());
				}
				fmt += U"\n";
			}
		}
		fmt += U"\n\n";
		fmt += detail::formatBlockComment(footer);
		fmt += U"\n";
		return fmt;
	}
}

