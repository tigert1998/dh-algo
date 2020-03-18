cc_library(
    name = "big_integer",
    srcs = ["big_integer.cc"],
    hdrs = ["big_integer.h"],
)

cc_binary(
    name = "task_1",
    srcs = ["task_1.cc"],
    copts = [],
    deps = [":big_integer"],
)

cc_binary(
    name = "task_2",
    srcs = ["task_2.cc"],
    copts = [],
    deps = [":big_integer"],
)

cc_test(
    name = "big_integer_test",
    srcs = ["big_integer_test.cc"],
    deps = [
        ":big_integer",
        "@googletest//:gtest_main"
    ]
)
