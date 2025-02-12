//
// Created by HanHaocheng on 2024/4/2.
//

#include "Config.h"

MYLIB_BEGIN

ConfigValueBasic::ConfigValueBasic(String name, String comment, String type_name)
    : m_name(std::move(name)),
      m_comment(std::move(comment)),
      m_type_name(std::move(type_name)) {}

[[nodiscard]] const String &ConfigValueBasic::type_name() const { return m_type_name; }
[[nodiscard]] const String &ConfigValueBasic::comment() const { return m_comment; }
[[nodiscard]] const String &ConfigValueBasic::name() const { return m_name; }
void ConfigValueBasic::setComment(const String &mComment) { m_comment = mComment; }
void ConfigValueBasic::setName(const String &mConfigName) { m_name = mConfigName; }

MYLIB_END
