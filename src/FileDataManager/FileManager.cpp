#include "FileManager.h"

DM::FileData::FileData(const std::string& importPath, const std::string& exportPath)
	: mExportPath(exportPath)
{
	initDataBuffer(importPath);
}

void DM::FileData::writeIntoFile()
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
	const char* base = mBuffer.data();
	const char* cursor = base;
	for (const auto& dataToken : mDataTokens) {
		if (cursor < dataToken.mPtrStart) {
			out.write(cursor, dataToken.mPtrStart - cursor);
		}
		if (dataToken.erase) {}
		else if (!dataToken.mNewData.empty()) {
			out.write(dataToken.mNewData.data(), dataToken.mNewData.size());
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

uint16_t DM::DataToken::get_uint16_t_Value()
{
	uint16_t value;
	std::from_chars(mPtrStart, mPtrStart + mLength, value);
	return value;
}

DM::FileManager::~FileManager()
{
	for (auto* obj : mFiles) {
		delete obj;
	}
	mFiles.clear();
}
