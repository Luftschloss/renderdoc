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

```makefile
# 是否定义环境变量
DEFINED ENV{env_name}
# 是否存在路径
EXISTS "Path"
# 获得STR变量的字符串值
&{<variable>}

# 字符串相等符
STREQUAL

# 项目名
project(<PROJECT-NAME> [<language-name>...])

# 设置变量值，同命令行的-D<variable>=<value>, 使用CACHE可以将该变量缓存至CMakeCache.txt，使用Force是Overwrite缓存值，并不是使用缓存值，使用
# PARENT_SCOPE调高变量作用域到父目录或调用该函数处（PARENT_SCOPE和CACHE互斥）
set(<variable> <value>
      [[CACHE <type> <docstring> [FORCE]] | PARENT_SCOPE])

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
list(GET <list> <elementindex> [<element index> ...]<output variable>) 		#返回list中index的element到value中
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

# 将指定的源文件生成链接文件，然后添加到工程中去
add_library(<name> [STATIC | SHARED | MODULE]
            [EXCLUDE_FROM_ALL]
            [source1] [source2] [...])

# 将目标文件与库文件进行链接
target_link_libraries(<target> [item1] [item2] [...]
                      [[debug|optimized|general] <item>] ...)
                   
# 设置C++、C编译选项，同set(CMAKE_CXX_FLAGS/CMAKE_C_FLAGS <cmdstring>)
# 常见的有：-fsanitize=thread(检测运行时的Data Race)、-fsanitize=address（检测运行时的内存错误）、-fno-omit-frame-pointer
add_compile_options(<option> ...)

add_executable(<name> [WIN32] [MACOSX_BUNDLE]
               [EXCLUDE_FROM_ALL]
               [source1] [source2 ...])

# 引入第三方库，REQUIRED：
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
# Reading 读
  file(READ <filename> <out-var> [...])
  file(STRINGS <filename> <out-var> [...])
  file(<HASH> <filename> <out-var>)
  file(TIMESTAMP <filename> <out-var> [...])

# Writing 写
  file({WRITE | APPEND} <filename> <content>...)
  file({TOUCH | TOUCH_NOCREATE} [<file>...])
  file(GENERATE OUTPUT <output-file> [...])

# Filesystem 常见文件操作
  file({GLOB | GLOB_RECURSE} <out-var> [...] [<globbing-expr>...])
  file(RENAME <oldname> <newname>)
  file({REMOVE | REMOVE_RECURSE } [<files>...])
  file(MAKE_DIRECTORY [<dir>...])
  file({COPY | INSTALL} <file>... DESTINATION <dir> [...])
  file(SIZE <filename> <out-var>)
  file(READ_SYMLINK <linkname> <out-var>)
  file(CREATE_LINK <original> <linkname> [...])

# Path Conversion
  file(RELATIVE_PATH <out-var> <directory> <file>)
  file({TO_CMAKE_PATH | TO_NATIVE_PATH} <path> <out-var>)

# Transfer 传输
  file(DOWNLOAD <url> <file> [...])
  file(UPLOAD <file> <url> [...])

# Locking 加锁
  file(LOCK <path> [...])
  
# string操作
# Search and Replace 查找和替换
  string(FIND <string> <substring> <out-var> [...])
  string(REPLACE <match-string> <replace-string> <out-var> <input>...)

# Regular Expressions 正则匹配
  string(REGEX MATCH <match-regex> <out-var> <input>...)
  string(REGEX MATCHALL <match-regex> <out-var> <input>...)
  string(REGEX REPLACE <match-regex> <replace-expr> <out-var> <input>...)

# Manipulation 常见字符串处理
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

# Comparison 字符串比较
  string(COMPARE <op> <string1> <string2> <out-var>)

# Hashing
  string(<HASH> <out-var> <input>)

# Generation 生成字符串
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
```

