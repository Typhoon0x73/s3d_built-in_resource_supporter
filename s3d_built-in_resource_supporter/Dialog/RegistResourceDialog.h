#pragma once
#include "IDialog.h"

namespace sip
{
	/// @brief 
	class RegistResourceDialog : public IDialog
	{
	public:

		/// @brief 
		enum class Result : std::int32_t
		{
			Failed = -1,
			None,
			Cancel,
			Success,
		};

		/// @brief 
		struct EditInfo
		{
			/// @brief 
			FilePath path{ U"" };

			/// @brief 
			String tag{ U"" };

			/// @brief 
			FilePath filter{ U"Resource Files" };

			/// @brief 
			String extensions{ U"" };
		};

	public:

		/// @brief
		/// @param edit 
		explicit RegistResourceDialog() noexcept;

		/// @brief 
		virtual ~RegistResourceDialog() noexcept override;

		/// @brief 
		/// @return 
		[[nodiscard]] virtual bool update() noexcept override;

		/// @brief 
		virtual void draw() /* const */ noexcept override;

		/// @brief
		/// @return 
		[[nodiscard]] virtual bool isModal() const noexcept override;

		/// @brief 
		/// @return 
		[[nodiscard]] virtual bool isEnableCover() const noexcept override;

		/// @brief 
		/// @return 
		[[nodiscard]] virtual std::int32_t result() const noexcept override;

	private:

		/// @brief 
		void load_resource() noexcept;

	private:

		/// @brief 
		Result result_;

		/// @brief 
		EditInfo edit_;

		/// @brief 
		ListBoxState tag_list_box_state_;

		/// @brief 
		ListBoxState filter_list_box_state_;

		/// @brief 
		RectF load_resource_button_region_;

		/// @brief 
		RectF add_resource_button_region_;

		/// @brief 
		RectF cancel_resource_button_region_;
	};
}

