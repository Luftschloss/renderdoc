### CMake基础

#### 常用网址

CMake Document：https://cmake.org/cmake/help/v3.15/index.html

API Mirror：https://apimirror.com/cmake~3.15



#### CMakeLists.txt示例

```makefile
# cmake 最低版本需求
cmake_minimum_required(VERSION 2.8.12)

# 工程名称
project(cmake_test)

# 设置
set(CMAKE_CXX_STANDARD 11)

# 分支语句
if(ANDROID)
	message(STATUS "Android")
else()
	message(STATUS "Not Adnroid")
endif()

# 添加编译源码生成目标
add_executable(cmake_test src/main.cc)
```



#### MinGW编译流程

```makefile
# Step1:执行cmake 生成Makefiles
cmake -DANDROID=On -G "MinGW Makefiles" ..
# 正常输出如下
-- The C compiler identification is GNU 6.3.0
-- The CXX compiler identification is GNU 6.3.0
-- Detecting C compiler ABI info
-- Detecting C compiler ABI info - done
-- Check for working C compiler: C:/MinGW/bin/gcc.exe - skipped
-- Detecting C compile features
-- Detecting C compile features - done
-- Detecting CXX compiler ABI info
-- Detecting CXX compiler ABI info - done
-- Check for working CXX compiler: C:/MinGW/bin/g++.exe - skipped
-- Detecting CXX compile features
-- Detecting CXX compile features - done
-- Android
-- Configuring done
-- Generating done
-- Build files have been written to: D:/WorkSpace/C++/CmakeTest/build

# Step2:make生成目标
mingw32-make
# 正常输出如下
[ 50%] Building CXX object CMakeFiles/cmake_test.dir/src/main.cc.obj
[100%] Linking CXX executable cmake_test.exe
[100%] Built target cmake_test
```



#### 常用Cmake指令

```makefile
# Create or update a cmake cache entry. 创建或更新CMake参数值（CmakeCache）
-D<var>[:<type>]=<value>      


```



#### 常用CMakeLists语法

```cmake
 是否定义环境变量
DEFINED ENV{env_name}
# 是否存在路径
EXISTS "Path"
# 获得STR变量的字符串值
&{<variable>}

# 字符串相等判断符
<string1> STREQUAL <string2>

# 项目名
project(<PROJECT-NAME> [<language-name>...])

# 设置变量值，同命令行的-D<variable>=<value>, 使用CACHE可以将该变量缓存至CMakeCache.txt，使用Force是Overwrite缓存值，并不是使用缓存值，使用
# PARENT_SCOPE调高变量作用域到父目录或调用该函数处（PARENT_SCOPE和CACHE互斥）
set(<variable> <value>
      [[CACHE <type> <docstring> [FORCE]] | PARENT_SCOPE])
# 移除变量
unset(<variable> [CACHE | PARENT_SCOPE])

# 定义一个Bool变量，值为ON或OFF，默认为OFF，通过命令行可以选择性开关
option(<variable> <docstring> [value])

# 打印日志
message(FATAL_ERROR/STATUS "message string")

# 执行命令行
execute_process(COMMAND <cmd1> [args1...]]
                  [COMMAND <cmd2> [args2...] [...]]
                  [WORKING_DIRECTORY <directory>]
                  [TIMEOUT <seconds>]
                  [RESULT_VARIABLE <variable>]
                  [OUTPUT_VARIABLE <variable>]
                  [ERROR_VARIABLE <variable>]
                  [INPUT_FILE <file>]
                  [OUTPUT_FILE <file>]
                  [ERROR_FILE <file>]
                  [OUTPUT_QUIET]
                  [ERROR_QUIET]
                  [OUTPUT_STRIP_TRAILING_WHITESPACE]
                  [ERROR_STRIP_TRAILING_WHITESPACE])

# 
add_custom_command(OUTPUT output1 [output2 ...]
                   COMMAND command1 [ARGS] [args1...]
                   [COMMAND command2 [ARGS] [args2...] ...]
                   [MAIN_DEPENDENCY depend]
                   [DEPENDS [depends...]]
                   [BYPRODUCTS [files...]]
                   [IMPLICIT_DEPENDS <lang1> depend1
                                    [<lang2> depend2] ...]
                   [WORKING_DIRECTORY dir]
                   [COMMENT comment]
                   [DEPFILE depfile]
                   [JOB_POOL job_pool]
                   [VERBATIM] [APPEND] [USES_TERMINAL]
                   [COMMAND_EXPAND_LISTS])

# list变量
list(LENGTH <list><output variable>)										#返回list的长度							
list(GET <list> <elementindex> [<element index> ...]<output variable>) 			#返回list中index的element到value中
list(APPEND <list><element> [<element> ...])								#添加新element到list中
list(FIND <list> <value><output variable>)									#返回list中element的index，没有找到返回-1
list(INSERT <list><element_index> <element> [<element> ...])				#将新element插入到list中index的位置
list(REMOVE_ITEM <list> <value>[<value> ...])								#从list中删除某个element
list(REMOVE_AT <list><index> [<index> ...])									#从list中删除指定index的element
list(REMOVE_DUPLICATES <list>)												#从list中删除重复的element
list(REVERSE <list>)														#将list的内容反转
list(SORT <list>)															#将list按字母顺序排序

# Add -D define flags to the compilation of source files.编译添加宏定义
add_definitions(-DFOO -DBAR ...)      

# 定义方法，通过fname([<arg1>...])调用，返回值在哪？
function(<fname> [<arg1> ...])
  <commands>
endfunction([<fname>])

# 字符串匹配
MATCHES

# 查找指定目录下的所有源文件，然后将结果存进指定变量
aux_source_directory(<dir> <variable>)

# 将指定目录添加到编译器的头文件搜索路径之下，指定的目录被解释成当前源码路径的相对路径，默认添加到最后（AFTER），该函数只作用于当前CMakeLists
include_directories([AFTER|BEFORE] [SYSTEM] dir1 [dir2 ...])

# 添加一个子目录并构建该子目录，子目录下应该包含CMakeLists.txt文件和代码文件。子目录可以是相对路径也可以是绝对路径，如果是相对路径，则是相对当前目录的一个相对路径，binary_dir指定输出目录，不指定默认为source_dir，EXCLUDE_FROM_ALL指定后子目录下的目标不会被父目录下的目标文件包含进去，父目录的CMakeLists.txt不会构建子目录的目标文件，必须在子目录下显式去构建
add_subdirectory(source_dir [binary_dir] [EXCLUDE_FROM_ALL])

# add_library
## 将指定的源文件生成链接文件，然后添加到工程中去，库的类型是STATIC(静态库)/SHARED(动态库)/MODULE(模块库)
add_library(<name> [STATIC | SHARED | MODULE]
            [EXCLUDE_FROM_ALL]
            [source1] [source2] [...])
## 导入已生成的库
add_library(<name> <SHARED|STATIC|MODULE|OBJECT|UNKNOWN> IMPORTED
            [GLOBAL])
## 库的类型固定为OBJECT，这种库编译了源文件，但不链接
add_library(<name> OBJECT <src>...)
## 为给定library添加一个别名，后续可使用<name>来替代<target>
add_library(<name> ALIAS <target>)
## 创建一个接口库
add_library(<name> INTERFACE [IMPORTED [GLOBAL]])

# 将目标文件与库文件进行链接，debug是DEBUG_CONFIGURATIONS属性配置，optimized是除了DEBUG_CONFIGURATIONS属性配置的所有配置，general是全部其他配置
target_link_libraries(<target> [item1] [item2] [...]
                      [[debug|optimized|general] <item>] ...)
                      
target_include_directories(<target> [SYSTEM] [AFTER|BEFORE]
  <INTERFACE|PUBLIC|PRIVATE> [items1...]
  [<INTERFACE|PUBLIC|PRIVATE> [items2...] ...])
                   
# 设置C++、C编译选项，同set(CMAKE_CXX_FLAGS/CMAKE_C_FLAGS <cmdstring>)
# 常见的有：-fsanitize=thread(检测运行时的Data Race)、-fsanitize=address（检测运行时的内存错误）、-fno-omit-frame-pointer
add_compile_options(<option> ...)

# 指定源文件设置属性（编译），以<prop>:<value>键值对的形式
set_source_files_properties(<files> ...
                            [DIRECTORY <dirs> ...]
                            [TARGET_DIRECTORY <targets> ...]
                            PROPERTIES <prop1> <value1> [<prop2> <value2>] ...)

# 添加编译目标
add_executable(<name> [WIN32] [MACOSX_BUNDLE]
               [EXCLUDE_FROM_ALL]
               [source1] [source2 ...])

# 引入第三方库，使用REQUIRED：必须有第三方库，没有找到的话CMake return faild
find_package(<PackageName> [version] [EXACT] [QUIET]
             [REQUIRED] [[COMPONENTS] [components...]]
             [OPTIONAL_COMPONENTS components...]
             [CONFIG|NO_MODULE]
             [NO_POLICY_SCOPE]
             [NAMES name1 [name2 ...]]
             [CONFIGS config1 [config2 ...]]
             [HINTS path1 [path2 ... ]]
             [PATHS path1 [path2 ... ]]
             [PATH_SUFFIXES suffix1 [suffix2 ...]]
             [NO_DEFAULT_PATH]
             [NO_PACKAGE_ROOT_PATH]
             [NO_CMAKE_PATH]
             [NO_CMAKE_ENVIRONMENT_PATH]
             [NO_SYSTEM_ENVIRONMENT_PATH]
             [NO_CMAKE_PACKAGE_REGISTRY]
             [NO_CMAKE_BUILDS_PATH] # Deprecated; does nothing.
             [NO_CMAKE_SYSTEM_PATH]
             [NO_CMAKE_SYSTEM_PACKAGE_REGISTRY]
             [CMAKE_FIND_ROOT_PATH_BOTH |
              ONLY_CMAKE_FIND_ROOT_PATH |
              NO_CMAKE_FIND_ROOT_PATH])

# 对文件操作
## Reading 读
  file(READ <filename> <out-var> [...])
  file(STRINGS <filename> <out-var> [...])
  file(<HASH> <filename> <out-var>)
  file(TIMESTAMP <filename> <out-var> [...])

## Writing 写
  file({WRITE | APPEND} <filename> <content>...)
  file({TOUCH | TOUCH_NOCREATE} [<file>...])
  file(GENERATE OUTPUT <output-file> [...])

## Filesystem 常见文件操作
  file({GLOB | GLOB_RECURSE} <out-var> [...] [<globbing-expr>...])
  file(RENAME <oldname> <newname>)
  file({REMOVE | REMOVE_RECURSE } [<files>...])
  file(MAKE_DIRECTORY [<dir>...])
  file({COPY | INSTALL} <file>... DESTINATION <dir> [...])
  file(SIZE <filename> <out-var>)
  file(READ_SYMLINK <linkname> <out-var>)
  file(CREATE_LINK <original> <linkname> [...])

## Path Conversion
  file(RELATIVE_PATH <out-var> <directory> <file>)
  file({TO_CMAKE_PATH | TO_NATIVE_PATH} <path> <out-var>)

## Transfer 传输
  file(DOWNLOAD <url> <file> [...])
  file(UPLOAD <file> <url> [...])

## Locking 加锁
  file(LOCK <path> [...])
  
# string操作
## Search and Replace 查找和替换
  string(FIND <string> <substring> <out-var> [...])
  string(REPLACE <match-string> <replace-string> <out-var> <input>...)

## Regular Expressions 正则匹配
  string(REGEX MATCH <match-regex> <out-var> <input>...)
  string(REGEX MATCHALL <match-regex> <out-var> <input>...)
  string(REGEX REPLACE <match-regex> <replace-expr> <out-var> <input>...)

## Manipulation 常见字符串处理
  string(APPEND <string-var> [<input>...])
  string(PREPEND <string-var> [<input>...])
  string(CONCAT <out-var> [<input>...])
  string(JOIN <glue> <out-var> [<input>...])
  string(TOLOWER <string1> <out-var>)
  string(TOUPPER <string1> <out-var>)
  string(LENGTH <string> <out-var>)
  string(SUBSTRING <string> <begin> <length> <out-var>)
  string(STRIP <string> <out-var>)
  string(GENEX_STRIP <string> <out-var>)
  string(REPEAT <string> <count> <out-var>)

## Comparison 字符串比较
  string(COMPARE <op> <string1> <string2> <out-var>)

## Hashing
  string(<HASH> <out-var> <input>)

## Generation 生成字符串
  string(ASCII <number>... <out-var>)
  string(CONFIGURE <string1> <out-var> [...])
  string(MAKE_C_IDENTIFIER <string> <out-var>)
  string(RANDOM [<option>...] <out-var>)
  string(TIMESTAMP <out-var> [<format string>] [UTC])
  string(UUID <out-var> ...)
  
# install
install(TARGETS <target>... [...])
install({FILES | PROGRAMS} <file>... [...])
install(DIRECTORY <dir>... [...])
install(SCRIPT <file> [...])
install(CODE <code> [...])
install(EXPORT <export-name> [...])
```



