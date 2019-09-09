cmake_minimum_required(VERSION 3.10)

#option(test "Build all tests." OFF) # Turn on with cmake -Dtest=ON

set(CMAKE_CXX_STANDARD 17)

SET(CMAKE_C_FLAGS_DEBUG "-D_DEBUG")

project(bidfx_public_api_cpp VERSION 1.0.0 DESCRIPTION "BidFX Public C++ API for accessing price streams")

configure_file(src/config.h.in config.h)
include_directories(. ${CMAKE_CURRENT_BINARY_DIR})

#####################
# Main Library
#####################

add_library(
        bidfx_public_api SHARED
        src/price/provider/pixie/ack_data.h
        src/price/provider/pixie/ack_data.cpp
        src/tools/varint.cpp
        src/tools/byte_buffer.h
        src/tools/byte_buffer.cpp
        include/tools/java_time.h
        src/tools/java_time.cpp
        src/price/provider/pixie/messages/ack_message.h
        src/price/provider/pixie/messages/ack_message.cpp
        src/price/provider/pixie/messages/control_operation.h
        src/price/provider/pixie/messages/control_operation.cpp
        src/price/provider/pixie/messages/pixie_message_type.h
        src/tools/varint.h
        src/tools/varint.cpp
        include/tools/java_time.h
        src/tools/java_time.cpp
        src/price/provider/pixie/Fields/field_encoding.h
        src/price/provider/pixie/Fields/field_encoding.cpp
        src/price/provider/pixie/Fields/field_type.h
        src/price/provider/pixie/Fields/field_type.cpp
        src/price/provider/pixie/Fields/data_dictionary.h
        src/price/provider/pixie/Fields/data_dictionary_utils.cpp
        src/price/provider/pixie/Fields/data_dictionary_utils.h
        src/price/provider/pixie/Fields/extendable_data_dictionary.cpp
        src/price/provider/pixie/Fields/extendable_data_dictionary.h
        src/price/provider/pixie/Fields/field_def.h
        src/price/provider/pixie/Fields/field_def.cpp
        src/price/provider/pixie/pixie_provider.h
        src/price/provider/pixie/pixie_provider.cpp
        src/tools/guid.h
        src/tools/guid.cpp
        src/tools/base64.cpp
        include/tools/base64.h
        src/price/provider/pixie/pixie_protocol_options.h
        src/price/provider/pixie/pixie_protocol_options.cpp
        src/tools/network/ssl_client.h
        src/tools/network/mbed_ssl_client.h
        src/tools/network/mbed_ssl_client.cpp
        include/session.h
        include/price_update_event.h
        include/subscriber.h
        include/price/subject/subject.h
        include/bulk_subscriber.h
        src/Price/pricing_subscriber.h
        src/Price/pricing_subscriber.cpp
        include/price/subject/subject_factory.h
        include/user_info.h
        src/basic_user_info.cpp
        src/basic_user_info.h
        src/Price/Subject/subject_factory.cpp
        include/price/subject/subject_builder.h
        src/Price/Subject/subject_builder.cpp
        src/Price/Subject/subject.cpp include/tenor.h
        src/tenor.cpp
        include/price/subject/subject_utils.h
        src/Price/Subject/subject_utils.cpp
        include/tools/date.h
        src/tools/date.cpp
        src/Price/Subject/subject_key.h
        src/Price/Subject/subject_key.cpp
        src/Price/request_type.h
        src/Price/request_type.cpp
        include/price/deal_type.h
        src/Price/deal_type.cpp
        include/price/asset_class.h
        src/Price/asset_class.cpp
        src/Price/level.h
        src/Price/level.cpp
        src/Price/pricing_session.h
        src/Price/pricing_session.cpp
        include/provider_status_event.h
        include/subscription_status_event.h
        include/api_session.h
        src/api_session.cpp
        include/pricing.h
        include/public_api.h
        src/public_api.cpp
        src/Price/currencies.h
        src/Price/currencies.cpp
        src/tools/string_char_iterator.h
        src/tools/string_char_iterator.cpp
        src/Price/Subject/subject_formatter.h
        src/Price/Subject/subject_formatter.cpp
        include/price/provider/provider.h
        include/price/provider/provider_properties.h
        src/Price/Provider/abstract_provider.h
        src/Price/Provider/provider_consumer.h
        src/Price/Provider/abstract_provider.cpp
        src/tools/network/tunnel_connector.h
        src/tools/network/tunnel_connector.cpp
        src/tools/network/input_stream.h
        src/tools/network/output_stream.h
        src/price/provider/pixie/messages/welcome_message.h
        src/tools/hex_encoder.h src/tools/hex_encoder.cpp
        src/price/provider/pixie/messages/welcome_message.cpp
        src/price/provider/pixie/messages/pixie_message_type.cpp
        src/price/provider/pixie/pixie_version.h
        src/price/provider/pixie/pixie_version.cpp
        src/price/provider/pixie/messages/pixie_message.h
        src/price/provider/pixie/messages/login_message.h
        src/price/provider/pixie/messages/login_message.cpp
        src/price/provider/pixie/messages/grant_message.h
        src/price/provider/pixie/messages/grant_message.cpp
        src/price/provider/pixie/run-state.h
        src/tools/buffer_util.h
        src/tools/buffer_util.cpp
        src/tools/countdown_latch.cpp
        src/tools/countdown_latch.h
        src/price/provider/pixie/messages/data_dictionary_message.h
        src/price/provider/pixie/messages/data_dictionary_message.cpp
        src/price/provider/pixie/subject_set_register.h
        src/price/provider/pixie/subject_set_register.cpp
        src/price/provider/pixie/messages/subscription_sync.h
        src/price/provider/pixie/messages/subscription_sync.cpp
        src/tools/buffer_compressor.h
        src/tools/uncompressed_buffer_compressor.h
        src/tools/uncompressed_buffer_compressor.cpp
        src/tools/zlib_buffer_inflator.h
        src/tools/zlib_buffer_inflator.cpp
        src/price/provider/pixie/messages/price_sync.h
        src/price/provider/pixie/messages/price_sync.cpp
        src/Price/Subject/subject_comparator.h
        src/Price/Subject/subject_comparator.cpp
        src/price/provider/pixie/messages/price_sync_decoder.h
        src/price/provider/pixie/messages/price_sync_decoder.cpp
        src/price/provider/pixie/messages/syncable.h
        include/price/price_field.h
        src/Price/price_field.cpp
        include/subscription_status.h
        src/price/provider/pixie/messages/price_event.h
        src/price/provider/pixie/messages/price_update_decoder.h
        src/price/provider/pixie/messages/price_update_decoder.cpp
        src/price/provider/pixie/messages/encoding_util.h
        src/price/provider/pixie/messages/encoding_util.cpp
        src/Price/price_update_event.cpp
        src/price/provider/pixie/messages/heartbeat_message.h
        src/price/provider/pixie/messages/heartbeat_message.cpp
        src/Price/subscription_status_event.cpp
        src/Price/Provider/subscription_status_codec.h
        src/Price/Provider/subscription_status_codec.cpp
        src/price/provider/pixie/messages/heartbeat_message.cpp
        src/tools/blocking_queue.h
        src/tools/zlib_buffer_compressor.h
        src/tools/zlib_buffer_compressor.cpp
        include/price/subject/invalid_subject_exception.h
        src/Price/Subject/invalid_subject_exception.cpp
        include/exceptions/timeout_exception.h
        src/Exceptions/timeout_exception.cpp
        include/tools/logger_factory.h
        src/tools/logger_factory.cpp
        include/price/field_name.h
        src/Price/field_name.cpp
        src/price/provider/pixie/messages/column.h
        src/Price/field_name.cpp
        src/Price/subscription.h
        src/Price/subscription.cpp
        src/Price/Provider/provider_properties.cpp
        src/Price/Provider/provider_status_event_impl.h
        include/tools/logging_utils.h
        src/Price/Provider/provider_status_event_impl.cpp
        src/tools/character_padding.h
        src/tools/character_padding.cpp
        src/tools/name_cache.cpp
        src/tools/name_cache.h
        src/Exceptions/bid_fx_exception.cpp
        include/exceptions/bid_fx_exception.h
        include/exceptions/compressor_exception.h
        src/Exceptions/compressor_exception.cpp
        include/exceptions/decoding_exception.h
        src/Exceptions/decoding_exception.cpp
        include/exceptions/illegal_state_exception.h
        src/Exceptions/illegal_state_exception.cpp
        src/tools/optional.h)

