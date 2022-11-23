#ifndef SIP_COMMANDMANAGER_H_
#define SIP_COMMANDMANAGER_H_

#include "ICommand.h"
#include <list>

namespace sip
{
	/**
	* @brief �R�}���h�Ǘ��N���X
	*/
	class CommandManager
	{
	public:

		/**
		* @brief �R���X�g���N�^
		*/
		explicit CommandManager();

		/**
		* @brief �f�X�g���N�^
		*/
		virtual ~CommandManager();

		/**
		* @brief �R�}���h�̓o�^
		* @param command �R�}���h�|�C���^
		* @return true : ����, false : ���s
		*/
		bool regist(CommandPtr&& command);

		/**
		* @brief �R�}���h�����̍폜
		*/
		void clear();

		/**
		* @brief ���ɖ߂�
		* @return true : ����, false : ���s
		*/
		bool undo();

		/**
		* @brief ��蒼��
		* @return true : ����, false : ���s
		*/
		bool redo();

		/**
		* @brief ���s�R�}���h���X�g�̎擾
		* @return ���s�R�}���h���X�g
		*/
		const std::list<CommandPtr>& executeList() const;

		/**
		* @brief ���ɖ߂��R�}���h���X�g�̎擾
		* @return ���ɖ߂��R�}���h���X�g
		*/
		const std::list<CommandPtr>& undoList() const;

		/**
		* @brief �R�}���h�L���ő吔�̍Đݒ�(def:50)
		* @param capacity �R�}���h�L���ő吔
		*/
		void setExecuteCommandCapacity(std::size_t capacity = 50);

	private:

		std::size_t           m_Capacity{ 50 }; //!< �R�}���h�L���ő吔
		std::list<CommandPtr> m_ExecuteList;    //!< �R�}���h���s���X�g
		std::list<CommandPtr> m_UndoList;       //!< �R�}���h���ɖ߂����X�g
	};

}

#endif // !SIP_COMMANDMANAGER_H_
