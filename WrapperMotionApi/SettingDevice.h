#pragma once

#include <map>

#include "Common/Signal/Define.h"
#include "Common/Iterator/Basis.h"
#include "Common/Archive/Convert.h"
#include "Identifier.h"

#include "cereal/types/map.hpp"

// 1�o�C�g�A���C�������g�ɕύX
#pragma pack(1)

namespace Standard
{
	namespace MotionApi
	{
		namespace Device
		{
			////////////////////////////////////////////////////////////////////////////////
			/// @enum			���W�n
			////////////////////////////////////////////////////////////////////////////////
			enum EnumCoordinate
			{
				CoodinateWork = 0,			// ���[�N���W�n
				CoodinateMachine = 1		// �@�B���W�n
			};

			////////////////////////////////////////////////////////////////////////////////
			/// @enum			����^�C�v
			////////////////////////////////////////////////////////////////////////////////
			enum EnumMove
			{
				MoveRelative = 0,			// �����l�w��A������or��]��
				MoveAbsolute = 1,			// ��Βl�w��A������
				MoveRotaryShort = 2,		// ��Βl�w��A��]��(�߉��)
				MoveRotaryPositive = 3,		// ��Βl�w��A��]��(���])
				MoveRotaryNegative = 4		// ��Βl�w��A��]��(�t�])
			};

			////////////////////////////////////////////////////////////////////////////////
			/// @enum			���x�^�C�v
			////////////////////////////////////////////////////////////////////////////////
			enum EnumVelocity
			{
				VelocityUnit = 0,			// ���x[�w�ߒP��/sec]
				VelocityPercent = 1			// ���[�^�̒�i��]���i��i���x�j�A�܂��́A����ō����x��%�w��(Velocity= 1�̂Ƃ��A0.01%)
			};

			////////////////////////////////////////////////////////////////////////////////
			/// @enum			�������^�C�v
			////////////////////////////////////////////////////////////////////////////////
			enum EnumAccDec
			{
				AccDecUnit = 0,				// �������x�w��[�w�ߒP��/sec^2]
				AccDecTime = 1,				// ���萔�w��[ms]
				AccDecKeep = 2				// ���݂̐ݒ��ێ�
			};

			////////////////////////////////////////////////////////////////////////////////
			/// @enum			�������t�B���^�^�C�v
			////////////////////////////////////////////////////////////////////////////////
			enum EnumFilter
			{
				FilterCurve = 0,			// �ړ����σt�B���^�i�Ȉ�S���j
				FilterExp = 1,				// �w���t�B���^
				FilterNothing = 2,			// �t�B���^�Ȃ�
				FilterKeep = 3				// ���݂̐ݒ��ێ�
			};

			////////////////////////////////////////////////////////////////////////////////
			/// @enum			�f�[�^�^�C�v
			////////////////////////////////////////////////////////////////////////////////
			enum EnumData
			{
				DataMaxVelocity = 0,		// ���x�̏���l�A����ō����x
				DataAcceleration = 1,		// �����x[�w�ߒP��/sec^2]�^�������萔[ms]
				DataDeceleration = 2,		// �����x[�w�ߒP��/sec^2]�^�������萔[ms]
				DataFilterTime = 3,			// EnumFilter��FilterCurve�܂���FilterExp��I�����ɗL��
				DataVelocity = 4,			// ���x
				DataApproachVelocity = 5,	// ���_���A�̃A�v���[�`���x
				DataCreepVelocity = 6		// ���_���A�̃N���[�v���x
			};
			typedef Iterator::CWorker<EnumData, DataMaxVelocity, DataCreepVelocity> DataIterator;

			////////////////////////////////////////////////////////////////////////////////
			/// @enum			���̕���
			////////////////////////////////////////////////////////////////////////////////
			enum EnumDirection
			{
				DirectionPositive = 0,		// ������
				DirectionNegative = 1		// �t����
			};

