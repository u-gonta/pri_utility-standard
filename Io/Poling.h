#pragma once

#include "Common/Io/Status.h"
#include "Signal/WorkerExpand.h"
#include "PolingSetting.h"

namespace Standard
{
	namespace Io
	{
		namespace Poling
		{
			namespace Worker
			{
				////////////////////////////////////////////////////////////////////////////////
				/// @class      CTemplate
				/// @brief      IO�̐���AIO�̊Ď��A��Ԃ�ʒm����e���v���[�g�̃N���X
				///				�� ��Ԃ��Ď��A�������݁A�ʒm����e���v���[�g���g�������N���X����h��
				////////////////////////////////////////////////////////////////////////////////
				class CTemplate
					: virtual public Signal::Worker::CTemplateExpand<CSetting, Status::CInput, Status::COutput>
				{
				public:
					////////////////////////////////////////////////////////////////////////////////
					/// @brief			�R���X�g���N�^
					////////////////////////////////////////////////////////////////////////////////
					CTemplate() : Signal::Worker::CTemplateExpand<CSetting, Status::CInput, Status::COutput>()
					{
						std::string name;

						// ���̂��X�V
						name = "Io::Poling::";
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
				};
			}
		}
	}
}