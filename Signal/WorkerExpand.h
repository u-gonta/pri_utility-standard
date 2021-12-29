#pragma once

#include "Signal/Worker.h"

namespace Standard
{
	namespace Signal
	{
		namespace Worker
		{
			////////////////////////////////////////////////////////////////////////////////
			/// @class      CTemplateExpand
			/// @brief      ��Ԃ��Ď��A�������݁A�ʒm����e���v���[�g���g�������N���X
			///				�� ��Ԃ��Ď��A�������݁A�ʒm����e���v���[�g�̃N���X����h��
			///				���������A�j���̊֐���ǉ�
			/// @param[in]	CSetting	��Ԃ��Ď��A�ʒm����ݒ�N���X(�e���v���[�g�Ŏw��)
			/// @param[in]	CArgument	�ʒm������̃N���X(�e���v���[�g�Ŏw��)
			/// @param[in]	CWrite	�������ޏ��̃N���X(�e���v���[�g�Ŏw��)
			////////////////////////////////////////////////////////////////////////////////
			template <typename CSetting, typename CArgument, typename CWrite>
			class CTemplateExpand
				: virtual public CTemplate<CSetting, CArgument, CWrite>
			{
			public:
				////////////////////////////////////////////////////////////////////////////////
				/// @brief			�R���X�g���N�^
				////////////////////////////////////////////////////////////////////////////////
				CTemplateExpand() : CTemplate<CSetting, CArgument, CWrite>()
				{

				}

				////////////////////////////////////////////////////////////////////////////////
				/// @brief			�f�X�g���N�^
				////////////////////////////////////////////////////////////////////////////////
				virtual ~CTemplateExpand()
				{

				}

				////////////////////////////////////////////////////////////////////////////////
				/// @brief			������
				////////////////////////////////////////////////////////////////////////////////
				void Initialize()
				{
					Logging::CObject logging;
					std::stringstream postscript;
					std::stringstream message;
					Utility::CStopWatch stopWatch;

					logging.Message << GetName() << "Initialize()" << Logging::ConstSeparator;

					// ���O�o��
					Transfer::Output(Logging::Join(logging, postscript.str()));
					postscript.str("");

					// �g���K�[��ҋ@����N���X
					Observer::CTemplate<Trigger::CInitialize> observer;

					// �g���K�[��ʒm������֐���o�^
					postscript << "Subject::RegistrationFunction()";
					std::shared_ptr<std::function<Trigger::OnWakeupInitialize>> onWakeup = std::make_shared<std::function<Trigger::OnWakeupInitialize>>(std::bind(&Observer::CTemplate<Trigger::CInitialize>::OnCalled, &observer, std::placeholders::_1));
					postscript.str("");

					// �g���K�[��ҋ@����^�C���A�E�g���X�V
					observer.Setting.Timeout = GetInitializeTimeout();

					// ���������Ď����ăg���K�̒ʒm������֐���o�^
					postscript << "AdditionWakeupInitialize()";
					AdditionWakeupInitialize(onWakeup);
					// ���O�o��
					Transfer::Output(Logging::Join(logging, postscript.str()));
					postscript.str("");

					// ��������v��
					postscript << "RequestInitialize()";
					this->RequestInitialize();
					postscript.str("");

					// �g���K�[��҂�
					postscript << "Observer::Wait()";
					observer.Wait();
					// ���O�o��
					postscript << Logging::ConstSeparator << stopWatch.Format(true);
					Transfer::Output(Logging::Join(logging, postscript.str()));
					postscript.str("");
				}

				////////////////////////////////////////////////////////////////////////////////
				/// @brief			�j��
				////////////////////////////////////////////////////////////////////////////////
				void Destroy()
				{
					Logging::CObject logging;
					std::stringstream postscript;
					std::stringstream message;
					Utility::CStopWatch stopWatch;

					logging.Message << GetName() << "Destroy()" << Logging::ConstSeparator;

					// ���O�o��
					Transfer::Output(Logging::Join(logging, postscript.str()));
					postscript.str("");

					// �g���K�[��ҋ@����N���X
					Observer::CTemplate<Trigger::CDestroy> observer;

					// �g���K�[��ʒm������֐���o�^
					postscript << "Subject::RegistrationFunction()";
					std::shared_ptr<std::function<Trigger::OnWakeupDestroy>> onWakeup = std::make_shared<std::function<Trigger::OnWakeupDestroy>>(std::bind(&Observer::CTemplate<Trigger::CDestroy>::OnCalled, &observer, std::placeholders::_1));
					postscript.str("");

					// �g���K�[��ҋ@����^�C���A�E�g���X�V
					observer.Setting.Timeout = GetDestroyTimeout();

					// �j�����Ď����ăg���K�̒ʒm������֐���o�^
					postscript << "AdditionWakeupDestroy()";
					AdditionWakeupDestroy(onWakeup);
					// ���O�o��
					Transfer::Output(Logging::Join(logging, postscript.str()));
					postscript.str("");

					// �j����v��
					postscript << "RequestDestroy()";
					this->RequestDestroy();
					postscript.str("");

					// �g���K�[��҂�
					postscript << "Observer::Wait()";
					observer.Wait();
					// ���O�o��
					postscript << Logging::ConstSeparator << stopWatch.Format(true);
					Transfer::Output(Logging::Join(logging, postscript.str()));
					postscript.str("");

					// ���ʂ��擾
					auto result = observer.GetArgument();

					// ���ʂ��m�F
					if (result.Suspension)
					{
						// ���f������ �� ��O�𔭖C

					}
				}

			protected:
				////////////////////////////////////////////////////////////////////////////////
				/// @brief			���̂��擾
				/// 				���X���b�h�̃N���X�ŕێ����Ă��閼�̂��擾����悤�ɒǉ�
				/// @return			����
				////////////////////////////////////////////////////////////////////////////////
				std::string GetName()
				{
					std::string ret = "";

					// ���̂��擾
					ret = Thread::Worker::CTemplate::GetName();

					return ret;
				}

				////////////////////////////////////////////////////////////////////////////////
				/// @brief			�������̃^�C���A�E�g���Ԃ��擾
				///	@detail			�������l��1000ms
				///					���h����ŋL�q����
				/// @return			�^�C���A�E�g����[ms]
				////////////////////////////////////////////////////////////////////////////////
				virtual int GetInitializeTimeout()
				{
					int ret = 1000;

					return ret;
				}

				////////////////////////////////////////////////////////////////////////////////
				/// @brief			�j���̃^�C���A�E�g���Ԃ��擾
				///	@detail			�������l��1000ms
				///					���h����ŋL�q����
				/// @return			�^�C���A�E�g����[ms]
				////////////////////////////////////////////////////////////////////////////////
				virtual int GetDestroyTimeout()
				{
					int ret = 1000;

					return ret;
				}

				////////////////////////////////////////////////////////////////////////////////
				/// @brief			�������̊������N��
				/// @param[in]		object	�������̌��ʃN���X
				////////////////////////////////////////////////////////////////////////////////
				void WakeupInitializeComplete(const Trigger::CInitialize& object) noexcept
				{
					Logging::CObject logging;

					logging.Message << GetName() << "WakeupInitializeComplete()" << Logging::ConstSeparator;

					try
					{
						// �r������
						std::lock_guard<std::recursive_mutex> lock(m_asyncInitialize);

						Subject::CTemplate<Trigger::CInitialize> subject;

						// �֐���o�^
						subject.RegistrationFunction(m_initialize);

						// �֐���ʒm
						subject.CallingFunction(object);
					}
					catch (const std::exception& e)
					{
						// ��O�̏��� �� ���O�֒ǋL
						logging.Message << e.what();
						// ���O��ۑ�
						Transfer::Output(logging);
					}
				}

				////////////////////////////////////////////////////////////////////////////////
				/// @brief			�j���̊������N��
				/// @param[in]		object	�j���̌��ʃN���X
				////////////////////////////////////////////////////////////////////////////////
				void WakeupDestroyComplete(const Trigger::CDestroy& object) noexcept
				{
					Logging::CObject logging;

					logging.Message << GetName() << "WakeupDestroyComplete()" << Logging::ConstSeparator;

					try
					{
						// �r������
						std::lock_guard<std::recursive_mutex> lock(m_asyncDestroy);

						Subject::CTemplate<Trigger::CDestroy> subject;

						// �֐���o�^
						subject.RegistrationFunction(m_destroy);

						// �֐���ʒm
						subject.CallingFunction(object);
					}
					catch (const std::exception& e)
					{
						// ��O�̏��� �� ���O�֒ǋL
						logging.Message << e.what();
						// ���O��ۑ�
						Transfer::Output(logging);
					}
				}

			private:
				////////////////////////////////////////////////////////////////////////////////
				/// @brief			�������̒ʒm��o�^
				/// param[in]		object	�ʒm����֐�
				////////////////////////////////////////////////////////////////////////////////
				void AdditionWakeupInitialize(std::weak_ptr<std::function<Trigger::OnWakeupInitialize>> object)
				{
					// �r������
					std::lock_guard<std::recursive_mutex> lock(m_asyncInitialize);

					// �֐����X�V
					m_initialize = object;
				}

				////////////////////////////////////////////////////////////////////////////////
				/// @brief			�j���̒ʒm��o�^
				/// param[in]		object	�ʒm����֐�
				////////////////////////////////////////////////////////////////////////////////
				void AdditionWakeupDestroy(std::weak_ptr<std::function<Trigger::OnWakeupDestroy>> object)
				{
					// �r������
					std::lock_guard<std::recursive_mutex> lock(m_asyncDestroy);

					// �֐����X�V
					m_destroy = object;
				}

			private:
				//! �r������N���X(�������̊�����ʒm����֐��p)
				std::recursive_mutex m_asyncInitialize;

				//! �������̊�����ʒm����֐�
				std::weak_ptr<std::function<Trigger::OnWakeupInitialize>> m_initialize;

				//! �r������N���X(�j���̊�����ʒm����֐��p)
				std::recursive_mutex m_asyncDestroy;

				//! �j���̊�����ʒm����֐�
				std::weak_ptr<std::function<Trigger::OnWakeupDestroy>> m_destroy;
			};
		}
	}
}