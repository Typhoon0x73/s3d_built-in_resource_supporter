#include "XMLInfo.h"
#include "XMLAttribute.h"
#include "XMLElement.h"
#include "XMLData.h"

namespace sip
{
	XMLInfo::XMLInfo() noexcept
		: version_{ U"1.0" }
		, encoding_{ U"utf-8" }
		, root_{ std::make_shared<XMLData>() }
	{
	}

	XMLInfo::~XMLInfo() noexcept
	{
	}

	bool XMLInfo::save(FilePathView path) const noexcept
	{
		TextWriter writer(path);
		if (not writer)
		{
			Logger << path << U" : ファイルへの書き込みに失敗しました。\n";
			return false;
		}
		writer.write(format());
		return true;
	}

	void XMLInfo::clear() noexcept
	{
		root_->clear();
	}

	void XMLInfo::set(const s3d::XMLElement& element) noexcept
	{
		clear();
		setElement(root_.get(), element);
	}

	const String& XMLInfo::getVersion() const noexcept
	{
		return version_;
	}

	const String& XMLInfo::getEncoding() const noexcept
	{
		return encoding_;
	}

	const XMLData* XMLInfo::getRoot() const noexcept
	{
		return root_.get();
	}

	XMLData* XMLInfo::getRoot() noexcept
	{
		return root_.get();
	}

	void XMLInfo::setVersion(std::size_t mejor, std::size_t miner) noexcept
	{
		std::string str(std::to_string(mejor) + "." + std::to_string(miner));
		version_ = Unicode::FromUTF8(str);
	}

	void XMLInfo::setEncoding(StringView encoding) noexcept
	{
		encoding_ = encoding;
	}

	String XMLInfo::format() const noexcept
	{
		String fmt = U"<?xml version=\"" + version_ + U"\" encoding=\"" + encoding_ + U"\"?>\n";
		fmt += formatElement(root_.get());
		if (fmt[fmt.length() - 1] == U'\n')
		{
			fmt.pop_back();
		}
		return fmt;
	}

	void XMLInfo::setElement(XMLData* data, const s3d::XMLElement& element) noexcept
	{
		if (data == nullptr)
		{
			Logger << U"データが nullptr です。\n";
			return;
		}
		data->getElement()->setTag(element.name());
		data->getElement()->setValue(element.text());
		for (const auto& attribute : element.attributes())
		{
			data->getElement()->registAttribute(attribute.first, attribute.second);
		}
		for (auto e = element.firstChild(); e; e = e.nextSibling())
		{
			XMLDataPtr tmp = std::make_shared<XMLData>();
			setElement(tmp.get(), e);
			data->registChild(std::move(tmp));
		}
	}

	String XMLInfo::formatElement(const XMLData* data, std::size_t tab_count) const noexcept
	{
		String fmt = U"";
		bool exist_value = data->getElement()->getValue().compare(U"") != 0;
		bool exist_child = data->getChildren().size() > 0;
		if (data->getElement()->getTag().compare(U"ItemGroup") == 0
			&& !exist_value
			&& !exist_child
			&& data->getElement()->getAttributes().size() == 0)
		{
			return fmt;
		}
		for (std::size_t i = 0; i < tab_count; i++)
		{
			fmt += U"  ";
		}
		fmt += U"<" + data->getElement()->getTag();
		for (const auto& attrib : data->getElement()->getAttributes())
		{
			fmt += U" " + attrib->format();
		}
		bool is_precompile   = (data->getElement()->getTag().compare(U"PrecompiledHeader") == 0);
		bool is_import_group = (data->getElement()->getTag().compare(U"ImportGroup") == 0);
		bool is_return = (is_import_group || (is_precompile && !exist_value));
		// 値も子供もなければ〆
		if (!exist_child && !exist_value && !is_return)
		{
			fmt += U" />\n";
			return fmt;
		}
		// 子供がいる場合は改行して続ける || 特殊改行パターン
		else if (exist_child || is_return)
		{
			fmt += U">\n";
			if (exist_value)
			{
				fmt += data->getElement()->getValue() + U"\n";
			}
			for (const auto& it : data->getChildren())
			{
				fmt += formatElement(it.second.get(), tab_count + 1);
			}
			for (std::size_t i = 0; i < tab_count; i++)
			{
				fmt += U"  ";
			}
			fmt += U"</" + data->getElement()->getTag() + U">\n";
		}
		// 子供がいないけど値はある場合は続けて書く
		else if (exist_value)
		{
			fmt += U">" + data->getElement()->getValue() + U"</" + data->getElement()->getTag() + U">\n";
		}
		return fmt;
	}
}