			////////////////////////////////////////////////////////////////////////////////
			/// @enum			�����^�C�v
			////////////////////////////////////////////////////////////////////////////////
			enum EnumCompleted
			{
				CompleteDistribution = 0,	// �����o������
				CompletePositioning = 1,	// �ʒu���ߊ���
				CompleteStart = 2			// �w�ߊJ�n
			};

			namespace Setting
			{
				////////////////////////////////////////////////////////////////////////////////
				/// @class      CTemplate
				/// @brief      �ݒ肷��e���v���[�g�̃N���X
				////////////////////////////////////////////////////////////////////////////////
				class CTemplate
				{
				public:
					////////////////////////////////////////////////////////////////////////////////
					/// @brief			�R���X�g���N�^
					////////////////////////////////////////////////////////////////////////////////
					CTemplate()
					{
						Handle = 0;
					}

					////////////////////////////////////////////////////////////////////////////////
					/// @brief			�R�s�[������Z�q
					/// @return			���N���X
					////////////////////////////////////////////////////////////////////////////////
					CTemplate& operator = (const CTemplate& object) noexcept
					{
						// �X�V
						Update(object);

						return *this;
					}

					////////////////////////////////////////////////////////////////////////////////
					/// @brief			�f�X�g���N�^
					////////////////////////////////////////////////////////////////////////////////
					virtual ~CTemplate()
					{

					}

				protected:
					////////////////////////////////////////////////////////////////////////////////
					/// @brief			���N���X���X�V
					/// @param[in]		object	���N���X
					////////////////////////////////////////////////////////////////////////////////
					void Update(const CTemplate& object)
					{
						this->Handle = object.Handle;
					}

				public:
					//! �@��̃n���h��
					unsigned long Handle;
				};

				////////////////////////////////////////////////////////////////////////////////
				/// @class      CDeclare
				/// @brief      �f�o�C�X�̃n���h�����쐬����ݒ�N���X
				////////////////////////////////////////////////////////////////////////////////
				class CDeclare
				{
				public:
					////////////////////////////////////////////////////////////////////////////////
					/// @brief			�R���X�g���N�^
					////////////////////////////////////////////////////////////////////////////////
					CDeclare()
					{

					}

					////////////////////////////////////////////////////////////////////////////////
					/// @brief			�R�s�[������Z�q
					/// @return			���N���X
					////////////////////////////////////////////////////////////////////////////////
					CDeclare& operator = (const CDeclare& object) noexcept
					{
						// �X�V
						Update(object);

						return *this;
					}

					////////////////////////////////////////////////////////////////////////////////
					/// @brief			�f�X�g���N�^
					////////////////////////////////////////////////////////////////////////////////
					~CDeclare()
					{

					}

				protected:
					////////////////////////////////////////////////////////////////////////////////
					/// @brief			���N���X���X�V
					/// @param[in]		object	���N���X
					////////////////////////////////////////////////////////////////////////////////
					void Update(const CDeclare& object)
					{
						this->Handles.clear();
						std::copy(object.Handles.begin(), object.Handles.end(), std::back_inserter(this->Handles));
					}

				public:
					//! ���̃n���h��
					std::vector<unsigned long> Handles;
				};

				////////////////////////////////////////////////////////////////////////////////
				/// @class      CClear
				/// @brief      �f�o�C�X�̃n���h����j������ݒ�N���X
				///				�� ���ʎq��ێ�����N���X����h��
				////////////////////////////////////////////////////////////////////////////////
				class CClear
					: virtual public CIdentifier
				{
				public:
					////////////////////////////////////////////////////////////////////////////////
					/// @brief			�R���X�g���N�^
					////////////////////////////////////////////////////////////////////////////////
					CClear()
					{

					}

					////////////////////////////////////////////////////////////////////////////////
					/// @brief			�R�s�[������Z�q
					/// @return			���N���X
					////////////////////////////////////////////////////////////////////////////////
					CClear& operator = (const CClear& object) noexcept
					{
						// �X�V
						Update(object);

						return *this;
					}

					////////////////////////////////////////////////////////////////////////////////
					/// @brief			�f�X�g���N�^
					////////////////////////////////////////////////////////////////////////////////
					~CClear()
					{

					}

				protected:
					////////////////////////////////////////////////////////////////////////////////
					/// @brief			���N���X���X�V
					/// @param[in]		object	���N���X
					////////////////////////////////////////////////////////////////////////////////
					void Update(const CClear& object)
					{
						// ����̊֐�
						CIdentifier::Update(object);
					}
				};

