#pragma once

#include "Execution.h"
#include "SettingRegister.h"
#include "Result.h"
#include "Register.h"

namespace Standard
{
	namespace Plc
	{
		namespace Register
		{
			////////////////////////////////////////////////////////////////////////////////
			/// @brief			���W�X�^�̒l���擾
			/// @param[in]		object ���W�X�^�̒l���擾����ݒ�N���X
			////////////////////////////////////////////////////////////////////////////////
			inline Result::CContent Get(const Setting::CGetContent& object)
			{
				Result::CContent ret;

				Logging::CObject logging;
				std::stringstream postscript;
				Exception::EnumCode errorCode = Exception::CodePlcRead;
				int deviceErrorCode = Exception::DeviceCodeSuccess;
				Utility::CStopWatch stopWatch;

				logging.Message << object.Name << "Get()" << Logging::ConstSeparator;

				try
				{
					Register::CSpecify specify(object.Category);

					// �ԍ����X�V
					specify.Index = object.Index;

					// �f�[�^�̃^�C�v
					auto type = specify.GetType();
					// �擾���鐔
					unsigned long size = object.Size;
					// �A�h���X
					std::string address = specify.GetAddress();

					// ���W�X�^�̃f�[�^�^�C�v���m�F
					if (Register::ConstTypeBit == type)
					{
						// �r�b�g�^ �� ���W�X�^���擾����T�C�Y�����[�h�^�̃T�C�Y�ōX�V
						size /= 16;
						// ���擪��0�r�b�g�ȊO���l������2�]���Ɋm��
						size += 2;
					}
					else if (Register::ConstTypeLong == type)
					{
						// �����O�^ �� ���W�X�^���擾����T�C�Y�����[�h�^�̃T�C�Y�ōX�V
						size *= 2;
					}

					// �f�[�^�̃^�C�v�����[�h�^�ɍX�V
					postscript << "Register::CSpecify::ChangeType(Register::ConstTypeWord)";
					specify.ChangeType(Register::ConstTypeWord);
					postscript.str("");

					MotionApi::Register::Setting::CDeclare declare;

					// ���W�X�^�̃A�h���X���X�V
					postscript << "Register::CSpecify::GetAddress()";
					declare.Address = specify.GetAddress();
					postscript.str("");

					// ���W�X�^�̃n���h�����쐬
					postscript << "MotionApi::DeclareRegister()";
					postscript << Logging::ConstSeparator << "�A�h���X:" << declare.Address;
					auto result = MotionApi::DeclareRegister(declare);
					// ���O�o��
					postscript << Logging::ConstSeparator << "�n���h��:" << FormatHandle(result.Handle);
					//@@@Transfer::Output(Logging::Join(logging, postscript.str()));
					postscript.str("");

					MotionApi::Register::Setting::CGetContent setting;

					// �n���h�����X�V
					setting.Handle = result.Handle;

					// �f�[�^�����X�V
					setting.Size = size;

					// ���W�X�^�̒l���擾
					postscript << "MotionApi::GetRegisterData()" << Logging::ConstSeparator << "�n���h��:" << FormatHandle(setting.Handle);
					postscript << Logging::ConstSeparator << "�v���T�C�Y:" << setting.Size;
					auto content = MotionApi::GetRegisterData(setting);
					// ���O�o��
					postscript << Logging::ConstSeparator << "�擾�T�C�Y:" << content.Size;
					//@@@Transfer::Output(Logging::Join(logging, postscript.str()));
					postscript.str("");

					// �f�[�^�𑖍�
					for (unsigned long index = 0; index < content.Size; index++)
					{
						unsigned long target = index;

						// ���W�X�^�̃f�[�^�^�C�v���m�F
						if (Register::ConstTypeBit == type)
						{
							// �r�b�g�^ �� ���W�X�^�̐擪�r�b�g�ԍ����擾
							auto first = std::stoi(std::string(&address.back()), nullptr, 16);
							// ���擪�̃��W�X�^�ȊO��0�r�b�g�ڂ��擪
							auto bit = index == 0 ? first : 0;

							// 1���[�h���𑖍�
							while (bit < 16)
							{
								// �f�[�^�̊i�[����m��
								target = (index * 16) + bit - first;

								// �i�[����m�F
								if (object.Size <= target)
								{
									// �T�C�Y�𒴂��� �� �����𔲂���
									break;
								}

								// �f�[�^���i�[
								ret.Bits[target] = content.Buffers.at(index) >> bit & 0x1;

								// ���̃r�b�g��
								bit++;
							}
						}
						else if (Register::ConstTypeWord == type)
						{
							// ���[�h�^ �� �f�[�^���i�[
							ret.Words[target] = content.Buffers.at(index);
						}
						else if (Register::ConstTypeLong == type)
						{
							// �����O�^ �� �f�[�^�̊i�[����m��
							target = index / 2 * 2;
							// �f�[�^���m�F
							if (ret.Longs.count(target) <= 0)
							{
								// ���o�^
								ret.Longs[target];
							}

							// �i�[����m�F
							if (index % 2)
							{
								// ��ʃ��[�h �� 1���[�h���V�t�g���čX�V
								ret.Longs[target] |= ((unsigned long)content.Buffers.at(index)) << 16;
							}
							else
							{
								// ���ʃ��[�h
								ret.Longs[target] |= (unsigned long)content.Buffers.at(index);
							}
						}
					}
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
			/// @brief			���W�X�^�̒l���X�V
			/// @param[in]		object	���W�X�^�̒l���X�V����ݒ�N���X
			////////////////////////////////////////////////////////////////////////////////
			inline void Set(const Setting::CSetContent& object)
			{
				Logging::CObject logging;
				std::stringstream postscript;
				Exception::EnumCode errorCode = Exception::CodePlcWrite;
				int deviceErrorCode = Exception::DeviceCodeSuccess;
				Utility::CStopWatch stopWatch;

				logging.Message << object.Name << "Set()" << Logging::ConstSeparator;

				try
				{
					Register::CSpecify specify(object.Category);

					// �ԍ����X�V
					specify.Index = object.Index;

					// �f�[�^�̃^�C�v
					auto type = specify.GetType();
					// �X�V���鐔
					unsigned long size = (unsigned long)object.Values.size();
					// �������މ�
					unsigned long loop = 1;

					// ���W�X�^�̃f�[�^�^�C�v���m�F
					if (Register::ConstTypeBit == type)
					{
						// �r�b�g�^ �� 1�f�[�^���̃T�C�Y�ōX�V
						size = 1;
						// �J��Ԃ��񐔂��m��
						loop = size;
					}
					else if (Register::ConstTypeLong == type)
					{
						// �����O�^ �� ���W�X�^���X�V����T�C�Y�����[�h�^�̃T�C�Y�ōX�V
						size *= 2;
						// �f�[�^�̃^�C�v�����[�h�^�ɍX�V
						specify.ChangeType(Register::ConstTypeWord);
					}

					for (unsigned long index = 0; index < loop; index++)
					{
						MotionApi::Register::Setting::CDeclare declare;

						// ���W�X�^�̃A�h���X���X�V
						postscript << "Register::CSpecify::GetAddress()";
						declare.Address = specify.GetAddress();
						postscript.str("");

						// ���W�X�^�̃n���h�����쐬
						postscript << "MotionApi::DeclareRegister()";
						postscript << Logging::ConstSeparator << "�A�h���X:" << declare.Address;
						auto result = MotionApi::DeclareRegister(declare);
						// ���O�o��
						postscript << Logging::ConstSeparator << "�n���h��:" << FormatHandle(result.Handle);
						Transfer::Output(Logging::Join(logging, postscript.str()));
						postscript.str("");

						MotionApi::Register::Setting::CSetContent setting;

						// �n���h�����X�V
						setting.Handle = result.Handle;

						// ���W�X�^�̃f�[�^�^�C�v���m�F
						if (Register::ConstTypeBit == type)
						{
							// �r�b�g�^	�� ���W�X�^�̃r�b�g�ԍ����擾
							auto first = std::stoi(std::string(&specify.GetAddress().back()), nullptr, 16);
							// ���擪�̃��W�X�^�ȊO��0�r�b�g�ڂ��擪
							auto bit = index == 0 ? first : 0;

							// �f�[�^���i�[
							setting.Buffers.emplace_back(object.Values[index]);
						}
						else if (Register::ConstTypeWord == type || Register::ConstTypeLong == type)
						{
							// ���[�h�^ or �����O�^ �� �f�[�^���i�[
							std::copy(object.Values.begin(), object.Values.end(), std::back_inserter(setting.Buffers));
						}

						// �f�[�^�����X�V
						setting.Size = size;

						// ���W�X�^�̒l���X�V
						postscript << "MotionApi::SetRegisterData()" << Logging::ConstSeparator << "�n���h��:" << FormatHandle(setting.Handle);
						postscript << Logging::ConstSeparator << "�X�V�T�C�Y:" << setting.Size;
						MotionApi::SetRegisterData(setting);
						// ���O�o��
						Transfer::Output(Logging::Join(logging, postscript.str()));
						postscript.str("");
					}
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