# Concurrency
#add_subdirectory(lib/junction-master)

# Logging
include_directories(lib/logging/)
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -DCPP_DEBUG")

# SSL client (mbedtls)
add_subdirectory(lib/mbedtls)
find_library(mbedtls mbedtls lib/mbedtls)

# zlib
find_package(ZLIB REQUIRED)
include_directories(${ZLIB_INCLUDE_DIRS})

target_link_libraries(bidfx_public_api mbedtls ${ZLIB_LIBRARIES})

if (${CMAKE_SYSTEM_NAME} MATCHES "Windows" OR CYGWIN)
    target_link_libraries(bidfx_public_api crypt32)
endif()


##############################
# TESTING
##############################
configure_file(CMakeLists.txt.in googletest-download/CMakeLists.txt)
execute_process(COMMAND ${CMAKE_COMMAND} -G "${CMAKE_GENERATOR}" .
        RESULT_VARIABLE result
        WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/googletest-download )
if(result)
    message(FATAL_ERROR "CMake step for googletest failed: ${result}")
endif()
execute_process(COMMAND ${CMAKE_COMMAND} --build .
        RESULT_VARIABLE result
        WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/googletest-download )
if(result)
    message(FATAL_ERROR "Build step for googletest failed: ${result}")
endif()

# Prevent overriding the parent project's compiler/linker
# settings on Windows
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

