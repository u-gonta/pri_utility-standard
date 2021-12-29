#pragma once

#include "Exception/Worker.h"
#include "Common/Utility/StopWatch.h"

namespace Standard
{
	namespace Device
	{
		namespace Worker
		{
			////////////////////////////////////////////////////////////////////////////////
			/// @class      CTemplate
			/// @brief      �f�o�C�X�̐�����s���e���v���[�g�̃N���X
			///				�� ��O�̐�����s���e���v���[�g�̃N���X����h��
			/// @param[in]	CDefault	�f�t�H���g�̐ݒ�N���X(�e���v���[�g�Ŏw��)
			/// @param[in]	CConnect	�ڑ��̐ݒ�N���X(�e���v���[�g�Ŏw��)
			////////////////////////////////////////////////////////////////////////////////
			template <typename CDefault, typename CConnect>
			class CTemplate
				: virtual public Exception::Worker::CTemplate
			{
			public:
				////////////////////////////////////////////////////////////////////////////////
				/// @brief			�R���X�g���N�^
				////////////////////////////////////////////////////////////////////////////////
				CTemplate() : Exception::Worker::CTemplate()
				{
					m_initialize = false;
					m_destroy = false;
					m_name = "Device::";
				}

				////////////////////////////////////////////////////////////////////////////////
				/// @brief			�f�X�g���N�^
				////////////////////////////////////////////////////////////////////////////////
				virtual ~CTemplate()
				{
					// �j��
					Destroy();
				}

			public:
				////////////////////////////////////////////////////////////////////////////////
				/// @brief			���̂��X�V
				/// @param[in]		name	����
				////////////////////////////////////////////////////////////////////////////////
				void SetName(const std::string& name)
				{
					// ���̂��X�V
					m_name = name;
					SetNameException(name + "Exception::");
				}

				////////////////////////////////////////////////////////////////////////////////
				/// @brief			������
				/// @detail			���C�u�����̏������܂ł��s��
				/// @param[in]		object	�f�t�H���g�̐ݒ�N���X
				////////////////////////////////////////////////////////////////////////////////
				void Initialize(const CDefault& object) throw(...)
				{
					Logging::CObject logging;
					std::stringstream postscript;

					logging.Message << GetName() << "Initialize()" << Logging::ConstSeparator;

					postscript << "�f�t�H���g�̐ݒ�N���X���X�V";
					SetDefaultSetting(object);
					postscript.str("");

					// ���O�o��
					Transfer::Output(Logging::Join(logging, postscript.str()));
					postscript.str("");

					// �r������
					std::lock_guard<std::recursive_mutex> lock(m_asyncFlag);

					// �j���ς݂��m�F
					IsDiscarded();

					// �������ς݂��m�F
					IsInitialize();

					// ������
					InitializeDerived();

					// �����t���O��ݒ�
					m_initialize = true;
				}

				////////////////////////////////////////////////////////////////////////////////
				/// @brief			�j��
				////////////////////////////////////////////////////////////////////////////////
				void Destroy() throw(...)
				{
					Logging::CObject logging;
					std::stringstream postscript;

					logging.Message << GetName() << "Destroy()" << Logging::ConstSeparator;

					// �r������
					std::lock_guard<std::recursive_mutex> lock(m_asyncFlag);

					do
					{
						// �����t���O���m�F
						if (m_initialize == false)
						{
							postscript << "��������";

							// ���O�o��
							Transfer::Output(Logging::Join(logging, postscript.str()));
							postscript.str("");
							break;
						}

						// �j���t���O���m�F
						if (m_destroy)
						{
							postscript << "�j���ς�";

							// ���O�o��
							Transfer::Output(Logging::Join(logging, postscript.str()));
							postscript.str("");
							break;
						}

						// ���O�o��
						Transfer::Output(Logging::Join(logging, postscript.str()));
						postscript.str("");

						// �ؒf
						Disconnect();

						// �j��
						DestroyDerived();

						// �����t���O������
						m_initialize = false;

						// �j���t���O��ݒ�
						m_destroy = true;
					} while (false);
				}

				////////////////////////////////////////////////////////////////////////////////
				/// @brief			�ڑ�
				/// @param[in]		object	�ڑ��̐ݒ�N���X
				////////////////////////////////////////////////////////////////////////////////
				void Connect(const CConnect& object) throw(...)
				{
					Logging::CObject logging;
					std::stringstream postscript;

					logging.Message << GetName() << "Connect()" << Logging::ConstSeparator;

					postscript << "�ڑ��̐ݒ�N���X���X�V";
					SetConnectSetting(object);
					postscript.str("");

					// ���O�o��
					Transfer::Output(Logging::Join(logging, postscript.str()));
					postscript.str("");

					// �r������
					std::lock_guard<std::recursive_mutex> lock(m_asyncFlag);

					// ���䂪�\���m�F
					IsControllable();

					// �ؒf
					Disconnect();

					// �ڑ�
					ConnectDerived();
				}

				////////////////////////////////////////////////////////////////////////////////
				/// @brief			�ؒf
				////////////////////////////////////////////////////////////////////////////////
				void Disconnect() throw(...)
				{
					Logging::CObject logging;
					std::stringstream postscript;

					logging.Message << GetName() << "Disconnect()" << Logging::ConstSeparator;

					do
					{
						// �ڑ��̏�Ԃ��m�F
						if (GetConnected() == false)
						{
							// ���ڑ�
							break;
						}

						// ���O�o��
						Transfer::Output(Logging::Join(logging, postscript.str()));
						postscript.str("");

						// �r������
						std::lock_guard<std::recursive_mutex> lock(m_asyncFlag);

						// ���䂪�\���m�F
						IsControllable();

						// �ؒf
						DisconnectDerived();
					} while (false);
				}

			protected:
				////////////////////////////////////////////////////////////////////////////////
				/// @brief			�������ς݂̊m�F
				////////////////////////////////////////////////////////////////////////////////
				void IsInitialize()
				{
					// �r������
					std::lock_guard<std::recursive_mutex> lock(m_asyncFlag);

					// �����t���O���m�F
					if (m_initialize)
					{
						// �������ς݂̗�O�𔭖C
						ExceptionInitialized();
					}
				}

				////////////////////////////////////////////////////////////////////////////////
				/// @brief			���䂪�\���m�F
				////////////////////////////////////////////////////////////////////////////////
				void IsControllable()
				{
					// �r������
					std::lock_guard<std::recursive_mutex> lock(m_asyncFlag);

					// �j���ς݂��m�F
					IsDiscarded();

					// �����������m�F
					IsUninitialized();
				}

			protected:
				////////////////////////////////////////////////////////////////////////////////
				/// @brief			�f�t�H���g�̐ݒ�N���X���擾
				/// @return			�f�t�H���g�̐ݒ�N���X
				////////////////////////////////////////////////////////////////////////////////
				CDefault GetDefaultSetting()
				{
					CDefault ret;

					// �r������
					std::lock_guard<std::recursive_mutex> lock(m_asyncDefaultSetting);

					// �f�t�H���g�̐ݒ�N���X���擾
					ret = m_defaultSetting;

					return ret;
				}

				////////////////////////////////////////////////////////////////////////////////
				/// @brief			�f�t�H���g�̐ݒ�N���X���X�V
				/// @param[in]		object	�f�t�H���g�̐ݒ�N���X
				////////////////////////////////////////////////////////////////////////////////
				void SetDefaultSetting(const CDefault& object)
				{
					// �r������
					std::lock_guard<std::recursive_mutex> lock(m_asyncDefaultSetting);

					// �f�t�H���g�̐ݒ�N���X���X�V
					m_defaultSetting = object;
				}

				////////////////////////////////////////////////////////////////////////////////
				/// @brief			�ڑ��̐ݒ�N���X���擾
				/// @return			�ڑ��̐ݒ�N���X
				////////////////////////////////////////////////////////////////////////////////
				CConnect GetConnectSetting()
				{
					CConnect ret;

					// �r������
					std::lock_guard<std::recursive_mutex> lock(m_asyncConnectSetting);

					// �ڑ��̐ݒ�N���X���擾
					ret = m_connectSetting;

					return ret;
				}

				////////////////////////////////////////////////////////////////////////////////
				/// @brief			�ڑ��̐ݒ�N���X���X�V
				/// @param[in]		object	�ڑ��̐ݒ�N���X
				////////////////////////////////////////////////////////////////////////////////
				void SetConnectSetting(const CConnect& object)
				{
					// �r������
					std::lock_guard<std::recursive_mutex> lock(m_asyncConnectSetting);

					// �ڑ��̐ݒ�N���X���X�V
					m_connectSetting = object;
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
				/// @brief			�������ς݂̗�O�𔭖C
				///	@detail			�������̗�O�𔭖C����^�C�~���O�ŌĂяo�����
				///					���h����ŋL�q����
				////////////////////////////////////////////////////////////////////////////////
				virtual void ExceptionInitialized() throw(...)
				{

				}

				////////////////////////////////////////////////////////////////////////////////
				/// @brief			�j���ς݂̗�O�𔭖C
				///	@detail			�j���̗�O�𔭖C����^�C�~���O�ŌĂяo�����
				///					���h����ŋL�q����
				////////////////////////////////////////////////////////////////////////////////
				virtual void ExceptionDiscarded() throw(...)
				{

				}

				////////////////////////////////////////////////////////////////////////////////
				/// @brief			���������̗�O�𔭖C
				///	@detail			���������̊m�F�ŗ�O�𔭖C����^�C�~���O�ŌĂяo�����
				///					���h����ŋL�q����
				////////////////////////////////////////////////////////////////////////////////
				virtual void ExceptionUninitialized() throw(...)
				{

				}

				////////////////////////////////////////////////////////////////////////////////
				/// @brief			�ڑ��̏�Ԃ��擾
				///	@detail			�ڑ��̏�Ԃ��擾����^�C�~���O�ŌĂяo�����
				///					���h����ŋL�q����
				/// @return			true:�ڑ��ς� / false:���ڑ�
				////////////////////////////////////////////////////////////////////////////////
				virtual bool GetConnected() throw(...)
				{
					bool ret = false;

					return ret;
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
				/// @brief			�ڑ�
				///	@detail			�ڑ����s���^�C�~���O�ŌĂяo�����
				///					���h����ŋL�q����
				////////////////////////////////////////////////////////////////////////////////
				virtual void ConnectDerived() throw(...)
				{

				}

				////////////////////////////////////////////////////////////////////////////////
				/// @brief			�ؒf
				///	@detail			�ؒf���s���^�C�~���O�ŌĂяo�����
				///					���h����ŋL�q����
				////////////////////////////////////////////////////////////////////////////////
				virtual void DisconnectDerived() throw(...)
				{

				}

			private:
				////////////////////////////////////////////////////////////////////////////////
				/// @brief			�j���ς݂̊m�F
				////////////////////////////////////////////////////////////////////////////////
				void IsDiscarded()
				{
					// �r������
					std::lock_guard<std::recursive_mutex> lock(m_asyncFlag);

					// �j���t���O���m�F
					if (m_destroy)
					{
						// �j���ς݂̗�O�𔭖C
						ExceptionDiscarded();
					}
				}

				////////////////////////////////////////////////////////////////////////////////
				/// @brief			���������̊m�F
				////////////////////////////////////////////////////////////////////////////////
				void IsUninitialized()
				{
					// �r������
					std::lock_guard<std::recursive_mutex> lock(m_asyncFlag);

					// �������t���O���m�F
					if (m_initialize == false)
					{
						// ���������̗�O�𔭖C
						ExceptionUninitialized();
					}
				}

			protected:
				//! �r������N���X(�t���O�p)
				std::recursive_mutex m_asyncFlag;

				//! �����t���O
				bool m_initialize;

				//! �j���t���O
				bool m_destroy;

			private:
				//! �r������N���X(�f�t�H���g�̐ݒ�N���X�p)
				std::recursive_mutex m_asyncDefaultSetting;

				//! �f�t�H���g�̐ݒ�N���X
				CDefault m_defaultSetting;

				//! �r������N���X(�ڑ��̐ݒ�N���X�p)
				std::recursive_mutex m_asyncConnectSetting;

				//! �ڑ��̐ݒ�N���X
				CConnect m_connectSetting;

				//! ����
				std::string m_name;
			};
		}
	}
}