#pragma once

#include "boost/any.hpp"

#include "Thread/Worker.h"
#include "Exception/Worker.h"
#include "Signal/Observer.h"
#include "Signal/Subject.h"
#include "Signal/Result.h"
#include "Common/Utility/StopWatch.h"

namespace Standard
{
	namespace Signal
	{
		namespace Trigger
		{
			////////////////////////////////////////////////////////////////////////////////
			/// @class      CInitialize
			/// @brief      �������̌��ʃN���X
			///				�� ��Ԃ��Ď��������ʂ�ێ�����e���v���[�g�̃N���X����p��
			////////////////////////////////////////////////////////////////////////////////
			class CInitialize
				: virtual public Result::CTemplate
			{
			public:
				////////////////////////////////////////////////////////////////////////////////
				/// @brief			�R���X�g���N�^
				////////////////////////////////////////////////////////////////////////////////
				CInitialize() : Result::CTemplate()
				{

				}

				////////////////////////////////////////////////////////////////////////////////
				/// @brief			�R�s�[������Z�q
				/// @return			���N���X
				////////////////////////////////////////////////////////////////////////////////
				CInitialize& operator = (const CInitialize& object) noexcept
				{
					// �X�V
					Update(object);

					return *this;
				}

				////////////////////////////////////////////////////////////////////////////////
				/// @brief			�f�X�g���N�^
				////////////////////////////////////////////////////////////////////////////////
				~CInitialize()
				{

				}

			protected:
				////////////////////////////////////////////////////////////////////////////////
				/// @brief			���N���X���X�V
				/// @param[in]		object	���N���X
				////////////////////////////////////////////////////////////////////////////////
				void Update(const CInitialize& object)
				{

				}
			};

			////////////////////////////////////////////////////////////////////////////////
			/// @class      CDestroy
			/// @brief      �j���̌��ʃN���X
			///				�� ��Ԃ��Ď��������ʂ�ێ�����e���v���[�g�̃N���X����p��
			////////////////////////////////////////////////////////////////////////////////
			class CDestroy
				: virtual public Result::CTemplate
			{
			public:
				////////////////////////////////////////////////////////////////////////////////
				/// @brief			�R���X�g���N�^
				////////////////////////////////////////////////////////////////////////////////
				CDestroy() : Result::CTemplate()
				{

				}

				////////////////////////////////////////////////////////////////////////////////
				/// @brief			�R�s�[������Z�q
				/// @return			���N���X
				////////////////////////////////////////////////////////////////////////////////
				CDestroy& operator = (const CDestroy& object) noexcept
				{
					// �X�V
					Update(object);

					return *this;
				}

				////////////////////////////////////////////////////////////////////////////////
				/// @brief			�f�X�g���N�^
				////////////////////////////////////////////////////////////////////////////////
				~CDestroy()
				{

				}

			protected:
				////////////////////////////////////////////////////////////////////////////////
				/// @brief			���N���X���X�V
				/// @param[in]		object	���N���X
				////////////////////////////////////////////////////////////////////////////////
				void Update(const CDestroy& object)
				{

				}
			};

			//! ��������ʒm����֐��̒�`
			typedef void(OnWakeupInitialize)(const CInitialize&);

			//! �j����ʒm����֐��̒�`
			typedef void(OnWakeupDestroy)(const CDestroy&);
		}

