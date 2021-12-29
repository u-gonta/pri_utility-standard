#pragma once

#include "Common/Grabber/Condition.h"

#include "StApi_TL.h"

namespace Standard
{
	namespace Grabber
	{
		////////////////////////////////////////////////////////////////////////////////
		/// @class      CParameter
		/// @brief      �J�����̃p�����[�^�𐧌䂷��N���X
		////////////////////////////////////////////////////////////////////////////////
		class CParameter
		{
		public:
			// �R���X�g���N�^
			CParameter();

			// �f�X�g���N�^
			~CParameter();

		public:
			// ���p�\���m�F
			void IsAvailable(const GenApi::INodeMap& nodeMap, const std::string& item) throw(...);

			// �ǂݍ��݉\���m�F
			void IsRead(const GenApi::INodeMap& nodeMap, const std::string& item) throw(...);

			// �������݉\���m�F
			void IsWrite(const GenApi::INodeMap& nodeMap, const std::string& item) throw(...);

		public:
			// �ݒ��I��
			void Selector(const GenApi::INodeMap& nodeMap, const std::string& item, const std::string& value) throw(...);

			// �ǂݍ���(�����l)
			int64_t ReadInteger(const GenApi::INodeMap& nodeMap, const std::string& item) throw(...);

			// �ǂݍ���(�����l)
			double ReadDouble(const GenApi::INodeMap& nodeMap, const std::string& item) throw(...);

			// ��������(������)
			void WriteText(const GenApi::INodeMap& nodeMap, const std::string& item, const std::string& value) throw(...);

			// ��������(�����l)
			int64_t WriteInteger(const GenApi::INodeMap& nodeMap, const std::string& item, const int64_t value) throw(...);

			// ��������(�����l)
			double WriteDouble(const GenApi::INodeMap& nodeMap, const std::string& item, const double value) throw(...);

		public:
			// �͈͂�ǂݍ���(����)
			Condition::CRangeDouble ReadRange(const GenApi::INodeMap& nodeMap, const std::string& item) throw(...);

		public:
			// ���s
			void Execute(const GenApi::INodeMap& nodeMap, const std::string& item) throw(...);
		};
	}
}