#### CMake常见特殊变量

```makefile
CMAKE_TOOLCHAIN_FILE:
CMAKE给交叉编译预留了一个变量CMAKE_TOOLCHAIN_FILE，它定义了一个文件的路径，这个文件即toolChain，里面set了一系列你需要改变的变量和属性，包括C_COMPILER,CXX_COMPILER。CMake为了不让用户每次交叉编译都要重新输入这些命令，因此它带来toolChain机制，简而言之就是一个cmake脚本，内嵌了你需要改变以及需要set的所有交叉环境的设置（即单元化打包设置脚本）
toolChain脚本中几个比较常见的重要变量(具体见NDK的android.toolchain.cmake)：
#CMAKE_SYSTEM_NAME
即你目标机target所在的操作系统名称，比如ARM/Linux为"Linux",Windows为"Windows"，Android平台为"Android",如果你的嵌入式平台没有相关OS你即需要写成"Generic",只有当CMAKE_SYSTEM_NAME这个变量被设置了，CMake才认为此时正在交叉编译，它会额外设置一个变量CMAKE_CROSSCOMPILING为TRUE
#CMAKE_C_COMPILER、CMAKE_CXX_COMPILER
即C/C++编译器，这里可以将变量设置成完整路径或者文件名，设置成完整路径有一个好处就是CMake会去这个路径下去寻找编译相关的其他工具比如linker,binutils等，如果你写的文件名带有arm-elf等等前缀，CMake会识别到并且去寻找相关的交叉编译器
#CMAKE_FIND_ROOT_PATH
该变量是一个list，指定了一个或者多个优先于其他搜索路径的搜索路径。比如你设置了/opt/arm/，所有的Find_xxx.cmake都会优先根据这个路径下的/usr/lib,/lib等进行查找，然后才会去你自己的/usr/lib和/lib进行查找，如果你有一些库是不被包含在/opt/arm里面的，也可以显示指定多个值给CMAKE_FIND_ROOT_PATH
#CMAKE_FIND_ROOT_PATH_MODE_PROGRAM
对find_program()起作用，有三种取值，NEVER,ONLY,BOTH,第一个表示不在你CMAKE_FIND_ROOT_PATH下进行查找，第二个表示只在这个路径下查找，第三个表示先查找这个路径，再查找全局路径，对于这个变量来说，一般都是调用宿主机的程序，所以一般都设置成NEVER
#CMAKE_FIND_ROOT_PATH_MODE_LIBRARY
find_library()起作用，表示在链接的时候的库的相关选项，因此这里需要设置成ONLY来保证我们的库是在交叉环境中找的
#CMAKE_FIND_ROOT_PATH_MODE_INCLUDE
对find_path()和find_file()起作用，一般来说也是ONLY,如果你想改变，一般也是在相关的FIND命令中增加option来改变局部设置，有NO_CMAKE_FIND_ROOT_PATH,ONLY_CMAKE_FIND_ROOT_PATH,BOTH_CMAKE_FIND_ROOT_PATH
#CMAKE_FIND_ROOT_PATH_MODE_PACKAGE
find_package()起作用，一般来说也是ONLY

CMAKE_BINARY_DIR/PROJECT_BINARY_DIR:
指的是同一个东西，工程编译发生的目录，如果是 in source编译（源码和编译同路径，稍微会有些混乱）,指得就是工程顶层目录,如果是 out of source 编译（在根目录中创建<build>目录进行编译）

CMAKE_CURRENT_SOURCE_DIR:
指的是当前处理的 CMakeLists.txt 所在的路径

COMPILE_FLAGS:
编译属性标识，常见GNUCXX编译属性有："-Wno-format-overflow"

DEBUG_CONFIGURATIONS:

```





### RDC Android编译CMake工程结构

代码仓库版本：Release/v1.17

