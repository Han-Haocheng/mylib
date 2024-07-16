//
// Created by HanHaocheng on 2024/4/2.
//

#include "config_value.h"

MYLIB_SPACE_BEGIN



ConfigValueBase::ConfigValueBase(String name, String comment, String type_name)
    : m_name(std::move(name)),
      m_comment(std::move(comment)),
      m_type_name(std::move(type_name)) {}



MYLIB_SPACE_END
