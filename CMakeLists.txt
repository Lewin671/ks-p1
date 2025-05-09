cmake_minimum_required(VERSION 3.10)
project(InputSystem LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# --- nlohmann/json --- FetchContent
include(FetchContent)
FetchContent_Declare(
    nlohmann_json
    GIT_REPOSITORY https://github.com/nlohmann/json.git
    GIT_TAG v3.11.3 # Or any other recent stable version
)
FetchContent_MakeAvailable(nlohmann_json)
# --- nlohmann/json --- End

# Add executable
add_executable(InputSystem
    ActionMap.cpp
    Command.cpp
    ConflictResolver.cpp
    DeviceManager.cpp
    GamepadAdapter.cpp
    KeyboardAdapter.cpp
    main.cpp
)

# Link nlohmann_json
# nlohmann_json is a header-only library, but it's good practice to specify it
# target_link_libraries(InputSystem PRIVATE nlohmann_json::nlohmann_json) # Modern CMake target
# For older CMake or if the above doesn't work directly, you might need to include directories
target_include_directories(InputSystem PRIVATE ${nlohmann_json_SOURCE_DIR}/include)

# Ensure bindings.json is accessible by the executable
# This command copies bindings.json to the directory where the executable will be run from after building.
configure_file(
    ${CMAKE_CURRENT_SOURCE_DIR}/bindings.json
    ${CMAKE_CURRENT_BINARY_DIR}/bindings.json
    COPYONLY
)

# Optional: Add include directories if your headers are in a separate 'include' folder
# target_include_directories(InputSystem PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)

# Enable C++17 features for the target
set_target_properties(InputSystem PROPERTIES CXX_STANDARD 17)

install(TARGETS InputSystem DESTINATION bin)