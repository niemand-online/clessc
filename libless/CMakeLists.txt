cmake_minimum_required(VERSION 3.5)

set(less_SOURCES
        src/css/CssParser.cpp
        src/css/CssPrettyWriter.cpp
        src/css/CssTokenizer.cpp
        src/css/CssWriter.cpp
        src/css/ParseException.cpp
        src/css/SourceMapWriter.cpp
        src/less/LessParser.cpp
        src/less/LessTokenizer.cpp
        src/lessstylesheet/Closure.cpp
        src/lessstylesheet/Extension.cpp
        src/lessstylesheet/LessAtRule.cpp
        src/lessstylesheet/LessMediaQuery.cpp
        src/lessstylesheet/LessRuleset.cpp
        src/lessstylesheet/LessSelector.cpp
        src/lessstylesheet/LessStylesheet.cpp
        src/lessstylesheet/MediaQueryRuleset.cpp
        src/lessstylesheet/Mixin.cpp
        src/lessstylesheet/MixinCall.cpp
        src/lessstylesheet/ProcessingContext.cpp
        src/lessstylesheet/UnprocessedStatement.cpp
        src/stylesheet/AtRule.cpp
        src/stylesheet/CssComment.cpp
        src/stylesheet/Declaration.cpp
        src/stylesheet/MediaQuery.cpp
        src/stylesheet/Ruleset.cpp
        src/stylesheet/RulesetStatement.cpp
        src/stylesheet/Selector.cpp
        src/stylesheet/Stylesheet.cpp
        src/stylesheet/StylesheetStatement.cpp
        src/value/BooleanValue.cpp
        src/value/Color.cpp
        src/value/FunctionLibrary.cpp
        src/value/NumberValue.cpp
        src/value/StringValue.cpp
        src/value/UnitValue.cpp
        src/value/UrlValue.cpp
        src/value/Value.cpp
        src/value/ValueProcessor.cpp
        src/LogStream.cpp
        src/Token.cpp
        src/TokenList.cpp
        src/VariableMap.cpp
        )

add_library(less ${less_SOURCES})
target_include_directories(less PUBLIC include)

find_package(GLOG)
if (GLOG_FOUND)
    target_compile_definitions(less PUBLIC WITH_LIBGLOG)
    target_include_directories(less PUBLIC ${GLOG_INCLUDE_DIRS})
    target_link_libraries(less ${GLOG_LIBRARIES})
    message(STATUS "Compile with libglog")
else (PNG_FOUND)
    message(WARNING "Did not find libglog")
endif (GLOG_FOUND)

find_package(PNG 1.2)
if (PNG_FOUND)
    target_compile_definitions(less PUBLIC WITH_LIBPNG)
    target_include_directories(less PUBLIC ${PNG_INCLUDE_DIRS})
    target_link_libraries(less ${PNG_LIBRARIES})
    message(STATUS "Compile with libpng")
else (PNG_FOUND)
    message(WARNING "Did not find libpng")
endif (PNG_FOUND)

find_package(JPEG)
if (JPEG_FOUND)
    target_compile_definitions(less PUBLIC WITH_LIBJPEG)
    target_include_directories(less PUBLIC ${JPEG_INCLUDE_DIRS})
    target_link_libraries(less ${JPEG_LIBRARIES})
    message(STATUS "Compile with libjpeg")
else (JPEG_FOUND)
    message(WARNING "Did not find libjpeg")
endif (JPEG_FOUND)
