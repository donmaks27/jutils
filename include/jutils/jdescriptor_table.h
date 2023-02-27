// Copyright © 2023 Leonov Maksim. All Rights Reserved.

#pragma once

#include "jarray.h"
#include "juid.h"

namespace jutils
{
	template<typename T, typename UIDType = uint32>
	class jdescriptor_table
	{
	public:

		using type = T;
		using uid_type = UIDType;

		class weak_pointer
		{
		public:
			constexpr weak_pointer() = default;
			constexpr weak_pointer(nullptr_t) : weak_pointer() {}
			constexpr weak_pointer(const weak_pointer&) = default;
			constexpr weak_pointer(weak_pointer&&) noexcept;
		protected:
			weak_pointer(jdescriptor_table* table, const int32 descriptorIndex, const uid_type uid)
				: descriptorTable(table) , descriptorIndex(descriptorIndex) , UID(uid)
			{}
		public:

			constexpr weak_pointer& operator=(nullptr_t);
			constexpr weak_pointer& operator=(const weak_pointer&) = default;
			constexpr weak_pointer& operator=(weak_pointer&&) noexcept;

			bool isValid() const;

			constexpr bool operator==(const weak_pointer& otherPointer) const
			{
				return (descriptorTable == otherPointer.descriptorTable) && 
					(descriptorIndex == otherPointer.descriptorIndex) && 
					(UID == otherPointer.UID);
			}
			constexpr bool operator!=(const weak_pointer& otherPointer) const { return !this->operator==(otherPointer); }

			bool operator==(nullptr_t) const { return !isValid(); }
			bool operator!=(nullptr_t) const { return isValid(); }

		protected:

			mutable jdescriptor_table* descriptorTable = nullptr;
			int32 descriptorIndex = -1;
			uid_type UID = juid<uid_type>::invalidUID;
		};
		class pointer : public weak_pointer
		{
			friend jdescriptor_table;

		public:
			constexpr pointer() = default;
			constexpr pointer(nullptr_t) : pointer() {}
			pointer(const weak_pointer& otherPointer) : weak_pointer(otherPointer) { _addReference(); }
			constexpr pointer(weak_pointer&& otherPointer) noexcept : weak_pointer(std::move(otherPointer)) { _addReference(); }
			pointer(const pointer& otherPointer) : weak_pointer(otherPointer) { _addReference(); }
			constexpr pointer(pointer&& otherPointer) noexcept : weak_pointer(std::move(otherPointer)) {}
			~pointer() { _removeReference(); }
		private:
			pointer(jdescriptor_table* table, const int32 descriptorIndex, const uid_type uid)
				: weak_pointer(table, descriptorIndex, uid)
			{}
		public:

			pointer& operator=(nullptr_t);
			pointer& operator=(const weak_pointer& otherPointer);
			pointer& operator=(weak_pointer&& otherPointer) noexcept;
			pointer& operator=(const pointer& otherPointer);
			pointer& operator=(pointer&& otherPointer) noexcept;

			type* get() const;
			type* operator->() const { return get(); }
			type& operator*() const { return *get(); }

		private:

			void _addReference();
			void _removeReference();
		};

		constexpr jdescriptor_table() = default;
		jdescriptor_table(const jdescriptor_table&) = delete;
		jdescriptor_table(jdescriptor_table&&) noexcept = delete;
		~jdescriptor_table()
		{
			_destroyObjects();
			descriptors.clear();
			firstEmptyDescriptor = -1;
		}

		jdescriptor_table& operator=(const jdescriptor_table&) = delete;
		jdescriptor_table& operator=(jdescriptor_table&&) noexcept = delete;

		bool isValid(const pointer& pointer) const;
		type* get(const pointer& pointer) const;

		template<typename Type, typename... Args>
		pointer create(Args&&... args);
		pointer createDescriptor(type* existingObject);
		void destroy(const pointer& pointer);
		void cleanup();

		void clear();

	private:

		struct descriptor
		{
			type* object = nullptr;
			uint64 references = 0;
			juid<uid_type> UID;
		};

		jarray<descriptor> descriptors;
		int32 firstEmptyDescriptor = -1;


		int32 _pullEmptyDescriptor();
		void _pushDescriptor(int32 descriptorIndex);

		void _addReference(const pointer& pointer);
		void _removeReference(const pointer& pointer);

		void _destroyObjects();
		void _destroyObject(int32 descriptorIndex);
	};

