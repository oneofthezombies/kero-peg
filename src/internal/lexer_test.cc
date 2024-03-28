#include "lexer.h"

#include "gtest/gtest.h"

TEST(LexerTest, OneLine) {
  auto lexer{kero::peg::Lexer{"A <- B"}};
  {
    auto token_res = lexer.Next();
    ASSERT_TRUE(token_res.IsOk());
    const auto token = *token_res.Ok();
    EXPECT_EQ(token.kind, kero::peg::TokenKind::kIdentifier);
    EXPECT_EQ(token.value, "A");
  }
  {
    auto token_res = lexer.Next();
    ASSERT_TRUE(token_res.IsOk());
    const auto token = *token_res.Ok();
    EXPECT_EQ(token.kind, kero::peg::TokenKind::kLeftArrow);
    EXPECT_EQ(token.value, "<-");
  }
  {
    auto token_res = lexer.Next();
    std::cout << *token_res.Error() << std::endl;
    ASSERT_TRUE(token_res.IsOk());
    const auto token = *token_res.Ok();
    EXPECT_EQ(token.kind, kero::peg::TokenKind::kExpression);
    EXPECT_EQ(token.value, "B");
  }
  {
    auto token_res = lexer.Next();
    ASSERT_TRUE(token_res.IsOk());
    const auto token = *token_res.Ok();
    EXPECT_EQ(token.kind, kero::peg::TokenKind::kEndOfFile);
    EXPECT_EQ(token.value, "");
  }
}
