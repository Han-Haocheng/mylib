﻿//
// Created by HanHaocheng on 2024/6/21.
//

#include "../core/mylib_def.h"
#include "../untils/converter.h"
#include "../untils/yaml_converter.h"

#include <algorithm>
#include <utility>

MYLIB_SPACE_BEGIN

class ConfigValueBase {
public:
  using ptr = std::shared_ptr<ConfigValueBase>;

  ConfigValueBase(String name, String comment, String type_name);
  virtual ~ConfigValueBase() = default;

  virtual void fromString(String string) = 0;
  [[nodiscard]] virtual String toString() const = 0;
  [[nodiscard]] const String &type_name() const { return m_type_name; }

  [[nodiscard]] const String &comment() const { return m_comment; }
  [[nodiscard]] const String &name() const { return m_name; }

  void setComment(const String &mComment) { m_comment = mComment; }
  void setName(const String &mConfigName) { m_name = mConfigName; }

private:
  String m_name;
  String m_comment;
  String m_type_name;
};

template<typename ConfTy, typename CastFunc = YamlConverter<ConfTy>>
class ConfigValue : public ConfigValueBase {
public:
  static_assert(!std::is_same<nullptr_t, CastFunc>::value, "static error: cast_func is nullptr");
  static_assert(IsStringConverter<CastFunc>::value, "static error:cast_func don't have toString and fromString");

  using value_type = ConfTy;
  using cast_func = YamlConverter<value_type>;
  using ptr = std::shared_ptr<ConfigValue<value_type, cast_func>>;

  explicit ConfigValue(String name, const value_type &val, String comment)
      : ConfigValueBase(std::move(name), std::move(comment), typeid(value_type).name()), m_value(val) {}

  explicit ConfigValue(String name, value_type &&conf, String comment)
      : ConfigValueBase(std::move(name), std::move(comment), typeid(value_type).name()), m_value(std::move(conf)) {}

  ~ConfigValue() override = default;

  void setValue(const value_type &val) { m_value = val; }
  void setValue(value_type &&val) { m_value = val; }
  value_type &value() { return m_value; }
  [[nodiscard]] const value_type &value() const { return m_value; }

  template<class AsType = value_type>
  [[nodiscard]] const AsType &as() const {
    if constexpr (std::is_same<value_type, AsType>::value) {
      return m_value;
    } else if constexpr (std::is_same<std::string, AsType>::value) {
      return cast_func{}.toString(m_value);
    }
    //static_assert(false, "static error: unknown error");
  }

  template<class AsType, class Converter = Converter<value_type, AsType>,
           typename = typename std::enable_if<!std::is_same<AsType, value_type>::value>::type>
  [[nodiscard]] const AsType &as() const {
    return Converter{}.cast(m_value);
  }

  [[nodiscard]] String toString() const override { return {cast_func{}.toString(m_value)}; }
  void fromString(String string) override { m_value = cast_func{}.fromString(string); }

private:
  value_type m_value;
};

MYLIB_SPACE_END