	template<typename T, typename UIDType>
	int32 jdescriptor_table<T, UIDType>::_pullEmptyDescriptor()
	{
		if (firstEmptyDescriptor == -1)
		{
			descriptors.addDefault();
			return descriptors.getSize() - 1;
		}

		int32 index = firstEmptyDescriptor;
		firstEmptyDescriptor = static_cast<int32>(descriptors[index].references) - 1;
		return index;
	}
	template<typename T, typename UIDType>
	void jdescriptor_table<T, UIDType>::_pushDescriptor(const int32 descriptorIndex)
	{
		descriptor& descriptor = descriptors[descriptorIndex];
		descriptor.UID.generateUID();
		if (descriptor.UID != juid<uid_type>::invalidUID)
		{
			descriptor.references = static_cast<uint64>(firstEmptyDescriptor + 1);
			firstEmptyDescriptor = descriptorIndex;
		}
	}

	template<typename T, typename UIDType>
	void jdescriptor_table<T, UIDType>::_addReference(const pointer& pointer)
	{
		if (isValid(pointer))
		{
			++descriptors[pointer.descriptorIndex].references;
		}
	}
	template<typename T, typename UIDType>
	void jdescriptor_table<T, UIDType>::_removeReference(const pointer& pointer)
	{
		if (isValid(pointer) && descriptors[pointer.descriptorIndex].references > 0)
		{
			--descriptors[pointer.descriptorIndex].references;
		}
	}

	template <typename T, typename UIDType>
	constexpr jdescriptor_table<T, UIDType>::weak_pointer::weak_pointer(weak_pointer&& otherPointer) noexcept
		: descriptorTable(otherPointer.descriptorTable), descriptorIndex(otherPointer.descriptorIndex), UID(otherPointer.UID)
	{
		otherPointer.descriptorTable = nullptr;
		otherPointer.descriptorIndex = -1;
		otherPointer.UID = juid<uid_type>::invalidUID;
	}
	template <typename T, typename UIDType>
	constexpr typename jdescriptor_table<T, UIDType>::weak_pointer& jdescriptor_table<T, UIDType>::weak_pointer::operator=(nullptr_t)
	{
		descriptorTable = nullptr;
		descriptorIndex = -1;
		UID = juid<uid_type>::invalidUID;
		return *this;
	}
	template <typename T, typename UIDType>
	constexpr typename jdescriptor_table<T, UIDType>::weak_pointer& jdescriptor_table<T, UIDType>::weak_pointer::operator=(weak_pointer&& otherPointer) noexcept
	{
		descriptorTable = otherPointer.descriptorTable;
		descriptorIndex = otherPointer.descriptorIndex;
		UID = otherPointer.UID;

		otherPointer.descriptorTable = nullptr;
		otherPointer.descriptorIndex = -1;
		otherPointer.UID = juid<uid_type>::invalidUID;

		return *this;
	}
	template <typename T, typename UIDType>
	bool jdescriptor_table<T, UIDType>::weak_pointer::isValid() const
	{
		if (descriptorTable == nullptr)
		{
			return false;
		}
		if (!descriptorTable->isValid(*this))
		{
			descriptorTable = nullptr;
			return false;
		}
		return true;
	}
	
	template<typename T, typename UIDType>
	typename jdescriptor_table<T, UIDType>::pointer& jdescriptor_table<T, UIDType>::pointer::operator=(nullptr_t)
	{
		_removeReference();
		weak_pointer::operator=(nullptr);
		return *this;
	}
	template<typename T, typename UIDType>
	typename jdescriptor_table<T, UIDType>::pointer& jdescriptor_table<T, UIDType>::pointer::operator=(const weak_pointer& otherPointer)
	{
		if ((this != &otherPointer) && (*this != otherPointer))
		{
			_removeReference();
			weak_pointer::operator=(otherPointer);
			_addReference();
		}
		return *this;
	}
	template<typename T, typename UIDType>
	typename jdescriptor_table<T, UIDType>::pointer& jdescriptor_table<T, UIDType>::pointer::operator=(weak_pointer&& otherPointer) noexcept
	{
		if (*this != otherPointer)
		{
			_removeReference();
			weak_pointer::operator=(std::move(otherPointer));
			_addReference();
		}
		return *this;
	}
	template<typename T, typename UIDType>
	typename jdescriptor_table<T, UIDType>::pointer& jdescriptor_table<T, UIDType>::pointer::operator=(const pointer& otherPointer)
	{
		if ((this != &otherPointer) && (*this != otherPointer))
		{
			_removeReference();
			weak_pointer::operator=(otherPointer);
			_addReference();
		}
		return *this;
	}
	template<typename T, typename UIDType>
	typename jdescriptor_table<T, UIDType>::pointer& jdescriptor_table<T, UIDType>::pointer::operator=(pointer&& otherPointer) noexcept
	{
		if (*this != otherPointer)
		{
			_removeReference();
			weak_pointer::operator=(std::move(otherPointer));
		}
		return *this;
	}

