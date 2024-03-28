cc_library(
    name = "kero_peg",
    srcs = [
        "src/internal/core.h",
        "src/internal/grammar/lexer.cc",
        "src/internal/grammar/lexer.h",
    ],
    hdrs = [
        "src/kero_peg.h",
    ],
    copts = ["-std=c++20"],
    includes = ["src"],
    deps = [
    ],
)

cc_test(
    name = "kero_peg_test",
    srcs = [
        "src/internal/core_test.cc",
        "src/internal/grammar/lexer_test.cc",
        "src/kero_peg_test.cc",
    ],
    copts = [
        "-std=c++20",
    ],
    deps = [
        ":kero_peg",
        "@googletest//:gtest_main",
    ],
)

cc_binary(
    name = "kero_peg_example",
    srcs = ["src/kero_peg_example.cc"],
    copts = [
        "-std=c++20",
    ],
    deps = [":kero_peg"],
)
