#include "Parameter.h"

#include "Common/Logging/Basis.h"
#include "Common/Tool/Logging.h"
#include "Transfer/Transfer.h"

#pragma comment(lib, "Transfer.lib")

namespace Standard
{
	namespace Grabber
	{
		////////////////////////////////////////////////////////////////////////////////
		/// @brief			�R���X�g���N�^
		////////////////////////////////////////////////////////////////////////////////
		CParameter::CParameter()
		{

		}

		////////////////////////////////////////////////////////////////////////////////
		/// @brief			�f�X�g���N�^
		////////////////////////////////////////////////////////////////////////////////
		CParameter::~CParameter()
		{

		}

		////////////////////////////////////////////////////////////////////////////////
		/// @brief			���p�\���m�F
		/// @param[in]		nodeMap	�m�[�h
		/// @param[in]		item	����
		////////////////////////////////////////////////////////////////////////////////
		void CParameter::IsAvailable(const GenApi::INodeMap& nodeMap, const std::string& item)
		{
			std::stringstream postscript;

			try
			{
				// ���ڂ��擾
				postscript << "GenApi::GetNode(" << item << ")";
				GenApi::CBasePtr node(nodeMap.GetNode(item.c_str()));
				postscript.str("");

				// ���p�\���m�F
				if (GenApi::IsAvailable(node) == false)
				{
					// ����
					postscript << "���p�s��";

					// ��O�𔭖C
					throw std::exception(postscript.str().c_str());
				}
			}
			catch (const std::exception& e)
			{
				// ��O�̏���
				std::stringstream message;

				message << postscript.str() << Logging::ConstSeparator << e.what();

				// ��O�𔭖C
				throw std::exception(message.str().c_str());
			}
		}

		////////////////////////////////////////////////////////////////////////////////
		/// @brief			�ǂݍ��݉\���m�F
		/// @param[in]		nodeMap	�m�[�h
		/// @param[in]		item	����
		////////////////////////////////////////////////////////////////////////////////
		void CParameter::IsRead(const GenApi::INodeMap& nodeMap, const std::string& item)
		{
			std::stringstream postscript;

			try
			{
				// ���p�\���m�F
				IsAvailable(nodeMap, item);

				// ���ڂ��擾
				postscript << "GenApi::GetNode(" << item << ")";
				GenApi::CBasePtr node(nodeMap.GetNode(item.c_str()));
				postscript.str("");

				// �ǂݍ��݉\���m�F
				if (GenApi::IsReadable(node) == false)
				{
					// �ǂݍ��ݕs��
					postscript << "�ǂݍ��ݕs��";

					// ��O�𔭖C
					throw std::exception(postscript.str().c_str());
				}
			}
			catch (const std::exception& e)
			{
				// ��O�̏���
				std::stringstream message;

				message << postscript.str() << Logging::ConstSeparator << e.what();

				// ��O�𔭖C
				throw std::exception(message.str().c_str());
			}
		}

		////////////////////////////////////////////////////////////////////////////////
		/// @brief			�������݉\���m�F
		/// @param[in]		nodeMap	�m�[�h
		/// @param[in]		item	����
		////////////////////////////////////////////////////////////////////////////////
		void CParameter::IsWrite(const GenApi::INodeMap& nodeMap, const std::string& item)
		{
			std::stringstream postscript;

			try
			{
				// ���p�\���m�F
				IsAvailable(nodeMap, item);

				// ���ڂ��擾
				postscript << "GenApi::GetNode(" << item << ")";
				GenApi::CBasePtr node(nodeMap.GetNode(item.c_str()));
				postscript.str("");

				// �������݉\���m�F
				if (GenApi::IsWritable(node) == false)
				{
					// �������ݕs��
					postscript << "�������ݕs��";

					// ��O�𔭖C
					throw std::exception(postscript.str().c_str());
				}
			}
			catch (const std::exception& e)
			{
				// ��O�̏���
				std::stringstream message;

				message << postscript.str() << Logging::ConstSeparator << e.what();

				// ��O�𔭖C
				throw std::exception(message.str().c_str());
			}
		}

