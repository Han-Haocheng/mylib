//
// Created by HanHaocheng on 2024/6/21.
//
#ifndef MYLIB_CONFIG_H
#define MYLIB_CONFIG_H

#include "../convert/Convert.h"

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
 * @tparam ConvertTy 转换函数
 */
template<typename ConfTy, ConvertType ConvertTy = ConvertType::CT_YAML>
class Config : public ConfigValueBasic {
public:
	using value_type = ConfTy;
	using ptr		 = std::shared_ptr<Config>;

	explicit Config(String name, value_type &&conf, String comment) : ConfigValueBasic(std::move(name), std::move(comment), typeid(value_type).name()), m_value(std::move(conf)) {}
	explicit Config(String name, const value_type &val, String comment) : ConfigValueBasic(std::move(name), std::move(comment), typeid(value_type).name()), m_value(val) {}
	~Config() override = default;

	void setValue(value_type &&val) { m_value = val; }
	void setValue(const value_type &val) { m_value = val; }

	[[nodiscard]] const value_type &value() const { return m_value; }
	value_type &value() { return m_value; }

	[[nodiscard]] String toString() const override { return Convert<String, value_type, ConvertTy>(m_value); }
	void fromString(const String string) override { m_value = Convert<value_type, String, ConvertTy>(string); }

	template<class AsType = value_type>
	[[nodiscard]] AsType as() const {
		if constexpr (std::is_same_v<value_type, AsType>) {
			return m_value;
		} else if constexpr (std::is_same_v<String, AsType>) {
			return toString(m_value);
		} else {
			return Convert<AsType, value_type>()(m_value);
		}
		return AsType{};
	}

protected:
	value_type m_value;
};

//=========================================================================================

MYLIB_END
#endif // !MYLIB_CONFIG_H
