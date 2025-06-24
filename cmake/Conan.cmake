function(run_conan)
  # Download automatically, you can also just copy the conan.cmake file
  if(NOT EXISTS "${CMAKE_CURRENT_BINARY_DIR}/conan.cmake")
    message(STATUS "Downloading conan.cmake from https://github.com/conan-io/cmake-conan")
    file(DOWNLOAD "https://github.com/conan-io/cmake-conan/raw/v0.15/conan.cmake" "${CMAKE_CURRENT_BINARY_DIR}/conan.cmake")
  endif()

  if (NOT CONAN_BUILD_STRATEGY)
    set(CONAN_BUILD_STRATEGY missing)
    message(STATUS "No CONAN_BUILD_STRATEGY is specified, using BUILD=${CONAN_BUILD_STRATEGY} by default")
  endif()

  # Add current binary dir to make CMake pick up generated FindXXX.cmake files
  list(APPEND CMAKE_MODULE_PATH ${CMAKE_BINARY_DIR})
  list(APPEND CMAKE_PREFIX_PATH ${CMAKE_BINARY_DIR})

  include(${CMAKE_CURRENT_BINARY_DIR}/conan.cmake)

  conan_cmake_run(
    BUILD
      ${CONAN_BUILD_STRATEGY}
    REQUIRES
      boost/1.72.0
      cpp-httplib/0.11.3
      date/3.0.1
      expected-lite/0.6.3
      tl-expected/20190710
      tl-function-ref/1.0.0
      fmt/11.0.2
      gtest/1.13.0
      benchmark/1.8.3
      libpqxx/7.7.5
      pistache/cci.20201127
      quickfix/1.15.1
      rapidjson/cci.20220822
      spdlog/1.14.1
      tinyxml2/9.0.0
      vincentlaucsb-csv-parser/2.1.3
      ms-gsl/4.0.0
    GENERATORS
      cmake_find_package
    OPTIONS
      boost:without_date_time=False
      boost:without_serialization=False
      boost:without_atomic=True
      boost:without_chrono=True
      boost:without_container=True
      boost:without_context=True
      boost:without_contract=True
      boost:without_coroutine=True
      boost:without_exception=True
      boost:without_fiber=True
      boost:without_filesystem=True
      boost:without_graph_parallel=True
      boost:without_graph=True
      boost:without_iostreams=True
      boost:without_locale=True
      boost:without_log=True
      boost:without_math=True
      boost:without_mpi=True
      boost:without_program_options=True
      boost:without_python=True
      boost:without_random=True
      boost:without_regex=True
      boost:without_stacktrace=True
      boost:without_system=True
      boost:without_test=True
      boost:without_thread=True
      boost:without_timer=True
      boost:without_type_erasure=True
      boost:without_wave=True
      date:header_only=False
      date:use_system_tz_db=True
      quickfix:with_ssl=True
    SETTINGS
      compiler.cppstd=20
    ENV
      CC=${CMAKE_C_COMPILER}
      CXX=${CMAKE_CXX_COMPILER}
      CONAN_CMAKE_PROGRAM=${CMAKE_COMMAND})


  find_package(Boost REQUIRED COMPONENTS date_time)
  find_package(GTest REQUIRED)
  find_package(benchmark REQUIRED)
  find_package(Pistache REQUIRED)
  find_package(RapidJSON REQUIRED)
  find_package(date REQUIRED)
  find_package(expected-lite REQUIRED)
  find_package(fmt REQUIRED)
  find_package(httplib REQUIRED)
  find_package(libpqxx REQUIRED)
  find_package(quickfix REQUIRED)
  find_package(spdlog REQUIRED)
  find_package(tinyxml2 REQUIRED)
  find_package(vincentlaucsb-csv-parser REQUIRED)
  find_package(tl-expected REQUIRED)
  find_package(tl-function-ref REQUIRED)
  find_package(Microsoft.GSL REQUIRED)

endfunction()