				////////////////////////////////////////////////////////////////////////////////
				/// @class      CServo
				/// @brief      �T�[�{�̐�����s���ݒ�N���X
				///				�� ���ʎq��ێ�����N���X����h��
				////////////////////////////////////////////////////////////////////////////////
				class CServo
					: virtual public CIdentifier
				{
				public:
					////////////////////////////////////////////////////////////////////////////////
					/// @brief			�R���X�g���N�^
					////////////////////////////////////////////////////////////////////////////////
					CServo() : CIdentifier()
					{
						Mode = Signal::StatusOff;
						Timeout = 5000;
					}

					////////////////////////////////////////////////////////////////////////////////
					/// @brief			�R�s�[������Z�q
					/// @return			���N���X
					////////////////////////////////////////////////////////////////////////////////
					CServo& operator = (const CServo& object) noexcept
					{
						// �X�V
						Update(object);

						return *this;
					}

					////////////////////////////////////////////////////////////////////////////////
					/// @brief			�f�X�g���N�^
					////////////////////////////////////////////////////////////////////////////////
					~CServo()
					{

					}

				protected:
					////////////////////////////////////////////////////////////////////////////////
					/// @brief			���N���X���X�V
					/// @param[in]		object	���N���X
					////////////////////////////////////////////////////////////////////////////////
					void Update(const CServo& object)
					{
						// ����̊֐�
						CIdentifier::Update(object);

						this->Mode = object.Mode;
						this->Timeout = object.Timeout;
					}

				public:
					//! �T�[�{�̏��
					Signal::EnumStatus Mode;

					//! �^�C���A�E�g[ms]
					unsigned short Timeout;
				};

				namespace Motion
				{
					////////////////////////////////////////////////////////////////////////////////
					/// @class      CTemplate
					/// @brief      ���̐�����s���e���v���[�g�̐ݒ�N���X
					///				�� ���ʎq��ێ�����N���X����h��
					////////////////////////////////////////////////////////////////////////////////
					class CTemplate
						: virtual public CIdentifier
					{
					public:
						////////////////////////////////////////////////////////////////////////////////
						/// @brief			�R���X�g���N�^
						////////////////////////////////////////////////////////////////////////////////
						CTemplate() : CIdentifier()
						{
							Coordinate = CoodinateMachine;
							MoveType = MoveAbsolute;
							VelocityType = VelocityUnit;
							AccDecType = AccDecKeep;
							FilterType = FilterKeep;
						}

						////////////////////////////////////////////////////////////////////////////////
						/// @brief			�R�s�[������Z�q
						/// @return			���N���X
						////////////////////////////////////////////////////////////////////////////////
						CTemplate& operator = (const CTemplate& object) noexcept
						{
							// �X�V
							Update(object);

							return *this;
						}

						////////////////////////////////////////////////////////////////////////////////
						/// @brief			�f�X�g���N�^
						////////////////////////////////////////////////////////////////////////////////
						~CTemplate()
						{

						}

					public:
						////////////////////////////////////////////////////////////////////////////////
						/// @brief			�V���A���C�Y
						////////////////////////////////////////////////////////////////////////////////
						friend class cereal::access;
						template<class T>
						void serialize(T& archive)
						{
							archive(cereal::make_nvp("Handle", Handle));
							archive(cereal::make_nvp("Coordinate", Coordinate));
							archive(cereal::make_nvp("MoveType", MoveType));
							archive(cereal::make_nvp("VelocityType", VelocityType));
							archive(cereal::make_nvp("AccDecType", AccDecType));
							archive(cereal::make_nvp("FilterType", FilterType));
						}

					protected:
						////////////////////////////////////////////////////////////////////////////////
						/// @brief			���N���X���X�V
						/// @param[in]		object	���N���X
						////////////////////////////////////////////////////////////////////////////////
						void Update(const CTemplate& object)
						{
							// ����̊֐�
							CIdentifier::Update(object);

							this->Coordinate = object.Coordinate;
							this->MoveType = object.MoveType;
							this->VelocityType = object.VelocityType;
							this->AccDecType = object.AccDecType;
							this->FilterType = object.FilterType;
							this->Datas = object.Datas;
						}

					public:
						//! ���W�n
						EnumCoordinate Coordinate;

						//! ����^�C�v
						EnumMove MoveType;

						//! ���x�^�C�v
						EnumVelocity VelocityType;

						//! �������^�C�v
						EnumAccDec AccDecType;

						//! �������t�B���^�^�C�v
						EnumFilter FilterType;

						//! �f�[�^
						std::map<EnumData, unsigned long> Datas;
					};

