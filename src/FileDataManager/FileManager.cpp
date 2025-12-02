#include "FileManager.h"

void DM::iFileDataBase::writeIntoFile()
{
	namespace fs = std::filesystem;

	fs::path path(mExportPath);
	fs::path dir = path.parent_path();

	if (!dir.empty() && !fs::exists(dir)) {
		if (!fs::create_directories(dir)) {
			std::cerr << "Failed to create directories: " << dir << "\n";
			return;
		}
	}


	std::ofstream out(mExportPath, std::ios::binary);
	if (!out.is_open()) {
		throw std::runtime_error("Failed to open output file: " + mExportPath);
	}


	std::vector<uint16_t> activeChangeIndexes(mActiveChanges.begin(), mActiveChanges.end());

	std::sort(activeChangeIndexes.begin(), activeChangeIndexes.end());


	for (auto& [index, keyDataToken] : mKeyToErase) {
		const char* ptr = mDataTokens[index].mPtrStart;
		while (*ptr != '=') --ptr;
		--ptr;
		while (*ptr == ' ' || *ptr == '\n' || *ptr == '\t') {
			--ptr;
		}
		while (*ptr != ' ' && *ptr != '\n' && *ptr != '\t') {
			--ptr;
		}
		keyDataToken.mPtrStart = ptr;
	}



	const char* base = mBuffer.data();
	const char* cursor = base;
	for (const auto index : activeChangeIndexes) {
		const auto& dataToken = mDataTokens[index];
		if (cursor < dataToken.mPtrStart) {
			if (auto it = mKeyToErase.find(index); it == mKeyToErase.end()) {
				out.write(cursor, dataToken.mPtrStart - cursor);
			}
			else {
				out.write(cursor, it->second.mPtrStart - cursor);
			}
		}
		if (dataToken.erase) {}
		else if (!dataToken.mNewData.empty()) {
			out.write(dataToken.mPrefix.data(), dataToken.mPrefix.size());
			out.write(dataToken.mNewData.data(), dataToken.mNewData.size());
			out.write(dataToken.mSuffix.data(), dataToken.mSuffix.size());
		}
		else {
			out.write(dataToken.mPtrStart, dataToken.mLength);
		}
		cursor = dataToken.mPtrStart + dataToken.mLength;
	}

	const char* end = base + mBuffer.size();
	if (cursor < end) {
		out.write(cursor, end - cursor);
	}
}

void DM::iFileDataBase::initDataBuffer(const std::string& importPath)
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

DM::DataToken::DataToken(const char* lastEntry, const std::string& newData)
{
	mPtrStart = lastEntry;
	mNewData = newData;
}

std::string DM::DataToken::getCurrentName()
{
	if (!mNewData.empty()) {
		return mNewData;
	}

	return getOriginName();
}

uint16_t DM::DataToken::getOrigin_uint16_t()
{
	uint16_t value;
	std::from_chars(mPtrStart, mPtrStart + mLength, value);
	return value;
}

uint16_t DM::DataToken::getCurrent_uint16_t()
{
	uint16_t value;
	if (!mNewData.empty()) {
		std::from_chars(mNewData.data(), mNewData.data() + mNewData.size(), value);
		return value;
	}

	std::from_chars(mPtrStart, mPtrStart + mLength, value);
	return value;
}
