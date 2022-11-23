#ifndef SIP_COMMANDMANAGER_H_
#define SIP_COMMANDMANAGER_H_

#include "ICommand.h"
#include <list>

namespace sip
{
	/**
	* @brief コマンド管理クラス
	*/
	class CommandManager
	{
	public:

		/**
		* @brief コンストラクタ
		*/
		explicit CommandManager();

		/**
		* @brief デストラクタ
		*/
		virtual ~CommandManager();

		/**
		* @brief コマンドの登録
		* @param command コマンドポインタ
		* @return true : 成功, false : 失敗
		*/
		bool regist(CommandPtr&& command);

		/**
		* @brief コマンド履歴の削除
		*/
		void clear();

		/**
		* @brief 元に戻す
		* @return true : 成功, false : 失敗
		*/
		bool undo();

		/**
		* @brief やり直し
		* @return true : 成功, false : 失敗
		*/
		bool redo();

		/**
		* @brief 実行コマンドリストの取得
		* @return 実行コマンドリスト
		*/
		const std::list<CommandPtr>& executeList() const;

		/**
		* @brief 元に戻すコマンドリストの取得
		* @return 元に戻すコマンドリスト
		*/
		const std::list<CommandPtr>& undoList() const;

		/**
		* @brief コマンド記憶最大数の再設定(def:50)
		* @param capacity コマンド記憶最大数
		*/
		void setExecuteCommandCapacity(std::size_t capacity = 50);

	private:

		std::size_t           m_Capacity{ 50 }; //!< コマンド記憶最大数
		std::list<CommandPtr> m_ExecuteList;    //!< コマンド実行リスト
		std::list<CommandPtr> m_UndoList;       //!< コマンド元に戻すリスト
	};

}

#endif // !SIP_COMMANDMANAGER_H_