					namespace Jog
					{
						////////////////////////////////////////////////////////////////////////////////
						/// @class      CStart
						/// @brief      ���̃W���O���s���s���ݒ�N���X
						///				�� ���̐�����s���e���v���[�g�̐ݒ�N���X����h��
						////////////////////////////////////////////////////////////////////////////////
						class CStart
							: virtual public CTemplate
						{
						private:
							//! �V���A���C�Y�̃^�C�g��
							const std::string ConstHeader = "CStart";

						public:
							////////////////////////////////////////////////////////////////////////////////
							/// @brief			�R���X�g���N�^
							////////////////////////////////////////////////////////////////////////////////
							CStart() : CTemplate()
							{
								Direction = DirectionPositive;
								Timeout = 0;
							}

							////////////////////////////////////////////////////////////////////////////////
							/// @brief			�R�s�[������Z�q
							/// @return			���N���X
							////////////////////////////////////////////////////////////////////////////////
							CStart& operator = (const CStart& object) noexcept
							{
								// �X�V
								Update(object);

								return *this;
							}

							////////////////////////////////////////////////////////////////////////////////
							/// @brief			�f�X�g���N�^
							////////////////////////////////////////////////////////////////////////////////
							~CStart()
							{

							}

						public:
							////////////////////////////////////////////////////////////////////////////////
							/// @brief			�V���A���C�Y
							////////////////////////////////////////////////////////////////////////////////
							friend class cereal::access;
							template<class T>
							void serialize(T& archive)
							{
								// ����̊֐�
								CTemplate::serialize(archive);

								archive(cereal::make_nvp("Direction", Direction));
								archive(cereal::make_nvp("Timeout", Timeout));
							}

							////////////////////////////////////////////////////////////////////////////////
							/// @brief			Json�`���ɃV���A���C�Y���������񂩂�X�V
							/// @param[in]		object	Json�`���ɃV���A���C�Y����������
							////////////////////////////////////////////////////////////////////////////////
							void Input(std::string object)
							{
								// Json�`������ϊ�
								Update(Archive::Input<CStart>(ConstHeader, object));
							}

							////////////////////////////////////////////////////////////////////////////////
							/// @brief			Json�`���ɃV���A���C�Y������������擾
							/// @return			�V���A���C�Y����������
							////////////////////////////////////////////////////////////////////////////////
							std::string Output()
							{
								std::string ret;

								// Json�`���ɕϊ�
								ret = Archive::Output<CStart>(ConstHeader, *this);

								return ret;
							}

						protected:
							////////////////////////////////////////////////////////////////////////////////
							/// @brief			���N���X���X�V
							/// @param[in]		object	���N���X
							////////////////////////////////////////////////////////////////////////////////
							void Update(const CStart& object)
							{
								// ����̊֐�
								CTemplate::Update(object);

								this->Direction = object.Direction;
								this->Timeout = object.Timeout;
							}

						public:
							//! ����
							EnumDirection Direction;

							//! �^�C���A�E�g[ms]
							unsigned short Timeout;
						};

