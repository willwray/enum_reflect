project('enum_reflect', 'cpp',
  default_options : 'cpp_std=c++17',
  version : '0.1'
)

subprojects = ['enum_traits', 'type_name_pp']

enum_traits_dep = subproject('enum_traits').get_variable('enum_traits_dep')
type_name_pp_dep = subproject('type_name_pp').get_variable('type_name_pp_dep')
ntbs_dep = subproject('ntbs').get_variable('ntbs_dep')

executable('enum_reflect', 'main.cpp',
  dependencies : [ntbs_dep, type_name_pp_dep, enum_traits_dep]
)
