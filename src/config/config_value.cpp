//
// Created by HanHaocheng on 2024/4/2.
//

#include "config_value.h"
#include <type_traits>

MYLIB_BEGIN

ConfigValueBase::ConfigValueBase(String name, String comment, String type_name)
    : m_name(std::move(name)),
      m_comment(std::move(comment)),
      m_type_name(std::move(type_name)) {}

[[nodiscard]] const String &ConfigValueBase::type_name() const { return m_type_name; }
[[nodiscard]] const String &ConfigValueBase::comment() const { return m_comment; }
[[nodiscard]] const String &ConfigValueBase::name() const { return m_name; }
void ConfigValueBase::setComment(const String &mComment) { m_comment = mComment; }
void ConfigValueBase::setName(const String &mConfigName) { m_name = mConfigName; }

MYLIB_END
