// Copyright © 2023 Leonov Maksim. All Rights Reserved.

#pragma once

#include "jdelegate_multicast.h"
#include "juid.h"

namespace jutils
{
	template<typename T, typename UIDType = uint32>
	class jdescriptor_table;

	template<typename UIDType = uint32>
	class jdescriptor_table_pointer
	{
		template<typename T, typename UIDType1>
		friend class jdescriptor_table;

	public:

		using uid_type = UIDType;

		constexpr jdescriptor_table_pointer() = default;
		constexpr jdescriptor_table_pointer(nullptr_t) : jdescriptor_table_pointer() {}
		constexpr jdescriptor_table_pointer(const jdescriptor_table_pointer&) = default;
	protected:
		constexpr jdescriptor_table_pointer(const int32 descriptorIndex, const uid_type uid)
			: descriptorIndex(descriptorIndex), UID(uid)
		{}
	public:

		constexpr jdescriptor_table_pointer& operator=(nullptr_t);
		constexpr jdescriptor_table_pointer& operator=(const jdescriptor_table_pointer&) = default;

		constexpr bool operator==(const jdescriptor_table_pointer& otherPointer) const
		{
			return (descriptorIndex == otherPointer.descriptorIndex) && (UID == otherPointer.UID);
		}
		constexpr bool operator!=(const jdescriptor_table_pointer& otherPointer) const { return !this->operator==(otherPointer); }

		constexpr bool operator<(const jdescriptor_table_pointer& otherPointer) const;

	protected:

		int32 descriptorIndex = -1;
		uid_type UID = juid<uid_type>::invalidUID;
	};

	template<typename T, typename UIDType>
	class jdescriptor_table
	{
	public:

		using type = T;
		using uid_type = UIDType;
		using base_poiter_type = jdescriptor_table_pointer<uid_type>;

		class weak_pointer : public base_poiter_type
		{
			friend jdescriptor_table;

		public:
			constexpr weak_pointer() = default;
			constexpr weak_pointer(nullptr_t) : weak_pointer() {}
			constexpr weak_pointer(const weak_pointer&) = default;
		protected:
			weak_pointer(jdescriptor_table* table, const int32 descriptorIndex, const uid_type uid)
				: base_poiter_type(descriptorIndex, uid), descriptorTable(table)
			{}
		public:

			constexpr weak_pointer& operator=(nullptr_t);
			constexpr weak_pointer& operator=(const weak_pointer&) = default;
			
			constexpr bool operator==(const weak_pointer& otherPointer) const
			{
				return (descriptorTable == otherPointer.descriptorTable) && base_poiter_type::operator==(otherPointer);
			}
			constexpr bool operator!=(const weak_pointer& otherPointer) const { return !this->operator==(otherPointer); }
			
			constexpr bool operator<(const weak_pointer& otherPointer) const
			{
				return (descriptorTable < otherPointer.descriptorTable) || base_poiter_type::operator<(otherPointer);
			}

		protected:

			jdescriptor_table* descriptorTable = nullptr;
		};
		class pointer : public weak_pointer
		{
			friend jdescriptor_table;

		public:
			constexpr pointer() = default;
			constexpr pointer(nullptr_t) : pointer() {}
			pointer(const pointer& otherPointer) : weak_pointer(otherPointer) { _addReference(); }
			constexpr pointer(pointer&& otherPointer) noexcept;
			pointer(const weak_pointer& otherPointer) : weak_pointer(otherPointer) { _addReference(); }
			~pointer() { _removeReference(); }
		private:
			pointer(jdescriptor_table* table, const int32 descriptorIndex, const uid_type uid)
				: weak_pointer(table, descriptorIndex, uid)
			{}
		public:

			pointer& operator=(nullptr_t);
			pointer& operator=(const pointer& otherPointer);
			pointer& operator=(const weak_pointer& otherPointer);
			pointer& operator=(pointer&& otherPointer) noexcept;

		private:

			void _addReference();
			void _removeReference();
		};
		
		JUTILS_CREATE_MULTICAST_DELEGATE1(OnObjectEvent, type*, object);
		
		constexpr jdescriptor_table() = default;
		jdescriptor_table(const jdescriptor_table&) = delete;
		jdescriptor_table(jdescriptor_table&&) noexcept = delete;
		~jdescriptor_table()
		{
			for (const auto& descriptor : descriptors)
			{
				delete descriptor.object;
			}
			descriptors.clear();
			firstEmptyDescriptor = -1;
		}

		jdescriptor_table& operator=(const jdescriptor_table&) = delete;
		jdescriptor_table& operator=(jdescriptor_table&&) noexcept = delete;

		OnObjectEvent onObjectDestroying;


		bool isValid(const base_poiter_type& pointer) const
		{
			return descriptors.isValidIndex(pointer.descriptorIndex) && (descriptors[pointer.descriptorIndex].UID == pointer.UID);
		}
		uint64 getRefsCount(const base_poiter_type& pointer) const
		{
			return this->isValid(pointer) ? descriptors[pointer.descriptorIndex].references : 0;
		}
		type* get(const base_poiter_type& pointer) const
		{
			return this->isValid(pointer) ? descriptors[pointer.descriptorIndex].object : nullptr;
		}

