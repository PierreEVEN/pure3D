#pragma once
#include <Types/String.h>
#include <filesystem>
#include "Array.h"

/*
struct Path final : public std::filesystem::path {

	using std::filesystem::path::path;
	using std::filesystem::path::operator =;

	explicit Path(const Path& inPath)
		: std::filesystem::path(inPath) {}

	explicit Path(const String& inPathStr) 
		: std::filesystem::path(inPathStr.GetData()) {}

	Path GetParent() const { return std::filesystem::path::parent_path(); }

	String GetFileName() const { return std::filesystem::path::filename().string().c_str(); }

	String GetFilePath() const { return string().c_str(); }

	bool IsDirectory() const { return std::filesystem::is_directory(*this); }

	bool IsFile() const { return !std::filesystem::is_directory(*this); }

	TVector<Path> GetChildren() const {
		TVector<Path> Children;
		for (const auto& p : std::filesystem::directory_iterator(*this)) Children.Add(p.path());
		return Children;
	}

	String GetExtension() const { return this->extension().string().c_str(); }

};
*/