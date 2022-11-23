#pragma once
#ifndef SIP_WINDOW_INFO_H_
#define SIP_WINDOW_INFO_H_

namespace sip
{
	/// @brief ウィンドウ情報
	struct WindowInfo
	{
		/// @brief 既定ウィンドウサイズ
		const Size default_size_;

		/// @brief 現在のサイズ / 既定ウィンドウサイズ
		SizeF scale_;
	};
}
#endif // !SIP_WINDOW_INFO_H_
