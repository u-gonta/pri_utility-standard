#pragma once

#include "WrapperMotionApi/SettingController.h"
#include "Setting.h"

namespace Standard
{
	namespace Plc
	{
		namespace Controller
		{
			namespace Setting
			{
				////////////////////////////////////////////////////////////////////////////////
				/// @class      CInitialize
				/// @brief      コントローラを初期化する設定クラス
				///				⇒ 設定するテンプレートのクラスから派生
				////////////////////////////////////////////////////////////////////////////////
				class CInitialize
					: virtual public CTemplate
				{
				public:
					////////////////////////////////////////////////////////////////////////////////
					/// @brief			コンストラクタ
					////////////////////////////////////////////////////////////////////////////////
					CInitialize() : CTemplate()
					{

					}

					////////////////////////////////////////////////////////////////////////////////
					/// @brief			コピー代入演算子
					/// @return			自クラス
					////////////////////////////////////////////////////////////////////////////////
					CInitialize& operator = (const CInitialize& object) noexcept
					{
						// 更新
						Update(object);

						return *this;
					}

					////////////////////////////////////////////////////////////////////////////////
					/// @brief			デストラクタ
					////////////////////////////////////////////////////////////////////////////////
					~CInitialize()
					{

					}

				protected:
					////////////////////////////////////////////////////////////////////////////////
					/// @brief			自クラスを更新
					/// @param[in]		object	自クラス
					////////////////////////////////////////////////////////////////////////////////
					void Update(const CInitialize& object)
					{
						// 既定の関数
						CTemplate::Update(object);

						this->Open = object.Open;
						this->ApiTimeout = object.ApiTimeout;
					}

				public:
					//! コントローラを開く設定のクラス
					MotionApi::Controller::Setting::COpen Open;

					//! Apiタイムアウトの設定クラス
					MotionApi::Controller::Setting::CApiTimeout ApiTimeout;
				};

				////////////////////////////////////////////////////////////////////////////////
				/// @class      CDestroy
				/// @brief      コントローラを破棄する設定クラス
				///				⇒ 設定するテンプレートのクラスから派生
				////////////////////////////////////////////////////////////////////////////////
				class CDestroy
					: virtual public CTemplate
				{
				public:
					////////////////////////////////////////////////////////////////////////////////
					/// @brief			コンストラクタ
					////////////////////////////////////////////////////////////////////////////////
					CDestroy() : CTemplate()
					{

					}

					////////////////////////////////////////////////////////////////////////////////
					/// @brief			コピー代入演算子
					/// @return			自クラス
					////////////////////////////////////////////////////////////////////////////////
					CDestroy& operator = (const CDestroy& object) noexcept
					{
						// 更新
						Update(object);

						return *this;
					}

					////////////////////////////////////////////////////////////////////////////////
					/// @brief			デストラクタ
					////////////////////////////////////////////////////////////////////////////////
					~CDestroy()
					{

					}

				protected:
					////////////////////////////////////////////////////////////////////////////////
					/// @brief			自クラスを更新
					/// @param[in]		object	自クラス
					////////////////////////////////////////////////////////////////////////////////
					void Update(const CDestroy& object)
					{
						// 既定の関数
						CTemplate::Update(object);

						this->Close = object.Close;
					}

				public:
					//! コントローラを閉じる設定のクラス
					MotionApi::Controller::Setting::CClose Close;
				};
			}
		}
	}
}