#ifndef SMART_PTR_H
#define SMART_PTR_H
#include <utility>
#include <atomic>

class shared_count {
public:
    shared_count() 
        : count_(1)
    {}
    ~shared_count() {}
    void add_count() { count_++; }
    long reduce_count() {return --count_; }
    long get_count() {return count_;}
private:
	std::atomic<long> count_;
};

template <typename T>
class smart_ptr 
{
private:
	T* ptr_;
    shared_count* shared_count_;	

public:
	explicit smart_ptr(T* ptr = nullptr)
        : ptr_(ptr) {
            if(ptr) {
                shared_count_ = 
                    new shared_count();
            }
        }

    // smart_ptr(smart_ptr<U>& other, T* ptr) for case function
    template <typename U>
    smart_ptr(const smart_ptr<U>& other, T* ptr) noexcept
    {
        ptr_ = ptr;
        if (ptr_) {
            other.shared_count_
                ->add_count();
            shared_count_ = 
                other.shared_count_;
        }
    }

	virtual ~smart_ptr() {
        if (ptr_ && !shared_count_->reduce_count()) {
            delete shared_count_;
            delete ptr_;
        }
	}

    template <typename U>
    friend class smart_ptr;
    
	// get real ptr 
	T* get() const {
		return ptr_;
	}

    operator bool() const {return ptr_;}
	T& operator*() const {return *ptr_;}
	T* operator->() const {return ptr_;}

    template <typename U>
    smart_ptr(const smart_ptr<U> &other) {
        ptr_ = other.ptr_;
        if (ptr_) {
            other.shared_count_->add_count();
            shared_count_ = 
                other.shared_count_;
        }
    }

	template <typename U>
	smart_ptr(smart_ptr<U>&& other) {
        ptr_ = other.ptr_;
        if (ptr_) {
            shared_count_ = 
                other.shared_count_;
            other.ptr_ = nullptr;
        }
    }
	smart_ptr& operator=(smart_ptr rhs) {
		rhs.swap(*this);
		return *this;
	}

    long use_count() const {
        if (ptr_) {
            return shared_count_
                ->get_count();
        } else {
            return 0;
        }
    }

	void swap(smart_ptr &rhs) {
		using std::swap;
		swap(ptr_, rhs.ptr_);
        swap(shared_count_, rhs.shared_count_);
	}
};

// swap swap two smart_pointer 
template <typename T>
void swap(smart_ptr<T>& lhs, 
        smart_ptr<T>& rhs) noexcept 
{
    lhs.swap(rhs);
}

// static_pointer_cast 
template <typename T, typename U>
smart_ptr<T> static_pointer_cast(
        const smart_ptr<U>& other) noexcept 
{
    T* ptr = static_cast<T*>(other.get());
    return smart_ptr<T>(other, ptr);
}

// dynamic_pointer_cast 
template <typename T, typename U> 
smart_ptr<T> dynamic_pointer_cast(
        const smart_ptr<U>& other
        ) noexcept 
{
    T* ptr = dynamic_cast<T*>(other.get());
    return smart_ptr<T>(other, ptr);
}

// reinterpret_pointer_cast 
template <typename T, typename U>
smart_ptr<T> reinterpret_pointer_cast(
        const smart_ptr<U>& other
        ) noexcept 
{
    T* ptr = reinterpret_cast<T*>(other.get());
    return smart_ptr<T>(other, ptr);
}

// const_pointer_cast  
template <typename T, typename U>
smart_ptr<T> const_pointer_cast(
        const smart_ptr<U>& other
        ) noexcept
{
    T* ptr = const_cast<T*>(other.get());
    return smart_ptr<T>(other, ptr);
}

#endif /* SMART_PTR_H */	