```cmake
RDCProjectRoot
├───CMakeLists.txt
├───renderdoc
    ├───CMakeLists.txt("rdoc","rdoc_version",OBJECT_LIBRARY|"renderdoc_libentry",STATIC_LIBRARY|"renderdoc",SHARED_LIBRARY)
    ├───api/app/renderdoc_app.h
    ├───api/replay/apidefs.h
    ├───api/replay/capture_options.h
    ├───api/replay/common_pipestate.h
    ├───api/replay/pipestate.h
    ├───api/replay/pipestate.inl
    ├───api/replay/control_types.h
    ├───api/replay/data_types.h
    ├───api/replay/rdcarray.h
    ├───api/replay/rdcdatetime.h
    ├───api/replay/rdcflatmap.h
    ├───api/replay/rdcpair.h
    ├───api/replay/rdcstr.h
    ├───api/replay/replay_enums.h
    ├───api/replay/resourceid.h
    ├───api/replay/shader_types.h
    ├───api/replay/stringise.h
    ├───api/replay/structured_data.h
    ├───api/replay/version.h
    ├───api/replay/gl_pipestate.h
    ├───api/replay/vk_pipestate.h
    ├───api/replay/renderdoc_replay.h
    ├───api/replay/renderdoc_tostr.inl
    ├───common/common.cpp
    ├───common/common.h
    ├───common/custom_assert.h
    ├───common/dds_readwrite.cpp
    ├───common/dds_readwrite.h
    ├───common/formatting.h
    ├───common/globalconfig.h
    ├───common/shader_cache.h
    ├───common/threading.h
    ├───common/timing.h
    ├───common/wrapped_pool.h
    ├───common/threading_tests.cpp
    ├───core/core.cpp
    ├───core/image_viewer.cpp
    ├───core/core.h
    ├───core/crash_handler.h
    ├───core/target_control.cpp
    ├───core/remote_server.cpp
    ├───core/remote_server.h
    ├───core/settings.cpp
    ├───core/settings.h
    ├───core/replay_proxy.cpp
    ├───core/replay_proxy.h
    ├───core/intervals.h
    ├───core/intervals_tests.cpp
    ├───core/bit_flag_iterator.h
    ├───core/bit_flag_iterator_tests.cpp
    ├───android/android.cpp
    ├───android/android_patch.cpp
    ├───android/android_tools.cpp
    ├───android/android_utils.cpp
    ├───android/android_manifest.cpp
    ├───android/android.h
    ├───android/android_utils.h
    ├───android/jdwp.h
    ├───android/jdwp.cpp
    ├───android/jdwp_util.cpp
    ├───android/jdwp_connection.cpp
    ├───core/plugins.cpp
    ├───core/plugins.h
    ├───core/resource_manager.cpp
    ├───core/resource_manager.h
    ├───core/sparse_page_table.cpp
    ├───core/sparse_page_table.h
    ├───data/glsl/glsl_ubos.h
    ├───data/glsl/glsl_ubos_cpp.h
    ├───hooks/hooks.cpp
    ├───hooks/hooks.h
    ├───maths/camera.cpp
    ├───maths/camera.h
    ├───maths/formatpacking.h
    ├───maths/formatpacking.cpp
    ├───maths/half_convert.h
    ├───maths/matrix.cpp
    ├───maths/matrix.h
    ├───maths/quat.h
    ├───maths/vec.cpp
    ├───maths/vec.h
    ├───os/os_specific.cpp
    ├───os/os_specific.h
    ├───replay/app_api.cpp
    ├───replay/basic_types_tests.cpp
    ├───replay/capture_options.cpp
    ├───replay/dummy_driver.cpp
    ├───replay/dummy_driver.h
    ├───replay/renderdoc_serialise.inl
    ├───replay/capture_file.cpp
    ├───replay/entry_points.cpp
    ├───replay/replay_driver.cpp
    ├───replay/replay_driver.h
    ├───replay/replay_output.cpp
    ├───replay/replay_controller.cpp
    ├───replay/replay_controller.h
    ├───serialise/serialiser.cpp
    ├───serialise/serialiser.h
    ├───serialise/lz4io.cpp
    ├───serialise/lz4io.h
    ├───serialise/zstdio.cpp
    ├───serialise/zstdio.h
    ├───serialise/streamio.cpp
    ├───serialise/streamio.h
    ├───serialise/rdcfile.cpp
    ├───serialise/rdcfile.h
    ├───serialise/codecs/xml_codec.cpp
    ├───serialise/codecs/chrome_json_codec.cpp
    ├───serialise/comp_io_tests.cpp
    ├───serialise/serialiser_tests.cpp
    ├───serialise/streamio_tests.cpp
    ├───strings/grisu2.cpp
    ├───strings/string_utils.cpp
    ├───strings/string_utils.h
    ├───strings/utf8printf.cpp
    ├───3rdparty/jpeg-compressor/jpgd.cpp
    ├───3rdparty/jpeg-compressor/jpgd.h
    ├───3rdparty/jpeg-compressor/jpge.cpp
    ├───3rdparty/jpeg-compressor/jpge.h
    ├───3rdparty/aosp/android_manifest.h
    ├───3rdparty/catch/catch.cpp
    ├───3rdparty/catch/catch.hpp
    ├───3rdparty/pugixml/pugixml.cpp
    ├───3rdparty/pugixml/pugixml.hpp
    ├───3rdparty/pugixml/pugiconfig.hpp
    ├───3rdparty/lz4/lz4.c
    ├───3rdparty/lz4/lz4.h
    ├───3rdparty/md5/md5.c
    ├───3rdparty/md5/md5.h
    ├───3rdparty/miniz/miniz.c
    ├───3rdparty/miniz/miniz.h
    ├───3rdparty/superluminal/superluminal.cpp
    ├───3rdparty/superluminal/superluminal.h
    ├───3rdparty/zstd/bitstream.h
    ├───3rdparty/zstd/compiler.h
    ├───3rdparty/zstd/cpu.h
    ├───3rdparty/zstd/debug.c
    ├───3rdparty/zstd/debug.h
    ├───3rdparty/zstd/entropy_common.c
    ├───3rdparty/zstd/error_private.c
    ├───3rdparty/zstd/error_private.h
    ├───3rdparty/zstd/fse.h
    ├───3rdparty/zstd/fse_compress.c
    ├───3rdparty/zstd/fse_decompress.c
    ├───3rdparty/zstd/hist.c
    ├───3rdparty/zstd/hist.h
    ├───3rdparty/zstd/huf.h
    ├───3rdparty/zstd/huf_compress.c
    ├───3rdparty/zstd/huf_decompress.c
    ├───3rdparty/zstd/mem.h
    ├───3rdparty/zstd/pool.c
    ├───3rdparty/zstd/pool.h
    ├───3rdparty/zstd/threading.c
    ├───3rdparty/zstd/threading.h
    ├───3rdparty/zstd/xxhash.c
    ├───3rdparty/zstd/xxhash.h
    ├───3rdparty/zstd/zstd.h
    ├───3rdparty/zstd/zstd_common.c
    ├───3rdparty/zstd/zstd_compress.c
    ├───3rdparty/zstd/zstd_compress_internal.h
    ├───3rdparty/zstd/zstd_decompress.c
    ├───3rdparty/zstd/zstd_double_fast.c
    ├───3rdparty/zstd/zstd_double_fast.h
    ├───3rdparty/zstd/zstd_errors.h
    ├───3rdparty/zstd/zstd_fast.c
    ├───3rdparty/zstd/zstd_fast.h
    ├───3rdparty/zstd/zstd_internal.h
    ├───3rdparty/zstd/zstd_lazy.c
    ├───3rdparty/zstd/zstd_lazy.h
    ├───3rdparty/zstd/zstd_ldm.c
    ├───3rdparty/zstd/zstd_ldm.h
    ├───3rdparty/zstd/zstd_opt.c
    ├───3rdparty/zstd/zstd_opt.h
    ├───3rdparty/zstd/zstdmt_compress.c
    ├───3rdparty/zstd/zstdmt_compress.h
    ├───3rdparty/stb/stb_image.h
    ├───3rdparty/stb/stb_image_write.h
    ├───3rdparty/stb/stb_image_resize.h
    ├───3rdparty/stb/stb_impl.c
    ├───3rdparty/stb/stb_truetype.h
    ├───3rdparty/tinyexr/tinyexr.cpp
    ├───3rdparty/tinyexr/tinyexr.h
    ├───3rdparty/tinyfiledialogs/tinyfiledialogs.c
    ├───3rdparty/tinyfiledialogs/tinyfiledialogs.h
    ├───if(ANDROID)
        ├───data/embedded_files.h
        ├───os/posix/android/android_stringio.cpp
        ├───os/posix/android/android_callstack.cpp
        ├───os/posix/android/android_process.cpp
        ├───os/posix/android/android_threading.cpp
        ├───os/posix/android/android_hook.cpp
        ├───os/posix/android/android_network.cpp
        ├───os/posix/posix_network.h
        ├───os/posix/posix_network.cpp
        ├───os/posix/posix_process.cpp
        ├───os/posix/posix_stringio.cpp
        ├───os/posix/posix_threading.cpp
        ├───os/posix/posix_specific.h
    ├───driver/gl (ENABLE_GL OR ENABLE_GLES)
        ├───CMakeLists.txt("rdoc_gl",OBJECT_LIBRARY)
        ├───gl_common.cpp
        ├───gl_common.h
        ├───gl_counters.cpp
        ├───gl_debug.cpp
        ├───gl_postvs.cpp
        ├───gl_overlay.cpp
        ├───gl_outputwindow.cpp
        ├───gl_rendermesh.cpp
        ├───gl_rendertexture.cpp
        ├───gl_rendertext.cpp
        ├───gl_msaa_array_conv.cpp
        ├───gl_driver.cpp
        ├───gl_driver.h
        ├───gl_enum.h
        ├───gl_dispatch_table_defs.h
        ├───gl_dispatch_table.h
        ├───gl_initstate.h
        ├───gl_initstate.cpp
        ├───gl_manager.cpp
        ├───gl_manager.h
        ├───gl_renderstate.cpp
        ├───gl_renderstate.h
        ├───gl_replay.cpp
        ├───gl_replay.h
        ├───gl_resources.cpp
        ├───gl_resources.h
        ├───gl_program_iterate.cpp
        ├───gl_shader_refl.cpp
        ├───gl_shader_refl.h
        ├───gl_stringise.cpp
        ├───official/gl32.h
        ├───official/gl3platform.h
        ├───official/glcorearb.h
        ├───official/glext.h
        ├───official/khrplatform.h
        ├───wrappers/gl_buffer_funcs.cpp
        ├───wrappers/gl_debug_funcs.cpp
        ├───wrappers/gl_draw_funcs.cpp
        ├───wrappers/gl_emulated.cpp
        ├───wrappers/gl_framebuffer_funcs.cpp
        ├───wrappers/gl_get_funcs.cpp
        ├───wrappers/gl_interop_funcs.cpp
        ├───wrappers/gl_query_funcs.cpp
        ├───wrappers/gl_sampler_funcs.cpp
        ├───wrappers/gl_shader_funcs.cpp
        ├───wrappers/gl_state_funcs.cpp
        ├───wrappers/gl_texture_funcs.cpp
        ├───wrappers/gl_uniform_funcs.cpp
        ├───gl_hooks.cpp
        ├───if(ANDROID AND ENABLE_GL)
            ├───official/glxext.h
            ├───glx_dispatch_table.h
            ├───glx_platform.cpp
            ├───glx_hooks.cpp
            ├───glx_fake_vk_hooks.cpp
        ├───if(ANDROID AND ENABLE_GLES)
            ├───official/glesext.h
        ├───if(ANDROID AND ENABLE_EGL)
            ├───official/egl.h
            ├───official/eglext.h
            ├───official/eglplatform.h
            ├───egl_dispatch_table.h
            ├───egl_platform.cpp
            ├───egl_hooks.cpp
    ├───driver/vulkan (ENABLE_VULKAN)
        ├───CMakeLists.txt("rdoc_vulkan",OBJECT_LIBRARY)
        ├───vk_common.cpp
        ├───vk_common.h
        ├───vk_next_chains.cpp
        ├───vk_core.cpp
        ├───vk_core.h
        ├───vk_counters.cpp
        ├───vk_debug.h
        ├───vk_debug.cpp
        ├───vk_postvs.cpp
        ├───vk_shader_feedback.cpp
        ├───vk_overlay.cpp
        ├───vk_msaa_array_conv.cpp
        ├───vk_outputwindow.cpp
        ├───vk_rendermesh.cpp
        ├───vk_rendertexture.cpp
        ├───vk_rendertext.h
        ├───vk_rendertext.cpp
        ├───vk_shader_cache.h
        ├───vk_shader_cache.cpp
        ├───vk_dispatchtables.cpp
        ├───vk_dispatchtables.h
        ├───vk_dispatch_defs.h
        ├───vk_hookset_defs.h
        ├───vk_image_states.cpp
        ├───vk_info.cpp
        ├───vk_info.h
        ├───vk_initstate.cpp
        ├───vk_manager.cpp
        ├───vk_manager.h
        ├───vk_memory.cpp
        ├───vk_pixelhistory.cpp
        ├───vk_replay.cpp
        ├───vk_replay.h
        ├───vk_resources.cpp
        ├───vk_resources.h
        ├───vk_shaderdebug.cpp
        ├───vk_state.cpp
        ├───vk_state.h
        ├───vk_serialise.cpp
        ├───vk_stringise.cpp
        ├───vk_layer.cpp
        ├───imagestate_tests.cpp
        ├───imgrefs_tests.cpp
        ├───official/vk_layer.h
        ├───official/vk_platform.h
        ├───official/vulkan.h
        ├───official/vulkan_android.h
        ├───official/vulkan_core.h
        ├───official/vulkan_ggp.h
        ├───official/vulkan_ios.h
        ├───official/vulkan_macos.h
        ├───official/vulkan_fuchsia.h
        ├───official/vulkan_vi.h
        ├───official/vulkan_wayland.h
        ├───official/vulkan_win32.h
        ├───official/vulkan_xcb.h
        ├───official/vulkan_xlib.h
        ├───official/vulkan_xlib_xrandr.h
        ├───wrappers/vk_cmd_funcs.cpp
        ├───wrappers/vk_descriptor_funcs.cpp
        ├───wrappers/vk_device_funcs.cpp
        ├───wrappers/vk_draw_funcs.cpp
        ├───wrappers/vk_dynamic_funcs.cpp
        ├───wrappers/vk_get_funcs.cpp
        ├───wrappers/vk_misc_funcs.cpp
        ├───wrappers/vk_queue_funcs.cpp
        ├───wrappers/vk_resource_funcs.cpp
        ├───wrappers/vk_shader_funcs.cpp
        ├───wrappers/vk_sync_funcs.cpp
        ├───wrappers/vk_wsi_funcs.cpp
        ├───if(ANDROID)
        	├───vk_posix.cpp
        	├───vk_android.cpp
        	├───vk_layer_android.cpp
    ├───driver/shaders/spirv (ENABLE_GL OR ENABLE_GLES OR ENABLE_VULKAN)
        ├───CMakeLists.txt("rdoc_spirv",OBJECT_LIBRARY)
        ├───glslang_compile.cpp
        ├───glslang_compile.h
        ├───spirv_common.cpp
        ├───spirv_common.h
        ├───spirv_editor.cpp
        ├───spirv_editor.h
        ├───spirv_gen.cpp
        ├───spirv_gen.h
        ├───spirv_op_helpers.h
        ├───spirv_compile.cpp
        ├───spirv_compile.h
        ├───spirv_debug_setup.cpp
        ├───spirv_debug_glsl450.cpp
        ├───spirv_debug.cpp
        ├───spirv_debug.h
        ├───spirv_reflect.cpp
        ├───spirv_reflect.h
        ├───spirv_processor.cpp
        ├───spirv_processor.h
        ├───spirv_disassemble.cpp
        ├───spirv_stringise.cpp
        ├───var_dispatch_helpers.h
        ├───(添加依赖的glslang_source_code)
        ├───3rdparty/glslang/OGLCompilersDLL/InitializeDll.cpp
        ├───3rdparty/glslang/OGLCompilersDLL/InitializeDll.h
        ├───3rdparty/glslang/SPIRV/GlslangToSpv.cpp
        ├───3rdparty/glslang/SPIRV/GlslangToSpv.h
        ├───3rdparty/glslang/SPIRV/GLSL.std.450.h
        ├───3rdparty/glslang/SPIRV/GLSL.ext.AMD.h
        ├───3rdparty/glslang/SPIRV/GLSL.ext.EXT.h
        ├───3rdparty/glslang/SPIRV/GLSL.ext.KHR.h
        ├───3rdparty/glslang/SPIRV/GLSL.ext.NV.h
        ├───3rdparty/glslang/SPIRV/hex_float.h
        ├───3rdparty/glslang/SPIRV/InReadableOrder.cpp
        ├───3rdparty/glslang/SPIRV/Logger.cpp
        ├───3rdparty/glslang/SPIRV/Logger.h
        ├───3rdparty/glslang/SPIRV/SpvBuilder.cpp
        ├───3rdparty/glslang/SPIRV/SpvBuilder.h
        ├───3rdparty/glslang/SPIRV/SpvTools.cpp
        ├───3rdparty/glslang/SPIRV/SpvTools.h
        ├───3rdparty/glslang/SPIRV/SpvPostProcess.cpp
        ├───3rdparty/glslang/SPIRV/spvIR.h
        ├───3rdparty/glslang/glslang/GenericCodeGen/CodeGen.cpp
        ├───3rdparty/glslang/glslang/GenericCodeGen/Link.cpp
        ├───3rdparty/glslang/glslang/Include/arrays.h
        ├───3rdparty/glslang/glslang/Include/BaseTypes.h
        ├───3rdparty/glslang/glslang/Include/Common.h
        ├───3rdparty/glslang/glslang/Include/ConstantUnion.h
        ├───3rdparty/glslang/glslang/Include/InfoSink.h
        ├───3rdparty/glslang/glslang/Include/InitializeGlobals.h
        ├───3rdparty/glslang/glslang/Include/intermediate.h
        ├───3rdparty/glslang/glslang/Include/PoolAlloc.h
        ├───3rdparty/glslang/glslang/Include/ResourceLimits.h
        ├───3rdparty/glslang/glslang/Include/revision.h
        ├───3rdparty/glslang/glslang/Include/ShHandle.h
        ├───3rdparty/glslang/glslang/Include/Types.h
        ├───3rdparty/glslang/glslang/MachineIndependent/Constant.cpp
        ├───3rdparty/glslang/glslang/MachineIndependent/glslang_tab.cpp
        ├───3rdparty/glslang/glslang/MachineIndependent/glslang_tab.cpp.h
        ├───3rdparty/glslang/glslang/MachineIndependent/gl_types.h
        ├───3rdparty/glslang/glslang/MachineIndependent/iomapper.cpp
        ├───3rdparty/glslang/glslang/MachineIndependent/iomapper.h
        ├───3rdparty/glslang/glslang/MachineIndependent/gl_types.h
        ├───3rdparty/glslang/glslang/MachineIndependent/InfoSink.cpp
        ├───3rdparty/glslang/glslang/MachineIndependent/Initialize.cpp
        ├───3rdparty/glslang/glslang/MachineIndependent/Initialize.h
        ├───3rdparty/glslang/glslang/MachineIndependent/Intermediate.cpp
        ├───3rdparty/glslang/glslang/MachineIndependent/intermOut.cpp
        ├───3rdparty/glslang/glslang/MachineIndependent/IntermTraverse.cpp
        ├───3rdparty/glslang/glslang/MachineIndependent/limits.cpp
        ├───3rdparty/glslang/glslang/MachineIndependent/linkValidate.cpp
        ├───3rdparty/glslang/glslang/MachineIndependent/LiveTraverser.h
        ├───3rdparty/glslang/glslang/MachineIndependent/localintermediate.h
        ├───3rdparty/glslang/glslang/MachineIndependent/parseConst.cpp
        ├───3rdparty/glslang/glslang/MachineIndependent/ParseContextBase.cpp
        ├───3rdparty/glslang/glslang/MachineIndependent/ParseHelper.cpp
        ├───3rdparty/glslang/glslang/MachineIndependent/ParseHelper.h
        ├───3rdparty/glslang/glslang/MachineIndependent/PoolAlloc.cpp
        ├───3rdparty/glslang/glslang/MachineIndependent/propagateNoContraction.cpp
        ├───3rdparty/glslang/glslang/MachineIndependent/propagateNoContraction.h
        ├───3rdparty/glslang/glslang/MachineIndependent/preprocessor/PpAtom.cpp
        ├───3rdparty/glslang/glslang/MachineIndependent/preprocessor/PpContext.cpp
        ├───3rdparty/glslang/glslang/MachineIndependent/preprocessor/PpContext.h
        ├───3rdparty/glslang/glslang/MachineIndependent/preprocessor/Pp.cpp
        ├───3rdparty/glslang/glslang/MachineIndependent/preprocessor/PpScanner.cpp
        ├───3rdparty/glslang/glslang/MachineIndependent/preprocessor/PpTokens.cpp
        ├───3rdparty/glslang/glslang/MachineIndependent/preprocessor/PpTokens.h
        ├───3rdparty/glslang/glslang/MachineIndependent/reflection.cpp
        ├───3rdparty/glslang/glslang/MachineIndependent/reflection.h
        ├───3rdparty/glslang/glslang/MachineIndependent/RemoveTree.cpp
        ├───3rdparty/glslang/glslang/MachineIndependent/RemoveTree.h
        ├───3rdparty/glslang/glslang/MachineIndependent/ScanContext.h
        ├───3rdparty/glslang/glslang/MachineIndependent/Scan.cpp
        ├───3rdparty/glslang/glslang/MachineIndependent/Scan.h
        ├───3rdparty/glslang/glslang/MachineIndependent/ShaderLang.cpp
        ├───3rdparty/glslang/glslang/MachineIndependent/SymbolTable.cpp
        ├───3rdparty/glslang/glslang/MachineIndependent/SymbolTable.h
        ├───3rdparty/glslang/glslang/MachineIndependent/Versions.cpp
        ├───3rdparty/glslang/glslang/MachineIndependent/Versions.h
        ├───3rdparty/glslang/glslang/MachineIndependent/attribute.cpp
        ├───3rdparty/glslang/glslang/MachineIndependent/attribute.h
        ├───3rdparty/glslang/glslang/OSDependent/osinclude.h
        ├───3rdparty/glslang/glslang/Public/ShaderLang.h)
    ├───ihv(Independent Hardware Vendor,独立硬件供应商,下面是针对不同硬件的库适配代码)
    ├───driver/ihv/amd
        ├───CMakeLists.txt("rdoc_amd",OBJECT_LIBRARY)
        ├───amd_counters.cpp
        ├───amd_counters.h
        ├───amd_isa.cpp
        ├───amd_isa.h
        ├───amd_isa_devices.cpp
        ├───amd_isa_devices.h
        ├───amd_isa_posix.cpp
        ├───amd_rgp.cpp
        ├───amd_rgp.h
        ├───official/RGP/DevDriverAPI.h
    ├───driver/ihv/intel(ENABLE_GL OR ENABLE_GLES)
        ├───CMakeLists.txt("rdoc_intel",OBJECT_LIBRARY)
        ├───intel_gl_counters.cpp
        ├───intel_gl_counters.h
    ├───driver/ihv/arm(ENABLE_GL OR ENABLE_GLES)
        ├───CMakeLists.txt("rdoc_arm",OBJECT_LIBRARY)
        ├───arm_counters.h
        ├───arm_counters.cpp
        ├───official/lizard/include/lizard/lizard_api.h
        ├───official/lizard/lizard_api.cpp
        ├───official/lizard/include/lizard/lizard.hpp
        ├───official/lizard/lizard.cpp
        ├───official/lizard/lizard_counter.cpp
        ├───official/lizard/lizard_communication.hpp
        ├───official/lizard/lizard_communication.cpp
        ├───official/lizard/hwcpipe_communication.hpp
        ├───official/lizard/hwcpipe_communication.cpp
        ├───official/lizard/hwcpipe_api.hpp
        ├───official/lizard/hwcpipe_api.cpp
        ├───official/lizard/gatord_xml_reader.cpp
        ├───official/lizard/gator_api.cpp
        ├───official/lizard/gator_constants.hpp
        ├───official/lizard/gator_message.cpp
        ├───official/lizard/gator_message.hpp
        ├───official/lizard/message_util.cpp
        ├───official/lizard/message_util.hpp
        ├───official/lizard/socket.cpp
        ├───official/lizard/thirdparty/hwcpipe/hwcpipe.h
        ├───official/lizard/thirdparty/hwcpipe/hwcpipe.cpp
        ├───official/lizard/thirdparty/hwcpipe/cpu_profiler.h
        ├───official/lizard/thirdparty/hwcpipe/gpu_profiler.h
        ├───official/lizard/thirdparty/hwcpipe/value.h
        ├───official/lizard/thirdparty/hwcpipe/vendor/arm/mali/hwc.hpp
        ├───official/lizard/thirdparty/hwcpipe/vendor/arm/mali/hwc_names.hpp
        ├───official/lizard/thirdparty/hwcpipe/vendor/arm/mali/mali_profiler.h
        ├───official/lizard/thirdparty/hwcpipe/vendor/arm/mali/mali_profiler.cpp
        ├───official/lizard/thirdparty/hwcpipe/vendor/arm/pmu/pmu_counter.h
        ├───official/lizard/thirdparty/hwcpipe/vendor/arm/pmu/pmu_counter.cpp
        ├───official/lizard/thirdparty/hwcpipe/vendor/arm/pmu/pmu_profiler.h
        ├───official/lizard/thirdparty/hwcpipe/vendor/arm/pmu/pmu_profiler.cpp
        ├───if((NOT ANDROID))
        	├───arm_counters_stub.cpp
    ├───3rdparty/interceptor-lib/lib(ANDROID AND USE_INTERCEPTOR_LIB，Android下通过USE_INTERCEPTOR_LIB打开，默认关闭，需要LLVM编译器，用途未知)
        ├───CMakeLists.txt("interceptor_lib",OBJECT_LIBRARY)
        ├───code_generator.cc
        ├───constant_pool_data_expr.cc
        ├───disassembler.cc
        ├───error.cc
        ├───interceptor.cc
        ├───linker.cc
        ├───memory_manager.cc
        ├───target.cc
        ├───${LLVM_TARGETS_TO_BUILD}/target_${TARGET}.cc
├───qrenderdoc(ENABLE_QRENDERDOC/ENABLE_PYRENDERDOC, Android编译不包含这部分代码，应该是QT编译相关，所以不做展开)
        ├───CMakeLists.txt
└───renderdoccmd(ENABLE_RENDERDOCCMD)
    ├───CMakeLists.txt
```

