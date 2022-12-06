#pragma once
#include "IDialog.h"
#include "../ComboBox/SimpleComboBox.h"

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
		virtual void draw() const noexcept override;

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
		RectF base_rect_;

		/// @brief 
		Font* font_;

		/// @brief 
		RectF path_rect_;

		/// @brief 
		RectF load_rect_;

		/// @brief 
		RectF cancel_rect_;

		/// @brief 
		RectF regist_rect_;

		/// @brief 
		Array<String> tag_names_;

		/// @brief 
		Array<String> filter_names_;

		/// @brief 
		SimpleComboBox filter_combo_box_;

		/// @brief 
		SimpleComboBox tag_combo_box_;

		/// @brief 
		Texture open_file_texture_;
	};
}

