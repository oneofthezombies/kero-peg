#include "core.h"

#include "gtest/gtest.h"

TEST(ResultTest, Ok) {
  auto res{kero::peg::Result<int, bool>{2}};
  EXPECT_TRUE(res.IsOk());
  EXPECT_FALSE(res.IsErr());
  EXPECT_EQ(*res.Ok(), 2);
  EXPECT_FALSE(res.IsOk());
}

TEST(ResultTest, Err) {
  auto res{kero::peg::Result<int, bool>{true}};
  EXPECT_FALSE(res.IsOk());
  EXPECT_TRUE(res.IsErr());
  EXPECT_EQ(*res.Err(), true);
  EXPECT_FALSE(res.IsErr());
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
  res.Ok();
  EXPECT_FALSE(res.IsOk());
}

TEST(ResultVoidTest, Err) {
  auto res{kero::peg::Result<void, bool>{true}};
  EXPECT_FALSE(res.IsOk());
  EXPECT_TRUE(res.IsErr());
  EXPECT_EQ(*res.Err(), true);
  EXPECT_FALSE(res.IsErr());
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

class Message {
public:
  Message(std::string&& data) noexcept : data{std::move(data)} {}

  Message(Message&&) noexcept = default;
  ~Message() noexcept = default;
  auto operator=(Message&&) noexcept -> Message& = default;

  Message(const Message&) = delete;
  auto operator=(const Message&) -> Message& = delete;

  std::string data;
};

TEST(ResultMoveTest, Ok) {
  auto res{kero::peg::Result<Message, bool>{Message{"a"}}};
  EXPECT_TRUE(res.IsOk());
  EXPECT_FALSE(res.IsErr());
  EXPECT_EQ(res.Ok()->data, "a");
  EXPECT_FALSE(res.IsOk());
}

TEST(ResultAndThen, Ok) {
  auto res{kero::peg::Result<Message, bool>{Message{"a"}}};
  res.AndThen([](auto&& message) {
       EXPECT_EQ(message.data, "a");
       return kero::peg::Result<int, bool>{2};
     })
      .AndThen([](auto&& value) {
        EXPECT_EQ(value, 2);
        return kero::peg::Result<void, bool>{};
      });
}

TEST(ResultOrElse, Ok) {
  auto res{kero::peg::Result<Message, bool>{true}};
  res.OrElse([](auto&& error) {
       EXPECT_EQ(error, true);
       return kero::peg::Result<Message, int>{2};
     })
      .OrElse([](auto&& error) {
        EXPECT_EQ(error, 2);
        return kero::peg::Result<Message, float>{3.0f};
      });
}
