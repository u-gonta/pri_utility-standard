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
		/// @brief			コンストラクタ
		////////////////////////////////////////////////////////////////////////////////
		CParameter::CParameter()
		{

		}

		////////////////////////////////////////////////////////////////////////////////
		/// @brief			デストラクタ
		////////////////////////////////////////////////////////////////////////////////
		CParameter::~CParameter()
		{

		}

		////////////////////////////////////////////////////////////////////////////////
		/// @brief			利用可能を確認
		/// @param[in]		nodeMap	ノード
		/// @param[in]		item	項目
		////////////////////////////////////////////////////////////////////////////////
		void CParameter::IsAvailable(const GenApi::INodeMap& nodeMap, const std::string& item)
		{
			std::stringstream postscript;

			try
			{
				// 項目を取得
				postscript << "GenApi::GetNode(" << item << ")";
				GenApi::CBasePtr node(nodeMap.GetNode(item.c_str()));
				postscript.str("");

				// 利用可能を確認
				if (GenApi::IsAvailable(node) == false)
				{
					// 無効
					postscript << "利用不可";

					// 例外を発砲
					throw std::exception(postscript.str().c_str());
				}
			}
			catch (const std::exception& e)
			{
				// 例外の処理
				std::stringstream message;

				message << postscript.str() << Logging::ConstSeparator << e.what();

				// 例外を発砲
				throw std::exception(message.str().c_str());
			}
		}

		////////////////////////////////////////////////////////////////////////////////
		/// @brief			読み込み可能を確認
		/// @param[in]		nodeMap	ノード
		/// @param[in]		item	項目
		////////////////////////////////////////////////////////////////////////////////
		void CParameter::IsRead(const GenApi::INodeMap& nodeMap, const std::string& item)
		{
			std::stringstream postscript;

			try
			{
				// 利用可能を確認
				IsAvailable(nodeMap, item);

				// 項目を取得
				postscript << "GenApi::GetNode(" << item << ")";
				GenApi::CBasePtr node(nodeMap.GetNode(item.c_str()));
				postscript.str("");

				// 読み込み可能を確認
				if (GenApi::IsReadable(node) == false)
				{
					// 読み込み不可
					postscript << "読み込み不可";

					// 例外を発砲
					throw std::exception(postscript.str().c_str());
				}
			}
			catch (const std::exception& e)
			{
				// 例外の処理
				std::stringstream message;

				message << postscript.str() << Logging::ConstSeparator << e.what();

				// 例外を発砲
				throw std::exception(message.str().c_str());
			}
		}

		////////////////////////////////////////////////////////////////////////////////
		/// @brief			書き込み可能を確認
		/// @param[in]		nodeMap	ノード
		/// @param[in]		item	項目
		////////////////////////////////////////////////////////////////////////////////
		void CParameter::IsWrite(const GenApi::INodeMap& nodeMap, const std::string& item)
		{
			std::stringstream postscript;

			try
			{
				// 利用可能を確認
				IsAvailable(nodeMap, item);

				// 項目を取得
				postscript << "GenApi::GetNode(" << item << ")";
				GenApi::CBasePtr node(nodeMap.GetNode(item.c_str()));
				postscript.str("");

				// 書き込み可能を確認
				if (GenApi::IsWritable(node) == false)
				{
					// 書き込み不可
					postscript << "書き込み不可";

					// 例外を発砲
					throw std::exception(postscript.str().c_str());
				}
			}
			catch (const std::exception& e)
			{
				// 例外の処理
				std::stringstream message;

				message << postscript.str() << Logging::ConstSeparator << e.what();

				// 例外を発砲
				throw std::exception(message.str().c_str());
			}
		}

		// 設定を選択
		////////////////////////////////////////////////////////////////////////////////
		/// @brief			設定を選択
		/// @param[in]		nodeMap	ノード
		/// @param[in]		item	項目
		/// @param[in]		value	値
		////////////////////////////////////////////////////////////////////////////////
		void CParameter::Selector(const GenApi::INodeMap& nodeMap, const std::string& item, const std::string& value)
		{
			Logging::CObject logging;
			std::stringstream postscript;

			logging.Message << "Grabber::Parameter::Selector()" << Logging::ConstSeparator;

			try
			{
				// 書き込み可能を確認
				IsWrite(nodeMap, item);

				// 項目を取得
				postscript << "GenApi::GetNode(" << item << ")";
				GenApi::CEnumerationPtr node(nodeMap.GetNode(item.c_str()));
				postscript.str("");

				// 書き込み
				postscript << "GenApi::FromString(" << value << ")";
				node->FromString(value.c_str());
				postscript.str("");

				// ログ出力
				postscript << "項目:" << item << Logging::ConstSeparator << "値:" << value;
				Transfer::Output(Logging::Join(logging, postscript.str()));
				postscript.str("");
			}
			catch (const std::exception& e)
			{
				// 例外の処理
				std::stringstream message;

				message << postscript.str() << Logging::ConstSeparator << e.what();

				// 例外を発砲
				throw std::exception(message.str().c_str());
			}
		}

		////////////////////////////////////////////////////////////////////////////////
		/// @brief			読み込み(整数値)
		/// @param[in]		nodeMap	ノード
		/// @param[in]		item	項目
		/// @return			int型の値
		////////////////////////////////////////////////////////////////////////////////
		int64_t CParameter::ReadInteger(const GenApi::INodeMap& nodeMap, const std::string& item)
		{
			int64_t ret = 0;

			Logging::CObject logging;
			std::stringstream postscript;

			logging.Message << "Grabber::Parameter::ReadInteger()" << Logging::ConstSeparator;

			try
			{
				// 読み込み可能を確認
				IsRead(nodeMap, item);

				// 項目を取得
				postscript << "GenApi::GetNode(" << item << ")";
				GenApi::CIntegerPtr node(nodeMap.GetNode(item.c_str()));
				postscript.str("");

				// 読み込み
				postscript << "GenApi::GetValue()";
				ret = node->GetValue();
				postscript.str("");

				// ログ出力
				postscript << "項目:" << item << Logging::ConstSeparator << "値:" << ret;
				Transfer::Output(Logging::Join(logging, postscript.str()));
				postscript.str("");
			}
			catch (const std::exception& e)
			{
				// 例外の処理
				std::stringstream message;

				message << postscript.str() << Logging::ConstSeparator << e.what();

				// 例外を発砲
				throw std::exception(message.str().c_str());
			}

			return ret;
		}

		////////////////////////////////////////////////////////////////////////////////
		/// @brief			読み込み(実数値)
		/// @param[in]		nodeMap	ノード
		/// @param[in]		item	項目
		/// @return			double型の値
		////////////////////////////////////////////////////////////////////////////////
		double CParameter::ReadDouble(const GenApi::INodeMap& nodeMap, const std::string& item)
		{
			double ret = 0;

			Logging::CObject logging;
			std::stringstream postscript;

			logging.Message << "Grabber::Parameter::ReadDouble()" << Logging::ConstSeparator;

			try
			{
				// 読み込み可能を確認
				IsRead(nodeMap, item);

				// 項目を取得
				postscript << "GenApi::GetNode(" << item << ")";
				GenApi::CFloatPtr node(nodeMap.GetNode(item.c_str()));
				postscript.str("");

				// 読み込み
				postscript << "GenApi::GetValue()";
				ret = node->GetValue();
				postscript.str("");

				// ログ出力
				postscript << "項目:" << item << Logging::ConstSeparator << "値:" << ret;
				Transfer::Output(Logging::Join(logging, postscript.str()));
				postscript.str("");
			}
			catch (const std::exception& e)
			{
				// 例外の処理
				std::stringstream message;

				message << postscript.str() << Logging::ConstSeparator << e.what();

				// 例外を発砲
				throw std::exception(message.str().c_str());
			}

			return ret;
		}

		////////////////////////////////////////////////////////////////////////////////
		/// @brief			書き込み(文字列)
		/// @param[in]		nodeMap	ノード
		/// @param[in]		item	項目
		/// @param[in]		value	値
		////////////////////////////////////////////////////////////////////////////////
		void CParameter::WriteText(const GenApi::INodeMap& nodeMap, const std::string& item, const std::string& value)
		{
			Logging::CObject logging;
			std::stringstream postscript;

			logging.Message << "Grabber::Parameter::WriteText()" << Logging::ConstSeparator;

			try
			{
				// 書き込み可能を確認
				IsWrite(nodeMap, item);

				// 項目を取得
				postscript << "GenApi::GetNode(" << item << ")";
				GenApi::CEnumerationPtr node(nodeMap.GetNode(item.c_str()));
				postscript.str("");

				// 書き込み
				postscript << "GenApi::FromString(" << value << ")";
				node->FromString(value.c_str());
				postscript.str("");

				// ログ出力
				postscript << "項目:" << item << Logging::ConstSeparator << "値:" << value;
				Transfer::Output(Logging::Join(logging, postscript.str()));
				postscript.str("");
			}
			catch (const std::exception& e)
			{
				// 例外の処理
				std::stringstream message;

				message << postscript.str() << Logging::ConstSeparator << e.what();

				// 例外を発砲
				throw std::exception(message.str().c_str());
			}
		}

		////////////////////////////////////////////////////////////////////////////////
		/// @brief			書き込み(整数値)
		/// @param[in]		nodeMap	ノード
		/// @param[in]		item	項目
		/// @param[in]		value	値
		/// @return			書き込んだint型の値
		////////////////////////////////////////////////////////////////////////////////
		int64_t CParameter::WriteInteger(const GenApi::INodeMap& nodeMap, const std::string& item, const int64_t value)
		{
			int64_t ret = 0;

			Logging::CObject logging;
			std::stringstream postscript;

			logging.Message << "Grabber::Parameter::WriteInteger()" << Logging::ConstSeparator;

			try
			{
				// 書き込み可能を確認
				IsWrite(nodeMap, item);

				// 項目を取得
				postscript << "GenApi::GetNode(" << item << ")";
				GenApi::CIntegerPtr node(nodeMap.GetNode(item.c_str()));
				postscript.str("");

				int64_t shift = 0;

				// インクリメントが有効か確認
				if (node->GetIncMode() != GenApi::EIncMode::noIncrement)
				{
					// インクリメント分をシフトを計算する
					shift = value % node->GetInc();
				}

				// 書き込む値を調整
				ret = value + shift;

				// 書き込み
				postscript << "GenApi::SetValue(" << value << ")";
				node->SetValue(ret);
				postscript.str("");

				// ログ出力
				postscript << "項目:" << item << Logging::ConstSeparator << "値:" << ret;
				Transfer::Output(Logging::Join(logging, postscript.str()));
				postscript.str("");
			}
			catch (const std::exception& e)
			{
				// 例外の処理
				std::stringstream message;

				message << postscript.str() << Logging::ConstSeparator << e.what();

				// 例外を発砲
				throw std::exception(message.str().c_str());
			}

			return ret;
		}
		////////////////////////////////////////////////////////////////////////////////
		/// @brief			書き込み(実数値)
		/// @param[in]		nodeMap	ノード
		/// @param[in]		item	項目
		/// @param[in]		value	値
		/// @return			書き込んだdouble型の値
		////////////////////////////////////////////////////////////////////////////////
		double CParameter::WriteDouble(const GenApi::INodeMap& nodeMap, const std::string& item, const double value)
		{
			double ret = 0;

			Logging::CObject logging;
			std::stringstream postscript;

			logging.Message << "Grabber::Parameter::WriteDouble()" << Logging::ConstSeparator;

			try
			{
				// 書き込み可能を確認
				IsWrite(nodeMap, item);

				// 項目を取得
				postscript << "GenApi::GetNode(" << item << ")";
				GenApi::CFloatPtr node(nodeMap.GetNode(item.c_str()));
				postscript.str("");

				double shift = 0;

				// インクリメントが有効か確認
				if (node->GetIncMode() != GenApi::EIncMode::noIncrement)
				{
					// インクリメント分をシフトを計算する
					//@@@shift = node->GetInc();
				}

				// 書き込む値を調整
				ret = value + shift;

				// 書き込み
				postscript << "GenApi::SetValue(" << ret << ")";
				node->SetValue(ret);
				postscript.str("");

				// ログ出力
				postscript << "項目:" << item << Logging::ConstSeparator << "値:" << ret;
				Transfer::Output(Logging::Join(logging, postscript.str()));
				postscript.str("");
			}
			catch (const std::exception& e)
			{
				// 例外の処理
				std::stringstream message;

				message << postscript.str() << Logging::ConstSeparator << e.what();

				// 例外を発砲
				throw std::exception(message.str().c_str());
			}

			return ret;
		}

		////////////////////////////////////////////////////////////////////////////////
		/// @brief			範囲を読み込み(実数)
		/// @param[in]		nodeMap	ノード
		/// @return			実数値の範囲クラス
		////////////////////////////////////////////////////////////////////////////////
		Condition::CRangeDouble CParameter::ReadRange(const GenApi::INodeMap& nodeMap, const std::string& item)
		{
			Condition::CRangeDouble ret;

			Logging::CObject logging;
			std::stringstream postscript;

			logging.Message << "Grabber::Parameter::ReadRange()" << Logging::ConstSeparator;

			try
			{
				// 読み込み可能を確認
				IsRead(nodeMap, item);

				// 項目を取得
				postscript << "GenApi::GetNode(" << item << ")";
				GenApi::CFloatPtr node(nodeMap.GetNode(item.c_str()));
				postscript.str("");

				// 最小値を取得
				ret.Minimum = node->GetMin();

				// 最大値を取得
				ret.Maximum = node->GetMax();

				// インクリメントを確認
				if (node->GetIncMode() != GenApi::EIncMode::noIncrement)
				{
					// インクリメント有効 ⇒ インクリメントを取得
					ret.Increment = node->GetInc();
				}

				// ログ出力
				postscript << "項目:" << item << Logging::ConstSeparator << "最小:" << ret.Minimum << Logging::ConstSeparator << "最大:" << ret.Maximum << Logging::ConstSeparator << "インクリメント:" << ret.Increment;
				Transfer::Output(Logging::Join(logging, postscript.str()));
				postscript.str("");
			}
			catch (const std::exception& e)
			{
				// 例外の処理
				std::stringstream message;

				message << postscript.str() << Logging::ConstSeparator << e.what();

				// 例外を発砲
				throw std::exception(message.str().c_str());
			}

			return ret;
		}

		////////////////////////////////////////////////////////////////////////////////
		/// @brief			実行
		/// @param[in]		nodeMap	ノード
		/// @param[in]		item	項目
		////////////////////////////////////////////////////////////////////////////////
		void CParameter::Execute(const GenApi::INodeMap& nodeMap, const std::string& item)
		{
			Logging::CObject logging;
			std::stringstream postscript;

			logging.Message << "Grabber::Parameter::Execute()" << Logging::ConstSeparator;

			try
			{
				// 利用可能を確認
				IsAvailable(nodeMap, item);

				// 項目を取得
				postscript << "GenApi::GetNode(" << item << ")";
				GenApi::CCommandPtr node(nodeMap.GetNode(item.c_str()));
				postscript.str("");

				// 完了済みを確認
				if (node->IsDone() == false)
				{
					// 完了
					postscript << Logging::ConstSeparator << "完了済み";

					// 例外を発砲
					throw std::exception(postscript.str().c_str());
				}

				// 実行
				postscript << "GenApi::Execute()";
				node->Execute();
				postscript.str("");

				// ログ出力
				postscript << "項目:" << item;
				Transfer::Output(Logging::Join(logging, postscript.str()));
				postscript.str("");
			}
			catch (const std::exception& e)
			{
				// 例外の処理
				std::stringstream message;

				message << postscript.str() << Logging::ConstSeparator << e.what();

				// 例外を発砲
				throw std::exception(message.str().c_str());
			}
		}
	}
}