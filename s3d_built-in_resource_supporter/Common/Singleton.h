#ifndef SIP_SINGLETON_H_
#define SIP_SINGLETON_H_

// INCLUDE
#include <cassert>

namespace sip
{
    /**
     * @brief シングルトンアクセス汎用クラス
     * (継承して使う)
     * @tparam T 継承先クラス
     */
    template < typename T >
    class Singleton
    {
    public:
        /**
         * @brief Get the Instance object
         * 
         * @return constexpr T& シングルトンインスタンス
         */
        static constexpr T& getInstance()
        {
            if (m_pInstance == nullptr)
            {
                create();
            }
            return *m_pInstance;
        }

        /**
         * @brief シングルトンインスタンスの破棄
         * 
         */
        static void release()
        {
            if (m_pInstance)
            {
                delete m_pInstance;
                m_pInstance = nullptr;
            }
        }

    protected:

        /**
         * @brief Construct a new Singleton object
         * 
         */
        Singleton() {}

        /**
         * @brief Destroy the Singleton object
         * 
         */
        virtual ~Singleton() { release(); }

        /**
         * @brief シングルトンインスタンスの生成
         * 
         */
        static void create()
        {
            assert(!m_pInstance);
            if (m_pInstance)
            {
                return;
            }
            m_pInstance = new T();
        }

    private:

        /*! コピー、ムーブコンストラクタの削除 */
        Singleton(const Singleton&) = delete;
        Singleton(Singleton&&) = delete;

        static T* m_pInstance; //!< シングルトンインスタンス
    };

    /*! staticメンバの初期化 */
    template < typename T >
    T* Singleton<T>::m_pInstance = nullptr;
}

#endif // !SIP_SINGLETON_H_
