// Copyright © 2023 Leonov Maksim. All Rights Reserved.

#pragma once

#include "jdelegate_multicast.h"
#include "juid.h"

namespace jutils
{
	template<typename T>
	class jdescriptor_table;

	class jdescriptor_table_pointer
	{
		template<typename T>
		friend class jdescriptor_table;

	public:

		using uid_type = uint32;

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

	template<typename T>
	class jdescriptor_table
	{
	public:

		using type = T;
		using poiter_type = jdescriptor_table_pointer;
		using uid_type = poiter_type::uid_type;

		class weak_pointer : public poiter_type
		{
			friend jdescriptor_table;

		public:
			constexpr weak_pointer() = default;
			constexpr weak_pointer(nullptr_t) : weak_pointer() {}
			constexpr weak_pointer(const weak_pointer&) = default;
		protected:
			weak_pointer(jdescriptor_table* table, const int32 descriptorIndex, const uid_type uid)
				: poiter_type(descriptorIndex, uid), descriptorTable(table)
			{}
		public:

			constexpr weak_pointer& operator=(nullptr_t);
			constexpr weak_pointer& operator=(const weak_pointer&) = default;
			
			constexpr bool operator==(const weak_pointer& otherPointer) const
			{
				return (descriptorTable == otherPointer.descriptorTable) && poiter_type::operator==(otherPointer);
			}
			constexpr bool operator!=(const weak_pointer& otherPointer) const { return !this->operator==(otherPointer); }
			
