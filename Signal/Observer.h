#pragma once

namespace Standard
{
	namespace Signal
	{
		namespace Observer
		{
			////////////////////////////////////////////////////////////////////////////////
			/// @class      CSetting
			/// @brief      �M���̎�M��ҋ@����ݒ�N���X
			////////////////////////////////////////////////////////////////////////////////
			class CSetting
			{
			public:
				////////////////////////////////////////////////////////////////////////////////
				/// @brief			�R���X�g���N�^
				////////////////////////////////////////////////////////////////////////////////
				CSetting()
				{
					Timeout = 1000;
				}

				////////////////////////////////////////////////////////////////////////////////
				/// @brief			�R�s�[������Z�q
				/// @return			���N���X
				////////////////////////////////////////////////////////////////////////////////
				CSetting& operator = (const CSetting& object) noexcept
				{
					// �X�V
					Update(object);

					return *this;
				}

				////////////////////////////////////////////////////////////////////////////////
				/// @brief			�f�X�g���N�^
				////////////////////////////////////////////////////////////////////////////////
				virtual ~CSetting()
				{

				}

			protected:
				////////////////////////////////////////////////////////////////////////////////
				/// @brief			���N���X���X�V
				/// @param[in]		object	���N���X
				////////////////////////////////////////////////////////////////////////////////
				void Update(const CSetting& object)
				{
					this->Timeout = object.Timeout;
				}

			public:
				//! �^�C���A�E�g[ms]
				int Timeout;
			};

			////////////////////////////////////////////////////////////////////////////////
			/// @class      CObserver
			/// @brief      �M���̎�M��ҋ@����e���v���[�g�̃N���X
			/// @param[in]	CArgument	��M������̃N���X(�e���v���[�g�Ŏw��)
			////////////////////////////////////////////////////////////////////////////////
			template <typename CArgument>
			class CTemplate
			{
			public:
				////////////////////////////////////////////////////////////////////////////////
				/// @brief			�R���X�g���N�^
				////////////////////////////////////////////////////////////////////////////////
				CTemplate()
				{

				}

				////////////////////////////////////////////////////////////////////////////////
				/// @brief			�R�s�[������Z�q
				////////////////////////////////////////////////////////////////////////////////
				CTemplate& operator = (const CTemplate& object)
				{
					// �X�V
					Update(object);

					return *this;
				}

				////////////////////////////////////////////////////////////////////////////////
				/// @brief			�f�X�g���N�^
				////////////////////////////////////////////////////////////////////////////////
				virtual ~CTemplate()
				{

				}

			public:
				////////////////////////////////////////////////////////////////////////////////
				/// @brief			�ʒm���Ăяo�����
				/// @param[in]		object	��M�f�[�^�̃N���X
				////////////////////////////////////////////////////////////////////////////////
				void OnCalled(const CArgument& object) noexcept
				{
					try
					{
						// ��M�f�[�^�̃N���X���X�V
						SetArgument(object);

						// ��M�f�[�^�̏���
						ReceiveDerived(object);
					}
					catch (const std::exception& e)
					{
						// ��O�̏��� �� ��O�̃N���X���X�V
						SetException(Exception::CodeUnknown, e.what());
					}

					// �C�x���g�������N��
					m_occurrence.notify_one();
				}

				////////////////////////////////////////////////////////////////////////////////
				/// @brief			��M�f�[�^�̃N���X���擾
				/// @return			��M�f�[�^�̃N���X
				////////////////////////////////////////////////////////////////////////////////
				CArgument GetArgument()
				{
					CArgument ret;

					// �r������
					std::unique_lock<std::mutex> lock(m_asyncArgument);

					ret = m_argument;

					return ret;
				}

				////////////////////////////////////////////////////////////////////////////////
				/// @brief			�Ăяo����҂�
				////////////////////////////////////////////////////////////////////////////////
				void Wait() throw(...)
				{
					CSetting setting = Setting;

					// �r������
					std::unique_lock<std::mutex> lock(m_asyncOccurrence);

					// �^�C���A�E�g�́����m�F
					if (setting.Timeout < 0)
					{
						// �^�C���A�E�g�Ȃ� �� �C�x���g�����҂�
						m_occurrence.wait(lock);
					}
					else
					{
						// �^�C���A�E�g���� �� �C�x���g�����҂�
						if (m_occurrence.wait_for(lock, std::chrono::milliseconds(setting.Timeout)) == std::cv_status::timeout)
						{
							std::stringstream postscript;

							postscript << "�^�C���A�E�g:" << setting.Timeout << "[ms]";

							// ��O�𔭖C
							throw std::exception(postscript.str().c_str());
						}

						// ��O�̃N���X
						Exception::CObject exception;

						// ��O�̃N���X���擾
						exception = GetException();

						// ��O�������������m�F
						if (exception.ErrorCode != Exception::CodeSuccess)
						{
							// ��O�𔭖C
							throw std::exception(exception.Message.c_str());
						}
					}
				}

			protected:
				////////////////////////////////////////////////////////////////////////////////
				/// @brief			���N���X���X�V
				/// @param[in]		object	���N���X
				////////////////////////////////////////////////////////////////////////////////
				void Update(const CTemplate& object)
				{
					this->m_argument = object.m_argument;
				}

				////////////////////////////////////////////////////////////////////////////////
				/// @brief			��M�f�[�^�̏���
				/// @detail			���h����ŋL�q
				/// @param[in]		object	��M�f�[�^�̃N���X
				////////////////////////////////////////////////////////////////////////////////
				virtual void ReceiveDerived(const CArgument& object)
				{

				}

			private:
				////////////////////////////////////////////////////////////////////////////////
				/// @brief			��O�̃N���X���X�V
				/// @param[in]		code	��O�R�[�h
				/// @param[in]		message	���b�Z�[�W
				////////////////////////////////////////////////////////////////////////////////
				void SetException(Exception::EnumCode code, const std::string& message)
				{
					// �r������
					std::unique_lock<std::mutex> lock(m_asyncException);

					m_exception.ErrorCode = code;
					m_exception.Message = message;
				}

				////////////////////////////////////////////////////////////////////////////////
				/// @brief			��O�̃N���X���擾
				/// @return			��O�̃N���X
				////////////////////////////////////////////////////////////////////////////////
				Exception::CObject GetException()
				{
					Exception::CObject ret;

					// �r������
					std::unique_lock<std::mutex> lock(m_asyncException);

					ret = m_exception;

					return ret;
				}

				////////////////////////////////////////////////////////////////////////////////
				/// @brief			��M�f�[�^�̃N���X���X�V
				/// @param[in]		object	��M�f�[�^�̃N���X
				////////////////////////////////////////////////////////////////////////////////
				void SetArgument(const CArgument& object)
				{
					// �r������
					std::unique_lock<std::mutex> lock(m_asyncArgument);

					m_argument = object;
				}

			public:
				//! �M���̎�M��ҋ@����ݒ�N���X
				CSetting Setting;

			private:
				//! �r������N���X(��O�̃N���X�p)
				std::mutex m_asyncException;

				//! ��O�̃N���X
				Exception::CObject m_exception;

				//! �r������N���X(�V�O�i�������p)
				std::mutex m_asyncOccurrence;

				//! �C�x���g����
				std::condition_variable m_occurrence;

				//! �r������N���X(��M�f�[�^�̃N���X�p)
				std::mutex m_asyncArgument;

				//! ��M�f�[�^�̃N���X
				CArgument m_argument;
			};
		}
	}
}