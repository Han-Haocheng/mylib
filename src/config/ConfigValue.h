﻿//
// Created by HanHaocheng on 2024/6/21.
//
#pragma once

#include "../core.h"
#include "../convert/type_cast.h"

#include <memory>
#include <type_traits>

MYLIB_BEGIN

/**
 * @brief 配置基类
 */
class ConfigValueBasic {
public:
	using ptr = std::shared_ptr<ConfigValueBasic>;

	ConfigValueBasic(String name, String comment, String type_name);
	virtual ~ConfigValueBasic()					  = default;

	virtual void fromString(String string)		  = 0;
	[[nodiscard]] virtual String toString() const = 0;

	[[nodiscard]] const String &type_name() const;
	[[nodiscard]] const String &comment() const;
	[[nodiscard]] const String &name() const;

	void setComment(const String &mComment);
	void setName(const String &mConfigName);

protected:
	String m_type_name;

private:
	String m_name;
	String m_comment;
};

/**
 * @brief 配置值类型
 * @tparam ConfTy 配置类型
 * @tparam CastFunc 转换函数
 */
template<typename ConfTy, typename CastFunc = YamlCast<ConfTy>>
class ConfigValue final : public ConfigValueBasic {
public:
	using cast_func	 = CastFunc;
	using value_type = ConfTy;
	using ptr		 = std::shared_ptr<ConfigValue>;

	static_assert(!std::is_same_v<std::nullptr_t, CastFunc>, "static error: cast_func is nullptr");
	static_assert(is_StringCast<CastFunc>::value, "static error:cast_func don't have toString and fromString");

	explicit ConfigValue(String name, const value_type &val, String comment);
	explicit ConfigValue(String name, value_type &&conf, String comment);
	~ConfigValue() override = default;

	void setValue(value_type &&val);
	void setValue(const value_type &val);

	[[nodiscard]] const value_type &value() const;
	value_type &value();

	[[nodiscard]] String toString() const override;
	void fromString(String string) override;

	template<class AsType = value_type>
	[[nodiscard]] AsType as() const;

	template<class AsType, class Converter = Cast<value_type, AsType>, typename = std::enable_if_t<!std::is_same_v<AsType, value_type>>>
	[[nodiscard]] AsType as() const;

private:
	value_type m_value;
};

//=========================================================================================

template<typename ConfTy, typename CastFunc>
ConfigValue<ConfTy, CastFunc>::ConfigValue(String name, const value_type &val, String comment)
	: ConfigValueBasic(std::move(name), std::move(comment), typeid(value_type).name()), m_value(val) {}

template<typename ConfTy, typename CastFunc>
ConfigValue<ConfTy, CastFunc>::ConfigValue(String name, value_type &&conf, String comment)
	: ConfigValueBasic(std::move(name), std::move(comment), typeid(value_type).name()), m_value(std::move(conf)) {}

template<typename ConfTy, typename CastFunc>
void ConfigValue<ConfTy, CastFunc>::setValue(const value_type &val) { m_value = val; }

template<typename ConfTy, typename CastFunc>
void ConfigValue<ConfTy, CastFunc>::setValue(value_type &&val) { m_value = val; }

template<typename ConfTy, typename CastFunc>
typename ConfigValue<ConfTy, CastFunc>::value_type &ConfigValue<ConfTy, CastFunc>::value() { return m_value; }

template<typename ConfTy, typename CastFunc>
[[nodiscard]] const typename ConfigValue<ConfTy, CastFunc>::value_type &ConfigValue<ConfTy, CastFunc>::value() const { return m_value; }

template<typename ConfTy, typename CastFunc>
[[nodiscard]] String ConfigValue<ConfTy, CastFunc>::toString() const {
	return cast_func::toString(m_value);
}

template<typename ConfTy, typename CastFunc>
void ConfigValue<ConfTy, CastFunc>::fromString(String string) { m_value = cast_func::fromString(string); }

template<typename ConfTy, typename CastFunc>
template<class AsType>
[[nodiscard]] AsType ConfigValue<ConfTy, CastFunc>::as() const {
	if constexpr (std::is_same_v<value_type, AsType>) {
		return m_value;
	} else if constexpr (std::is_same_v<String, AsType>) {
		return cast_func::toString(m_value);
	}
	//static_assert(false, "static error: unknown error");
	return AsType{};
}

template<typename ConfTy, typename CastFunc>
template<class AsType, class Converter, typename>
[[nodiscard]] inline AsType ConfigValue<ConfTy, CastFunc>::as() const {
	return Converter::cast(m_value);
}
MYLIB_END
