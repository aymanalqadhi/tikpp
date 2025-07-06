cmake_minimum_required(VERSION 3.24)

project(tikpp
  VERSION 0.1.0
  LANGUAGES CXX
)

# ##############################################################################
# Target

add_library(tikpp
  src/error.cpp
  src/crypto.cpp
  src/request.cpp
  src/response.cpp
)

# headers
target_sources(tikpp
  PUBLIC
    FILE_SET HEADERS
    BASE_DIRS
      include
    FILES
      include/tikpp/api.hpp
      include/tikpp/basic_api.hpp
      include/tikpp/commands/add.hpp
      include/tikpp/commands/getall.hpp
      include/tikpp/commands/listen.hpp
      include/tikpp/commands/login.hpp
      include/tikpp/commands/remove.hpp
      include/tikpp/commands/set.hpp
      include/tikpp/data/converters/creator.hpp
      include/tikpp/data/converters/dissolver.hpp
      include/tikpp/data/converters/proplist_collector.hpp
      include/tikpp/data/model.hpp
      include/tikpp/data/query.hpp
      include/tikpp/data/repository.hpp
      include/tikpp/data/types/bytes.hpp
      include/tikpp/data/types/duration.hpp
      include/tikpp/data/types/identity.hpp
      include/tikpp/data/types/wrapper.hpp
      include/tikpp/detail/async_result.hpp
      include/tikpp/detail/convert.hpp
      include/tikpp/detail/crypto.hpp
      include/tikpp/detail/operations/async_connect.hpp
      include/tikpp/detail/operations/async_read_response.hpp
      include/tikpp/detail/operations/async_read_word.hpp
      include/tikpp/detail/operations/async_read_word_length.hpp
      include/tikpp/detail/ssl_wrapper.hpp
      include/tikpp/detail/type_traits/error_handler.hpp
      include/tikpp/detail/type_traits/macros.hpp
      include/tikpp/detail/type_traits/model.hpp
      include/tikpp/detail/type_traits/operators.hpp
      include/tikpp/detail/type_traits/stream.hpp
      include/tikpp/error_code.hpp
      include/tikpp/io_context.hpp
      include/tikpp/models/interface.hpp
      include/tikpp/models/ip/address.hpp
      include/tikpp/models/ip/arp.hpp
      include/tikpp/models/ip/hotspot/active.hpp
      include/tikpp/models/ip/hotspot/cookie.hpp
      include/tikpp/models/ip/hotspot/host.hpp
      include/tikpp/models/ip/hotspot/ip_binding.hpp
      include/tikpp/models/ip/hotspot/profile.hpp
      include/tikpp/models/ip/hotspot/user.hpp
      include/tikpp/models/ip/hotspot/user_profile.hpp
      include/tikpp/models/ip/hotspot.hpp
      include/tikpp/request.hpp
      include/tikpp/response.hpp
      include/tikpp/sentence.hpp
      include/tikpp/ssl_api.hpp
      include/tikpp/tokens.hpp
)

target_compile_features(tikpp PRIVATE cxx_std_17)

# ##############################################################################
# Dependencies

add_subdirectory(lib/fmt)

# fmt::fmt
target_link_libraries(tikpp PUBLIC fmt::fmt)

# Boost
find_package(Boost CONFIG REQUIRED COMPONENTS system thread)
target_link_libraries(tikpp PUBLIC Boost::system)
target_link_libraries(tikpp PRIVATE Boost::thread)

# OpenSSL
find_package(OpenSSL REQUIRED COMPONENTS Crypto SSL)
target_link_libraries(tikpp PRIVATE OpenSSL::Crypto OpenSSL::Crypto)

if(BUILD_TESTING)
  add_subdirectory(lib/googletest)
  add_subdirectory(tests)
endif()