		template<typename Type, typename... Args>
		base_poiter_type create(Args&&... args) { return this->createDescriptor(new Type(std::forward<Args>(args)...)); }
		base_poiter_type createDescriptor(type* existingObject);

		bool addReference(const base_poiter_type& pointer);
		bool removeReference(const base_poiter_type& pointer);
		weak_pointer createWeakPointer(const base_poiter_type& pointer)
		{
			return this->isValid(pointer) ? weak_pointer(this, pointer.descriptorIndex, pointer.UID) : nullptr;
		}
		pointer createPointer(const base_poiter_type& pointer);

		void destroy(const base_poiter_type& pointer);
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
		void _destroyObject(int32 descriptorIndex);
	};

	template<typename UIDType>
	constexpr jdescriptor_table_pointer<UIDType>& jdescriptor_table_pointer<UIDType>::operator=(nullptr_t)
	{
		descriptorIndex = -1;
		UID = juid<uid_type>::invalidUID;
		return *this;
	}
	template<typename UIDType>
	constexpr bool jdescriptor_table_pointer<UIDType>::operator<(const jdescriptor_table_pointer& otherPointer) const
	{
		if (descriptorIndex < otherPointer.descriptorIndex)
		{
			return true;
		}
		return (descriptorIndex == otherPointer.descriptorIndex) && (UID < otherPointer.UID);
	}
	
	template<typename T, typename UIDType>
	constexpr typename jdescriptor_table<T, UIDType>::weak_pointer& jdescriptor_table<T, UIDType>::weak_pointer::operator=(nullptr_t)
	{
		descriptorTable = nullptr;
		base_poiter_type::operator=(nullptr);
		return *this;
	}

	template<typename T, typename UIDType>
    constexpr jdescriptor_table<T, UIDType>::pointer::pointer(pointer&& otherPointer) noexcept
		: weak_pointer(otherPointer)
	{
		otherPointer.descriptorTable = nullptr;
		otherPointer.descriptorIndex = -1;
		otherPointer.UID = juid<uid_type>::invalidUID;
	}

	template<typename T, typename UIDType>
	typename jdescriptor_table<T, UIDType>::pointer& jdescriptor_table<T, UIDType>::pointer::operator=(nullptr_t)
	{
		_removeReference();
		weak_pointer::operator=(nullptr);
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
	typename jdescriptor_table<T, UIDType>::pointer& jdescriptor_table<T, UIDType>::pointer::operator=(pointer&& otherPointer) noexcept
	{
		_removeReference();

		weak_pointer::operator=(otherPointer);

		otherPointer.descriptorTable = nullptr;
		otherPointer.descriptorIndex = -1;
		otherPointer.UID = juid<uid_type>::invalidUID;
		return *this;
	}

	template<typename T, typename UIDType>
	void jdescriptor_table<T, UIDType>::pointer::_addReference()
	{
		if (weak_pointer::descriptorTable != nullptr)
		{
			weak_pointer::descriptorTable->addReference(*this);
		}
	}
	template<typename T, typename UIDType>
	void jdescriptor_table<T, UIDType>::pointer::_removeReference()
	{
		if (weak_pointer::descriptorTable != nullptr)
		{
			weak_pointer::descriptorTable->removeReference(*this);
		}
	}
	
	template<typename T, typename UIDType>
	typename jdescriptor_table<T, UIDType>::base_poiter_type jdescriptor_table<T, UIDType>::createDescriptor(type* existingObject)
	{
		const int32 descriptorIndex = _pullEmptyDescriptor();
		descriptor& descriptor = descriptors[descriptorIndex];
		descriptor.object = existingObject;
		descriptor.references = 0;
		return base_poiter_type(descriptorIndex, descriptor.UID);
	}

	template<typename T, typename UIDType>
	bool jdescriptor_table<T, UIDType>::addReference(const base_poiter_type& pointer)
	{
		if (!this->isValid(pointer))
		{
			return false;
		}
		++descriptors[pointer.descriptorIndex].references;
		return true;
	}
	template<typename T, typename UIDType>
	bool jdescriptor_table<T, UIDType>::removeReference(const base_poiter_type& pointer)
	{
		if (!this->isValid(pointer))
		{
			return false;
		}
		if (descriptors[pointer.descriptorIndex].references > 0)
		{
			--descriptors[pointer.descriptorIndex].references;
		}
		return true;
	}
	template<typename T, typename UIDType>
	typename jdescriptor_table<T, UIDType>::pointer jdescriptor_table<T, UIDType>::createPointer(const base_poiter_type& tablePointer)
	{
		if (!this->isValid(tablePointer) || !this->addReference(tablePointer))
		{
			return nullptr;
		}
		return pointer(this, tablePointer.descriptorIndex, tablePointer.UID);
	}

	template<typename T, typename UIDType>
	void jdescriptor_table<T, UIDType>::destroy(const base_poiter_type& pointer)
	{
		if (this->isValid(pointer))
		{
			this->_destroyObject(pointer.descriptorIndex);
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
	void jdescriptor_table<T, UIDType>::_destroyObject(const int32 descriptorIndex)
	{
		descriptor& descriptor = descriptors[descriptorIndex];
		if (descriptor.object != nullptr)
		{
			onObjectDestroying.call(descriptor.object);

			delete descriptor.object;
			descriptor.object = nullptr;
			_pushDescriptor(descriptorIndex);
		}
	}
}
