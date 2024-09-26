//
// Created by HanHaocheng on 2024/6/21.
//

#include "../base/mylib_def.h"
#include "../cast/yaml_cast.h"

#include <memory>
#include <type_traits>

MYLIB_BEGIN

/**
 * @brief 配置基类
 */
class ConfigValueBase {
public:
  using ptr = std::shared_ptr<ConfigValueBase>;

  ConfigValueBase(String name, String comment, String type_name);
  virtual ~ConfigValueBase() = default;

  virtual void fromString(String string) = 0;
  [[nodiscard]] virtual String toString() const = 0;

  [[nodiscard]] const String &type_name() const;
  [[nodiscard]] const String &comment() const;
  [[nodiscard]] const String &name() const;

  void setComment(const String &mComment);
  void setName(const String &mConfigName);

private:
  String m_name;
  String m_comment;
  String m_type_name;
};

/**
 * @brief 配置值类型
 * @tparam ConfTy 配置类型
 * @tparam CastFunc 转换函数
 */
template<typename ConfTy, typename CastFunc = YamlCast<ConfTy>>
class ConfigValue : public ConfigValueBase {
public:
  using cast_func = CastFunc;

  static_assert(!std::is_same_v<nullptr_t, CastFunc>, "static error: cast_func is nullptr");
  static_assert(is_StringCast<CastFunc>::value, "static error:cast_func don't have toString and fromString");

  using value_type = ConfTy;
  using ptr = std::shared_ptr<ConfigValue>;

  explicit ConfigValue(String name, const value_type &val, String comment);
  explicit ConfigValue(String name, value_type &&conf, String comment);
  ~ConfigValue() override = default;

  void setValue(const value_type &val);
  void setValue(value_type &&val);
  value_type &value();
  [[nodiscard]] const value_type &value() const;

  [[nodiscard]] String toString() const override;
  void fromString(String string) override;

  template<class AsType = value_type>
  [[nodiscard]] const AsType &as() const;
  template<class AsType, class Converter = Cast<value_type, AsType>,
           typename = typename std::enable_if_t<!std::is_same_v<AsType, value_type>>>
  [[nodiscard]] const AsType &as() const;

private:
  value_type m_value;
};

template<typename ConfTy, typename CastFunc>
inline ConfigValue<ConfTy, CastFunc>::ConfigValue(String name, const value_type &val, String comment)
    : ConfigValueBase(std::move(name), std::move(comment), typeid(value_type).name()), m_value(val) {}

template<typename ConfTy, typename CastFunc>
inline ConfigValue<ConfTy, CastFunc>::ConfigValue(String name, value_type &&conf, String comment)
    : ConfigValueBase(std::move(name), std::move(comment), typeid(value_type).name()), m_value(std::move(conf)) {}

template<typename ConfTy, typename CastFunc>
inline void ConfigValue<ConfTy, CastFunc>::setValue(const value_type &val) { m_value = val; }

template<typename ConfTy, typename CastFunc>
inline void ConfigValue<ConfTy, CastFunc>::setValue(value_type &&val) { m_value = val; }

template<typename ConfTy, typename CastFunc>
inline typename ConfigValue<ConfTy, CastFunc>::value_type &ConfigValue<ConfTy, CastFunc>::value() { return m_value; }

template<typename ConfTy, typename CastFunc>
[[nodiscard]] inline const typename ConfigValue<ConfTy, CastFunc>::value_type &ConfigValue<ConfTy, CastFunc>::value() const { return m_value; }

template<typename ConfTy, typename CastFunc>
[[nodiscard]] inline String ConfigValue<ConfTy, CastFunc>::toString() const {
  return cast_func::toString(m_value);
}

template<typename ConfTy, typename CastFunc>
inline void ConfigValue<ConfTy, CastFunc>::fromString(String string) { m_value = cast_func::fromString(string); }

template<typename ConfTy, typename CastFunc>
template<class AsType>
[[nodiscard]] inline const AsType &ConfigValue<ConfTy, CastFunc>::as() const {
  if constexpr (std::is_same<value_type, AsType>::value) {
    return m_value;
  } else if constexpr (std::is_same<String, AsType>::value) {
    return cast_func::toString(m_value);
  }
  //static_assert(false, "static error: unknown error");
}
template<typename ConfTy, typename CastFunc>
template<class AsType, class Converter, typename>
[[nodiscard]] inline const AsType &ConfigValue<ConfTy, CastFunc>::as() const {
  return Converter::cast(m_value);
}
MYLIB_END