						////////////////////////////////////////////////////////////////////////////////
						/// @class      CStop
						/// @brief      ���̃W���O��~���s���ݒ�N���X
						///				�� ���ʎq��ێ�����N���X����h��
						////////////////////////////////////////////////////////////////////////////////
						class CStop
							: virtual public CIdentifier
						{
						private:
							//! �V���A���C�Y�̃^�C�g��
							const std::string ConstHeader = "CStop";

						public:
							////////////////////////////////////////////////////////////////////////////////
							/// @brief			�R���X�g���N�^
							////////////////////////////////////////////////////////////////////////////////
							CStop() : CIdentifier()
							{
								Completed = CompletePositioning;
							}

							////////////////////////////////////////////////////////////////////////////////
							/// @brief			�R�s�[������Z�q
							/// @return			���N���X
							////////////////////////////////////////////////////////////////////////////////
							CStop& operator = (const CStop& object) noexcept
							{
								// �X�V
								Update(object);

								return *this;
							}

							////////////////////////////////////////////////////////////////////////////////
							/// @brief			�f�X�g���N�^
							////////////////////////////////////////////////////////////////////////////////
							~CStop()
							{

							}

						public:
							////////////////////////////////////////////////////////////////////////////////
							/// @brief			�V���A���C�Y
							////////////////////////////////////////////////////////////////////////////////
							friend class cereal::access;
							template<class T>
							void serialize(T& archive)
							{
								archive(cereal::make_nvp("Handle", Handle));
								archive(cereal::make_nvp("Completed", Completed));
							}

							////////////////////////////////////////////////////////////////////////////////
							/// @brief			Json�`���ɃV���A���C�Y���������񂩂�X�V
							/// @param[in]		object	Json�`���ɃV���A���C�Y����������
							////////////////////////////////////////////////////////////////////////////////
							void Input(std::string object)
							{
								// Json�`������ϊ�
								Update(Archive::Input<CStop>(ConstHeader, object));
							}

							////////////////////////////////////////////////////////////////////////////////
							/// @brief			Json�`���ɃV���A���C�Y������������擾
							/// @return			�V���A���C�Y����������
							////////////////////////////////////////////////////////////////////////////////
							std::string Output()
							{
								std::string ret;

								// Json�`���ɕϊ�
								ret = Archive::Output<CStop>(ConstHeader, *this);

								return ret;
							}

						protected:
							////////////////////////////////////////////////////////////////////////////////
							/// @brief			���N���X���X�V
							/// @param[in]		object	���N���X
							////////////////////////////////////////////////////////////////////////////////
							void Update(const CStop& object)
							{
								// ����̊֐�
								CIdentifier::Update(object);

								this->Completed = object.Completed;
							}

						public:
							//! �����^�C�v
							EnumCompleted Completed;
						};
					}
				}

