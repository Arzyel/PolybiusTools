#include "FileManager.h"

void DM::FileData::initDataBuffer(const std::string& importPath)
{
	std::ifstream file(importPath, std::ios::binary | std::ios::ate);
	if (!file) {
		throw std::runtime_error("Failed to open File : " + importPath);
	}

	const auto size = file.tellg();
	if (size == std::ifstream::pos_type(-1)) {
		throw std::runtime_error("Failed to determine file size : " + importPath);
	}

	if (size == 0) {
		mBuffer.clear();
		return;
	}

	try {
		mBuffer.resize(size);
	}
	catch (const std::bad_alloc&) {
		mBuffer.clear();
		throw std::runtime_error("Insufficient memory to read file: " + importPath +
			" (size: " + std::to_string(size) + " bytes)");
	}
	file.seekg(0);
	if (!file.read(mBuffer.data(), size)) {
		throw std::runtime_error("Failed to read complete file : " + importPath);
	}

	const auto bytes_read = file.gcount();

	if (bytes_read != size) {
		throw std::runtime_error("Partial read: expected " + std::to_string(size) + " bytes, got " + std::to_string(bytes_read));
	}
}

std::string DM::DataToken::getCurrentName()
{
	if (!mNewData.empty()) {
		return mNewData;
	}

	return getOriginName();
}
