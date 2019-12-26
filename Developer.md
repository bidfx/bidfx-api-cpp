# Developer Documentation

This document serves as a guide to developers of the API.

## Releasing new versions of the API

1. Ensure all tests pass.

1. Check the version in CMakeLists.txt is correct. [Semantic Versioning](https://semver.org) convention should be used.

    If the version number needs to be incremented this should be done as a lone commit with message

    `prepare release bidfx-api-cpp-[MAJOR].[MINOR].[PATCH]`

1. Tag HEAD as the release version. The tag should have the following format

     `bidfx-api-cpp-[MAJOR].[MINOR].[PATCH]`

     Push the tag to the remote Source Control

1. Increment the PATCH number and commit with message

    `prepare for next development iteration`
