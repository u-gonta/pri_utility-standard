#pragma once

#include <algorithm>

#include "Common/Axis/Status.h"
#include "Signal/WorkerExpand.h"
#include "PolingSetting.h"

namespace Standard
{
	namespace Axis
	{
		namespace Execution
		{
			////////////////////////////////////////////////////////////////////////////////
			/// @class      CFinish
			/// @brief      ����̌��ʂ�ێ�����N���X
			///				�� ��Ԃ��Ď��������ʂ�ێ�����e���v���[�g�̃N���X����p��
			////////////////////////////////////////////////////////////////////////////////
			class CFinish
				: virtual public Signal::Result::CTemplate
			{
			public:
				////////////////////////////////////////////////////////////////////////////////
				/// @brief			�R���X�g���N�^
				////////////////////////////////////////////////////////////////////////////////
				CFinish() : Signal::Result::CTemplate()
				{

				}

				////////////////////////////////////////////////////////////////////////////////
				/// @brief			�R�s�[������Z�q
				/// @return			���N���X
				////////////////////////////////////////////////////////////////////////////////
				CFinish& operator = (const CFinish& object) noexcept
				{
					// �X�V
					Update(object);

					return *this;
				}

				////////////////////////////////////////////////////////////////////////////////
				/// @brief			�f�X�g���N�^
				////////////////////////////////////////////////////////////////////////////////
				~CFinish()
				{

				}
			};

			//! ���䂵�����ʂ�ʒm����֐��̒�`
			typedef void(OnWakeup)(const Execution::CFinish&);

			////////////////////////////////////////////////////////////////////////////////
			/// @class      CSetting
			/// @brief      ��������s����ݒ�̃N���X
			////////////////////////////////////////////////////////////////////////////////
			class CSetting
			{
			public:
				////////////////////////////////////////////////////////////////////////////////
				/// @brief			�R���X�g���N�^
				////////////////////////////////////////////////////////////////////////////////
				CSetting()
				{
					Wakeup.reset();
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
				~CSetting()
				{

				}

			protected:
				////////////////////////////////////////////////////////////////////////////////
				/// @brief			���N���X���X�V
				/// @param[in]		object	���N���X
				////////////////////////////////////////////////////////////////////////////////
				void Update(const CSetting& object)
				{
					this->Output = object.Output;
					this->Wakeup = object.Wakeup;
				}

			public:
				//! ���̏��(�o��)���w�肷��N���X
				Status::COutput Output;

				//! ���䂵�����ʂ̒ʒm���Ăяo���֐�
				std::weak_ptr<std::function<OnWakeup>> Wakeup;
			};
		}

