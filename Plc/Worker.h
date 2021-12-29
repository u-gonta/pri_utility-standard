#pragma once

#include "Common/Plc/Setting.h"
#include "Device/Worker.h"

namespace Standard
{
	namespace Plc
	{
		namespace Worker
		{
			////////////////////////////////////////////////////////////////////////////////
			/// @class      CTemplate
			/// @brief      Plc�̐�����s���e���v���[�g�̃N���X
			///				�� �f�o�C�X�̐�����s���e���v���[�g�̃N���X����p��
			/// @param[in]	CDefault	�f�t�H���g�̐ݒ�N���X���w��
			/// @param[in]	CConnect	�ڑ��̐ݒ�N���X���w��
			////////////////////////////////////////////////////////////////////////////////
			class CTemplate
				: virtual public Device::Worker::CTemplate<Setting::CDefault, Setting::CConnect>
			{
			public:
				////////////////////////////////////////////////////////////////////////////////
				/// @brief			�R���X�g���N�^
				////////////////////////////////////////////////////////////////////////////////
				CTemplate() : Device::Worker::CTemplate<Setting::CDefault, Setting::CConnect>()
				{
					// ���̂��X�V
					SetName("Plc::");
				}

				////////////////////////////////////////////////////////////////////////////////
				/// @brief			�f�X�g���N�^
				////////////////////////////////////////////////////////////////////////////////
				virtual ~CTemplate()
				{

				}

			protected:
				////////////////////////////////////////////////////////////////////////////////
				/// @brief			�������ς݂̗�O�𔭖C
				////////////////////////////////////////////////////////////////////////////////
				void ExceptionInitialized() override
				{
					Logging::CObject logging;
					std::stringstream postscript;

					logging.Message << GetName() << "ExceptionInitialized()" << Logging::ConstSeparator;

					// ��O�𔭖C
					OutputException(Exception::CodePlcInitialized, logging, postscript.str());
				}

				////////////////////////////////////////////////////////////////////////////////
				/// @brief			�j���ς݂̗�O�𔭖C
				////////////////////////////////////////////////////////////////////////////////
				void ExceptionDiscarded() override
				{
					Logging::CObject logging;
					std::stringstream postscript;

					logging.Message << GetName() << "ExceptionDiscarded()" << Logging::ConstSeparator;

					// ��O�𔭖C
					OutputException(Exception::CodePlcDiscarded, logging, postscript.str());
				}

				////////////////////////////////////////////////////////////////////////////////
				/// @brief			���������̗�O�𔭖C
				////////////////////////////////////////////////////////////////////////////////
				void ExceptionUninitialized() override
				{
					Logging::CObject logging;
					std::stringstream postscript;

					logging.Message << GetName() << "ExceptionUninitialized()" << Logging::ConstSeparator;

					// ��O�𔭖C
					OutputException(Exception::CodePlcUninitialized, logging, postscript.str());
				}
			};
		}
	}
}