#include "ResourceParser.h"
#include "ResourceInfo.h"
#include "ResourceSection.h"
#include "ResourceTag.h"
#include "ResourceFactory.h"

namespace sip
{
	namespace detail
	{
		/// @brief 文字列の抜き出し
		/// 
		/// @details strのindex位置から[prefix : suffix]をoutへ出力し、suffix + 1の位置を返す
		/// 	     見つからなければString::nposを返す
		/// @param str 
		/// @param index 
		/// @param prefix 
		/// @param suffix 
		/// @param out 
		/// @return 
		std::size_t extraction(const String& str, std::size_t index, StringView prefix, StringView suffix, String& out)
		{
			const std::size_t prefix_len = prefix.length();
			auto pre_pos = str.indexOf(prefix, index);
			if (pre_pos == String::npos)
			{
				return String::npos;
			}
			auto suf_pos = str.indexOf(suffix, pre_pos + 1);
			if (suf_pos == String::npos)
			{
				return String::npos;
			}
			out = str
				.substr(pre_pos, suf_pos - pre_pos)
				.substr(prefix_len);
			return (suf_pos + 1);
		}

		RcTag findFirstTag(const String& data, std::size_t index)
		{
			RcTag tag = RcTag::unknown;
			std::size_t diff = SIZE_MAX;
			for (int i = 0; i < RcTagCount; i++)
			{
				auto pos = data.indexOf(prefix_rc_tag_strs[i], index);
				if (pos == String::npos)
				{
					continue;
				}
				if (pos < diff)
				{
					diff = pos;
					tag = static_cast<RcTag>(i);
				}
			}
			return tag;
		}
		bool rcInfoBlockCommentParse(const String& data, ResourceInfo* pInfo)
		{
			if (pInfo == nullptr)
			{
				Logger << U"リソース情報がありません。\n";
				return false;
			}
			pInfo->setFooterComment(data);
			return true;
		}

		bool rcInfoSectionParse(const String& data, ResourceInfo* pInfo)
		{
			if (pInfo == nullptr)
			{
				Logger << U"リソース情報がありません。\n";
				return false;
			}
			return pInfo->registSection(data);
		}

		bool rcInfoTagParse(const String& data, ResourceInfo* pInfo)
		{
			if (pInfo == nullptr)
			{
				Logger << U"リソース情報がありません。\n";
				return false;
			}
			const auto& size = pInfo->getSectionSize();
			auto section_ = pInfo->getSection(size - 1);
			if (section_ == nullptr)
			{
				Logger << (size - 1) << U" : セクションが存在しません。\n";
				return false;
			}
			return section_->registTag(data);
		}

		bool rcInfoDefineResourceParseBase(const String& data, ResourceInfo* pInfo, bool enable)
		{
			if (pInfo == nullptr)
			{
				Logger << U"リソース情報がありません。\n";
				return false;
			}
			const auto& section_count = pInfo->getSectionSize();
			auto section_ = pInfo->getSection(section_count - 1);
			if (section_ == nullptr)
			{
				Logger << (section_count - 1) << U" : セクションが存在しません。\n";
				return false;
			}
			std::uint64_t id;
			String        define;
			String        path;
			const auto& id_comma_pos  = data.indexOf(U',');
			const auto& def_comma_pos = data.indexOf(U',', id_comma_pos + 1);
			id     = ParseOr<std::uint64_t>(data.substr(0, id_comma_pos), 0);
			define = data.substr(id_comma_pos + 1, def_comma_pos - (id_comma_pos + 1)).trim();
			path   = data.substr(def_comma_pos + 1).trim();

			const auto& tag_count = section_->getTagSize();
			ResourceTag* tag = nullptr;
			if (tag_count < 1)
			{
				tag = section_->getTag();
			}
			else
			{
				tag = section_->getTag(tag_count - 1);
			}
			if (tag == nullptr)
			{
				Logger << (tag_count - 1) << U" : タグが存在しません。\n";
				return false;
			}
			tag->regist(ResourceFactory::Create(id, define, path, enable));
			return true;
		}

		bool rcInfoResourceParseBase(const String& data, ResourceInfo* pInfo, bool enable)
		{
			if (pInfo == nullptr)
			{
				Logger << U"リソース情報がありません。\n";
				return false;
			}
			const auto& section_count = pInfo->getSectionSize();
			auto section_ = pInfo->getSection(section_count - 1);
			if (section_ == nullptr)
			{
				Logger << (section_count - 1) << U" : セクションが存在しません。\n";
				return false;
			}
			const auto& tag_count = section_->getTagSize();
			ResourceTag* tag = nullptr;
			if (tag_count < 1)
			{
				tag = section_->getTag();
			}
			else
			{
				tag = section_->getTag(tag_count - 1);
			}
			if (tag == nullptr)
			{
				Logger << (tag_count - 1) << U" : タグが存在しません。\n";
				return false;
			}
			tag->regist(ResourceFactory::Create(data, enable));
			return true;
		}

		bool rcInfoDefineResourceParse(const String& data, ResourceInfo* pInfo)
		{
			return rcInfoDefineResourceParseBase(data, pInfo, true);
		}

		bool rcInfoResourceParse(const String& data, ResourceInfo* pInfo)
		{
			return rcInfoResourceParseBase(data, pInfo, true);
		}

		bool rcInfoNoteDefineResourceParse(const String& data, ResourceInfo* pInfo)
		{
			return rcInfoDefineResourceParseBase(data, pInfo, false);
		}

		bool rcInfoNoteResourceParse(const String& data, ResourceInfo* pInfo)
		{
			return rcInfoResourceParseBase(data, pInfo, false);
		}

		bool rcInfoIdentifierParse(const String& data, ResourceInfo* pInfo)
		{
			auto index = data.indexOf(U'"');
			return rcInfoResourceParse(data.substr(index + 1), pInfo);
		}

		bool rcInfoNoteIdentifierParse(const String& data, ResourceInfo* pInfo)
		{
			auto index = data.indexOf(U'"');
			return rcInfoNoteResourceParse(data.substr(index + 1), pInfo);
		}

		using ParseFunc = std::function<bool(const String&, ResourceInfo*)>;
		ParseFunc parseFuncTable[] = {
			rcInfoBlockCommentParse,
			rcInfoSectionParse,
			rcInfoTagParse,
			rcInfoDefineResourceParse,
			rcInfoResourceParse,
			rcInfoIdentifierParse,
			rcInfoNoteDefineResourceParse,
			rcInfoNoteResourceParse,
			rcInfoNoteIdentifierParse,
		};
	}

	bool ResourceParser::parse(const String& text, ResourceInfo* rc) noexcept
	{
		using namespace detail;
		for (std::size_t index = 0; index < text.length();)
		{
			RcTag tag = findFirstTag(text, index);
			if (tag == RcTag::unknown)
			{
				return true;
			}
			String data;
			const int tag_no = static_cast<int>(tag);
			const auto& pre_str = prefix_rc_tag_strs[tag_no];
			const auto& suf_str = suffix_rc_tag_strs[tag_no];
			index = extraction(text, index, pre_str, suf_str, data);
			if (index == String::npos)
			{
				Logger << tag_no << U"パースに失敗しました。\n";
				return false;
			}
			if (!detail::parseFuncTable[tag_no](data, rc))
			{
				Logger << tag_no << U" : パース関数で失敗しました。\n";
				return false;
			}
		}
		return true;
	}
}

