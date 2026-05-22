idf_component_register(
    SRCS
        "main.cc"
        "st7305/st7305.cc"
    INCLUDE_DIRS
        "."
        "st7305"
        "images"
    REQUIRES driver esp_timer
)
