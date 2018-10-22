# Add external dependencies here.

add_library(spdlog INTERFACE IMPORTED GLOBAL)

target_include_directories(spdlog INTERFACE spdlog/include)