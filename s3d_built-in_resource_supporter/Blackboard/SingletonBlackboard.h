#ifndef SIP_SINGLETONBLACKBOARD_H_
#define SIP_SINGLETONBLACKBOARD_H_

// INCLUDE
#include "Blackboard.h"
#include "../Common/Singleton.h"

namespace sip
{
    /**
     * @brief ブラックボードのシングルトンアクセスクラス
     * 
     * @tparam Types 暗黒変数掲示板クラスへ登録する変数の型
     */
    template < typename... Types >
    class SingletonBlackboard : public Singleton<Blackboard<Types...>>
    {
        friend class Singleton<Blackboard<Types...>>;
    };

    /*! 簡易アクセス用マクロ */
    #define g_pGetBlackboard(x)    SingletonBlackboard< x >::getInstance().get< x >()
    #define g_BlackboardRelease(x) SingletonBlackboard< x >::release()
}

#endif // !SIP_SINGLETONBLACKBOARD_H_