		namespace Poling
		{
			namespace Worker
			{
				////////////////////////////////////////////////////////////////////////////////
				/// @class      CTemplate
				/// @brief      ���̐���A���̊Ď��A��Ԃ�ʒm����e���v���[�g�̃N���X
				///				�� ��Ԃ��Ď��A�������݁A�ʒm����e���v���[�g�̃N���X����h��
				/// @detail		���̐���A���̊Ď��A��Ԃ̒ʒm���s��
				////////////////////////////////////////////////////////////////////////////////
				class CTemplate
					: virtual public Signal::Worker::CTemplateExpand<Poling::CSetting, Status::CInput, Execution::CSetting>
				{
				public:
					////////////////////////////////////////////////////////////////////////////////
					/// @brief			�R���X�g���N�^
					////////////////////////////////////////////////////////////////////////////////
					CTemplate() : Signal::Worker::CTemplateExpand<Poling::CSetting, Status::CInput, Execution::CSetting>()
					{
						std::string name;

						// ���̂��X�V
						name = "Axis::Poling::";
						SetName(name);
						SetNameException(name + "Exception::");
						SetNameArgument(name + "Notice::");
					}

					////////////////////////////////////////////////////////////////////////////////
					/// @brief			�f�X�g���N�^
					////////////////////////////////////////////////////////////////////////////////
					virtual ~CTemplate()
					{

					}

				protected:
					////////////////////////////////////////////////////////////////////////////////
					/// @brief			�����̊������N��
					/// @param[in]		object	��������s�������ʂ̃N���X
					/// @param[in]		function ��������s�������ʂ̒ʒm���Ăяo���֐�
					////////////////////////////////////////////////////////////////////////////////
					void WakeupFinish(const Execution::CFinish& object, const std::weak_ptr<std::function<Execution::OnWakeup>>& function) noexcept
					{
						Logging::CObject logging;

						logging.Message << GetName() << "WakeupFinish()" << Logging::ConstSeparator;

						try
						{
							Signal::Subject::CTemplate<Execution::CFinish> subject;

							// �֐���o�^
							subject.RegistrationFunction(function);

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

					//////////////////////////////////////////////////////////////////////////////////
					///// @brief			�������ރf�[�^�̃N���X���m�F �����䂵�������̗L�����擾
					///// @detail			���䂵���������Ď����̎��Ɋ܂܂�Ă���ꍇ�ɂ̓f�[�^�Ȃ���Ԃ�
					///// @return			true:�������ރf�[�^���� / false:�������ރf�[�^�Ȃ�
					//////////////////////////////////////////////////////////////////////////////////
					//bool IsWrite()
					//{
					//	bool ret = false;

					//	std::stringstream postscript;
					//	Exception::EnumCode errorCode = Exception::CodeUnknown;
					//	int deviceErrorCode = Exception::DeviceCodeSuccess;

					//	postscript << GetName() << "IsWrite()";

					//	try
					//	{
					//		// �r������
					//		std::lock_guard<std::recursive_mutex> lock(m_asyncWrite);

					//		// �������݉\�Ȏ��̏��N���X���擾
					//		if (GetWrite() != nullptr)
					//		{
					//			// �������݉\�Ȏ��̏��N���X����
					//			ret = true;
					//		}
					//	}
					//	catch (const std::exception& e)
					//	{
					//		// ��O�̏���
					//		postscript << Logging::ConstSeparator << e.what();

					//		// ��O�𔭖C
					//		throw Exception::CObject(errorCode, deviceErrorCode, postscript.str());
					//	}

					//	return ret;
					//}

					//////////////////////////////////////////////////////////////////////////////////
					///// @brief			�������ݒ����m�F �����̎��{�����擾
					///// @return			true:�������ݒ� / false:�������݂Ȃ�
					//////////////////////////////////////////////////////////////////////////////////
					//bool IsWriting() override
					//{
					//	bool ret = false;

					//	// �r������
					//	std::lock_guard<std::recursive_mutex> lock(m_asyncSurveillances);

					//	// �Ď����Ă��鎲�̏����m�F
					//	if (0 < m_surveillances.size())
					//	{
					//		// �Ď����̎�����
					//		ret = true;
					//	}

					//	return ret;
					//}

					//////////////////////////////////////////////////////////////////////////////////
					///// @brief			�������݉\�Ȏ��̃X�[�p�[�N���X���擾 �����䂷�鎖���\�Ȏ��̏����擾
					///// @detail			���䂵�����������Ă��Ď����̎��Ɋ܂܂�Ă���ꍇ�ɂ�nullptr��Ԃ�
					///// @param[in]		remove	ture:�߂�l�ɊY�����鏑�����݉\�Ȏ��̏��N���X���폜 / false:�߂�l�ɊY�����鏑�����݉\�Ȏ��̏��N���X�͍폜���Ȃ�
					///// @return			���̏��N���X
					//////////////////////////////////////////////////////////////////////////////////
					//std::shared_ptr<Write::CSuper> GetWrite(bool remove = false)
					//{
					//	std::shared_ptr<Write::CSuper> ret = nullptr;

					//	// �r������
					//	std::lock_guard<std::recursive_mutex> lock(m_asyncWrite);

					//	// �������݉\�Ȏ��̃C�e���[�^
					//	std::deque<std::shared_ptr<Write::CSuper>>::iterator extraction = m_writes.end();

					//	// �������݉\�Ȏ���������瑖��
					//	for (auto write = m_writes.begin(); write != m_writes.end(); write++)
					//	{
					//		// ���̔ԍ�����v�t���O
					//		bool valid = false;

					//		// �r������
					//		std::lock_guard<std::recursive_mutex> lock(m_asyncSurveillances);

					//		// �Ď����Ă��鎲�̏��𑖍�
					//		std::for_each(m_surveillances.begin(), m_surveillances.end(), [this, write, &valid](auto surveillance)
					//		{
					//			// �������݉\�Ȏ��̔ԍ��𑖍�
					//			for (const auto& source : (*write)->Indexs)
					//			{
					//				// �Ď����Ă��鎲�̔ԍ��𑖍�
					//				for (const auto& target : surveillance->Indexs)
					//				{
					//					// �������݉\�Ȏ��̔ԍ��ƊĎ����Ă��鎲�̔ԍ����r
					//					if (source == target)
					//					{
					//						// �ԍ�����v
					//						valid = true;
					//						break;
					//					}
					//				}
					//			}
					//		});

					//		// ���̔ԍ�����v���m�F
					//		if (valid == false)
					//		{
					//			// ��v����ԍ��Ȃ�
					//			extraction = write;
					//		}
					//	}

					//	// �������݉\�Ȏ��̃C�e���[�^���m�F
					//	if (extraction != m_writes.end())
					//	{
					//		// �������݉\�Ȏ��̏��N���X���m��
					//		ret = *extraction;

					//		// �폜���m�F
					//		if (remove)
					//		{
					//			// �������݉\�Ȏ����폜
					//			m_writes.erase(extraction);
					//		}
					//	}

					//	return ret;
					//}

				protected:
					////! �r������N���X(�Ď��f�[�^�̃N���X�p)
					//std::recursive_mutex m_asyncSurveillances;

					////! �Ď����Ă��鎲�̎w��N���X
					//std::vector<std::shared_ptr<Status::Output::CTemplate>> m_surveillances;
				};
			}
		}
	}
}