		namespace Worker
		{
			////////////////////////////////////////////////////////////////////////////////
			/// @class      CTemplate
			/// @brief      ��Ԃ��Ď��A�������݁A�ʒm����e���v���[�g�̃N���X
			///				�� �X���b�h�̐�����s���e���v���[�g�̃N���X����h��
			///				�� ��O�̐�����s���e���v���[�g�̃N���X����h��
			/// @param[in]	CSetting	��Ԃ��Ď��A�ʒm����ݒ�N���X(�e���v���[�g�Ŏw��)
			/// @param[in]	CArgument	�ʒm������̃N���X(�e���v���[�g�Ŏw��)
			/// @param[in]	CWrite	�������ޏ��̃N���X(�e���v���[�g�Ŏw��)
			////////////////////////////////////////////////////////////////////////////////
			template <typename CSetting, typename CArgument, typename CWrite>
			class CTemplate
				: virtual public Thread::Worker::CTemplate, virtual public Exception::Worker::CTemplate
			{
			private:
				////////////////////////////////////////////////////////////////////////////////
				/// @enum			�����̏���
				////////////////////////////////////////////////////////////////////////////////
				enum EnumOrder
				{
					OrderWait,			// �ҋ@
					OrderInitialize,	// ������
					OrderDestroy,		// �j��
					OrderRetry,			// ���g���C
					OrderRead,			// �ǂݍ���
					OrderWrite			// ��������
				};

			public:
				////////////////////////////////////////////////////////////////////////////////
				/// @brief			�R���X�g���N�^
				////////////////////////////////////////////////////////////////////////////////
				CTemplate() : Thread::Worker::CTemplate(), Exception::Worker::CTemplate()
				{
					m_order = OrderWait;
					m_requestInitialize = false;
					m_requestDestroy = false;
					m_retry = 0;

					std::string name;

					// ���̂��X�V
					name = "Signal::";
					SetNameException(name + "Exception::");
					SetNameArgument(name + "Notice::");
				}

				////////////////////////////////////////////////////////////////////////////////
				/// @brief			�f�X�g���N�^
				////////////////////////////////////////////////////////////////////////////////
				virtual ~CTemplate()
				{
					// �ʒm����֐�������
					DetachFunctionCalled();
				}

			public:
				////////////////////////////////////////////////////////////////////////////////
				/// @brief			�ʒm�̖��̂��X�V
				/// @param[in]		name	�ʒm�̖���
				////////////////////////////////////////////////////////////////////////////////
				void SetNameArgument(const std::string& name)
				{
					// ���̂��X�V
					m_noticeArgument.SetName(name);
				}

				////////////////////////////////////////////////////////////////////////////////
				/// @brief			��Ԃ̒ʒm���J�n
				/// @detail			�ʒm����֐���o�^����O�ɌĂяo��
				/// @param[in]		cycle	��Ԃ̒ʒm���Ď�����Ԋu[ms]
				////////////////////////////////////////////////////////////////////////////////
				void StartNotice(int cycle = 60000)
				{
					// ��Ԃ̒ʒm���s��������ݒ�
					m_noticeArgument.SetCycle(cycle);

					// ��Ԃ̒ʒm���J�n
					m_noticeArgument.Start();

					// ��Ԃ̒ʒm�ŏ�������v��
					m_noticeArgument.RequestInitialize();
				}

				////////////////////////////////////////////////////////////////////////////////
				/// @brief			��Ԃ̒ʒm���~
				/// @detail			�ʒm����֐���������ɌĂяo��
				////////////////////////////////////////////////////////////////////////////////
				void StopNotice()
				{
					// ��Ԃ̒ʒm�Ŕj����v��
					m_noticeArgument.RequestDestroy();

					// ��Ԃ̒ʒm���~
					m_noticeArgument.Stop();
				}

				////////////////////////////////////////////////////////////////////////////////
				/// @brief			�ʒm����֐��̓o�^
				/// @detail			�ʒm���ɌĂяo���ꂽ���֐����w�肷��
				/// @param[in]		object	�Ăяo���ꂽ���֐�
				////////////////////////////////////////////////////////////////////////////////
				void AttachFunctionCalled(std::function<void(const CArgument&)> object)
				{
					// ��Ԃ̒ʒm���J�n
					StartNotice();

					// �ʒm����֐���o�^
					m_noticeArgument.AttachFunctionCalled(object);
				}

				////////////////////////////////////////////////////////////////////////////////
				/// @brief			�ʒm����֐��̉���
				/// @detail			�ʒm���ɌĂяo�����֐�����������
				////////////////////////////////////////////////////////////////////////////////
				void DetachFunctionCalled()
				{
					// �ʒm����֐�������
					m_noticeArgument.DetachFunctionCalled();

					// ��Ԃ̒ʒm���~
					StopNotice();
				}

				////////////////////////////////////////////////////////////////////////////////
				/// @brief			�ݒ�̃N���X���擾
				/// @return			�ݒ�̃N���X
				////////////////////////////////////////////////////////////////////////////////
				CSetting GetSetting()
				{
					CSetting ret;

					Logging::CObject logging;
					std::stringstream postscript;
					Exception::EnumCode errorCode = Exception::CodeUnknown;
					int deviceErrorCode = Exception::DeviceCodeSuccess;

					logging.Message << GetName() << "GetSetting()" << Logging::ConstSeparator;

					try
					{
						// �r������
						std::lock_guard<std::recursive_mutex> lock(m_asyncSetting);

						// �ݒ�̃N���X���擾
						ret = m_setting;
					}
					catch (const std::exception& e)
					{
						// ��O�̏��� �� ��O�𔭖C
						throw Exception::CObject(errorCode, Exception::Convert::Message(errorCode, deviceErrorCode, logging, postscript.str(), e));
					}

					return ret;
				}

				////////////////////////////////////////////////////////////////////////////////
				/// @brief			�ݒ�̃N���X���X�V
				/// @param[in]		object	�ݒ�̃N���X
				////////////////////////////////////////////////////////////////////////////////
				void SetSetting(const CSetting& object)
				{
					Logging::CObject logging;
					std::stringstream postscript;
					Exception::EnumCode errorCode = Exception::CodeUnknown;
					int deviceErrorCode = Exception::DeviceCodeSuccess;

					logging.Message << GetName() << "SetSetting()" << Logging::ConstSeparator;

					try
					{
						// �r������
						std::lock_guard<std::recursive_mutex> lock(m_asyncSetting);

						// �ݒ�̃N���X���X�V
						m_setting = object;
					}
					catch (const std::exception& e)
					{
						// ��O�̏��� �� ��O�𔭖C
						throw Exception::CObject(errorCode, Exception::Convert::Message(errorCode, deviceErrorCode, logging, postscript.str(), e));
					}
				}

				////////////////////////////////////////////////////////////////////////////////
				/// @brief			��������v��
				////////////////////////////////////////////////////////////////////////////////
				void RequestInitialize()
				{
					Logging::CObject logging;
					std::stringstream postscript;
					Exception::EnumCode errorCode = Exception::CodeUnknown;
					int deviceErrorCode = Exception::DeviceCodeSuccess;

					logging.Message << GetName() << "RequestInitialize()" << Logging::ConstSeparator;

					try
					{
						// �r������
						std::lock_guard<std::recursive_mutex> lock(m_asyncRequestInitialize);

						// �������̗v����ݒ�
						m_requestInitialize = true;

						// ����v�����N��
						WakeupRequestAction();
					}
					catch (const std::exception& e)
					{
						// ��O�̏��� �� ��O�𔭖C
						throw Exception::CObject(errorCode, Exception::Convert::Message(errorCode, deviceErrorCode, logging, postscript.str(), e));
					}
				}

				////////////////////////////////////////////////////////////////////////////////
				/// @brief			�j����v��
				////////////////////////////////////////////////////////////////////////////////
				void RequestDestroy()
				{
					Logging::CObject logging;
					std::stringstream postscript;
					Exception::EnumCode errorCode = Exception::CodeUnknown;
					int deviceErrorCode = Exception::DeviceCodeSuccess;

					logging.Message << GetName() << "RequestDestroy()" << Logging::ConstSeparator;

					try
					{
						// �r������
						std::lock_guard<std::recursive_mutex> lock(m_asyncRequestDestroy);

						// �j���̗v����ݒ�
						m_requestDestroy = true;

						// ����v�����N��
						WakeupRequestAction();
					}
					catch (const std::exception& e)
					{
						// ��O�̏��� �� ��O�𔭖C
						throw Exception::CObject(errorCode, Exception::Convert::Message(errorCode, deviceErrorCode, logging, postscript.str(), e));
					}
				}

				////////////////////////////////////////////////////////////////////////////////
				/// @brief			�������݂�v��
				/// @param[in]		object	�������ޏ��̃N���X
				////////////////////////////////////////////////////////////////////////////////
				void RequestWrite(const CWrite& object)
				{
					Logging::CObject logging;
					std::stringstream postscript;
					Exception::EnumCode errorCode = Exception::CodeUnknown;
					int deviceErrorCode = Exception::DeviceCodeSuccess;

					logging.Message << GetName() << "RequestWrite()" << Logging::ConstSeparator;

					try
					{
						// �r������
						std::lock_guard<std::recursive_mutex> lock(m_asyncWrite);

						// �������ޏ��̃N���X��ǉ�
						m_writes.push_back(object);

						// ����v�����N��
						WakeupRequestAction();
					}
					catch (const std::exception& e)
					{
						// ��O�̏��� �� ��O�𔭖C
						throw Exception::CObject(errorCode, Exception::Convert::Message(errorCode, deviceErrorCode, logging, postscript.str(), e));
					}
				}

			protected:
				////////////////////////////////////////////////////////////////////////////////
				/// @brief			��Ԃ�ʒm
				/// @param[in]		object	��O�̃N���X
				////////////////////////////////////////////////////////////////////////////////
				void NotifyArgument(const CArgument& object)
				{
					// ��Ԃ�ʒm
					m_noticeArgument.RequestNotice(object);
				}

				////////////////////////////////////////////////////////////////////////////////
				/// @brief			���g���C�񐔂�������
				////////////////////////////////////////////////////////////////////////////////
				void InitializeRetry()
				{
					// �r������
					std::lock_guard<std::recursive_mutex> lock(m_asyncRetry);

					// ���g���C�񐔂�������
					m_retry = 0;
				}

				////////////////////////////////////////////////////////////////////////////////
				/// @brief			���g���C�񐔂��X�V
				////////////////////////////////////////////////////////////////////////////////
				void CountupRetry()
				{
					// �r������
					std::lock_guard<std::recursive_mutex> lock(m_asyncRetry);

					// ���g���C�񐔂��X�V
					m_retry += 1;
				}

				////////////////////////////////////////////////////////////////////////////////
				/// @brief			���g���C�񐔂��擾
				/// @return			���g���C��
				////////////////////////////////////////////////////////////////////////////////
				int GetRetry()
				{
					int ret = 0;

					// �r������
					std::lock_guard<std::recursive_mutex> lock(m_asyncRetry);

					// ���g���C�񐔂��擾
					ret = m_retry;

					return ret;
				}

				////////////////////////////////////////////////////////////////////////////////
				/// @brief			���g���C���m�F
				///	@detail			�������l�͖���
				///					���h����ŋL�q����
				/// @return			true:���g���C���L�� / false:���g���C������
				////////////////////////////////////////////////////////////////////////////////
				virtual bool IsRetry()
				{
					bool ret = false;

					return ret;
				}

				////////////////////////////////////////////////////////////////////////////////
				/// @brief			���g���C�̒x�����Ԃ��擾
				///	@detail			�������l��1000ms
				///					���h����ŋL�q����
				/// @return			���g���C�̒x������[ms]
				////////////////////////////////////////////////////////////////////////////////
				virtual int GetDelayRetry()
				{
					int ret = 1000;

					return ret;
				}

				////////////////////////////////////////////////////////////////////////////////
				/// @brief			�������ޏ��̃N���X�����擾
				/// @return			�������ޏ��̃N���X��
				////////////////////////////////////////////////////////////////////////////////
				size_t GetWriteSize()
				{
					size_t ret = 0;

					// �r������
					std::lock_guard<std::recursive_mutex> lock(m_asyncWrite);

					// �������ރf�[�^�̃N���X�����擾
					ret = m_writes.size();

					return ret;
				}

				////////////////////////////////////////////////////////////////////////////////
				/// @brief			�������݂��m�F
				///	@detail			�������ރf�[�^�̃N���X�����݂��邩�ȊO�̔���͔h����ŋL�q����
				///					���h����ŋL�q����
				/// @return			true:�������ރf�[�^���� / false:�������ރf�[�^�Ȃ�
				////////////////////////////////////////////////////////////////////////////////
				virtual bool IsWrite()
				{
					bool ret = false;

					Logging::CObject logging;
					std::stringstream postscript;
					Exception::EnumCode errorCode = Exception::CodeUnknown;
					int deviceErrorCode = Exception::DeviceCodeSuccess;

					logging.Message << GetName() << "IsWrite()" << Logging::ConstSeparator;

					try
					{
						// �������ޏ��̃N���X�����m�F
						if (0 < GetWriteSize())
						{
							// �������ރf�[�^����
							ret = true;
						}
					}
					catch (const std::exception& e)
					{
						// ��O�̏��� �� ��O�𔭖C
						throw Exception::CObject(errorCode, Exception::Convert::Message(errorCode, deviceErrorCode, logging, postscript.str(), e));
					}

					return ret;
				}

				////////////////////////////////////////////////////////////////////////////////
				/// @brief			�������ޏ��̃N���X���擾
				/// @param[in]		�������ޏ��̃C���f�b�N�X�ԍ�
				/// @return			�������ޏ��̃N���X
				////////////////////////////////////////////////////////////////////////////////
				std::shared_ptr<CWrite> GetWrite(size_t index)
				{
					std::shared_ptr<CWrite> ret = nullptr;

					Logging::CObject logging;
					std::stringstream postscript;
					Exception::EnumCode errorCode = Exception::CodeUnknown;
					int deviceErrorCode = Exception::DeviceCodeSuccess;

					logging.Message << GetName() << "GetWrite()" << Logging::ConstSeparator;

					try
					{
						// �r������
						std::lock_guard<std::recursive_mutex> lock(m_asyncWrite);

						// �������ރf�[�^�̃N���X�����m�F
						if (m_writes.size() <= index)
						{
							// �������ރf�[�^�Ȃ�
							std::stringstream message;
							message << "�o�^��:" << m_writes.size() << Logging::ConstSeparator << "�w��:" << index;
							throw message.str();
						}

						// �������ރf�[�^����
						ret = std::make_shared<CWrite>(*(m_writes.begin() + index));
					}
					catch (const std::exception& e)
					{
						// ��O�̏��� �� ��O�𔭖C
						throw Exception::CObject(errorCode, Exception::Convert::Message(errorCode, deviceErrorCode, logging, postscript.str(), e));
					}

					return ret;
				}

				////////////////////////////////////////////////////////////////////////////////
				/// @brief			�������ޏ��̃N���X���폜
				/// @param[in]		indexs	�������ޏ��̃C���f�b�N�X�ԍ�
				////////////////////////////////////////////////////////////////////////////////
				void PopWrite(const std::vector<size_t>& indexs)
				{
					Logging::CObject logging;
					std::stringstream postscript;
					Exception::EnumCode errorCode = Exception::CodeUnknown;
					int deviceErrorCode = Exception::DeviceCodeSuccess;

					logging.Message << GetName() << "PopWrite()" << Logging::ConstSeparator;

					try
					{
						// �r������
						std::lock_guard<std::recursive_mutex> lock(m_asyncWrite);

						std::vector<size_t> targets;

						// �C���f�b�N�X�ԍ����R�s�[
						std::copy(indexs.begin(), indexs.end(), std::back_inserter(targets));

						// �C���f�b�N�X�ԍ����~���Ń\�[�g
						std::sort(targets.begin(), targets.end(), std::greater<size_t>());

						// �C���f�b�N�X�ԍ��𑖍�
						for (const auto& index : targets)
						{
							// �������ރf�[�^�̃N���X�����m�F
							if (m_writes.size() <= index)
							{
								// �������ރf�[�^�͈̔͊O
								continue;
							}

							// �f�[�^���폜
							m_writes.erase(m_writes.begin() + index);
						}
					}
					catch (const std::exception& e)
					{
						// ��O�̏��� �� ��O�𔭖C
						throw Exception::CObject(errorCode, Exception::Convert::Message(errorCode, deviceErrorCode, logging, postscript.str(), e));
					}
				}

				////////////////////////////////////////////////////////////////////////////////
				/// @brief			�������ޏ��̃N���X���擾
				///	@detail			�擪�ɓo�^���Ă���f�[�^���擾����
				/// @return			�������ޏ��̃N���X
				////////////////////////////////////////////////////////////////////////////////
				std::shared_ptr<CWrite> GetWriteFirst()
				{
					std::shared_ptr<CWrite> ret = nullptr;

					Logging::CObject logging;
					std::stringstream postscript;
					Exception::EnumCode errorCode = Exception::CodeUnknown;
					int deviceErrorCode = Exception::DeviceCodeSuccess;

					logging.Message << GetName() << "GetWriteFirst()" << Logging::ConstSeparator;

					try
					{
						// �r������
						std::lock_guard<std::recursive_mutex> lock(m_asyncWrite);

						// �������ރf�[�^�̃N���X�����m�F
						if (0 < m_writes.size())
						{
							// �������ރf�[�^����
							ret = std::make_shared<CWrite>(m_writes.front());
						}
					}
					catch (const std::exception& e)
					{
						// ��O�̏��� �� ��O�𔭖C
						throw Exception::CObject(errorCode, Exception::Convert::Message(errorCode, deviceErrorCode, logging, postscript.str(), e));
					}

					return ret;
				}

				////////////////////////////////////////////////////////////////////////////////
				/// @brief			�������ޏ��̃N���X���폜
				///	@detail			�擪�ɓo�^���Ă���f�[�^���폜����
				////////////////////////////////////////////////////////////////////////////////
				void PopWriteFirst()
				{
					Logging::CObject logging;
					std::stringstream postscript;
					Exception::EnumCode errorCode = Exception::CodeUnknown;
					int deviceErrorCode = Exception::DeviceCodeSuccess;

					logging.Message << GetName() << "PopWriteFirst()" << Logging::ConstSeparator;

					try
					{
						// �r������
						std::lock_guard<std::recursive_mutex> lock(m_asyncWrite);

						// �������ރf�[�^�̃N���X�����m�F
						if (0 < m_writes.size())
						{
							// �������ރf�[�^���� �� �擪�̃f�[�^���폜
							m_writes.pop_front();
						}
					}
					catch (const std::exception& e)
					{
						// ��O�̏��� �� ��O�𔭖C
						throw Exception::CObject(errorCode, Exception::Convert::Message(errorCode, deviceErrorCode, logging, postscript.str(), e));
					}
				}

				////////////////////////////////////////////////////////////////////////////////
				/// @brief			������
				///	@detail			���������s���^�C�~���O�ŌĂяo�����
				///					���h����ŋL�q����
				////////////////////////////////////////////////////////////////////////////////
				virtual void InitializeDerived() throw(...)
				{

				}

				////////////////////////////////////////////////////////////////////////////////
				/// @brief			�j��
				///	@detail			�j�����s���^�C�~���O�ŌĂяo�����
				///					���h����ŋL�q����
				////////////////////////////////////////////////////////////////////////////////
				virtual void DestroyDerived() throw(...)
				{

				}

				////////////////////////////////////////////////////////////////////////////////
				/// @brief			�ǂݍ���
				///	@detail			�ǂݍ��݂��s���^�C�~���O�ŌĂяo�����
				///					���h����ŋL�q����
				////////////////////////////////////////////////////////////////////////////////
				virtual void ReadDerived() throw(...)
				{

				}

				////////////////////////////////////////////////////////////////////////////////
				/// @brief			��������
				///	@detail			�������݂��s���^�C�~���O�ŌĂяo�����
				///					���h����ŋL�q����
				////////////////////////////////////////////////////////////////////////////////
				virtual void WriteDerived() throw(...)
				{

				}

				////////////////////////////////////////////////////////////////////////////////
				/// @brief			���C���֐��̏���
				/// @return			�^�C���A�E�g����[ms]
				////////////////////////////////////////////////////////////////////////////////
				int MainAction() override
				{
					int ret = 0;

					// ����̊֐�
					ret = Thread::Worker::CTemplate::MainAction();

					try
					{
						// �������݃t���O
						bool write = false;

						switch (GetOrder())
						{
						case OrderWait:
							// �ҋ@ �� �������̗v�����m�F
							if (IsRequestInitialize())
							{
								// �^�]�����X�V
								SetRunning(true);

								// �������ֈڍs
								SetOrder(OrderInitialize);

								// �^�C���A�E�g��������
								ret = 0;
								break;
							}

							// �j���̗v�����m�F
							if (IsRequestDestroy())
							{
								// �^�]�����X�V
								SetRunning(true);

								// ��~�v�� �� �j���ֈڍs
								SetOrder(OrderDestroy);

								// �^�C���A�E�g��������
								ret = 0;
								break;
							}

							// �^�]��������
							SetRunning(false);
							break;

						case OrderInitialize:
							// ������
							InitializeDerived();

							// �j���̗v��������
							ReleaseDestroy();

							// �ǂݍ��݂ֈڍs
							SetOrder(OrderRead);

							// �^�C���A�E�g��������
							ret = 0;
							break;

						case OrderDestroy:
							// �j��
							DestroyDerived();

							// �ҋ@�ֈڍs
							SetOrder(OrderWait);

							// �^�C���A�E�g��������
							ret = 0;
							break;

						case OrderRetry:
							// ���g���C �� �������ֈڍs
							SetOrder(OrderInitialize);

							// �^�C���A�E�g��������
							ret = 0;
							break;

						case OrderRead:
							// �ǂݍ��� �� �������ރf�[�^�̃N���X���m�F
							write = IsWrite();

							// �������ރf�[�^���Ȃ� and ��~�̗v�� or �j���̗v�����m�F
							if (write == false && (GetRequestStop() || IsRequestDestroy()))
							{
								// ��~�v�� �� �j���ֈڍs
								SetOrder(OrderDestroy);

								// �^�C���A�E�g��������
								ret = 0;
								break;
							}

							// �ǂݍ���
							ReadDerived();

							// ���g���C��������
							InitializeRetry();

							if (write)
							{
								// �������݃f�[�^���� �� �������݂ֈڍs
								SetOrder(OrderWrite);

								// �^�C���A�E�g��������
								ret = 0;
								break;
							}

							// ���C���֐��̎����Ń^�C���A�E�g��ݒ�
							ret = GetCycle();
							break;

						case OrderWrite:
							// ��������
							WriteDerived();

							// ���g���C��������
							InitializeRetry();

							// �ǂݍ��݂ֈڍs
							SetOrder(OrderRead);

							// �^�C���A�E�g��������
							ret = 0;
							break;
						}
					}
					catch (const Exception::CObject& e)
					{
						// ��O�̏��� �� ��O��ʒm
						OutputException((Exception::EnumCode)e.ErrorCode, e.DeviceErrorCode, e.Message.c_str());

						// �^�C���A�E�g��������
						ret = 0;

						switch (GetOrder())
						{
						case OrderDestroy:
							// �j�� �� �ҋ@�ֈڍs
							SetOrder(OrderWait);
							break;

						default:
							// ���̑� �� ���g���C�񐔂��X�V
							CountupRetry();

							// ���g���C���m�F
							if (IsRetry())
							{
								// ���g���C�̒x�����Ԃ��擾
								ret = GetDelayRetry();

								// ���g���C�ֈڍs
								SetOrder(OrderRetry);
							}
							else
							{
								// �j���ֈڍs
								SetOrder(OrderDestroy);
							}
							break;
						}
					}

					return ret;
				}

			private:
				////////////////////////////////////////////////////////////////////////////////
				/// @brief			�����̏������擾
				/// @return			�����̏���
				////////////////////////////////////////////////////////////////////////////////
				EnumOrder GetOrder()
				{
					EnumOrder ret = OrderWait;

					Logging::CObject logging;
					std::stringstream postscript;
					Exception::EnumCode errorCode = Exception::CodeUnknown;
					int deviceErrorCode = Exception::DeviceCodeSuccess;

					logging.Message << GetName() << "GetOrder()" << Logging::ConstSeparator;

					try
					{
						// �r������
						std::lock_guard<std::recursive_mutex> lock(m_asyncOrder);

						ret = m_order;
					}
					catch (const std::exception& e)
					{
						// ��O�̏��� �� ��O�𔭖C
						throw Exception::CObject(errorCode, Exception::Convert::Message(errorCode, deviceErrorCode, logging, postscript.str(), e));
					}

					return ret;
				}

				////////////////////////////////////////////////////////////////////////////////
				/// @brief			�����̏������X�V
				/// @param[in]		value	�����̏���
				////////////////////////////////////////////////////////////////////////////////
				void SetOrder(EnumOrder value)
				{
					Logging::CObject logging;
					std::stringstream postscript;
					Exception::EnumCode errorCode = Exception::CodeUnknown;
					int deviceErrorCode = Exception::DeviceCodeSuccess;

					logging.Message << GetName() << "SetOrder()" << Logging::ConstSeparator;

					try
					{
						// �r������
						std::lock_guard<std::recursive_mutex> lock(m_asyncOrder);

						m_order = value;
					}
					catch (const std::exception& e)
					{
						// ��O�̏��� �� ��O�𔭖C
						throw Exception::CObject(errorCode, Exception::Convert::Message(errorCode, deviceErrorCode, logging, postscript.str(), e));
					}
				}

				////////////////////////////////////////////////////////////////////////////////
				/// @brief			�������̗v�����m�F
				/// @detail			�m�F��ɏ������̗v���͉�������
				/// @return			true:��������v�� / false:�������͖��v��
				////////////////////////////////////////////////////////////////////////////////
				bool IsRequestInitialize()
				{
					bool ret = false;

					Logging::CObject logging;
					std::stringstream postscript;
					Exception::EnumCode errorCode = Exception::CodeUnknown;
					int deviceErrorCode = Exception::DeviceCodeSuccess;

					logging.Message << GetName() << "IsRequestInitialize()" << Logging::ConstSeparator;

					try
					{
						// �r������
						std::lock_guard<std::recursive_mutex> lock(m_asyncRequestInitialize);

						// �������̗v�����擾
						ret = m_requestInitialize;

						// �������̗v��������
						m_requestInitialize = false;
					}
					catch (const std::exception& e)
					{
						// ��O�̏��� �� ��O�𔭖C
						throw Exception::CObject(errorCode, Exception::Convert::Message(errorCode, deviceErrorCode, logging, postscript.str(), e));
					}

					return ret;
				}

				////////////////////////////////////////////////////////////////////////////////
				/// @brief			�j���̗v�����m�F
				/// @detail			�m�F��ɔj���̗v���͉�������
				/// @return			true:�j����v�� / false:�j���͖��v��
				////////////////////////////////////////////////////////////////////////////////
				bool IsRequestDestroy()
				{
					bool ret = false;

					Logging::CObject logging;
					std::stringstream postscript;
					Exception::EnumCode errorCode = Exception::CodeUnknown;
					int deviceErrorCode = Exception::DeviceCodeSuccess;

					logging.Message << GetName() << "IsRequestDestroy()" << Logging::ConstSeparator;

					try
					{
						// �r������
						std::lock_guard<std::recursive_mutex> lock(m_asyncRequestDestroy);

						// �j���̗v�����擾
						ret = m_requestDestroy;

						// �j���̗v��������
						m_requestDestroy = false;
					}
					catch (const std::exception& e)
					{
						// ��O�̏��� �� ��O�𔭖C
						throw Exception::CObject(errorCode, Exception::Convert::Message(errorCode, deviceErrorCode, logging, postscript.str(), e));
					}

					return ret;
				}

				////////////////////////////////////////////////////////////////////////////////
				/// @brief			�j���̗v��������
				////////////////////////////////////////////////////////////////////////////////
				void ReleaseDestroy()
				{
					Logging::CObject logging;
					std::stringstream postscript;
					Exception::EnumCode errorCode = Exception::CodeUnknown;
					int deviceErrorCode = Exception::DeviceCodeSuccess;

					logging.Message << GetName() << "ReleaseDestroy()" << Logging::ConstSeparator;

					try
					{
						// �r������
						std::lock_guard<std::recursive_mutex> lock(m_asyncRequestDestroy);

						// �j���̗v��������
						m_requestDestroy = false;
					}
					catch (const std::exception& e)
					{
						// ��O�̏��� �� ��O�𔭖C
						throw Exception::CObject(errorCode, Exception::Convert::Message(errorCode, deviceErrorCode, logging, postscript.str(), e));
					}
				}

			private:
				//! �r������N���X(�����̏����p)
				std::recursive_mutex m_asyncOrder;

				//! �����̏���
				EnumOrder m_order;

				//! �r������N���X(�������̗v���p)
				std::recursive_mutex m_asyncRequestInitialize;

				//! �������̗v��
				bool m_requestInitialize;

				//! �r������N���X(�j���̗v���p)
				std::recursive_mutex m_asyncRequestDestroy;

				//! �j���̗v��
				bool m_requestDestroy;

				//! �r������N���X(�ݒ�p)
				std::recursive_mutex m_asyncSetting;

				//! �ݒ�̃N���X
				CSetting m_setting;

				//! �r������N���X(���g���C�p)
				std::recursive_mutex m_asyncRetry;

				//! ���g���C�̉�
				int m_retry;

				//! �r������N���X(�������ރf�[�^�̃N���X�p)
				std::recursive_mutex m_asyncWrite;

				//! �������ރf�[�^�̃N���X
				std::deque<CWrite> m_writes;

				//! ��Ԃ�ʒm����N���X
				Notice::Worker::CTemplate<void(const CArgument&), CArgument> m_noticeArgument;
			};
		}
	}
}