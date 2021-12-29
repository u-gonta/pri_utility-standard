#pragma once

#include <mutex>

#include "Common/Tool/Logging.h"
#include "Exception/Convert.h"
#include "Transfer/Transfer.h"

#pragma comment(lib, "Transfer.lib")

namespace Standard
{
	namespace Thread
	{
		namespace Worker
		{
			////////////////////////////////////////////////////////////////////////////////
			/// @class      CTemplate
			/// @brief      �X���b�h�̐�����s���e���v���[�g�̃N���X
			////////////////////////////////////////////////////////////////////////////////
			class CTemplate
			{
			public:
				////////////////////////////////////////////////////////////////////////////////
				/// @brief			�R���X�g���N�^
				////////////////////////////////////////////////////////////////////////////////
				CTemplate()
				{
					m_requestStop = false;
					m_running = false;
					m_cycle = 10;
					m_name = "Thread::";
				}

				////////////////////////////////////////////////////////////////////////////////
				/// @brief			�R�s�[������Z�q
				/// @detail			�������ɂ���
				////////////////////////////////////////////////////////////////////////////////
				CTemplate& operator = (const CTemplate& object) = delete;

				////////////////////////////////////////////////////////////////////////////////
				/// @brief			�f�X�g���N�^
				////////////////////////////////////////////////////////////////////////////////
				virtual ~CTemplate()
				{
					Logging::CObject logging;
					std::stringstream postscript;

					logging.Message << GetName() << "~CTemplate()" << Logging::ConstSeparator;

					// ��~
					Stop();

					// ���O�o��
					Transfer::Output(Logging::Join(logging, postscript.str()));
					postscript.str("");
				}

				////////////////////////////////////////////////////////////////////////////////
				/// @brief			���C���֐��̎������X�V
				/// @param[in]		value	���C���֐��̎���[ms]
				////////////////////////////////////////////////////////////////////////////////
				void SetCycle(int value)
				{
					// �r������
					std::lock_guard<std::recursive_mutex> lock(m_asyncCycle);

					// ���C���֐��̎������X�V
					m_cycle = value;
				}

				////////////////////////////////////////////////////////////////////////////////
				/// @brief			���̂��X�V
				/// @param[in]		name	����
				////////////////////////////////////////////////////////////////////////////////
				void SetName(const std::string& name)
				{
					// ���̂��X�V
					m_name = name;
				}

				////////////////////////////////////////////////////////////////////////////////
				/// @brief			�J�n
				////////////////////////////////////////////////////////////////////////////////
				void Start() throw(...)
				{
					Logging::CObject logging;
					std::stringstream postscript;

					logging.Message << GetName() << "Start()" << Logging::ConstSeparator;

					// ��~
					Stop();

					// ���C���֐��̊J�n
					StartMain();

					// ���O�o��
					Transfer::Output(Logging::Join(logging, postscript.str()));
					postscript.str("");
				}

				////////////////////////////////////////////////////////////////////////////////
				/// @brief			��~
				////////////////////////////////////////////////////////////////////////////////
				void Stop() throw(...)
				{
					Logging::CObject logging;
					std::stringstream postscript;

					logging.Message << GetName() << "Stop()" << Logging::ConstSeparator;

					do
					{
						// ���C���֐��̒�~���m�F
						if (IsStopMain())
						{
							// ��~��
							break;
						}

						// ��~�v����ON
						SetRequestStop(true);

						// ����v�����N��
						WakeupRequestAction();

						// ���C���֐��̒�~�҂�
						StopMain();

						// ��~�v����OFF
						SetRequestStop(false);

						// ���O�o��
						Transfer::Output(Logging::Join(logging, postscript.str()));
						postscript.str("");
					} while (false);
				}

				////////////////////////////////////////////////////////////////////////////////
				/// @brief			����v�����N��
				////////////////////////////////////////////////////////////////////////////////
				void WakeupRequestAction()
				{
					// ����v�����N��
					m_requestAction.notify_one();
				}

