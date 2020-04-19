#pragma once
template <typename T>
class SingletonBase
{
protected:
	static T* mInstance;

	SingletonBase() {};
	virtual ~SingletonBase() {};

	virtual friend class SingletonBase<T>;
public:
	static T* Get(void);
	static void Delete(void);
};

template <typename T>
T* SingletonBase<T>::mInstance = nullptr;

template <typename T>
T* SingletonBase<T>::Get(void)
{
	if (SingletonBase<T>::mInstance == nullptr)
		SingletonBase<T>::mInstance = new T;
	return SingletonBase<T>::mInstance;
}


template<typename T>
inline void SingletonBase<T>::Delete(void)
{
	if (SingletonBase<T>::mInstance)
	{
		delete SingletonBase<T>::mInstance;
		SingletonBase<T>::mInstance = nullptr;
	}
}