		// �ݒ��I��
		////////////////////////////////////////////////////////////////////////////////
		/// @brief			�ݒ��I��
		/// @param[in]		nodeMap	�m�[�h
		/// @param[in]		item	����
		/// @param[in]		value	�l
		////////////////////////////////////////////////////////////////////////////////
		void CParameter::Selector(const GenApi::INodeMap& nodeMap, const std::string& item, const std::string& value)
		{
			Logging::CObject logging;
			std::stringstream postscript;

			logging.Message << "Grabber::Parameter::Selector()" << Logging::ConstSeparator;

			try
			{
				// �������݉\���m�F
				IsWrite(nodeMap, item);

				// ���ڂ��擾
				postscript << "GenApi::GetNode(" << item << ")";
				GenApi::CEnumerationPtr node(nodeMap.GetNode(item.c_str()));
				postscript.str("");

				// ��������
				postscript << "GenApi::FromString(" << value << ")";
				node->FromString(value.c_str());
				postscript.str("");

				// ���O�o��
				postscript << "����:" << item << Logging::ConstSeparator << "�l:" << value;
				Transfer::Output(Logging::Join(logging, postscript.str()));
				postscript.str("");
			}
			catch (const std::exception& e)
			{
				// ��O�̏���
				std::stringstream message;

				message << postscript.str() << Logging::ConstSeparator << e.what();

				// ��O�𔭖C
				throw std::exception(message.str().c_str());
			}
		}

		////////////////////////////////////////////////////////////////////////////////
		/// @brief			�ǂݍ���(�����l)
		/// @param[in]		nodeMap	�m�[�h
		/// @param[in]		item	����
		/// @return			int�^�̒l
		////////////////////////////////////////////////////////////////////////////////
		int64_t CParameter::ReadInteger(const GenApi::INodeMap& nodeMap, const std::string& item)
		{
			int64_t ret = 0;

			Logging::CObject logging;
			std::stringstream postscript;

			logging.Message << "Grabber::Parameter::ReadInteger()" << Logging::ConstSeparator;

			try
			{
				// �ǂݍ��݉\���m�F
				IsRead(nodeMap, item);

				// ���ڂ��擾
				postscript << "GenApi::GetNode(" << item << ")";
				GenApi::CIntegerPtr node(nodeMap.GetNode(item.c_str()));
				postscript.str("");

				// �ǂݍ���
				postscript << "GenApi::GetValue()";
				ret = node->GetValue();
				postscript.str("");

				// ���O�o��
				postscript << "����:" << item << Logging::ConstSeparator << "�l:" << ret;
				Transfer::Output(Logging::Join(logging, postscript.str()));
				postscript.str("");
			}
			catch (const std::exception& e)
			{
				// ��O�̏���
				std::stringstream message;

				message << postscript.str() << Logging::ConstSeparator << e.what();

				// ��O�𔭖C
				throw std::exception(message.str().c_str());
			}

			return ret;
		}

		////////////////////////////////////////////////////////////////////////////////
		/// @brief			�ǂݍ���(�����l)
		/// @param[in]		nodeMap	�m�[�h
		/// @param[in]		item	����
		/// @return			double�^�̒l
		////////////////////////////////////////////////////////////////////////////////
		double CParameter::ReadDouble(const GenApi::INodeMap& nodeMap, const std::string& item)
		{
			double ret = 0;

			Logging::CObject logging;
			std::stringstream postscript;

			logging.Message << "Grabber::Parameter::ReadDouble()" << Logging::ConstSeparator;

			try
			{
				// �ǂݍ��݉\���m�F
				IsRead(nodeMap, item);

				// ���ڂ��擾
				postscript << "GenApi::GetNode(" << item << ")";
				GenApi::CFloatPtr node(nodeMap.GetNode(item.c_str()));
				postscript.str("");

				// �ǂݍ���
				postscript << "GenApi::GetValue()";
				ret = node->GetValue();
				postscript.str("");

				// ���O�o��
				postscript << "����:" << item << Logging::ConstSeparator << "�l:" << ret;
				Transfer::Output(Logging::Join(logging, postscript.str()));
				postscript.str("");
			}
			catch (const std::exception& e)
			{
				// ��O�̏���
				std::stringstream message;

				message << postscript.str() << Logging::ConstSeparator << e.what();

				// ��O�𔭖C
				throw std::exception(message.str().c_str());
			}

			return ret;
		}