	template <typename T, typename UIDType>
	void jdescriptor_table<T, UIDType>::pointer::_addReference()
	{
		if (weak_pointer::descriptorTable != nullptr)
		{
			weak_pointer::descriptorTable->_addReference(*this);
		}
	}
	template <typename T, typename UIDType>
	void jdescriptor_table<T, UIDType>::pointer::_removeReference()
	{
		if (weak_pointer::descriptorTable != nullptr)
		{
			weak_pointer::descriptorTable->_removeReference(*this);
		}
	}
	
	template<typename T, typename UIDType>
	typename jdescriptor_table<T, UIDType>::type* jdescriptor_table<T, UIDType>::pointer::get() const
	{
		if (weak_pointer::descriptorTable == nullptr)
		{
			return nullptr;
		}
		type* object = weak_pointer::descriptorTable->get(*this);
		if (object == nullptr)
		{
			weak_pointer::descriptorTable = nullptr;
			return nullptr;
		}
		return object;
	}

	template<typename T, typename UIDType>
	bool jdescriptor_table<T, UIDType>::isValid(const pointer& pointer) const
	{
		return (pointer.descriptorTable == this) && descriptors.isValidIndex(pointer.descriptorIndex) && 
			(descriptors[pointer.descriptorIndex].UID == pointer.UID);
	}
	template<typename T, typename UIDType>
	typename jdescriptor_table<T, UIDType>::type* jdescriptor_table<T, UIDType>::get(const pointer& pointer) const
	{
		return isValid(pointer) ? descriptors[pointer.descriptorIndex].object : nullptr;
	}

	template<typename T, typename UIDType>
	template<typename Type, typename ... Args>
	typename jdescriptor_table<T, UIDType>::pointer jdescriptor_table<T, UIDType>::create(Args&&... args)
	{
		return createDescriptor(new Type(std::forward<Args>(args)...));
	}
	template<typename T, typename UIDType>
	typename jdescriptor_table<T, UIDType>::pointer jdescriptor_table<T, UIDType>::createDescriptor(type* existingObject)
	{
		const int32 descriptorIndex = _pullEmptyDescriptor();
		descriptor& descriptor = descriptors[descriptorIndex];
		descriptor.object = existingObject;
		descriptor.references = 1;
		return { this, descriptorIndex, descriptor.UID };
	}
	template<typename T, typename UIDType>
	void jdescriptor_table<T, UIDType>::destroy(const pointer& pointer)
	{
		if (isValid(pointer))
		{
			_destroyObject(pointer.descriptorIndex);
		}
	}

	template<typename T, typename UIDType>
	void jdescriptor_table<T, UIDType>::_destroyObjects()
	{
		for (const auto& descriptor : descriptors)
		{
			delete descriptor.object;
		}
	}
	template<typename T, typename UIDType>
	void jdescriptor_table<T, UIDType>::_destroyObject(const int32 descriptorIndex)
	{
		descriptor& descriptor = descriptors[descriptorIndex];
		if (descriptor.object != nullptr)
		{
			delete descriptor.object;
			descriptor.object = nullptr;
			_pushDescriptor(descriptorIndex);
		}
	}
	template<typename T, typename UIDType>
	void jdescriptor_table<T, UIDType>::cleanup()
	{
		for (int32 descriptorIndex = 0; descriptorIndex < descriptors.getSize(); descriptorIndex++)
		{
			if (descriptors[descriptorIndex].references == 0)
			{
				_destroyObject(descriptorIndex);
			}
		}
	}
	template<typename T, typename UIDType>
	void jdescriptor_table<T, UIDType>::clear()
	{
		for (int32 descriptorIndex = 0; descriptorIndex < descriptors.getSize(); descriptorIndex++)
		{
			_destroyObject(descriptorIndex);
		}
	}
}