# Add googletest directly to our build. This defines
# the gtest and gtest_main targets.
add_subdirectory(${CMAKE_BINARY_DIR}/googletest-src
        ${CMAKE_BINARY_DIR}/googletest-build
        EXCLUDE_FROM_ALL)

# The gtest/gtest_main targets carry header search path
# dependencies automatically when using CMake 2.8.11 or
# later. Otherwise we have to add them here ourselves.
if (CMAKE_VERSION VERSION_LESS 2.8.11)
    include_directories("${gtest_SOURCE_DIR}/include")
endif()

enable_testing()

# Add Threading support
set(CMAKE_THREAD_PREFER_PTHREAD TRUE)
set(THREADS_PREFER_PTHREAD_FLAG TRUE)
find_package(Threads REQUIRED)

add_executable(runTests
        test/Tools/byte_buffer_test.cpp
        test/Tools/varint_test.cpp
        test/Tools/varint_test.h
        test/Tools/varint_test.cpp
        test/Tools/varint_test.cpp
        test/price/provider/pixie/messages/ack_message_test.cpp
        test/Price/Provider/ack_data_test.cpp
        test/Price/Provider/Pixie/Fields/field_encoding_test.cpp
        test/Price/Provider/Pixie/Fields/field_type_test.cpp
        test/Price/Provider/Pixie/Fields/data_dictionary_utils_test.cpp
        test/Price/Provider/Pixie/Fields/extended_data_dictionary_test.cpp
        test/price/provider/pixie/messages/control_operation_test.cpp
        test/price/provider/pixie/messages/data_dictionary_message_test.cpp
        test/Tools/mock_ssl_client.h
        test/Price/Subject/subject_builder_test.cpp
        test/Tools/date_test.cpp test/Price/Subject/subject_factory_test.cpp
        test/Price/currencies_test.cpp
        test/Tools/string_char_iterator_test.cpp
        test/Price/tenor_test.cpp
        test/Tools/hex_encoder_test.cpp
        test/Price/Provider/Pixie/pixie_version_test.cpp
        test/price/provider/pixie/messages/grant_message_test.cpp
        test/Tools/buffer_util_test.cpp
        test/price/provider/pixie/messages/login_message_test.cpp
        test/price/provider/pixie/messages/welcome_message_test.cpp
        test/price/provider/pixie/messages/price_sync_decoder_test.cpp
        test/price/provider/pixie/messages/price_field_test.cpp
        test/price/provider/pixie/messages/pixie_test_helper.cpp
        test/price/provider/pixie/messages/pixie_test_helper.h
        test/price/provider/pixie/messages/price_update_decoder_test.cpp
        test/price/provider/pixie/messages/encoding_util_test.cpp
        test/Tools/countdown_latch_test.cpp
        test/price/provider/pixie/messages/subscription_sync_test.cpp
        test/Price/Subject/subject_formatter_test.cpp
        test/price/provider/pixie/messages/price_sync_test.cpp
        test/Price/Provider/Pixie/subject_set_register_tests.cpp
        test/Price/Subject/subject_comparator_test.cpp
        test/price/provider/pixie/messages/real_subscription_example.h
        test/price/provider/pixie/messages/real_subscription_example.cpp
        test/Price/Subject/subject_test.cpp
        test/price/provider/pixie/messages/heartbeat_message_test.cpp
        test/Tools/blocking_queue_test.cpp
        test/Tools/zlib_buffer_compressor_test.cpp
        test/Tools/character_padding_test.cpp
        test/price/provider/pixie/messages/mock_syncable.h
        test/Tools/blocking_queue_test.cpp
        test/Tools/zlib_buffer_compressor_test.cpp
        test/Price/subscription_test.cpp
        test/Price/subscription_test.cpp
        test/Tools/guid_test.cpp
        test/Tools/base64_test.cpp
        test/Tools/name_cache_test.cpp
        test/Exceptions/bid_fx_exception_test.cpp
        test/Tools/name_cache_test.cpp
        test/Price/Provider/provider_status_test.cpp
        test/subscription_status_test.cpp)

target_link_libraries(runTests gtest gmock_main bidfx_public_api mbedtls Threads::Threads)

if (${CMAKE_SYSTEM_NAME} MATCHES "Windows" OR CYGWIN)
    target_link_libraries(runTests crypt32)
endif()

####################
# Example
####################
add_executable(bidfx_public_api_example example/api_example.cpp example/api_example.h)

include_directories(lib/mbedtls/include)

target_link_libraries(bidfx_public_api_example bidfx_public_api mbedtls Threads::Threads)

if (${CMAKE_SYSTEM_NAME} MATCHES "Windows" OR CYGWIN)
    target_link_libraries(bidfx_public_api_example crypt32)
endif()
