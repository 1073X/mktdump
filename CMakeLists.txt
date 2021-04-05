cmake_minimum_required (VERSION 3.19)
project("${CMAKE_PROJECT_NAME}")
include("$ENV{TOOLCHAIN_PATH}/CMakeLists.txt.in")

add_lib(libimpl "source/lib")
add_svc(mktdump "source/bin" libimpl ${LIB_MKT} ${LIB_REF} ${LIB_MIU})
add_unittest(libimpl
    ${LIB_MKT} ${LIB_REF}
    ${LIB_JOB} ${LIB_CMD} ${LIB_ASP}
    ${LIB_NET} ${LIB_SHM} ${LIB_CFG} ${LIB_LOG}
    ${LIB_COM}
)