		////////////////////////////////////////////////////////////////////////////////
		/// @brief			��������(������)
		/// @param[in]		nodeMap	�m�[�h
		/// @param[in]		item	����
		/// @param[in]		value	�l
		////////////////////////////////////////////////////////////////////////////////
		void CParameter::WriteText(const GenApi::INodeMap& nodeMap, const std::string& item, const std::string& value)
		{
			Logging::CObject logging;
			std::stringstream postscript;

			logging.Message << "Grabber::Parameter::WriteText()" << Logging::ConstSeparator;

			try
			{
				// �������݉\���m�F
				IsWrite(nodeMap, item);

				// ���ڂ��擾
				postscript << "GenApi::GetNode(" << item << ")";
				GenApi::CEnumerationPtr node(nodeMap.GetNode(item.c_str()));
				postscript.str("");

				// ��������
				postscript << "GenApi::FromString(" << value << ")";
				node->FromString(value.c_str());
				postscript.str("");

				// ���O�o��
				postscript << "����:" << item << Logging::ConstSeparator << "�l:" << value;
				Transfer::Output(Logging::Join(logging, postscript.str()));
				postscript.str("");
			}
			catch (const std::exception& e)
			{
				// ��O�̏���
				std::stringstream message;

				message << postscript.str() << Logging::ConstSeparator << e.what();

				// ��O�𔭖C
				throw std::exception(message.str().c_str());
			}
		}

		////////////////////////////////////////////////////////////////////////////////
		/// @brief			��������(�����l)
		/// @param[in]		nodeMap	�m�[�h
		/// @param[in]		item	����
		/// @param[in]		value	�l
		/// @return			��������int�^�̒l
		////////////////////////////////////////////////////////////////////////////////
		int64_t CParameter::WriteInteger(const GenApi::INodeMap& nodeMap, const std::string& item, const int64_t value)
		{
			int64_t ret = 0;

			Logging::CObject logging;
			std::stringstream postscript;

			logging.Message << "Grabber::Parameter::WriteInteger()" << Logging::ConstSeparator;

			try
			{
				// �������݉\���m�F
				IsWrite(nodeMap, item);

				// ���ڂ��擾
				postscript << "GenApi::GetNode(" << item << ")";
				GenApi::CIntegerPtr node(nodeMap.GetNode(item.c_str()));
				postscript.str("");

				int64_t shift = 0;

				// �C���N�������g���L�����m�F
				if (node->GetIncMode() != GenApi::EIncMode::noIncrement)
				{
					// �C���N�������g�����V�t�g���v�Z����
					shift = value % node->GetInc();
				}

				// �������ޒl�𒲐�
				ret = value + shift;

				// ��������
				postscript << "GenApi::SetValue(" << value << ")";
				node->SetValue(ret);
				postscript.str("");

				// ���O�o��
				postscript << "����:" << item << Logging::ConstSeparator << "�l:" << ret;
				Transfer::Output(Logging::Join(logging, postscript.str()));
				postscript.str("");
			}
			catch (const std::exception& e)
			{
				// ��O�̏���
				std::stringstream message;

				message << postscript.str() << Logging::ConstSeparator << e.what();

				// ��O�𔭖C
				throw std::exception(message.str().c_str());
			}

			return ret;
		}
		////////////////////////////////////////////////////////////////////////////////
		/// @brief			��������(�����l)
		/// @param[in]		nodeMap	�m�[�h
		/// @param[in]		item	����
		/// @param[in]		value	�l
		/// @return			��������double�^�̒l
		////////////////////////////////////////////////////////////////////////////////
		double CParameter::WriteDouble(const GenApi::INodeMap& nodeMap, const std::string& item, const double value)
		{
			double ret = 0;

			Logging::CObject logging;
			std::stringstream postscript;

			logging.Message << "Grabber::Parameter::WriteDouble()" << Logging::ConstSeparator;

			try
			{
				// �������݉\���m�F
				IsWrite(nodeMap, item);

				// ���ڂ��擾
				postscript << "GenApi::GetNode(" << item << ")";
				GenApi::CFloatPtr node(nodeMap.GetNode(item.c_str()));
				postscript.str("");

				double shift = 0;

				// �C���N�������g���L�����m�F
				if (node->GetIncMode() != GenApi::EIncMode::noIncrement)
				{
					// �C���N�������g�����V�t�g���v�Z����
					//@@@shift = node->GetInc();
				}

				// �������ޒl�𒲐�
				ret = value + shift;

				// ��������
				postscript << "GenApi::SetValue(" << ret << ")";
				node->SetValue(ret);
				postscript.str("");

				// ���O�o��
				postscript << "����:" << item << Logging::ConstSeparator << "�l:" << ret;
				Transfer::Output(Logging::Join(logging, postscript.str()));
				postscript.str("");
			}
			catch (const std::exception& e)
			{
				// ��O�̏���
				std::stringstream message;

				message << postscript.str() << Logging::ConstSeparator << e.what();

				// ��O�𔭖C
				throw std::exception(message.str().c_str());
			}

			return ret;
		}