				////////////////////////////////////////////////////////////////////////////////
				/// @class      CStartJog
				/// @brief      ���̃W���O�����s����ݒ�N���X
				///				�� ���ʎq��ێ�����N���X����h��
				////////////////////////////////////////////////////////////////////////////////
				class CStartJog
					: virtual public CIdentifier
				{
				public:
					////////////////////////////////////////////////////////////////////////////////
					/// @brief			�R���X�g���N�^
					////////////////////////////////////////////////////////////////////////////////
					CStartJog() : CIdentifier()
					{

					}

					////////////////////////////////////////////////////////////////////////////////
					/// @brief			�R�s�[������Z�q
					/// @return			���N���X
					////////////////////////////////////////////////////////////////////////////////
					CStartJog& operator = (const CStartJog& object) noexcept
					{
						// �X�V
						Update(object);

						return *this;
					}

					////////////////////////////////////////////////////////////////////////////////
					/// @brief			�f�X�g���N�^
					////////////////////////////////////////////////////////////////////////////////
					~CStartJog()
					{

					}

				public:
					////////////////////////////////////////////////////////////////////////////////
					/// @brief			�f�o�C�X�̃n���h�����쐬����ݒ�N���X���擾
					/// @param[in]		object	���N���X
					////////////////////////////////////////////////////////////////////////////////
					CDeclare GetDeclare()
					{
						CDeclare ret;

						// ���̃W���O���s���s���ݒ�N���X�𑖍�
						for (const auto& motion : Motions)
						{
							// �n���h����ǉ�
							ret.Handles.emplace_back(motion.Handle);
						}

						return ret;
					}

				protected:
					////////////////////////////////////////////////////////////////////////////////
					/// @brief			���N���X���X�V
					/// @param[in]		object	���N���X
					////////////////////////////////////////////////////////////////////////////////
					void Update(const CStartJog& object)
					{
						// ����̊֐�
						CIdentifier::Update(object);

						this->Motions.clear();
						std::copy(object.Motions.begin(), object.Motions.end(), std::back_inserter(this->Motions));
					}

				public:
					//! ���̃W���O���s���s���ݒ�N���X
					std::vector<Motion::Jog::CStart> Motions;
				};

				////////////////////////////////////////////////////////////////////////////////
				/// @class      CStopJog
				/// @brief      ���̃W���O���~����ݒ�N���X
				///				�� ���ʎq��ێ�����N���X����h��
				////////////////////////////////////////////////////////////////////////////////
				class CStopJog
					: virtual public CIdentifier
				{
				public:
					////////////////////////////////////////////////////////////////////////////////
					/// @brief			�R���X�g���N�^
					////////////////////////////////////////////////////////////////////////////////
					CStopJog() : CIdentifier()
					{

					}

					////////////////////////////////////////////////////////////////////////////////
					/// @brief			�R�s�[������Z�q
					/// @return			���N���X
					////////////////////////////////////////////////////////////////////////////////
					CStopJog& operator = (const CStopJog& object) noexcept
					{
						// �X�V
						Update(object);

						return *this;
					}

					////////////////////////////////////////////////////////////////////////////////
					/// @brief			�f�X�g���N�^
					////////////////////////////////////////////////////////////////////////////////
					~CStopJog()
					{

					}

				public:
					////////////////////////////////////////////////////////////////////////////////
					/// @brief			�f�o�C�X�̃n���h�����쐬����ݒ�N���X���擾
					/// @param[in]		object	���N���X
					////////////////////////////////////////////////////////////////////////////////
					CDeclare GetDeclare()
					{
						CDeclare ret;

						// ���̃W���O���s���s���ݒ�N���X�𑖍�
						for (const auto& motion : Motions)
						{
							// �n���h����ǉ�
							ret.Handles.emplace_back(motion.Handle);
						}

						return ret;
					}

				protected:
					////////////////////////////////////////////////////////////////////////////////
					/// @brief			���N���X���X�V
					/// @param[in]		object	���N���X
					////////////////////////////////////////////////////////////////////////////////
					void Update(const CStopJog& object)
					{
						// ����̊֐�
						CIdentifier::Update(object);

						this->Motions.clear();
						std::copy(object.Motions.begin(), object.Motions.end(), std::back_inserter(this->Motions));
					}

				public:
					//! ���̃W���O��~���s���ݒ�N���X
					std::vector<Motion::Jog::CStop> Motions;
				};
			}
		}
	}
}

// �o�C�g�A���C�������g��߂�
#pragma pack()