			constexpr bool operator<(const weak_pointer& otherPointer) const
			{
				return (descriptorTable < otherPointer.descriptorTable) || poiter_type::operator<(otherPointer);
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


		bool isValid(const poiter_type& pointer) const
		{
			return descriptors.isValidIndex(pointer.descriptorIndex) && (descriptors[pointer.descriptorIndex].UID == pointer.UID);
		}
		uint64 getRefsCount(const poiter_type& pointer) const
		{
			return this->isValid(pointer) ? descriptors[pointer.descriptorIndex].references : 0;
		}
		type* get(const poiter_type& pointer) const
		{
			return this->isValid(pointer) ? descriptors[pointer.descriptorIndex].object : nullptr;
		}

		template<typename Type, typename... Args>
		poiter_type create(Args&&... args) { return this->createDescriptor(new Type(std::forward<Args>(args)...)); }
		poiter_type createDescriptor(type* existingObject);

		bool addReference(const poiter_type& pointer);
		bool removeReference(const poiter_type& pointer);
		weak_pointer createWeakPointer(const poiter_type& pointer)
		{
			return this->isValid(pointer) ? weak_pointer(this, pointer.descriptorIndex, pointer.UID) : nullptr;
		}
		pointer createPointer(const poiter_type& pointer);

		void destroy(const poiter_type& pointer);
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
	
	constexpr jdescriptor_table_pointer& jdescriptor_table_pointer::operator=(nullptr_t)
	{
		descriptorIndex = -1;
		UID = juid<uid_type>::invalidUID;
		return *this;
	}
	constexpr bool jdescriptor_table_pointer::operator<(const jdescriptor_table_pointer& otherPointer) const
	{
		if (descriptorIndex < otherPointer.descriptorIndex)
		{
			return true;
		}
		return (descriptorIndex == otherPointer.descriptorIndex) && (UID < otherPointer.UID);
	}
	
	template<typename T>
	constexpr typename jdescriptor_table<T>::weak_pointer& jdescriptor_table<T>::weak_pointer::operator=(nullptr_t)
	{
		descriptorTable = nullptr;
		poiter_type::operator=(nullptr);
		return *this;
	}

	template<typename T>
    constexpr jdescriptor_table<T>::pointer::pointer(pointer&& otherPointer) noexcept
		: weak_pointer(otherPointer)
	{
		otherPointer.descriptorTable = nullptr;
		otherPointer.descriptorIndex = -1;
		otherPointer.UID = juid<uid_type>::invalidUID;
	}

	template<typename T>
	typename jdescriptor_table<T>::pointer& jdescriptor_table<T>::pointer::operator=(nullptr_t)
	{
		_removeReference();
		weak_pointer::operator=(nullptr);
		return *this;
	}
	template<typename T>
	typename jdescriptor_table<T>::pointer& jdescriptor_table<T>::pointer::operator=(const pointer& otherPointer)
	{
		if ((this != &otherPointer) && (*this != otherPointer))
		{
			_removeReference();
			weak_pointer::operator=(otherPointer);
			_addReference();
		}
		return *this;
	}
	template<typename T>
	typename jdescriptor_table<T>::pointer& jdescriptor_table<T>::pointer::operator=(const weak_pointer& otherPointer)
	{
		if ((this != &otherPointer) && (*this != otherPointer))
		{
			_removeReference();
			weak_pointer::operator=(otherPointer);
			_addReference();
		}
		return *this;
	}
	template<typename T>
	typename jdescriptor_table<T>::pointer& jdescriptor_table<T>::pointer::operator=(pointer&& otherPointer) noexcept
	{
		_removeReference();

		weak_pointer::operator=(otherPointer);

		otherPointer.descriptorTable = nullptr;
		otherPointer.descriptorIndex = -1;
		otherPointer.UID = juid<uid_type>::invalidUID;
		return *this;
	}

	template<typename T>
	void jdescriptor_table<T>::pointer::_addReference()
	{
		if (weak_pointer::descriptorTable != nullptr)
		{
			weak_pointer::descriptorTable->addReference(*this);
		}
	}
	template<typename T>
	void jdescriptor_table<T>::pointer::_removeReference()
	{
		if (weak_pointer::descriptorTable != nullptr)
		{
			weak_pointer::descriptorTable->removeReference(*this);
		}
	}
	
	template<typename T>
	typename jdescriptor_table<T>::poiter_type jdescriptor_table<T>::createDescriptor(type* existingObject)
	{
		const int32 descriptorIndex = _pullEmptyDescriptor();
		descriptor& descriptor = descriptors[descriptorIndex];
		descriptor.object = existingObject;
		descriptor.references = 0;
		return poiter_type(descriptorIndex, descriptor.UID);
	}

	template<typename T>
	bool jdescriptor_table<T>::addReference(const poiter_type& pointer)
	{
		if (!this->isValid(pointer))
		{
			return false;
		}
		++descriptors[pointer.descriptorIndex].references;
		return true;
	}
	template<typename T>
	bool jdescriptor_table<T>::removeReference(const poiter_type& pointer)
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
	template<typename T>
	typename jdescriptor_table<T>::pointer jdescriptor_table<T>::createPointer(const poiter_type& tablePointer)
	{
		if (!this->isValid(tablePointer) || !this->addReference(tablePointer))
		{
			return nullptr;
		}
		return pointer(this, tablePointer.descriptorIndex, tablePointer.UID);
	}

	template<typename T>
	void jdescriptor_table<T>::destroy(const poiter_type& pointer)
	{
		if (this->isValid(pointer))
		{
			this->_destroyObject(pointer.descriptorIndex);
		}
	}
	template<typename T>
	void jdescriptor_table<T>::cleanup()
	{
		for (int32 descriptorIndex = 0; descriptorIndex < descriptors.getSize(); descriptorIndex++)
		{
			if (descriptors[descriptorIndex].references == 0)
			{
				_destroyObject(descriptorIndex);
			}
		}
	}
	template<typename T>
	void jdescriptor_table<T>::clear()
	{
		for (int32 descriptorIndex = 0; descriptorIndex < descriptors.getSize(); descriptorIndex++)
		{
			_destroyObject(descriptorIndex);
		}
	}

	template<typename T>
	int32 jdescriptor_table<T>::_pullEmptyDescriptor()
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
	template<typename T>
	void jdescriptor_table<T>::_pushDescriptor(const int32 descriptorIndex)
	{
		descriptor& descriptor = descriptors[descriptorIndex];
		descriptor.UID.generateUID();
		if (descriptor.UID != juid<uid_type>::invalidUID)
		{
			descriptor.references = static_cast<uint64>(firstEmptyDescriptor + 1);
			firstEmptyDescriptor = descriptorIndex;
		}
	}
	template<typename T>
	void jdescriptor_table<T>::_destroyObject(const int32 descriptorIndex)
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
