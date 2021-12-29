#pragma once

#include <mutex>

#include "boost/signals2.hpp"

namespace Standard
{
	namespace Signal
	{
		namespace Subject
		{
			////////////////////////////////////////////////////////////////////////////////
			/// @class      CTemplate
			/// @brief      �M����ʒm����N���X
			/// @param[in]	CArgument	�ʒm������̃N���X(�e���v���[�g�Ŏw��)
			////////////////////////////////////////////////////////////////////////////////
			template <typename CArgument>
			class CTemplate
			{
			public:
				//! �ʒm����֐��̒�`
				typedef void(OnCall)(const CArgument&);

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
					// �ʒm������֐��̉���
					CancellationFunction();
				}

				////////////////////////////////////////////////////////////////////////////////
				/// @brief			�ʒm������֐��̓o�^
				/// @param[in]		object	�ʒm������֐�
				////////////////////////////////////////////////////////////////////////////////
				void RegistrationFunction(std::weak_ptr<std::function<OnCall>> object)
				{
					// �r������
					std::lock_guard<std::recursive_mutex> lock(m_asyncFunction);

					_Function = object;
				}

				////////////////////////////////////////////////////////////////////////////////
				/// @brief			�ʒm������֐��̉���
				////////////////////////////////////////////////////////////////////////////////
				void CancellationFunction()
				{
					// �r������
					std::lock_guard<std::recursive_mutex> lock(m_asyncFunction);

					_Function.reset();
				}

				////////////////////////////////////////////////////////////////////////////////
				/// @brief			�ʒm���Ăяo��
				/// @param[in]		object	�ʒm����f�[�^�̃N���X
				////////////////////////////////////////////////////////////////////////////////
				void CallingFunction(const CArgument& object)
				{
					// �֐��̃V�O�i��
					boost::signals2::signal<OnCall> signal;

					// �r������
					std::lock_guard<std::recursive_mutex> lock(m_asyncFunction);

					// �֐��̃|�C���^
					std::shared_ptr<std::function<OnCall>> function = _Function.lock();

					// �֐����m�F
					if (function == nullptr)
					{
						// �֐��̎��ԂȂ�
						throw std::exception("�֐��̎��ԂȂ�");
					}

					// �֐��֐ڑ�
					boost::signals2::connection connection = signal.connect(*function);

					// �ڑ����m�F
					if (connection.connected() == false)
					{
						// ���ڑ�
						throw std::exception("�֐������ڑ�");
					}

					// �Ăяo��
					signal(object);

					// �ؒf
					connection.disconnect();
				}

			protected:
				////////////////////////////////////////////////////////////////////////////////
				/// @brief			���N���X���X�V
				/// @param[in]		object	���N���X
				////////////////////////////////////////////////////////////////////////////////
				void Update(const CTemplate& object)
				{
					this->_Function = object._Function;
				}

			private:
				//! �r������N���X(�ʒm����֐��p)
				std::recursive_mutex m_asyncFunction;

				//! �ʒm����֐�
				std::weak_ptr<std::function<OnCall>> _Function;
			};
		}
	}
}