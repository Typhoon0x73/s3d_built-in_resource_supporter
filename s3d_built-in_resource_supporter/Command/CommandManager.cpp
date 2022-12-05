#include "CommandManager.h"

namespace sip
{
	/**
	* @brief コンストラクタ
	*/
	CommandManager::CommandManager()
		: m_ExecuteList{}
		, m_UndoList{}
		, m_FailedList{}
	{
	}

	/**
	* @brief デストラクタ
	*/
	CommandManager::~CommandManager()
	{
		clear();
	}

	/**
	* @brief コマンドの登録
	* @param command コマンドポインタ
	* @return true : 成功, false : 失敗
	*/
	bool CommandManager::regist(CommandPtr&& command)
	{
		if (!command->execute())
		{
			m_FailedList.emplace_back(std::move(command));
			while (m_FailedList.size() > m_Capacity)
			{
				m_FailedList.pop_front();
			}
			return false;
		}
		m_ExecuteList.emplace_back(std::move(command));
		m_UndoList.clear();
		while (m_ExecuteList.size() > m_Capacity)
		{
			m_ExecuteList.pop_front();
		}
		return true;
	}

	/**
	* @brief コマンド履歴の削除
	*/
	void CommandManager::clear()
	{
		m_ExecuteList.clear();
		m_UndoList.clear();
	}

	/**
	* @brief 元に戻す
	* @return true : 成功, false : 失敗
	*/
	bool CommandManager::undo()
	{
		if (m_ExecuteList.size() == 0)
		{
			return false;
		}
		auto command = std::move(m_ExecuteList.back());
		m_ExecuteList.pop_back();
		if (!command->undo())
		{
			m_FailedList.emplace_back(std::move(command));
			while (m_FailedList.size() > m_Capacity)
			{
				m_FailedList.pop_front();
			}
			return false;
		}
		m_UndoList.emplace_back(std::move(command));
		return true;
	}

	/**
	* @brief やり直し
	* @return true : 成功, false : 失敗
	*/
	bool CommandManager::redo()
	{
		if (m_UndoList.size() == 0)
		{
			return false;
		}
		auto command = std::move(m_UndoList.back());
		m_UndoList.pop_back();
		if (!command->redo())
		{
			m_FailedList.emplace_back(std::move(command));
			while (m_FailedList.size() > m_Capacity)
			{
				m_FailedList.pop_front();
			}
			return false;
		}
		m_ExecuteList.emplace_back(std::move(command));
		while (m_ExecuteList.size() > m_Capacity)
		{
			m_ExecuteList.pop_front();
		}
		return true;
	}

	/**
	* @brief 実行コマンドリストの取得
	* @return 実行コマンドリスト
	*/
	const std::list<CommandPtr>& CommandManager::executeList() const
	{
		return m_ExecuteList;
	}

	/**
	* @brief 元に戻すコマンドリストの取得
	* @return 元に戻すコマンドリスト
	*/
	const std::list<CommandPtr>& CommandManager::undoList() const
	{
		return m_UndoList;
	}

	/**
	* @brief 失敗コマンドリストの取得
	* @return 失敗コマンドリスト
	*/
	const std::list<CommandPtr>& CommandManager::failedList() const
	{
		return m_FailedList;
	}

	/**
	* @brief コマンド記憶最大数の再設定(def:50)
	* @param capacity コマンド記憶最大数
	*/
	void CommandManager::setExecuteCommandCapacity(std::size_t capacity)
	{
		m_Capacity = capacity;
		while (m_ExecuteList.size() > m_Capacity)
		{
			m_ExecuteList.pop_front();
		}
	}
}
