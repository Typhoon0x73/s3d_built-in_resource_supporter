#pragma once
#ifndef SIP_RESOURCE_DEFINE_H_
#define SIP_RESOURCE_DEFINE_H_

#include <Siv3D.hpp>

namespace sip
{
	class ResourceData;
	class ResourceTag;
	class ResourceSection;
	class ResourceInfo;
	using ResourceDataPtr    = std::unique_ptr<ResourceData>;
	using ResourceTagPtr     = std::unique_ptr<ResourceTag>;
	using ResourceSectionPtr = std::unique_ptr<ResourceSection>;
	using ResourceDatas      = std::vector<ResourceDataPtr>;
	using ResourceTags       = std::vector<ResourceTagPtr>;
	using ResourceSections   = std::vector<ResourceSectionPtr>;

	/// @brief 
	enum class ResourceDataType
	{
		Resource,
		DefineResource,
	};

	/// @brief ツール内リソース判別タグ
	enum class RcTag : int
	{
		block_comment,    // [/*.**/] とマッチする
		section,          // [//////////////////////////////////////////////////////] で挟まれている
		tag,              // [//\n//\t.*\n//\n] とマッチする
		def_resource,     // [DefineResource(.*)] とマッチする
		resource,         // [Resource(.*)] とマッチする
		identifier,       // [MYIDENTIFIER_X FILE .*] とマッチする
		note_def_resource,// [//DefineResource(.*)] とマッチする
		note_resource,    // [//Resource(.*)] とマッチする
		note_identifier,  // [//MYIDENTIFIER_X FILE .*] とマッチする

		rc_tag_count,
		unknown = -1,
	};
	/// @brief タグ最大数
	constexpr int RcTagCount = static_cast<int>(RcTag::rc_tag_count);

	/// @brief リソースタグ検索用接頭文字列
	constexpr const char32_t* const prefix_rc_tag_strs[] =
	{
		U"/*",
		U"\n//////////////////////////////////////////////////////\n//\n//\t",
		U"//\n//\t",
		U"\nDefineResource(",
		U"\nResource(",
		U"\nMYIDENTIFIER_",
		U"\n//DefineResource(",
		U"\n//Resource(",
		U"\n//MYIDENTIFIER_",
	};

	/// @brief リソースタグ検索用接尾文字列
	constexpr const char32_t* const suffix_rc_tag_strs[] =
	{
		U"*/",
		U"\n//\n//////////////////////////////////////////////////////\n",
		U"\n//\n",
		U")\n",
		U")\n",
		U"\"\n",
		U")\n",
		U")\n",
		U"\"\n",
	};
}

#endif // !SIP_RESOURCE_DEFINE_H_
