#pragma once

#include "common.h"

namespace GLWRAPPER_NS {

	template<typename T>
	class ResourceOwner {

		T data;

	public:

		ResourceOwner() : data() {}

		// Disable copy
		ResourceOwner(const ResourceOwner&) = delete;
		ResourceOwner &operator=(const ResourceOwner&) = delete;

		// Move constructor
		ResourceOwner(ResourceOwner&& other) noexcept : data(other.Release())
		{}

		// Move operator
		ResourceOwner &operator=(ResourceOwner&& other) noexcept
		{
			Reset(other.Release());
			return *this;
		}

		// Delete resource
		void Reset(T&& new_data = T())
		{
			data.Reset();
			data = std::move(new_data);
		}

		// Get raw data
		const T& Get() const {
			return data;
		}

		~ResourceOwner()
		{
			Reset();
		}

	private:

		// Release ownership
		T Release()
		{
			T ret = std::move(data);
			data = T();
			return ret;
		}

	};

}