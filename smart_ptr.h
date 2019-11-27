#ifndef SMART_PTR_H
#define SMART_PTR_H
#include <utility>

template <typename T>
class smart_ptr 
{
private:
	T* ptr_;
	
//	// noncopyable 
//	smart_ptr(const smart_ptr& ptr) 
//		= delete;
//	smart_ptr& operator=(const smart_ptr& ptr) 
//		= delete;

public:
	explicit smart_ptr(T* ptr = nullptr)
		: ptr_(ptr) {}
	virtual ~smart_ptr() {
		delete ptr_;
	}
    
	// get real ptr 
	T* get() const {
		return ptr_;
	}

	T& operator*() const {return *ptr_;}
	T* operator->() const {return ptr_;}

	template <typename U>
	smart_ptr(smart_ptr<U>&& ptr) {ptr_ = ptr.release();}
	smart_ptr& operator=(smart_ptr rhs) {
		rhs.swap(*this);
		return *this;
	}

	T* release() {
		T* ptr = ptr_;
		ptr_ = nullptr;
		return ptr;
	}

	void swap(smart_ptr &rhs) {
		using std::swap;
		swap(ptr_, rhs.ptr_);
	}operator bool() const {return ptr_;}
};

#endif /* SMART_PTR_H */	