对应的CMake的Log如下：

```tex
$ cmake -DBUILD_ANDROID=On -DANDROID_ABI=armeabi-v7a -G "MinGW Makefiles" ..
-- Using JAVA_HOME = D:\Envs\Java\jdk1.8.0_181
-- Java in JAVA_HOME is 1.8
-- Using Android SDK found in D:\Envs\RDCAndroidSDK
-- Using Android NDK found in D:\Envs\RDCAndroidSDK\android-ndk-r14b
-- Android: Targeting API '21' with architecture 'arm', ABI 'armeabi-v7a', and processor 'armv7-a'
-- Android: Selected Clang toolchain 'arm-linux-androideabi-clang' with GCC toolchain 'arm-linux-androideabi-4.9'
-- Detecting CXX compiler ABI info
-- Detecting CXX compiler ABI info - done
-- Check for working CXX compiler: D:/Envs/RDCAndroidSDK/android-ndk-r14b/toolchains/llvm/prebuilt/windows-x86_64/bin/clang++.exe - skipped
-- Detecting CXX compile features
-- Detecting CXX compile features - done
-- Detecting C compiler ABI info
-- Detecting C compiler ABI info - done
-- Check for working C compiler: D:/Envs/RDCAndroidSDK/android-ndk-r14b/toolchains/llvm/prebuilt/windows-x86_64/bin/clang.exe - skipped
-- Detecting C compile features
-- Detecting C compile features - done
-- Calculating version
-- Building RenderDoc version 1.17
-- Disabling GL driver on android
-- Disabling qrenderdoc for android build
-- Disabling renderdoc python modules for android build
-- Using Android ABI armeabi-v7a
-- Using Android native API level 21
-- Building RenderDoc in Release mode: RelWithDebInfo
-- Interceptor-lib not enabled (USE_INTERCEPTOR_LIB) - android hooking will use sometimes less reliable PLT-interception method.
-- Found Java: D:/Envs/Java/jdk1.8.0_181/bin/java.exe (found version "1.8.0_181")
-- Using Java of version 1.8.0.181
-- Using Android build-tools version 26.0.1
-- Using android.jar from platform android-23
-- Building APK versionCode 117, versionName 75d05bfb4b0324adc91c49854371dc786e70a0ee
-- Enabled APIs:
--   - OpenGL ES (EGL)
--   - Vulkan
-- Configuring done
-- Generating done
-- Build files have been written to: D:/WorkSpace/C++/RenderDoc/android32
```

mingw32-make的Log如下：

