#ifndef SIP_ICOMMAND_H_
#define SIP_ICOMMAND_H_

#include <memory>
#include <string_view>

namespace sip
{
	/**
	* @brief コマンド抽象クラス
	*/
	class ICommand
	{
	public:

		/**
		* @brief デストラクタ
		*/
		virtual ~ICommand() noexcept = default;

		/**
		* @brief 実行
		* @return true : 成功, false : 失敗
		*/
		virtual bool execute() noexcept = 0;

		/**
		* @brief 元に戻す
		* @return true : 成功, false : 失敗
		*/
		virtual bool undo() noexcept = 0;

		/**
		* @brief やり直し
		* @return true : 成功, false : 失敗
		*/
		virtual bool redo() noexcept = 0;

		/**
		* @brief ログ文字列の取得
		* @return ログ文字列
		*/
		virtual std::string log() const noexcept = 0;
	};

	// コマンドポインタ
	using CommandPtr = std::unique_ptr<ICommand>;
}

#endif // !SIP_ICOMMAND_H_
