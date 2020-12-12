#pragma once

template<typename RClass>
struct RTypeName;

template<typename RClass>
struct RIsReflected { constexpr static const bool Value = false; };

void ReflEnsure(bool inCondition, const char* inMessage);

struct ReflectionObject {
	inline ReflectionObject(const ReflectionObject&) = delete;
	inline ReflectionObject(const ReflectionObject&&) = delete;
protected:
	ReflectionObject() = default;
};