```tex
$ mingw32-make
[  0%] Building CXX object renderdoc/driver/ihv/arm/CMakeFiles/rdoc_arm.dir/arm_counters.cpp.o
[  0%] Building CXX object renderdoc/driver/ihv/arm/CMakeFiles/rdoc_arm.dir/official/lizard/lizard_api.cpp.o
[  1%] Building CXX object renderdoc/driver/ihv/arm/CMakeFiles/rdoc_arm.dir/official/lizard/lizard.cpp.o
[  1%] Building CXX object renderdoc/driver/ihv/arm/CMakeFiles/rdoc_arm.dir/official/lizard/lizard_counter.cpp.o
[  1%] Building CXX object renderdoc/driver/ihv/arm/CMakeFiles/rdoc_arm.dir/official/lizard/lizard_communication.cpp.o
[  2%] Building CXX object renderdoc/driver/ihv/arm/CMakeFiles/rdoc_arm.dir/official/lizard/hwcpipe_communication.cpp.o
[  2%] Building CXX object renderdoc/driver/ihv/arm/CMakeFiles/rdoc_arm.dir/official/lizard/hwcpipe_api.cpp.o
[  2%] Building CXX object renderdoc/driver/ihv/arm/CMakeFiles/rdoc_arm.dir/official/lizard/gatord_xml_reader.cpp.o
[  3%] Building CXX object renderdoc/driver/ihv/arm/CMakeFiles/rdoc_arm.dir/official/lizard/gator_api.cpp.o
[  3%] Building CXX object renderdoc/driver/ihv/arm/CMakeFiles/rdoc_arm.dir/official/lizard/gator_message.cpp.o
[  3%] Building CXX object renderdoc/driver/ihv/arm/CMakeFiles/rdoc_arm.dir/official/lizard/message_util.cpp.o
[  4%] Building CXX object renderdoc/driver/ihv/arm/CMakeFiles/rdoc_arm.dir/official/lizard/socket.cpp.o
[  4%] Building CXX object renderdoc/driver/ihv/arm/CMakeFiles/rdoc_arm.dir/official/lizard/thirdparty/hwcpipe/hwcpipe.cpp.o
[  4%] Building CXX object renderdoc/driver/ihv/arm/CMakeFiles/rdoc_arm.dir/official/lizard/thirdparty/hwcpipe/vendor/arm/mali/mali_profiler.cpp.o
[  4%] Building CXX object renderdoc/driver/ihv/arm/CMakeFiles/rdoc_arm.dir/official/lizard/thirdparty/hwcpipe/vendor/arm/pmu/pmu_counter.cpp.o
[  5%] Building CXX object renderdoc/driver/ihv/arm/CMakeFiles/rdoc_arm.dir/official/lizard/thirdparty/hwcpipe/vendor/arm/pmu/pmu_profiler.cpp.o
[  5%] Built target rdoc_arm
[  5%] Building CXX object renderdoc/CMakeFiles/rdoc_version.dir/replay/version.cpp.o
[  5%] Built target rdoc_version
[  5%] Building CXX object renderdoc/CMakeFiles/renderdoc_libentry.dir/os/posix/posix_libentry.cpp.o
[  5%] Linking CXX static library librenderdoc_libentry.a
[  5%] Built target renderdoc_libentry
[  6%] Building CXX object renderdoc/driver/ihv/amd/CMakeFiles/rdoc_amd.dir/amd_counters.cpp.o
[  6%] Building CXX object renderdoc/driver/ihv/amd/CMakeFiles/rdoc_amd.dir/amd_isa.cpp.o
[  6%] Building CXX object renderdoc/driver/ihv/amd/CMakeFiles/rdoc_amd.dir/amd_isa_devices.cpp.o
[  7%] Building CXX object renderdoc/driver/ihv/amd/CMakeFiles/rdoc_amd.dir/amd_isa_posix.cpp.o
[  7%] Building CXX object renderdoc/driver/ihv/amd/CMakeFiles/rdoc_amd.dir/amd_rgp.cpp.o
[  7%] Built target rdoc_amd
[  7%] Building CXX object renderdoc/driver/shaders/spirv/CMakeFiles/rdoc_spirv.dir/glslang_compile.cpp.o
[  7%] Building CXX object renderdoc/driver/shaders/spirv/CMakeFiles/rdoc_spirv.dir/spirv_common.cpp.o
[  8%] Building CXX object renderdoc/driver/shaders/spirv/CMakeFiles/rdoc_spirv.dir/spirv_editor.cpp.o
[  8%] Building CXX object renderdoc/driver/shaders/spirv/CMakeFiles/rdoc_spirv.dir/spirv_gen.cpp.o
[  8%] Building CXX object renderdoc/driver/shaders/spirv/CMakeFiles/rdoc_spirv.dir/spirv_compile.cpp.o
[  9%] Building CXX object renderdoc/driver/shaders/spirv/CMakeFiles/rdoc_spirv.dir/spirv_debug_setup.cpp.o
[  9%] Building CXX object renderdoc/driver/shaders/spirv/CMakeFiles/rdoc_spirv.dir/spirv_debug_glsl450.cpp.o
[  9%] Building CXX object renderdoc/driver/shaders/spirv/CMakeFiles/rdoc_spirv.dir/spirv_debug.cpp.o
[ 10%] Building CXX object renderdoc/driver/shaders/spirv/CMakeFiles/rdoc_spirv.dir/spirv_reflect.cpp.o
[ 10%] Building CXX object renderdoc/driver/shaders/spirv/CMakeFiles/rdoc_spirv.dir/spirv_processor.cpp.o
[ 10%] Building CXX object renderdoc/driver/shaders/spirv/CMakeFiles/rdoc_spirv.dir/spirv_disassemble.cpp.o
[ 11%] Building CXX object renderdoc/driver/shaders/spirv/CMakeFiles/rdoc_spirv.dir/spirv_stringise.cpp.o
[ 11%] Building CXX object renderdoc/driver/shaders/spirv/CMakeFiles/rdoc_spirv.dir/__/__/__/3rdparty/glslang/OGLCompilersDLL/InitializeDll.cpp.o
[ 11%] Building CXX object renderdoc/driver/shaders/spirv/CMakeFiles/rdoc_spirv.dir/__/__/__/3rdparty/glslang/SPIRV/GlslangToSpv.cpp.o
[ 11%] Building CXX object renderdoc/driver/shaders/spirv/CMakeFiles/rdoc_spirv.dir/__/__/__/3rdparty/glslang/SPIRV/InReadableOrder.cpp.o
[ 12%] Building CXX object renderdoc/driver/shaders/spirv/CMakeFiles/rdoc_spirv.dir/__/__/__/3rdparty/glslang/SPIRV/Logger.cpp.o
[ 12%] Building CXX object renderdoc/driver/shaders/spirv/CMakeFiles/rdoc_spirv.dir/__/__/__/3rdparty/glslang/SPIRV/SpvBuilder.cpp.o
[ 12%] Building CXX object renderdoc/driver/shaders/spirv/CMakeFiles/rdoc_spirv.dir/__/__/__/3rdparty/glslang/SPIRV/SpvTools.cpp.o
[ 13%] Building CXX object renderdoc/driver/shaders/spirv/CMakeFiles/rdoc_spirv.dir/__/__/__/3rdparty/glslang/SPIRV/SpvPostProcess.cpp.o
[ 13%] Building CXX object renderdoc/driver/shaders/spirv/CMakeFiles/rdoc_spirv.dir/__/__/__/3rdparty/glslang/glslang/GenericCodeGen/CodeGen.cpp.o
[ 13%] Building CXX object renderdoc/driver/shaders/spirv/CMakeFiles/rdoc_spirv.dir/__/__/__/3rdparty/glslang/glslang/GenericCodeGen/Link.cpp.o
[ 14%] Building CXX object renderdoc/driver/shaders/spirv/CMakeFiles/rdoc_spirv.dir/__/__/__/3rdparty/glslang/glslang/MachineIndependent/Constant.cpp.o
[ 14%] Building CXX object renderdoc/driver/shaders/spirv/CMakeFiles/rdoc_spirv.dir/__/__/__/3rdparty/glslang/glslang/MachineIndependent/glslang_tab.cpp.o
[ 14%] Building CXX object renderdoc/driver/shaders/spirv/CMakeFiles/rdoc_spirv.dir/__/__/__/3rdparty/glslang/glslang/MachineIndependent/iomapper.cpp.o
[ 14%] Building CXX object renderdoc/driver/shaders/spirv/CMakeFiles/rdoc_spirv.dir/__/__/__/3rdparty/glslang/glslang/MachineIndependent/InfoSink.cpp.o
[ 15%] Building CXX object renderdoc/driver/shaders/spirv/CMakeFiles/rdoc_spirv.dir/__/__/__/3rdparty/glslang/glslang/MachineIndependent/Initialize.cpp.o
[ 15%] Building CXX object renderdoc/driver/shaders/spirv/CMakeFiles/rdoc_spirv.dir/__/__/__/3rdparty/glslang/glslang/MachineIndependent/Intermediate.cpp.o
[ 15%] Building CXX object renderdoc/driver/shaders/spirv/CMakeFiles/rdoc_spirv.dir/__/__/__/3rdparty/glslang/glslang/MachineIndependent/intermOut.cpp.o
[ 16%] Building CXX object renderdoc/driver/shaders/spirv/CMakeFiles/rdoc_spirv.dir/__/__/__/3rdparty/glslang/glslang/MachineIndependent/IntermTraverse.cpp.o
[ 16%] Building CXX object renderdoc/driver/shaders/spirv/CMakeFiles/rdoc_spirv.dir/__/__/__/3rdparty/glslang/glslang/MachineIndependent/limits.cpp.o
[ 16%] Building CXX object renderdoc/driver/shaders/spirv/CMakeFiles/rdoc_spirv.dir/__/__/__/3rdparty/glslang/glslang/MachineIndependent/linkValidate.cpp.o
[ 17%] Building CXX object renderdoc/driver/shaders/spirv/CMakeFiles/rdoc_spirv.dir/__/__/__/3rdparty/glslang/glslang/MachineIndependent/parseConst.cpp.o
[ 17%] Building CXX object renderdoc/driver/shaders/spirv/CMakeFiles/rdoc_spirv.dir/__/__/__/3rdparty/glslang/glslang/MachineIndependent/ParseContextBase.cpp.o
[ 17%] Building CXX object renderdoc/driver/shaders/spirv/CMakeFiles/rdoc_spirv.dir/__/__/__/3rdparty/glslang/glslang/MachineIndependent/ParseHelper.cpp.o
[ 18%] Building CXX object renderdoc/driver/shaders/spirv/CMakeFiles/rdoc_spirv.dir/__/__/__/3rdparty/glslang/glslang/MachineIndependent/PoolAlloc.cpp.o
[ 18%] Building CXX object renderdoc/driver/shaders/spirv/CMakeFiles/rdoc_spirv.dir/__/__/__/3rdparty/glslang/glslang/MachineIndependent/propagateNoContraction.cpp.o
[ 18%] Building CXX object renderdoc/driver/shaders/spirv/CMakeFiles/rdoc_spirv.dir/__/__/__/3rdparty/glslang/glslang/MachineIndependent/preprocessor/PpAtom.cpp.o
[ 18%] Building CXX object renderdoc/driver/shaders/spirv/CMakeFiles/rdoc_spirv.dir/__/__/__/3rdparty/glslang/glslang/MachineIndependent/preprocessor/PpContext.cpp.o
[ 19%] Building CXX object renderdoc/driver/shaders/spirv/CMakeFiles/rdoc_spirv.dir/__/__/__/3rdparty/glslang/glslang/MachineIndependent/preprocessor/Pp.cpp.o
[ 19%] Building CXX object renderdoc/driver/shaders/spirv/CMakeFiles/rdoc_spirv.dir/__/__/__/3rdparty/glslang/glslang/MachineIndependent/preprocessor/PpScanner.cpp.o
[ 19%] Building CXX object renderdoc/driver/shaders/spirv/CMakeFiles/rdoc_spirv.dir/__/__/__/3rdparty/glslang/glslang/MachineIndependent/preprocessor/PpTokens.cpp.o
[ 20%] Building CXX object renderdoc/driver/shaders/spirv/CMakeFiles/rdoc_spirv.dir/__/__/__/3rdparty/glslang/glslang/MachineIndependent/reflection.cpp.o
[ 20%] Building CXX object renderdoc/driver/shaders/spirv/CMakeFiles/rdoc_spirv.dir/__/__/__/3rdparty/glslang/glslang/MachineIndependent/RemoveTree.cpp.o
[ 20%] Building CXX object renderdoc/driver/shaders/spirv/CMakeFiles/rdoc_spirv.dir/__/__/__/3rdparty/glslang/glslang/MachineIndependent/Scan.cpp.o
[ 21%] Building CXX object renderdoc/driver/shaders/spirv/CMakeFiles/rdoc_spirv.dir/__/__/__/3rdparty/glslang/glslang/MachineIndependent/ShaderLang.cpp.o
[ 21%] Building CXX object renderdoc/driver/shaders/spirv/CMakeFiles/rdoc_spirv.dir/__/__/__/3rdparty/glslang/glslang/MachineIndependent/SymbolTable.cpp.o
[ 21%] Building CXX object renderdoc/driver/shaders/spirv/CMakeFiles/rdoc_spirv.dir/__/__/__/3rdparty/glslang/glslang/MachineIndependent/Versions.cpp.o
[ 22%] Building CXX object renderdoc/driver/shaders/spirv/CMakeFiles/rdoc_spirv.dir/__/__/__/3rdparty/glslang/glslang/MachineIndependent/attribute.cpp.o
[ 22%] Building CXX object renderdoc/driver/shaders/spirv/CMakeFiles/rdoc_spirv.dir/__/__/__/3rdparty/glslang/glslang/OSDependent/Unix/ossource.cpp.o
[ 22%] Built target rdoc_spirv
[ 22%] Building CXX object renderdoc/driver/gl/CMakeFiles/rdoc_gl.dir/gl_common.cpp.o
[ 22%] Building CXX object renderdoc/driver/gl/CMakeFiles/rdoc_gl.dir/gl_counters.cpp.o
[ 23%] Building CXX object renderdoc/driver/gl/CMakeFiles/rdoc_gl.dir/gl_debug.cpp.o
[ 23%] Building CXX object renderdoc/driver/gl/CMakeFiles/rdoc_gl.dir/gl_postvs.cpp.o
[ 23%] Building CXX object renderdoc/driver/gl/CMakeFiles/rdoc_gl.dir/gl_overlay.cpp.o
[ 24%] Building CXX object renderdoc/driver/gl/CMakeFiles/rdoc_gl.dir/gl_outputwindow.cpp.o
[ 24%] Building CXX object renderdoc/driver/gl/CMakeFiles/rdoc_gl.dir/gl_rendermesh.cpp.o
[ 24%] Building CXX object renderdoc/driver/gl/CMakeFiles/rdoc_gl.dir/gl_rendertexture.cpp.o
[ 25%] Building CXX object renderdoc/driver/gl/CMakeFiles/rdoc_gl.dir/gl_rendertext.cpp.o
[ 25%] Building CXX object renderdoc/driver/gl/CMakeFiles/rdoc_gl.dir/gl_msaa_array_conv.cpp.o
[ 25%] Building CXX object renderdoc/driver/gl/CMakeFiles/rdoc_gl.dir/gl_driver.cpp.o
[ 25%] Building CXX object renderdoc/driver/gl/CMakeFiles/rdoc_gl.dir/gl_initstate.cpp.o
[ 26%] Building CXX object renderdoc/driver/gl/CMakeFiles/rdoc_gl.dir/gl_manager.cpp.o
[ 26%] Building CXX object renderdoc/driver/gl/CMakeFiles/rdoc_gl.dir/gl_renderstate.cpp.o
[ 26%] Building CXX object renderdoc/driver/gl/CMakeFiles/rdoc_gl.dir/gl_replay.cpp.o
[ 27%] Building CXX object renderdoc/driver/gl/CMakeFiles/rdoc_gl.dir/gl_resources.cpp.o
[ 27%] Building CXX object renderdoc/driver/gl/CMakeFiles/rdoc_gl.dir/gl_program_iterate.cpp.o
[ 27%] Building CXX object renderdoc/driver/gl/CMakeFiles/rdoc_gl.dir/gl_shader_refl.cpp.o
[ 28%] Building CXX object renderdoc/driver/gl/CMakeFiles/rdoc_gl.dir/gl_stringise.cpp.o
[ 28%] Building CXX object renderdoc/driver/gl/CMakeFiles/rdoc_gl.dir/wrappers/gl_buffer_funcs.cpp.o
[ 28%] Building CXX object renderdoc/driver/gl/CMakeFiles/rdoc_gl.dir/wrappers/gl_debug_funcs.cpp.o
[ 29%] Building CXX object renderdoc/driver/gl/CMakeFiles/rdoc_gl.dir/wrappers/gl_draw_funcs.cpp.o
[ 29%] Building CXX object renderdoc/driver/gl/CMakeFiles/rdoc_gl.dir/wrappers/gl_emulated.cpp.o
[ 29%] Building CXX object renderdoc/driver/gl/CMakeFiles/rdoc_gl.dir/wrappers/gl_framebuffer_funcs.cpp.o
[ 29%] Building CXX object renderdoc/driver/gl/CMakeFiles/rdoc_gl.dir/wrappers/gl_get_funcs.cpp.o
[ 30%] Building CXX object renderdoc/driver/gl/CMakeFiles/rdoc_gl.dir/wrappers/gl_interop_funcs.cpp.o
[ 30%] Building CXX object renderdoc/driver/gl/CMakeFiles/rdoc_gl.dir/wrappers/gl_query_funcs.cpp.o
[ 30%] Building CXX object renderdoc/driver/gl/CMakeFiles/rdoc_gl.dir/wrappers/gl_sampler_funcs.cpp.o
[ 31%] Building CXX object renderdoc/driver/gl/CMakeFiles/rdoc_gl.dir/wrappers/gl_shader_funcs.cpp.o
[ 31%] Building CXX object renderdoc/driver/gl/CMakeFiles/rdoc_gl.dir/wrappers/gl_state_funcs.cpp.o
[ 31%] Building CXX object renderdoc/driver/gl/CMakeFiles/rdoc_gl.dir/wrappers/gl_texture_funcs.cpp.o
[ 32%] Building CXX object renderdoc/driver/gl/CMakeFiles/rdoc_gl.dir/wrappers/gl_uniform_funcs.cpp.o
[ 32%] Building CXX object renderdoc/driver/gl/CMakeFiles/rdoc_gl.dir/gl_hooks.cpp.o
[ 32%] Building CXX object renderdoc/driver/gl/CMakeFiles/rdoc_gl.dir/egl_platform.cpp.o
[ 33%] Building CXX object renderdoc/driver/gl/CMakeFiles/rdoc_gl.dir/egl_hooks.cpp.o
[ 33%] Built target rdoc_gl
[ 33%] Building CXX object renderdoc/driver/vulkan/CMakeFiles/rdoc_vulkan.dir/vk_common.cpp.o
[ 34%] Building CXX object renderdoc/driver/vulkan/CMakeFiles/rdoc_vulkan.dir/vk_next_chains.cpp.o
[ 34%] Building CXX object renderdoc/driver/vulkan/CMakeFiles/rdoc_vulkan.dir/vk_core.cpp.o
[ 34%] Building CXX object renderdoc/driver/vulkan/CMakeFiles/rdoc_vulkan.dir/vk_counters.cpp.o
[ 35%] Building CXX object renderdoc/driver/vulkan/CMakeFiles/rdoc_vulkan.dir/vk_debug.cpp.o
[ 35%] Building CXX object renderdoc/driver/vulkan/CMakeFiles/rdoc_vulkan.dir/vk_postvs.cpp.o
[ 35%] Building CXX object renderdoc/driver/vulkan/CMakeFiles/rdoc_vulkan.dir/vk_shader_feedback.cpp.o
[ 36%] Building CXX object renderdoc/driver/vulkan/CMakeFiles/rdoc_vulkan.dir/vk_overlay.cpp.o
[ 36%] Building CXX object renderdoc/driver/vulkan/CMakeFiles/rdoc_vulkan.dir/vk_msaa_array_conv.cpp.o
[ 36%] Building CXX object renderdoc/driver/vulkan/CMakeFiles/rdoc_vulkan.dir/vk_outputwindow.cpp.o
[ 37%] Building CXX object renderdoc/driver/vulkan/CMakeFiles/rdoc_vulkan.dir/vk_rendermesh.cpp.o
[ 37%] Building CXX object renderdoc/driver/vulkan/CMakeFiles/rdoc_vulkan.dir/vk_rendertexture.cpp.o
[ 37%] Building CXX object renderdoc/driver/vulkan/CMakeFiles/rdoc_vulkan.dir/vk_rendertext.cpp.o
[ 37%] Building CXX object renderdoc/driver/vulkan/CMakeFiles/rdoc_vulkan.dir/vk_shader_cache.cpp.o
[ 38%] Building CXX object renderdoc/driver/vulkan/CMakeFiles/rdoc_vulkan.dir/vk_dispatchtables.cpp.o
[ 38%] Building CXX object renderdoc/driver/vulkan/CMakeFiles/rdoc_vulkan.dir/vk_image_states.cpp.o
[ 38%] Building CXX object renderdoc/driver/vulkan/CMakeFiles/rdoc_vulkan.dir/vk_info.cpp.o
[ 39%] Building CXX object renderdoc/driver/vulkan/CMakeFiles/rdoc_vulkan.dir/vk_initstate.cpp.o
[ 39%] Building CXX object renderdoc/driver/vulkan/CMakeFiles/rdoc_vulkan.dir/vk_manager.cpp.o
[ 39%] Building CXX object renderdoc/driver/vulkan/CMakeFiles/rdoc_vulkan.dir/vk_memory.cpp.o
[ 40%] Building CXX object renderdoc/driver/vulkan/CMakeFiles/rdoc_vulkan.dir/vk_pixelhistory.cpp.o
[ 40%] Building CXX object renderdoc/driver/vulkan/CMakeFiles/rdoc_vulkan.dir/vk_replay.cpp.o
[ 40%] Building CXX object renderdoc/driver/vulkan/CMakeFiles/rdoc_vulkan.dir/vk_resources.cpp.o
[ 41%] Building CXX object renderdoc/driver/vulkan/CMakeFiles/rdoc_vulkan.dir/vk_shaderdebug.cpp.o
[ 41%] Building CXX object renderdoc/driver/vulkan/CMakeFiles/rdoc_vulkan.dir/vk_state.cpp.o
[ 41%] Building CXX object renderdoc/driver/vulkan/CMakeFiles/rdoc_vulkan.dir/vk_serialise.cpp.o
[ 41%] Building CXX object renderdoc/driver/vulkan/CMakeFiles/rdoc_vulkan.dir/vk_stringise.cpp.o
[ 42%] Building CXX object renderdoc/driver/vulkan/CMakeFiles/rdoc_vulkan.dir/vk_layer.cpp.o
[ 42%] Building CXX object renderdoc/driver/vulkan/CMakeFiles/rdoc_vulkan.dir/imagestate_tests.cpp.o
[ 42%] Building CXX object renderdoc/driver/vulkan/CMakeFiles/rdoc_vulkan.dir/imgrefs_tests.cpp.o
[ 43%] Building CXX object renderdoc/driver/vulkan/CMakeFiles/rdoc_vulkan.dir/wrappers/vk_cmd_funcs.cpp.o
[ 43%] Building CXX object renderdoc/driver/vulkan/CMakeFiles/rdoc_vulkan.dir/wrappers/vk_descriptor_funcs.cpp.o
[ 43%] Building CXX object renderdoc/driver/vulkan/CMakeFiles/rdoc_vulkan.dir/wrappers/vk_device_funcs.cpp.o
[ 44%] Building CXX object renderdoc/driver/vulkan/CMakeFiles/rdoc_vulkan.dir/wrappers/vk_draw_funcs.cpp.o
[ 44%] Building CXX object renderdoc/driver/vulkan/CMakeFiles/rdoc_vulkan.dir/wrappers/vk_dynamic_funcs.cpp.o
[ 44%] Building CXX object renderdoc/driver/vulkan/CMakeFiles/rdoc_vulkan.dir/wrappers/vk_get_funcs.cpp.o
[ 45%] Building CXX object renderdoc/driver/vulkan/CMakeFiles/rdoc_vulkan.dir/wrappers/vk_misc_funcs.cpp.o
[ 45%] Building CXX object renderdoc/driver/vulkan/CMakeFiles/rdoc_vulkan.dir/wrappers/vk_queue_funcs.cpp.o
[ 45%] Building CXX object renderdoc/driver/vulkan/CMakeFiles/rdoc_vulkan.dir/wrappers/vk_resource_funcs.cpp.o
[ 45%] Building CXX object renderdoc/driver/vulkan/CMakeFiles/rdoc_vulkan.dir/wrappers/vk_shader_funcs.cpp.o
[ 46%] Building CXX object renderdoc/driver/vulkan/CMakeFiles/rdoc_vulkan.dir/wrappers/vk_sync_funcs.cpp.o
[ 46%] Building CXX object renderdoc/driver/vulkan/CMakeFiles/rdoc_vulkan.dir/wrappers/vk_wsi_funcs.cpp.o
[ 46%] Building CXX object renderdoc/driver/vulkan/CMakeFiles/rdoc_vulkan.dir/vk_posix.cpp.o
[ 47%] Building CXX object renderdoc/driver/vulkan/CMakeFiles/rdoc_vulkan.dir/vk_android.cpp.o
[ 47%] Building CXX object renderdoc/driver/vulkan/CMakeFiles/rdoc_vulkan.dir/vk_layer_android.cpp.o
[ 47%] Built target rdoc_vulkan
[ 47%] Building CXX object renderdoc/CMakeFiles/rdoc.dir/common/common.cpp.o
[ 48%] Building CXX object renderdoc/CMakeFiles/rdoc.dir/common/dds_readwrite.cpp.o
[ 48%] Building CXX object renderdoc/CMakeFiles/rdoc.dir/common/threading_tests.cpp.o
[ 48%] Building CXX object renderdoc/CMakeFiles/rdoc.dir/core/core.cpp.o
[ 49%] Building CXX object renderdoc/CMakeFiles/rdoc.dir/core/image_viewer.cpp.o
[ 49%] Building CXX object renderdoc/CMakeFiles/rdoc.dir/core/target_control.cpp.o
[ 49%] Building CXX object renderdoc/CMakeFiles/rdoc.dir/core/remote_server.cpp.o
[ 50%] Building CXX object renderdoc/CMakeFiles/rdoc.dir/core/settings.cpp.o
[ 50%] Building CXX object renderdoc/CMakeFiles/rdoc.dir/core/replay_proxy.cpp.o
[ 50%] Building CXX object renderdoc/CMakeFiles/rdoc.dir/core/intervals_tests.cpp.o
[ 50%] Building CXX object renderdoc/CMakeFiles/rdoc.dir/core/bit_flag_iterator_tests.cpp.o
[ 51%] Building CXX object renderdoc/CMakeFiles/rdoc.dir/android/android.cpp.o
[ 51%] Building CXX object renderdoc/CMakeFiles/rdoc.dir/android/android_patch.cpp.o
[ 51%] Building CXX object renderdoc/CMakeFiles/rdoc.dir/android/android_tools.cpp.o
[ 52%] Building CXX object renderdoc/CMakeFiles/rdoc.dir/android/android_utils.cpp.o
[ 52%] Building CXX object renderdoc/CMakeFiles/rdoc.dir/android/android_manifest.cpp.o
[ 52%] Building CXX object renderdoc/CMakeFiles/rdoc.dir/android/jdwp.cpp.o
[ 53%] Building CXX object renderdoc/CMakeFiles/rdoc.dir/android/jdwp_util.cpp.o
[ 53%] Building CXX object renderdoc/CMakeFiles/rdoc.dir/android/jdwp_connection.cpp.o
[ 53%] Building CXX object renderdoc/CMakeFiles/rdoc.dir/core/plugins.cpp.o
[ 54%] Building CXX object renderdoc/CMakeFiles/rdoc.dir/core/resource_manager.cpp.o
[ 54%] Building CXX object renderdoc/CMakeFiles/rdoc.dir/core/sparse_page_table.cpp.o
[ 54%] Building CXX object renderdoc/CMakeFiles/rdoc.dir/hooks/hooks.cpp.o
[ 54%] Building CXX object renderdoc/CMakeFiles/rdoc.dir/maths/camera.cpp.o
[ 55%] Building CXX object renderdoc/CMakeFiles/rdoc.dir/maths/formatpacking.cpp.o
[ 55%] Building CXX object renderdoc/CMakeFiles/rdoc.dir/maths/matrix.cpp.o
[ 55%] Building CXX object renderdoc/CMakeFiles/rdoc.dir/maths/vec.cpp.o
[ 56%] Building CXX object renderdoc/CMakeFiles/rdoc.dir/os/os_specific.cpp.o
[ 56%] Building CXX object renderdoc/CMakeFiles/rdoc.dir/replay/app_api.cpp.o
[ 56%] Building CXX object renderdoc/CMakeFiles/rdoc.dir/replay/basic_types_tests.cpp.o
[ 57%] Building CXX object renderdoc/CMakeFiles/rdoc.dir/replay/capture_options.cpp.o
[ 57%] Building CXX object renderdoc/CMakeFiles/rdoc.dir/replay/dummy_driver.cpp.o
[ 57%] Building CXX object renderdoc/CMakeFiles/rdoc.dir/replay/capture_file.cpp.o
[ 58%] Building CXX object renderdoc/CMakeFiles/rdoc.dir/replay/entry_points.cpp.o
[ 58%] Building CXX object renderdoc/CMakeFiles/rdoc.dir/replay/replay_driver.cpp.o
[ 58%] Building CXX object renderdoc/CMakeFiles/rdoc.dir/replay/replay_output.cpp.o
[ 58%] Building CXX object renderdoc/CMakeFiles/rdoc.dir/replay/replay_controller.cpp.o
[ 59%] Building CXX object renderdoc/CMakeFiles/rdoc.dir/serialise/serialiser.cpp.o
[ 59%] Building CXX object renderdoc/CMakeFiles/rdoc.dir/serialise/lz4io.cpp.o
[ 59%] Building CXX object renderdoc/CMakeFiles/rdoc.dir/serialise/zstdio.cpp.o
[ 60%] Building CXX object renderdoc/CMakeFiles/rdoc.dir/serialise/streamio.cpp.o
[ 60%] Building CXX object renderdoc/CMakeFiles/rdoc.dir/serialise/rdcfile.cpp.o
[ 60%] Building CXX object renderdoc/CMakeFiles/rdoc.dir/serialise/codecs/xml_codec.cpp.o
[ 61%] Building CXX object renderdoc/CMakeFiles/rdoc.dir/serialise/codecs/chrome_json_codec.cpp.o
[ 61%] Building CXX object renderdoc/CMakeFiles/rdoc.dir/serialise/comp_io_tests.cpp.o
[ 61%] Building CXX object renderdoc/CMakeFiles/rdoc.dir/serialise/serialiser_tests.cpp.o
[ 62%] Building CXX object renderdoc/CMakeFiles/rdoc.dir/serialise/streamio_tests.cpp.o
[ 62%] Building CXX object renderdoc/CMakeFiles/rdoc.dir/strings/grisu2.cpp.o
[ 62%] Building CXX object renderdoc/CMakeFiles/rdoc.dir/strings/string_utils.cpp.o
[ 62%] Building CXX object renderdoc/CMakeFiles/rdoc.dir/strings/utf8printf.cpp.o
[ 63%] Building CXX object renderdoc/CMakeFiles/rdoc.dir/3rdparty/jpeg-compressor/jpgd.cpp.o
[ 63%] Building CXX object renderdoc/CMakeFiles/rdoc.dir/3rdparty/jpeg-compressor/jpge.cpp.o
[ 63%] Building CXX object renderdoc/CMakeFiles/rdoc.dir/3rdparty/catch/catch.cpp.o
[ 64%] Building CXX object renderdoc/CMakeFiles/rdoc.dir/3rdparty/pugixml/pugixml.cpp.o
[ 64%] Building C object renderdoc/CMakeFiles/rdoc.dir/3rdparty/lz4/lz4.c.o
[ 64%] Building C object renderdoc/CMakeFiles/rdoc.dir/3rdparty/md5/md5.c.o
[ 65%] Building C object renderdoc/CMakeFiles/rdoc.dir/3rdparty/miniz/miniz.c.o
[ 65%] Building CXX object renderdoc/CMakeFiles/rdoc.dir/3rdparty/superluminal/superluminal.cpp.o
[ 65%] Building C object renderdoc/CMakeFiles/rdoc.dir/3rdparty/zstd/debug.c.o
[ 66%] Building C object renderdoc/CMakeFiles/rdoc.dir/3rdparty/zstd/entropy_common.c.o
[ 66%] Building C object renderdoc/CMakeFiles/rdoc.dir/3rdparty/zstd/error_private.c.o
[ 66%] Building C object renderdoc/CMakeFiles/rdoc.dir/3rdparty/zstd/fse_compress.c.o
[ 66%] Building C object renderdoc/CMakeFiles/rdoc.dir/3rdparty/zstd/fse_decompress.c.o
[ 67%] Building C object renderdoc/CMakeFiles/rdoc.dir/3rdparty/zstd/hist.c.o
[ 67%] Building C object renderdoc/CMakeFiles/rdoc.dir/3rdparty/zstd/huf_compress.c.o
[ 67%] Building C object renderdoc/CMakeFiles/rdoc.dir/3rdparty/zstd/huf_decompress.c.o
[ 68%] Building C object renderdoc/CMakeFiles/rdoc.dir/3rdparty/zstd/pool.c.o
[ 68%] Building C object renderdoc/CMakeFiles/rdoc.dir/3rdparty/zstd/threading.c.o
[ 68%] Building C object renderdoc/CMakeFiles/rdoc.dir/3rdparty/zstd/xxhash.c.o
[ 69%] Building C object renderdoc/CMakeFiles/rdoc.dir/3rdparty/zstd/zstd_common.c.o
[ 69%] Building C object renderdoc/CMakeFiles/rdoc.dir/3rdparty/zstd/zstd_compress.c.o
[ 69%] Building C object renderdoc/CMakeFiles/rdoc.dir/3rdparty/zstd/zstd_decompress.c.o
[ 70%] Building C object renderdoc/CMakeFiles/rdoc.dir/3rdparty/zstd/zstd_double_fast.c.o
[ 70%] Building C object renderdoc/CMakeFiles/rdoc.dir/3rdparty/zstd/zstd_fast.c.o
[ 70%] Building C object renderdoc/CMakeFiles/rdoc.dir/3rdparty/zstd/zstd_lazy.c.o
[ 70%] Building C object renderdoc/CMakeFiles/rdoc.dir/3rdparty/zstd/zstd_ldm.c.o
[ 71%] Building C object renderdoc/CMakeFiles/rdoc.dir/3rdparty/zstd/zstd_opt.c.o
[ 71%] Building C object renderdoc/CMakeFiles/rdoc.dir/3rdparty/zstd/zstdmt_compress.c.o
[ 71%] Building C object renderdoc/CMakeFiles/rdoc.dir/3rdparty/stb/stb_impl.c.o
[ 72%] Building CXX object renderdoc/CMakeFiles/rdoc.dir/3rdparty/tinyexr/tinyexr.cpp.o
[ 72%] Building C object renderdoc/CMakeFiles/rdoc.dir/3rdparty/tinyfiledialogs/tinyfiledialogs.c.o
[ 72%] Building CXX object renderdoc/CMakeFiles/rdoc.dir/os/posix/android/android_stringio.cpp.o
[ 73%] Building CXX object renderdoc/CMakeFiles/rdoc.dir/os/posix/android/android_callstack.cpp.o
[ 73%] Building CXX object renderdoc/CMakeFiles/rdoc.dir/os/posix/android/android_process.cpp.o
[ 73%] Building CXX object renderdoc/CMakeFiles/rdoc.dir/os/posix/android/android_threading.cpp.o
[ 73%] Building CXX object renderdoc/CMakeFiles/rdoc.dir/os/posix/android/android_hook.cpp.o
[ 74%] Building CXX object renderdoc/CMakeFiles/rdoc.dir/os/posix/android/android_network.cpp.o
[ 74%] Building CXX object renderdoc/CMakeFiles/rdoc.dir/os/posix/posix_network.cpp.o
[ 74%] Building CXX object renderdoc/CMakeFiles/rdoc.dir/os/posix/posix_process.cpp.o
[ 75%] Building CXX object renderdoc/CMakeFiles/rdoc.dir/os/posix/posix_stringio.cpp.o
[ 75%] Building CXX object renderdoc/CMakeFiles/rdoc.dir/os/posix/posix_threading.cpp.o
[ 75%] Building CXX object renderdoc/CMakeFiles/rdoc.dir/data/glsl_shaders.cpp.o
[ 75%] Built target rdoc
[ 75%] Building CXX object renderdoc/driver/ihv/intel/CMakeFiles/rdoc_intel.dir/intel_gl_counters.cpp.o
[ 75%] Built target rdoc_intel
[ 75%] Generating ../bin/include-bin
[ 75%] Generating CMakeFiles/data.src/driver/vulkan/renderdoc.json.c
[ 76%] Generating CMakeFiles/data.src/data/glsl/array2ms.comp.c
[ 76%] Generating CMakeFiles/data.src/data/glsl/blit.vert.c
[ 77%] Generating CMakeFiles/data.src/data/glsl/checkerboard.frag.c
[ 78%] Generating CMakeFiles/data.src/data/glsl/deptharr2ms.frag.c
[ 78%] Generating CMakeFiles/data.src/data/glsl/depthms2arr.frag.c
[ 78%] Generating CMakeFiles/data.src/data/glsl/discard.frag.c
[ 78%] Generating CMakeFiles/data.src/data/glsl/fixedcol.frag.c
[ 78%] Generating CMakeFiles/data.src/data/glsl/gl_texsample.h.c
[ 79%] Generating CMakeFiles/data.src/data/glsl/gles_texsample.h.c
[ 79%] Generating CMakeFiles/data.src/data/glsl/glsl_globals.h.c
[ 79%] Generating CMakeFiles/data.src/data/glsl/glsl_ubos.h.c
[ 79%] Generating CMakeFiles/data.src/data/glsl/gltext.frag.c
[ 80%] Generating CMakeFiles/data.src/data/glsl/gltext.vert.c
[ 81%] Generating CMakeFiles/data.src/data/glsl/histogram.comp.c
[ 81%] Generating CMakeFiles/data.src/data/glsl/mesh.comp.c
[ 81%] Generating CMakeFiles/data.src/data/glsl/mesh.frag.c
[ 82%] Generating CMakeFiles/data.src/data/glsl/mesh.geom.c
[ 82%] Generating CMakeFiles/data.src/data/glsl/mesh.vert.c
[ 82%] Generating CMakeFiles/data.src/data/glsl/minmaxresult.comp.c
[ 83%] Generating CMakeFiles/data.src/data/glsl/minmaxtile.comp.c
[ 83%] Generating CMakeFiles/data.src/data/glsl/ms2array.comp.c
[ 84%] Generating CMakeFiles/data.src/data/glsl/pixelhistory_mscopy.comp.c
[ 84%] Generating CMakeFiles/data.src/data/glsl/pixelhistory_mscopy_depth.comp.c
[ 84%] Generating CMakeFiles/data.src/data/glsl/pixelhistory_primid.frag.c
[ 84%] Generating CMakeFiles/data.src/data/glsl/quadresolve.frag.c
[ 84%] Generating CMakeFiles/data.src/data/glsl/quadwrite.frag.c
[ 84%] Generating CMakeFiles/data.src/data/glsl/shaderdebug_sample.vert.c
[ 85%] Generating CMakeFiles/data.src/data/glsl/texdisplay.frag.c
[ 85%] Generating CMakeFiles/data.src/data/glsl/texremap.frag.c
[ 85%] Generating CMakeFiles/data.src/data/glsl/trisize.frag.c
[ 85%] Generating CMakeFiles/data.src/data/glsl/trisize.geom.c
[ 85%] Generating CMakeFiles/data.src/data/glsl/vk_texsample.h.c
[ 85%] Generating CMakeFiles/data.src/data/glsl/vktext.frag.c
[ 85%] Generating CMakeFiles/data.src/data/glsl/vktext.vert.c
[ 86%] Generating CMakeFiles/data.src/data/sourcecodepro.ttf.c
[ 86%] Generating ../bin/include-bin
[ 86%] Generating CMakeFiles/data.src/data/glsl/blit.vert.c
[ 87%] Building C object renderdoc/CMakeFiles/renderdoc.dir/CMakeFiles/data.src/data/glsl/blit.vert.c.o
[ 87%] Generating CMakeFiles/data.src/data/glsl/checkerboard.frag.c
[ 87%] Building C object renderdoc/CMakeFiles/renderdoc.dir/CMakeFiles/data.src/data/glsl/checkerboard.frag.c.o
[ 87%] Generating CMakeFiles/data.src/data/glsl/glsl_ubos.h.c
[ 87%] Building C object renderdoc/CMakeFiles/renderdoc.dir/CMakeFiles/data.src/data/glsl/glsl_ubos.h.c.o
[ 87%] Generating CMakeFiles/data.src/data/glsl/glsl_globals.h.c
[ 88%] Building C object renderdoc/CMakeFiles/renderdoc.dir/CMakeFiles/data.src/data/glsl/glsl_globals.h.c.o
[ 88%] Generating CMakeFiles/data.src/data/glsl/fixedcol.frag.c
[ 88%] Building C object renderdoc/CMakeFiles/renderdoc.dir/CMakeFiles/data.src/data/glsl/fixedcol.frag.c.o
[ 88%] Generating CMakeFiles/data.src/data/glsl/histogram.comp.c
[ 88%] Building C object renderdoc/CMakeFiles/renderdoc.dir/CMakeFiles/data.src/data/glsl/histogram.comp.c.o
[ 88%] Generating CMakeFiles/data.src/data/glsl/mesh.comp.c
[ 88%] Building C object renderdoc/CMakeFiles/renderdoc.dir/CMakeFiles/data.src/data/glsl/mesh.comp.c.o
[ 88%] Generating CMakeFiles/data.src/data/glsl/mesh.frag.c
[ 89%] Building C object renderdoc/CMakeFiles/renderdoc.dir/CMakeFiles/data.src/data/glsl/mesh.frag.c.o
[ 89%] Generating CMakeFiles/data.src/data/glsl/mesh.geom.c
[ 89%] Building C object renderdoc/CMakeFiles/renderdoc.dir/CMakeFiles/data.src/data/glsl/mesh.geom.c.o
[ 89%] Generating CMakeFiles/data.src/data/glsl/mesh.vert.c
[ 89%] Building C object renderdoc/CMakeFiles/renderdoc.dir/CMakeFiles/data.src/data/glsl/mesh.vert.c.o
[ 89%] Generating CMakeFiles/data.src/data/glsl/minmaxresult.comp.c
[ 90%] Building C object renderdoc/CMakeFiles/renderdoc.dir/CMakeFiles/data.src/data/glsl/minmaxresult.comp.c.o
[ 90%] Generating CMakeFiles/data.src/data/glsl/minmaxtile.comp.c
[ 90%] Building C object renderdoc/CMakeFiles/renderdoc.dir/CMakeFiles/data.src/data/glsl/minmaxtile.comp.c.o
[ 90%] Generating CMakeFiles/data.src/data/glsl/quadresolve.frag.c
[ 90%] Building C object renderdoc/CMakeFiles/renderdoc.dir/CMakeFiles/data.src/data/glsl/quadresolve.frag.c.o
[ 90%] Generating CMakeFiles/data.src/data/glsl/quadwrite.frag.c
[ 91%] Building C object renderdoc/CMakeFiles/renderdoc.dir/CMakeFiles/data.src/data/glsl/quadwrite.frag.c.o
[ 91%] Generating CMakeFiles/data.src/data/glsl/pixelhistory_mscopy.comp.c
[ 91%] Building C object renderdoc/CMakeFiles/renderdoc.dir/CMakeFiles/data.src/data/glsl/pixelhistory_mscopy.comp.c.o
[ 91%] Generating CMakeFiles/data.src/data/glsl/pixelhistory_mscopy_depth.comp.c
[ 91%] Building C object renderdoc/CMakeFiles/renderdoc.dir/CMakeFiles/data.src/data/glsl/pixelhistory_mscopy_depth.comp.c.o
[ 91%] Generating CMakeFiles/data.src/data/glsl/pixelhistory_primid.frag.c
[ 92%] Building C object renderdoc/CMakeFiles/renderdoc.dir/CMakeFiles/data.src/data/glsl/pixelhistory_primid.frag.c.o
[ 92%] Generating CMakeFiles/data.src/data/glsl/shaderdebug_sample.vert.c
[ 92%] Building C object renderdoc/CMakeFiles/renderdoc.dir/CMakeFiles/data.src/data/glsl/shaderdebug_sample.vert.c.o
[ 92%] Generating CMakeFiles/data.src/data/glsl/texdisplay.frag.c
[ 92%] Building C object renderdoc/CMakeFiles/renderdoc.dir/CMakeFiles/data.src/data/glsl/texdisplay.frag.c.o
[ 92%] Generating CMakeFiles/data.src/data/glsl/texremap.frag.c
[ 92%] Building C object renderdoc/CMakeFiles/renderdoc.dir/CMakeFiles/data.src/data/glsl/texremap.frag.c.o
[ 92%] Generating CMakeFiles/data.src/data/glsl/gl_texsample.h.c
[ 93%] Building C object renderdoc/CMakeFiles/renderdoc.dir/CMakeFiles/data.src/data/glsl/gl_texsample.h.c.o
[ 93%] Generating CMakeFiles/data.src/data/glsl/gles_texsample.h.c
[ 93%] Building C object renderdoc/CMakeFiles/renderdoc.dir/CMakeFiles/data.src/data/glsl/gles_texsample.h.c.o
[ 93%] Generating CMakeFiles/data.src/data/glsl/vk_texsample.h.c
[ 93%] Building C object renderdoc/CMakeFiles/renderdoc.dir/CMakeFiles/data.src/data/glsl/vk_texsample.h.c.o
[ 93%] Generating CMakeFiles/data.src/data/glsl/gltext.frag.c
[ 94%] Building C object renderdoc/CMakeFiles/renderdoc.dir/CMakeFiles/data.src/data/glsl/gltext.frag.c.o
[ 94%] Generating CMakeFiles/data.src/data/glsl/gltext.vert.c
[ 94%] Building C object renderdoc/CMakeFiles/renderdoc.dir/CMakeFiles/data.src/data/glsl/gltext.vert.c.o
[ 94%] Generating CMakeFiles/data.src/data/glsl/vktext.frag.c
[ 94%] Building C object renderdoc/CMakeFiles/renderdoc.dir/CMakeFiles/data.src/data/glsl/vktext.frag.c.o
[ 94%] Generating CMakeFiles/data.src/data/glsl/vktext.vert.c
[ 95%] Building C object renderdoc/CMakeFiles/renderdoc.dir/CMakeFiles/data.src/data/glsl/vktext.vert.c.o
[ 95%] Generating CMakeFiles/data.src/data/glsl/array2ms.comp.c
[ 95%] Building C object renderdoc/CMakeFiles/renderdoc.dir/CMakeFiles/data.src/data/glsl/array2ms.comp.c.o
[ 95%] Generating CMakeFiles/data.src/data/glsl/ms2array.comp.c
[ 95%] Building C object renderdoc/CMakeFiles/renderdoc.dir/CMakeFiles/data.src/data/glsl/ms2array.comp.c.o
[ 95%] Generating CMakeFiles/data.src/data/glsl/trisize.frag.c
[ 96%] Building C object renderdoc/CMakeFiles/renderdoc.dir/CMakeFiles/data.src/data/glsl/trisize.frag.c.o
[ 96%] Generating CMakeFiles/data.src/data/glsl/trisize.geom.c
[ 96%] Building C object renderdoc/CMakeFiles/renderdoc.dir/CMakeFiles/data.src/data/glsl/trisize.geom.c.o
[ 96%] Generating CMakeFiles/data.src/data/glsl/deptharr2ms.frag.c
[ 96%] Building C object renderdoc/CMakeFiles/renderdoc.dir/CMakeFiles/data.src/data/glsl/deptharr2ms.frag.c.o
[ 96%] Generating CMakeFiles/data.src/data/glsl/depthms2arr.frag.c
[ 96%] Building C object renderdoc/CMakeFiles/renderdoc.dir/CMakeFiles/data.src/data/glsl/depthms2arr.frag.c.o
[ 96%] Generating CMakeFiles/data.src/data/glsl/discard.frag.c
[ 97%] Building C object renderdoc/CMakeFiles/renderdoc.dir/CMakeFiles/data.src/data/glsl/discard.frag.c.o
[ 97%] Generating CMakeFiles/data.src/data/sourcecodepro.ttf.c
[ 97%] Building C object renderdoc/CMakeFiles/renderdoc.dir/CMakeFiles/data.src/data/sourcecodepro.ttf.c.o
[ 97%] Generating CMakeFiles/data.src/driver/vulkan/renderdoc.json.c
[ 97%] Building C object renderdoc/CMakeFiles/renderdoc.dir/CMakeFiles/data.src/driver/vulkan/renderdoc.json.c.o
[ 98%] Linking CXX shared library ..\lib\libVkLayer_GLES_RenderDoc.so
[ 98%] Built target renderdoc
[ 99%] Building CXX object renderdoccmd/CMakeFiles/renderdoccmd.dir/renderdoccmd.cpp.o
[ 99%] Building CXX object renderdoccmd/CMakeFiles/renderdoccmd.dir/renderdoccmd_android.cpp.o
[ 99%] Building C object renderdoccmd/CMakeFiles/renderdoccmd.dir/D_/Envs/RDCAndroidSDK/android-ndk-r14b/sources/android/native_app_glue/android_native_app_glue.c.o
[100%] Linking CXX shared library ..\lib\librenderdoccmd.so
[100%] Built target renderdoccmd
[100%] Generating debug.keystore

Warning:
JKS Կʹרøʽʹ "keytool -importkeystore -srckeystore D:/WorkSpace/C++/RenderDoc/android32/renderdoccmd/debug.keystore -destkeystore D:/WorkSpace/C++/RenderDoc/android32/renderdoccmd/debug.keystore -deststoretype pkcs12" ǨƵҵ׼ʽ PKCS12
[100%] Generating ../bin/org.renderdoc.renderdoccmd.arm32.apk
[100%] Built target apk
```



### CMake NDK 交叉编译

可参考：https://developer.android.google.cn/ndk/guides/cmake?hl=zh-cn

#### 编译APK的Demo