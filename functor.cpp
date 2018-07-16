// functor.cpp : アプリケーションのエントリ ポイントを定義します。
//

#include "stdafx.h"

class cTest
{
    int mParam;
    int mParam2;
public:
    cTest()
        : mParam(), mParam2() {}
    cTest(const int param)
        : mParam(param), mParam2(param) {}

    void Set(const int& param) { mParam = param; printf("%s const arg: %d\n", __FUNCTION__, mParam); }
    int Get(void) const { printf("%s const\n", __FUNCTION__); return mParam; }

    void Set2(const int& param) { mParam2 = param; printf("%s const arg: %d\n", __FUNCTION__, mParam2); }
    int Get2(void) const { printf("%s const\n", __FUNCTION__); return mParam2; }
};




//----------------------------------------------------------------------------
// ターゲットクラス（利便性を考えて用意した）
template<class T>
class TargetPtr
{
protected:
    T* mTarget;

public:
    TargetPtr(void)
        : mTarget() {}
    TargetPtr(T* obj)
        : mTarget(obj) {}

    T* Target(void) { return mTarget; }
    const T* ConstTarget(void) const { return mTarget; }
};






//----------------------------------------------------------------------------
// 静的メソッド用ゲッターファンクター
template<class T>
class StaticGetFunctor
{
public:
    using FUNCTION_T = T* (*)();

protected:
    FUNCTION_T mfGet;

public:
    StaticGetFunctor(void)
        : mfGet() {}
    StaticGetFunctor(FUNCTION_T fn)
        : mfGet(fn) {}

    T* Get(void) { return mfGet(); }
    const T* ConstGet(void) const { return mfGet(); }
};






//----------------------------------------------------------------------------
// クラスメソッド用ゲッターファンクター
template<class T, class A>
class GetFunctor
    : public TargetPtr<T>
{
public:
    using FUNCTION_T = A(T::*)() const;

protected:
    FUNCTION_T mfGet;

public:
    GetFunctor(void)
        : TargetPtr<T>()
        , mfGet() {}
    GetFunctor(
        T* obj,
        FUNCTION_T fn)
        : TargetPtr<T>(obj)
        , mfGet(fn) {}

    A operator()(void) const { return (TargetPtr<T>::ConstTarget()->*mfGet)(); }
};

//----------------------------------------------------------------------------
// クラスメソッド用セッターファンクター
template<class T, class A>
class SetFunctor
    : public TargetPtr<T>
{
public:
    using FUNCTION_T = void (T::*)(const A&);

protected:
    FUNCTION_T mfSet;

public:
    SetFunctor(void)
        : TargetPtr<T>()
        , mfSet() {}
    SetFunctor(
        T* obj,
        FUNCTION_T fn)
        : TargetPtr<T>(obj)
        , mfSet(fn) {}

    SetFunctor& operator()(const A& a) { (TargetPtr<T>::Target()->*mfSet)(a); return *this; }
};





//----------------------------------------------------------------------------
// ゲッターセッターを呼び出すファンクター
template<class T, class A>
class GetSetFunctor
{
public:
    using GET_FUNCTION_T = typename GetFunctor<T, A>::FUNCTION_T;
    using SET_FUNCTION_T = typename SetFunctor<T, A>::FUNCTION_T;

protected:
    GetFunctor<T, A>   mfGet;
    SetFunctor<T, A>   mfSet;

public:
    GetSetFunctor()
        : mfGet()
        , mfSet() {}
    GetSetFunctor(
        T* obj,                 // ターゲットインスタンス
        GET_FUNCTION_T getFn,   // ゲッターメソッド
        SET_FUNCTION_T setFn)   // セッターメソッド
        : mfGet(obj, getFn)
        , mfSet(obj, setFn) {}

    A operator()(void) const { return mfGet(); }
    GetSetFunctor& operator()(const A& a) { mfSet(a); return *this; }
};






//----------------------------------------------------------------------------
// シングルトンからインスタンスを取得し、ゲッターセッターを呼び出すファンクター
template<class T, class A>
class StaticInstanceGetSetFunctor
{
public:
    using STATIC_GET_FUNCTION_T = typename StaticGetFunctor<T>::FUNCTION_T;
    using GET_FUNCTION_T = A(T::*)() const;
    using SET_FUNCTION_T = void (T::*)(const A&);

protected:
    StaticGetFunctor<T>    mGetInstanceFn;
    GET_FUNCTION_T         mfGet;
    SET_FUNCTION_T         mfSet;

public:
    StaticInstanceGetSetFunctor()
        : mGetInstanceFn(), mfGet(), mfSet() {}
    StaticInstanceGetSetFunctor(
        STATIC_GET_FUNCTION_T getInstanceFn,    // ターゲットインスタンス取得ゲッターメソッド
        GET_FUNCTION_T getFn,                   // ゲッターメソッド
        SET_FUNCTION_T setFn)                   // セッターメソッド
        : mGetInstanceFn(getInstanceFn)
        , mfGet(getFn)
        , mfSet(setFn) {}

    A operator()(void) const { return (mGetInstanceFn.ConstGet()->*mfGet)(); }
    StaticInstanceGetSetFunctor& operator()(const A& a) { (mGetInstanceFn.Get()->*mfSet)(a); return *this; }
};






//----------------------------------------------------------------------------
// Instance でもインスタンスを取得できるように外部に定義（シングルトンから取得できるイメージ）
cTest        a;
static cTest* Instance(void) { return &a; }


//----------------------------------------------------------------------------
int main()
{
    SetFunctor<cTest, int>  ffnc(&a, &cTest::Set);
    GetFunctor<cTest, int>  ffnc2(&a, &cTest::Get);

    ffnc(55);
    printf("%d\n", ffnc2());

    GetSetFunctor<cTest, int>               gsFunc(&a, &cTest::Get, &cTest::Set);
    GetSetFunctor<cTest, int>               gs2Func(&a, &cTest::Get2, &cTest::Set2);
    StaticInstanceGetSetFunctor<cTest, int> siGsFunc(Instance, &cTest::Get, &cTest::Set);
    StaticInstanceGetSetFunctor<cTest, int> siGs2Func(Instance, &cTest::Get2, &cTest::Set2);

    gsFunc(123);
    gs2Func(321);
    printf("Set: %d\n", gsFunc());
    printf("Set2: %d\n", gs2Func());
    siGsFunc(999);
    printf("Set: %d\n", siGsFunc());
    printf("Set2: %d\n", siGs2Func());

    return 0;
}

