#include "core.h"

#include "gtest/gtest.h"

TEST(ResultTest, Ok) {
  auto res{kero::peg::Result<int, bool>{2}};
  EXPECT_TRUE(res.IsOk());
  EXPECT_FALSE(res.IsErr());
  EXPECT_EQ(*res.Ok(), 2);
}

TEST(ResultTest, Err) {
  auto res{kero::peg::Result<int, bool>{true}};
  EXPECT_FALSE(res.IsOk());
  EXPECT_TRUE(res.IsErr());
  EXPECT_EQ(*res.Err(), true);
}

TEST(ResultTest, PrintOk) {
  auto res{kero::peg::Result<int, bool>{2}};
  std::ostringstream oss;
  oss << res;
  EXPECT_EQ(oss.str(), "Result{Ok{2}}");
}

TEST(ResultTest, PrintErr) {
  auto res{kero::peg::Result<int, bool>{true}};
  std::ostringstream oss;
  oss << res;
  EXPECT_EQ(oss.str(), "Result{Err{1}}");
}

TEST(ResultVoidTest, Ok) {
  auto res{kero::peg::Result<void, bool>{}};
  EXPECT_TRUE(res.IsOk());
  EXPECT_FALSE(res.IsErr());
}

TEST(ResultVoidTest, Err) {
  auto res{kero::peg::Result<void, bool>{true}};
  EXPECT_FALSE(res.IsOk());
  EXPECT_TRUE(res.IsErr());
  EXPECT_EQ(*res.Err(), true);
}

TEST(ResultVoidTest, PrintOk) {
  auto res{kero::peg::Result<void, bool>{}};
  std::ostringstream oss;
  oss << res;
  EXPECT_EQ(oss.str(), "Result{Ok{void}}");
}

TEST(ResultVoidTest, PrintErr) {
  auto res{kero::peg::Result<void, bool>{true}};
  std::ostringstream oss;
  oss << res;
  EXPECT_EQ(oss.str(), "Result{Err{1}}");
}
