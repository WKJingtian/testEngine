#pragma once

namespace tengine
{
	template<typename T>
	class ownedPtr;
	template<typename T>
	class weakPtr;
	class controller;
	template<typename T>
	class default_controller;
	template<typename Ta, typename Tb>
	class regular_controller;
	template<typename T>
	class temp_control;


	class controller
	{
	private:
		int cnt = 1;
		template<typename T>
		friend class weakPtr;
		template<typename T>
		friend class ownedPtr;
	protected:
		void inc() noexcept { cnt++; }
		void weak_dec()
		{
			if (cnt > 0 && --cnt == 0) { destroy(); delete this; }
			else if (cnt < 0 && ++cnt == 0) { delete this; }
		}
		void owned_dec()
		{
			destroy();
			if (--cnt == 0) delete this;
			else cnt = cnt * -1;
		}
		int report_cnt() { return cnt; }
	public:
		virtual ~controller() = default;
		virtual void destroy() = 0;
	};

	template<typename T>
	class default_controller : public controller
	{
	protected:
		T* obj;
	public:
		void destroy() override { if (obj != nullptr) delete obj; }
		default_controller(T* obj) noexcept : obj(obj) {}
	};

	template<typename Ta, typename Tb>
	class regular_controller : public default_controller<Ta>, private Tb
	{
	public:
		regular_controller(Ta* ptr, Tb deleter) : default_controller<Ta>(ptr), Tb(std::move(deleter)) {}
		void destroy() override { if (default_controller<Ta>::obj != nullptr) Tb::operator()(default_controller<Ta>::obj); }
	};

	template<typename T>
	class temp_control : public controller
	{
	private:
		template<typename Ty, typename ...Args>
		friend ownedPtr<Ty> make_owned(Args &&...);
		std::aligned_storage_t<sizeof(T), alignof(T)> obj;
		T* get_obj() noexcept { return reinterpret_cast<T*>(&obj); }
	protected:
		void destroy() override { if (get_obj() != nullptr) get_obj()->~T(); }
	public:
		template<typename ...Args>
		explicit temp_control(Args ...args) { new(&obj) T(std::forward<Args>(args)...); }
	};

	// ownedPtr implementation
	// cannot copy, can move
	template<typename T>
	class ownedPtr// : public my_base_ptr
	{
	protected:
		T* ptr;
		controller* owner;
	public:
		ownedPtr(controller* o, T* obj) : owner(o), ptr(obj) {}
		ownedPtr() :ptr(nullptr) { owner = new default_controller<T>(ptr); };
		explicit ownedPtr(T* obj) : ptr(obj) { owner = new default_controller<T>(ptr); }
		ownedPtr(ownedPtr&& obj) : ptr(obj.ptr), owner(obj.owner) { obj.release(); }
		void release() { ptr = nullptr; owner = new default_controller<T>(ptr); }
		void reset(T* target) { owner->owned_dec(); ptr = target; owner = new default_controller<T>(ptr); }
		void reset(ownedPtr<T> target) { owner->owned_dec(); ptr = target.ptr; owner = new default_controller<T>(ptr); }
		void reset() { owner->owned_dec(); ptr = nullptr; owner = new default_controller<T>(ptr); }
		ownedPtr<T>& operator=(ownedPtr<T>&& obj)
		{
			owner->owned_dec();
			owner = obj.owner;
			ptr = obj.ptr;
			obj.ptr = nullptr;
			obj.owner = new default_controller<T>(obj.ptr);
			return *this;
		}
		bool operator==(const ownedPtr<T>& other) const
		{
			return other.get() == this->get();
		}
		T operator*() const { return *ptr; }
		T* operator->() const { return ptr; }
		T* get() const { return ptr; }
		~ownedPtr() { owner->owned_dec(); }
		void report_cnt() { cout << "owned pointer report its cnt: " << (*owner).report_cnt() << '\n'; }
		void report_pos() { cout << "owned pointer report its position: " << ptr << '\n'; }
	private:
		friend class weakPtr<T>;
	};

	template<typename T>
	class weakPtr
	{
	protected:
		T* ptr;
		controller* owner;
	public:
		weakPtr() :ptr(nullptr) { owner = new default_controller<T>(ptr); };
		explicit weakPtr(T* obj) : ptr(obj) { owner = new default_controller<T>(ptr); }
		weakPtr(const ownedPtr<T>& obj) : ptr(obj.ptr), owner(obj.owner) { owner->inc(); }
		weakPtr(ownedPtr<T>&& obj) : ptr(obj.ptr), owner(obj.owner) { owner->inc(); }
		weakPtr(const weakPtr<T>& obj) : ptr(obj.ptr), owner(obj.owner) { owner->inc(); }
		weakPtr(weakPtr<T>&& obj) : ptr(obj.ptr), owner(obj.owner) { owner->inc(); }
		void reset(const ownedPtr<T>& target) { owner->weak_dec(); ptr = target.ptr; owner = target.owner; }
		void reset(const weakPtr<T>& target) { owner->weak_dec(); ptr = target.ptr; owner = target.owner; }
		weakPtr<T>& operator=(const weakPtr<T>& obj)
		{
			reset(obj);
			owner->inc();
			return *this;
		}
		weakPtr<T>& operator=(const ownedPtr<T>& obj)
		{
			reset(obj);
			owner->inc();
			return *this;
		}
		bool operator==(const ownedPtr<T>& other) const
		{
			return other.get() == this.get();
		}
		bool operator==(const weakPtr<T>& other) const
		{
			return other.get() == this.get();
		}
		T operator*() const { return *ptr; }
		T* operator->() const { return ptr; }
		T* get() { return ptr; }
		bool is_valid() { return (owner->cnt > 0) && (ptr != nullptr); }
		~weakPtr() { owner->weak_dec(); }
		void report_cnt() { cout << "owned pointer report its cnt: " << (*owner).report_cnt() << '\n'; }
		void report_pos() { cout << "weak pointer report its position: " << ptr << '\n'; }
	private:
		friend class ownedPtr<T>;
	};

	template<typename T, typename ...Args>
	inline ownedPtr<T> make_owned(Args&& ... args)
	{
		auto* temp = new temp_control<T>(std::forward<Args>(args)...);
		return ownedPtr<T>(static_cast<controller*>(temp), temp->get_obj());
	}
}