cmake_minimum_required(VERSION 3.10)

#option(test "Build all tests." OFF) # Turn on with cmake -Dtest=ON

set(CMAKE_CXX_STANDARD 17)

SET(CMAKE_C_FLAGS_DEBUG "-D_DEBUG")

project(bidfx_public_api_cpp VERSION 1.0.3 DESCRIPTION "BidFX Public C++ API for accessing price streams")

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
        src/price/provider/pixie/fields/field_encoding.h
        src/price/provider/pixie/fields/field_encoding.cpp
        src/price/provider/pixie/fields/field_type.h
        src/price/provider/pixie/fields/field_type.cpp
        src/price/provider/pixie/fields/data_dictionary.h
        src/price/provider/pixie/fields/data_dictionary_utils.cpp
        src/price/provider/pixie/fields/data_dictionary_utils.h
        src/price/provider/pixie/fields/extendable_data_dictionary.cpp
        src/price/provider/pixie/fields/extendable_data_dictionary.h
        src/price/provider/pixie/fields/field_def.h
        src/price/provider/pixie/fields/field_def.cpp
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
        src/price/pricing_subscriber.h
        src/price/pricing_subscriber.cpp
        include/price/subject/subject_factory.h
        include/user_info.h
        src/basic_user_info.cpp
        src/basic_user_info.h
        src/price/subject/subject_factory.cpp
        include/price/subject/subject_builder.h
        src/price/subject/subject_builder.cpp
        src/price/subject/subject.cpp
       	include/tenor.h
        src/tenor.cpp
        include/price/subject/subject_utils.h
        src/price/subject/subject_utils.cpp
        include/tools/date.h
        src/tools/date.cpp
        src/price/subject/subject_key.h
        src/price/subject/subject_key.cpp
        src/price/request_type.h
        src/price/request_type.cpp
        include/price/deal_type.h
        src/price/deal_type.cpp
        include/price/asset_class.h
        src/price/asset_class.cpp
        src/price/level.h
        src/price/level.cpp
        src/price/pricing_session.h
        src/price/pricing_session.cpp
        include/provider_status_event.h
        include/subscription_status_event.h
        include/api_session.h
        src/api_session.cpp
        include/pricing.h
        include/public_api.h
        src/public_api.cpp
        src/price/currencies.h
        src/price/currencies.cpp
        src/tools/string_char_iterator.h
        src/tools/string_char_iterator.cpp
        src/price/subject/subject_formatter.h
        src/price/subject/subject_formatter.cpp
        include/price/provider/provider.h
        include/price/provider/provider_properties.h
        src/price/provider/abstract_provider.h
        src/price/provider/provider_consumer.h
        src/price/provider/abstract_provider.cpp
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
        src/price/subject/subject_comparator.h
        src/price/subject/subject_comparator.cpp
        src/price/provider/pixie/messages/price_sync_decoder.h
        src/price/provider/pixie/messages/price_sync_decoder.cpp
        src/price/provider/pixie/messages/syncable.h
        include/price/price_field.h
        src/price/price_field.cpp
        include/subscription_status.h
        src/price/provider/pixie/messages/price_event.h
        src/price/provider/pixie/messages/price_update_decoder.h
        src/price/provider/pixie/messages/price_update_decoder.cpp
        src/price/provider/pixie/messages/encoding_util.h
        src/price/provider/pixie/messages/encoding_util.cpp
        src/price/price_update_event.cpp
        src/price/provider/pixie/messages/heartbeat_message.h
        src/price/provider/pixie/messages/heartbeat_message.cpp
        src/price/subscription_status_event.cpp
        src/price/provider/subscription_status_codec.h
        src/price/provider/subscription_status_codec.cpp
        src/price/provider/pixie/messages/heartbeat_message.cpp
        src/tools/blocking_queue.h
        src/tools/zlib_buffer_compressor.h
        src/tools/zlib_buffer_compressor.cpp
        include/price/subject/invalid_subject_exception.h
        src/price/subject/invalid_subject_exception.cpp
        include/exceptions/timeout_exception.h
        src/exceptions/timeout_exception.cpp
        include/tools/logger_factory.h
        src/tools/logger_factory.cpp
        include/price/field_name.h
        src/price/field_name.cpp
        src/price/provider/pixie/messages/column.h
        src/price/field_name.cpp
        src/price/subscription.h
        src/price/subscription.cpp
        src/price/provider/provider_properties.cpp
        src/price/provider/provider_status_event_impl.h
        include/tools/logging_utils.h
        src/price/provider/provider_status_event_impl.cpp
        src/tools/character_padding.h
        src/tools/character_padding.cpp
        src/tools/name_cache.cpp
        src/tools/name_cache.h
        src/exceptions/bid_fx_exception.cpp
        include/exceptions/bid_fx_exception.h
        include/exceptions/compressor_exception.h
        src/exceptions/compressor_exception.cpp
        include/exceptions/decoding_exception.h
        src/exceptions/decoding_exception.cpp
        include/exceptions/illegal_state_exception.h
        src/exceptions/illegal_state_exception.cpp
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
        test/tools/byte_buffer_test.cpp
        test/tools/varint_test.cpp
        test/tools/varint_test.h
        test/tools/varint_test.cpp
        test/tools/varint_test.cpp
        test/price/provider/pixie/messages/ack_message_test.cpp
        test/price/provider/ack_data_test.cpp
        test/price/provider/pixie/fields/field_encoding_test.cpp
        test/price/provider/pixie/fields/field_type_test.cpp
        test/price/provider/pixie/fields/data_dictionary_utils_test.cpp
        test/price/provider/pixie/fields/extended_data_dictionary_test.cpp
        test/price/provider/pixie/messages/control_operation_test.cpp
        test/price/provider/pixie/messages/data_dictionary_message_test.cpp
        test/price/subject/subject_builder_test.cpp
        test/tools/date_test.cpp test/price/subject/subject_factory_test.cpp
        test/price/currencies_test.cpp
        test/tools/string_char_iterator_test.cpp
        test/price/tenor_test.cpp
        test/tools/hex_encoder_test.cpp
        test/price/provider/pixie/pixie_version_test.cpp
        test/price/provider/pixie/messages/grant_message_test.cpp
        test/tools/buffer_util_test.cpp
        test/price/provider/pixie/messages/login_message_test.cpp
        test/price/provider/pixie/messages/welcome_message_test.cpp
        test/price/provider/pixie/messages/price_sync_decoder_test.cpp
        test/price/provider/pixie/messages/price_field_test.cpp
        test/price/provider/pixie/messages/pixie_test_helper.cpp
        test/price/provider/pixie/messages/pixie_test_helper.h
        test/price/provider/pixie/messages/price_update_decoder_test.cpp
        test/price/provider/pixie/messages/encoding_util_test.cpp
        test/tools/countdown_latch_test.cpp
        test/price/provider/pixie/messages/subscription_sync_test.cpp
        test/price/subject/subject_formatter_test.cpp
        test/price/provider/pixie/messages/price_sync_test.cpp
        test/price/provider/pixie/subject_set_register_tests.cpp
        test/price/subject/subject_comparator_test.cpp
        test/price/provider/pixie/messages/real_subscription_example.h
        test/price/provider/pixie/messages/real_subscription_example.cpp
        test/price/subject/subject_test.cpp
        test/price/provider/pixie/messages/heartbeat_message_test.cpp
        test/tools/blocking_queue_test.cpp
        test/tools/zlib_buffer_compressor_test.cpp
        test/tools/character_padding_test.cpp
        test/price/provider/pixie/messages/mock_syncable.h
        test/tools/blocking_queue_test.cpp
        test/tools/zlib_buffer_compressor_test.cpp
        test/price/subscription_test.cpp
        test/price/subscription_test.cpp
        test/tools/guid_test.cpp
        test/tools/base64_test.cpp
        test/tools/name_cache_test.cpp
        test/exceptions/bid_fx_exception_test.cpp
        test/tools/name_cache_test.cpp
        test/price/provider/provider_status_test.cpp
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
