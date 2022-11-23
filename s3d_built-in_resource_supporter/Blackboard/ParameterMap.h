#ifndef SIP_PARAMETERMAP_H_
#define SIP_PARAMETERMAP_H_

// INCLUDE
#include <cassert>
#include <unordered_map>
#include <string>
#include <memory>

namespace sip
{
    /*! パラメータ検索用キー */
    using ParameterMapKeyType = std::string;

    /**
     * @brief 変数マップクラス
     * 
     * @tparam T マップ管理する変数の型
     */
    template < typename T >
    class ParameterMap
    {
    public:
        /*! 名前の置き換え */
        using Parameter = T;

    public:
        /**
         * @brief Construct a new Parameter Map object
         * 
         */
        ParameterMap() : m_parameters() {}

        /**
         * @brief Destroy the Parameter Map object
         * 
         */
        ~ParameterMap() { m_parameters.clear(); }

        /**
         * @brief 変数をマップへ挿入する
         * 
         * @param key 呼び出すために紐づけるキー
         * @param value 登録する変数
         * @return ParameterMap& 自身
         */
        ParameterMap& insert(const ParameterMapKeyType& key, Parameter& value)
        {
            m_parameters.insert(std::pair<ParameterMapKeyType, Parameter&>(key, value));
            return *this;
        }

        /**
         * @brief 変数をマップから削除する
         * 
         * @param key 削除する変数に紐づいたキー
         * @return ParameterMap& 自身
         */
        ParameterMap& erase(const ParameterMapKeyType& key)
        {
            m_parameters.erase(key);
            return *this;
        }

        /**
         * @brief キーから変数を取得する
         * (登録されていないキーで呼び出すとアサートにかかります。)
         * @param key 変数に紐づいたキー
         * @return Parameter& 対応した変数
         */
        Parameter& get(const ParameterMapKeyType& key)
        {
            auto it = m_parameters.find(key);
            assert(!(it == m_parameters.end()));
            return it->second;
        }

        /**
         * @brief キーから変数を取得する(const版)
         * (登録されていないキーで呼び出すとアサートにかかります。)
         * @param key 変数に紐づいたキー
         * @return Parameter& 対応した変数
         */
        const Parameter& get(const ParameterMapKeyType& key) const
        {
            auto it = m_parameters.find(key);
            assert(!(it == m_parameters.end()));
            return it->second;
        }

        /**
         * @brief キーから変数を取得する
         * (登録されていないキーで呼び出すとアサートにかかります。)
         * @param key 変数に紐づいたキー
         * @return Parameter& 対応した変数
         */
        Parameter& operator[] (const ParameterMapKeyType& key)
        { 
            return get(key);
        }

        /**
         * @brief キーから変数を取得する(const版)
         * (登録されていないキーで呼び出すとアサートにかかります。)
         * @param key 変数に紐づいたキー
         * @return Parameter& 対応した変数
         */
        const Parameter& operator[] (const ParameterMapKeyType& key) const 
        {
            return get(key);
        }

        /**
         * @brief マップに登録されている変数の数を取得する
         * 
         * @return size_t 変数の登録数
         */
        size_t getSize() const
        { 
            return m_parameters.size();
        }

        /**
         * @brief マップ本体を取得する
         * 
         * @return std::unordered_map<ParameterMapKeyType, Parameter>& 本体
         */
        std::unordered_map<ParameterMapKeyType, Parameter>& parameters()
        { 
            return m_parameters;
        }

    private:

        std::unordered_map< ParameterMapKeyType, Parameter > m_parameters; //!< 変数登録マップ
    };

    /*! 名前の置き換え */
    template < typename T > 
    using ParameterMapPtr = std::unique_ptr<ParameterMap<T>>;
}

#endif // !SIP_PARAMETERMAP_H_