		////////////////////////////////////////////////////////////////////////////////
		/// @brief			�͈͂�ǂݍ���(����)
		/// @param[in]		nodeMap	�m�[�h
		/// @return			�����l�͈̔̓N���X
		////////////////////////////////////////////////////////////////////////////////
		Condition::CRangeDouble CParameter::ReadRange(const GenApi::INodeMap& nodeMap, const std::string& item)
		{
			Condition::CRangeDouble ret;

			Logging::CObject logging;
			std::stringstream postscript;

			logging.Message << "Grabber::Parameter::ReadRange()" << Logging::ConstSeparator;

			try
			{
				// �ǂݍ��݉\���m�F
				IsRead(nodeMap, item);

				// ���ڂ��擾
				postscript << "GenApi::GetNode(" << item << ")";
				GenApi::CFloatPtr node(nodeMap.GetNode(item.c_str()));
				postscript.str("");

				// �ŏ��l���擾
				ret.Minimum = node->GetMin();

				// �ő�l���擾
				ret.Maximum = node->GetMax();

				// �C���N�������g���m�F
				if (node->GetIncMode() != GenApi::EIncMode::noIncrement)
				{
					// �C���N�������g�L�� �� �C���N�������g���擾
					ret.Increment = node->GetInc();
				}

				// ���O�o��
				postscript << "����:" << item << Logging::ConstSeparator << "�ŏ�:" << ret.Minimum << Logging::ConstSeparator << "�ő�:" << ret.Maximum << Logging::ConstSeparator << "�C���N�������g:" << ret.Increment;
				Transfer::Output(Logging::Join(logging, postscript.str()));
				postscript.str("");
			}
			catch (const std::exception& e)
			{
				// ��O�̏���
				std::stringstream message;

				message << postscript.str() << Logging::ConstSeparator << e.what();

				// ��O�𔭖C
				throw std::exception(message.str().c_str());
			}

			return ret;
		}

		////////////////////////////////////////////////////////////////////////////////
		/// @brief			���s
		/// @param[in]		nodeMap	�m�[�h
		/// @param[in]		item	����
		////////////////////////////////////////////////////////////////////////////////
		void CParameter::Execute(const GenApi::INodeMap& nodeMap, const std::string& item)
		{
			Logging::CObject logging;
			std::stringstream postscript;

			logging.Message << "Grabber::Parameter::Execute()" << Logging::ConstSeparator;

			try
			{
				// ���p�\���m�F
				IsAvailable(nodeMap, item);

				// ���ڂ��擾
				postscript << "GenApi::GetNode(" << item << ")";
				GenApi::CCommandPtr node(nodeMap.GetNode(item.c_str()));
				postscript.str("");

				// �����ς݂��m�F
				if (node->IsDone() == false)
				{
					// ����
					postscript << Logging::ConstSeparator << "�����ς�";

					// ��O�𔭖C
					throw std::exception(postscript.str().c_str());
				}

				// ���s
				postscript << "GenApi::Execute()";
				node->Execute();
				postscript.str("");

				// ���O�o��
				postscript << "����:" << item;
				Transfer::Output(Logging::Join(logging, postscript.str()));
				postscript.str("");
			}
			catch (const std::exception& e)
			{
				// ��O�̏���
				std::stringstream message;

				message << postscript.str() << Logging::ConstSeparator << e.what();

				// ��O�𔭖C
				throw std::exception(message.str().c_str());
			}
		}
	}
}