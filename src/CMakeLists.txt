project(GFrost.Resources)

include_directories(.)

set(SOURCE
        ProIni.h

        Ini/Block.h
        Ini/Context.hpp
        Ini/Ini.h
        Ini/Lexer.cpp
        Ini/Lexer.h
        Ini/Parser.cpp
        Ini/Parser.h
        Ini/Property.cpp
        Ini/Property.h
        Ini/StringUtils.hpp
        Ini/Token.cpp
        Ini/Token.h
        Ini/TokenStream.hpp

        INIFile.cpp
        INIFile.h
#        LuaScript.cpp
#        LuaScript.h
)

    add_library(ProIni SHARED ${SOURCE})

target_compile_definitions(ProIni PRIVATE
        -DPROINI_EXPORTS
        )

#target_link_libraries(ProIni PRIVATE
#        GFrost
#        )