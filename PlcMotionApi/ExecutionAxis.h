#pragma once

#include "Execution.h"
#include "SettingAxis.h"
#include "Result.h"

namespace Standard
{
	namespace Plc
	{
		namespace Axis
		{
			////////////////////////////////////////////////////////////////////////////////
			/// @brief			����������
			/// @param[in]		object ��������������ݒ�N���X
			/// @return			�����������������ʃN���X
			////////////////////////////////////////////////////////////////////////////////
			inline Result::CInitialize Initialize(const Setting::CInitialize& object)
			{
				Result::CInitialize ret;

				Logging::CObject logging;
				std::stringstream postscript;
				Exception::EnumCode errorCode = Exception::CodeAxisInitialize;
				int deviceErrorCode = Exception::DeviceCodeSuccess;
				Utility::CStopWatch stopWatch;

				logging.Message << object.Name << "Initialize()" << Logging::ConstSeparator;

				try
				{
					// ���̃n���h�����쐬����ݒ�N���X
					MotionApi::Axis::Setting::CDeclare setting = object.Declare;

					// ���̃n���h�����쐬
					postscript << "MotionApi::DeclareAxis()";
					postscript << Logging::ConstSeparator << "�X���b�gNo:" << object.Declare.SlotNo;
					postscript << Logging::ConstSeparator << "�T�u�X���b�gNo:" << object.Declare.SubSlotNo;
					postscript << Logging::ConstSeparator << "�X�e�[�V����No:" << object.Declare.StationNo;
					postscript << Logging::ConstSeparator << "�_����No:" << object.Declare.LogicalNo;
					postscript << Logging::ConstSeparator << "���^�C�v:" << object.Declare.Type;
					ret.Axis = MotionApi::DeclareAxis(setting);
					// ���O�o��
					postscript << Logging::ConstSeparator << "�n���h��:" << FormatHandle(ret.Axis.Handle) << Logging::ConstSeparator << stopWatch.Format(true);
					Transfer::Output(Logging::Join(logging, postscript.str()));
					postscript.str("");
				}
				catch (const MotionApi::CException& e)
				{
					// ��O�̏��� �� ��O�𔭖C
					throw Exception::CObject(errorCode, Exception::Convert::Message(errorCode, e.ErrorCode, logging, postscript.str(), e));
				}
				catch (const std::exception& e)
				{
					// ��O�̏��� �� ��O�𔭖C
					throw Exception::CObject(errorCode, Exception::Convert::Message(errorCode, deviceErrorCode, logging, postscript.str(), e));
				}

				return ret;
			}

			////////////////////////////////////////////////////////////////////////////////
			/// @brief			����j��
			/// @param[in]		object ����j������ݒ�N���X
			////////////////////////////////////////////////////////////////////////////////
			inline void Destroy(const Setting::CDestroy& object)
			{
				Logging::CObject logging;
				std::stringstream postscript;
				Exception::EnumCode errorCode = Exception::CodeAxisDestroy;
				int deviceErrorCode = Exception::DeviceCodeSuccess;
				Utility::CStopWatch stopWatch;

				logging.Message << object.Name << "Destroy()" << Logging::ConstSeparator;

				try
				{
					// ���̃n���h����j��
					postscript << "MotionApi::ClearAllAxis()";
					MotionApi::ClearAllAxis();
					// ���O�o��
					Transfer::Output(Logging::Join(logging, postscript.str()));
					postscript.str("");
				}
				catch (const MotionApi::CException& e)
				{
					// ��O�̏��� �� ��O�𔭖C
					throw Exception::CObject(errorCode, Exception::Convert::Message(errorCode, e.ErrorCode, logging, postscript.str(), e));
				}
				catch (const std::exception& e)
				{
					// ��O�̏��� �� ��O�𔭖C
					throw Exception::CObject(errorCode, Exception::Convert::Message(errorCode, deviceErrorCode, logging, postscript.str(), e));
				}
			}
		}
	}
}