			protected:
				////////////////////////////////////////////////////////////////////////////////
				/// @brief			���̂��擾
				/// @return			����
				////////////////////////////////////////////////////////////////////////////////
				std::string GetName()
				{
					std::string ret = "";

					// ���̂��擾
					ret = m_name;

					return ret;
				}

				////////////////////////////////////////////////////////////////////////////////
				/// @brief			��~�v�����擾
				/// @return			��~�v���t���O
				////////////////////////////////////////////////////////////////////////////////
				bool GetRequestStop()
				{
					bool ret = false;

					// �r������
					std::lock_guard<std::recursive_mutex> lock(m_asyncRequestStop);

					// ��~�v���t���O���擾
					ret = m_requestStop;

					return ret;
				}

				////////////////////////////////////////////////////////////////////////////////
				/// @brief			���C���֐��̎������擾
				/// @return			���C���֐��̎���[ms]
				////////////////////////////////////////////////////////////////////////////////
				int GetCycle()
				{
					int ret = 0;

					// �r������
					std::lock_guard<std::recursive_mutex> lock(m_asyncCycle);

					// ���C���֐��̎������擾
					ret = m_cycle;

					return ret;
				}

				////////////////////////////////////////////////////////////////////////////////
				/// @brief			�I���̏���
				///	@detail			���h����ŋL�q����
				////////////////////////////////////////////////////////////////////////////////
				virtual void Finish() throw(...)
				{

				}

				////////////////////////////////////////////////////////////////////////////////
				/// @brief			���C���֐��̏���
				///	@detail			���h����ŋL�q����
				/// @return			�^�C���A�E�g����[ms]
				///					�������l��10000ms
				////////////////////////////////////////////////////////////////////////////////
				virtual int MainAction() throw(...)
				{
					return 10000;
				}

				////////////////////////////////////////////////////////////////////////////////
				/// @brief			�^�]�����X�V
				/// @param[in]		value	true:�^�]�� / false:��~��
				////////////////////////////////////////////////////////////////////////////////
				void SetRunning(bool value)
				{
					// �r������
					std::lock_guard<std::recursive_mutex> lock(m_asyncRunning);

					// �^�]���t���O���X�V
					m_running = value;
				}

			private:
				////////////////////////////////////////////////////////////////////////////////
				/// @brief			��~�v�����X�V
				/// @param[in]		value	true:��~�v����ݒ� / false:��~�v��������
				////////////////////////////////////////////////////////////////////////////////
				void SetRequestStop(bool value)
				{
					// �r������
					std::lock_guard<std::recursive_mutex> lock(m_asyncRequestStop);

					// ��~�v���t���O���X�V
					m_requestStop = value;
				}

				////////////////////////////////////////////////////////////////////////////////
				/// @brief			�^�]�����擾
				/// @return			�^�]���t���O
				////////////////////////////////////////////////////////////////////////////////
				bool GetRunning()
				{
					bool ret = false;

					// �r������
					std::lock_guard<std::recursive_mutex> lock(m_asyncRunning);

					// �^�]���t���O���擾
					ret = m_running;

					return ret;
				}

				////////////////////////////////////////////////////////////////////////////////
				/// @brief			���C���֐��X���b�h���J�n
				////////////////////////////////////////////////////////////////////////////////
				void StartMain()
				{
					// �r������
					std::lock_guard<std::recursive_mutex> lock(m_asyncMainThread);

					// ���C���֐��X�^�[�g
					m_mainThread = std::thread([this] { Main(); });
				}

				////////////////////////////////////////////////////////////////////////////////
				/// @brief			���C���֐��X���b�h�̒�~���擾
				/// @return			true:��~�� / false:���쒆
				////////////////////////////////////////////////////////////////////////////////
				bool IsStopMain()
				{
					bool ret = true;

					// �r������
					std::lock_guard<std::recursive_mutex> lock(m_asyncMainThread);

					// ���C���֐��̋N�����m�F
					if (m_mainThread.joinable())
					{
						ret = false;
					}

					return ret;
				}

