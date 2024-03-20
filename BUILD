cc_library(
    name = "kero_peg",
    srcs = [
    ],
    hdrs = [
        "src/kero_peg.h",
    ],
    copts = ["-std=c++20"],
    includes = ["src"],
    deps = [
        "@kero_log",
        "@kero_mpsc",
    ],
)

cc_test(
    name = "kero_peg_test",
    srcs = ["src/kero_peg_test.cc"],
    copts = ["-std=c++20"],
    deps = [
        ":kero_peg",
        "@googletest//:gtest_main",
    ],
)

cc_binary(
    name = "kero_peg_example",
    srcs = ["src/kero_peg_example.cc"],
    copts = ["-std=c++20"],
    deps = [":kero_peg"],
)
