#ifndef SIP_BLACKBOARD_H_
#define SIP_BLACKBOARD_H_

// INCLUDE
#include "ParameterMap.h"
#include <tuple>

namespace sip
{
    /**
     * @brief 暗黒変数掲示板クラス
     * 
     * @tparam Types 登録する変数の型
     */
    template < typename... Types >
    class Blackboard
    {
    public:
        /**
         * @brief Construct a new Blackboard object
         * 
         */
        Blackboard() : m_elements() {}

        /**
         * @brief Destroy the Blackboard object
         * 
         */
        ~Blackboard() {}

        /**
         * @brief 暗黒変数マップを取得する
         * 
         * @tparam T 取得する暗黒変数マップの型
         * @return ParameterMapPtr<T>& 変数群
         */
        template < typename T >
        ParameterMapPtr<T>& get()
        {
            constexpr unsigned int INDEX = FindElement<ParameterMapPtr<T>>::value;
            auto& result = std::get<INDEX>(m_elements);
            if (result == nullptr)
            {
                result = std::make_unique<ParameterMap<T>>();
            }
            return result;
        }

        /**
         * @brief 暗黒変数マップを取得する
         * 
         * @tparam T 取得する暗黒変数マップの型
         * @return ParameterMapPtr<T>& 変数群
         */
        template < typename T >
        const ParameterMapPtr<T>& get() const
        {
            return const_cast<Blackboard*>(this)->get<T>();
        }

        /**
         * @brief マップに登録されている変数の数を取得する
         * 
         * @tparam T 取得するマップの型
         * @return std::size_t マップ登録数
         */
        template < typename T >
        std::size_t getSize() const
        {
            return get<T>()->getSize();
        }

    private:

        std::tuple<ParameterMapPtr<Types>...> m_elements; //!< 暗黒マップ群

        /*! データタイプ置き換え */
        template < unsigned int Index >
        using ElementType = typename std::tuple_element<Index, decltype(m_elements)>::type;

        /*! エレメントチェック置き換え */
        template < typename T, unsigned int Index >
        using IsElement = std::is_same<ElementType<Index>, T>;

        /*! エレメントの検索構造体 */
        template < typename T, unsigned int Index = 0 >
        struct FindElement : public std::conditional_t<
            IsElement<T, Index>::value,
            std::integral_constant< decltype(Index), Index>,
            FindElement<T, Index + 1>
        >{};
    };

    /*! 名前の置き換え */
    template < typename... Types >
    using BlackboardPtr = std::unique_ptr<Blackboard<Types...>>;
}

#endif // !SIP_BLACKBOARD_H_