				////////////////////////////////////////////////////////////////////////////////
				/// @brief			���C���֐��X���b�h���~
				////////////////////////////////////////////////////////////////////////////////
				void StopMain()
				{
					// �r������
					std::lock_guard<std::recursive_mutex> lock(m_asyncMainThread);

					// ���C���֐��̋N�����m�F
					if (m_mainThread.joinable())
					{
						// ���C���֐��̒�~�҂�
						m_mainThread.join();
					}
				}

				////////////////////////////////////////////////////////////////////////////////
				/// @brief			����v���̊Ď��҂�
				/// @param[in]		timeout	�^�C���A�E�g[ms] ��0��菬�����ꍇ�́�
				////////////////////////////////////////////////////////////////////////////////
				std::cv_status WaitRequestAction(int timeout)
				{
					std::cv_status ret = std::cv_status::timeout;

					do
					{
						// �^�C���A�E�g���m�F
						if (timeout == 0)
						{
							// �^�C���A�E�g�Ȃ�
							break;
						}

						// �r������
						std::unique_lock<std::mutex> lock(m_asyncRequestAction);

						// �^�C���A�E�g�́����m�F
						if (timeout < 0)
						{
							// ����v���҂�
							m_requestAction.wait(lock);

							// ����
							ret = std::cv_status::no_timeout;
							break;
						}

						// ����v���҂�
						ret = m_requestAction.wait_for(lock, std::chrono::milliseconds(timeout));
					} while (false);

					return ret;
				}

				////////////////////////////////////////////////////////////////////////////////
				/// @brief			���C���֐�
				////////////////////////////////////////////////////////////////////////////////
				void Main()
				{
					Logging::CObject logging;
					std::stringstream postscript;
					Exception::EnumCode errorCode = Exception::CodeUnknown;
					int deviceErrorCode = Exception::DeviceCodeSuccess;

					logging.Message << GetName() << "Main()" << Logging::ConstSeparator;

					int timeout;

					// �^�C���A�E�g��������
					timeout = 0;

					while (true)
					{
						try
						{
							// ����v���̊Ď��҂�
							std::cv_status result = WaitRequestAction(timeout);

							// ���C���֐��̏���
							timeout = MainAction();

							// ��~�v�����m�F
							if (GetRequestStop() == false)
							{
								continue;
							}

							// �^�]�����m�F
							if (GetRunning())
							{
								// �^�C���A�E�g��������
								timeout = 0;
								continue;
							}

							// �I��
							Finish();
							break;
						}
						catch (const std::exception& e)
						{
							// ��O�̏��� �� ���O�̃N���X�ɕϊ����ďo��
							Transfer::Output(Exception::Convert::Logging(logging, postscript.str(), e));
							postscript.str("");
							break;
						}
					}
				}

			protected:
				//! �r������N���X(��~�v���t���O�p)
				std::recursive_mutex m_asyncRequestStop;

				//! ��~�v���t���O
				bool m_requestStop;

				//! �r������N���X(�^�]���t���O�p)
				std::recursive_mutex m_asyncRunning;

				//! �^�]���t���O
				bool m_running;

				//! �r������N���X(���C���֐��̎����p)
				std::recursive_mutex m_asyncCycle;

				//! ���C���֐��̎���[ms]
				int m_cycle;

			private:
				//! �r������N���X(����v���p)
				std::mutex m_asyncRequestAction;

				//! ����v��
				std::condition_variable m_requestAction;

				//! �r������N���X(���C���֐��X���b�h�p)
				std::recursive_mutex m_asyncMainThread;

				//! ���C���֐��X���b�h
				std::thread m_mainThread;

				//! ����
				std::string m_name;
			};
		}
	}
}