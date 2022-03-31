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



### CMake NDK 交叉编译

可参考：https://developer.android.google.cn/ndk/guides/cmake?hl=zh-cn

