#pragma once
#include <string>
#include <iostream>

#if defined(_WIN32) || defined(_WIN64)
	#define PLATFORM_WINDOWS 1
	#include <windows.h>
#else
	#define PLATFORM_WINDOWS 0
	#include <sys/mman.h>
	#include <sys/stat.h>
	#include <fcntl.h>
	#include <unistd.h>
#endif


namespace mmap {
#if PLATFORM_WINDOWS
	struct Handle {
		HANDLE file_handle = INVALID_HANDLE_VALUE;
		HANDLE mapping_handle = INVALID_HANDLE_VALUE;
		const char* data = nullptr;
		size_t size = 0;
	};
#else
	struct Handle {
		int fd = -1;
		const char* data = nulltpr;
		size_t size = 0;
	}
#endif

	inline bool open(const std::string& filePath, Handle& handle) {

#if PLATFORM_WINDOWS
		handle.file_handle = CreateFileA(
			filePath.c_str(),
			GENERIC_READ,
			FILE_SHARE_READ,
			NULL,
			OPEN_EXISTING,
			FILE_ATTRIBUTE_NORMAL,
			NULL
		);
		if (handle.file_handle == INVALID_HANDLE_VALUE) {
			return false;
		}

		LARGE_INTEGER file_size;
		if (!GetFileSizeEx(handle.file_handle, &file_size)) {
			CloseHandle(handle.file_handle);
			return false;
		}
		handle.size = static_cast<size_t>(file_size.QuadPart);
		handle.mapping_handle = CreateFileMappingA(
			handle.file_handle,
			NULL,
			PAGE_READONLY,
			0, 0,
			NULL
		);

		if (handle.mapping_handle == NULL) {
			CloseHandle(handle.file_handle);
			return false;
		}

		handle.data = static_cast<const char*>(MapViewOfFile(
			handle.mapping_handle,
			FILE_MAP_READ,
			0, 0,
			handle.size
		));
		if (!handle.data) {
			CloseHandle(handle.mapping_handle);
			CloseHandle(handle.file_handle);
			return false;
		}
		return true;

#else
		handle.fd = ::open(filePath.c_str(), O_RDONLY);
		if (handle.fd == -1) return false;

		struct stat sb;
		if (fstat(handle.fd, &sb) == -1) {
			::close(handle.fd);
			return false;
		}
		handle.size = sb.st_size;

		handle.data = static_cast<const char*>(
			::mmap(nullptr, handle.size, PROT_READ, MAP_PRIVATE, handle.fd, 0));

		if (handle.data == MAP_FAILED) {
			handle.data = nullptr;
			::close(handle.fd);
			return false;
		}
		return true;
#endif
	}

	inline void close(Handle& handle) {
#if PLATFORM_WINDOWS
		if (handle.data) {
			UnmapViewOfFile(handle.data);
		}
		if (handle.mapping_handle != INVALID_HANDLE_VALUE) {
			CloseHandle(handle.mapping_handle);
		}
		if (handle.file_handle != INVALID_HANDLE_VALUE) {
			CloseHandle(handle.file_handle);
		}
#else
		if (handle.data) {
			munmap(const_cast<char*>(handle.data), handle.size);
		}
		if (handle.fd != -1) {
			::close(handle.fd)
		}
#endif
		handle = Handle{};
	}
}