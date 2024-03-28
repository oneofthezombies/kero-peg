#include "lexer.h"

#include "gtest/gtest.h"

TEST(LexerTest, Empty) {
  auto lexer{kero::peg::Lexer{""}};
  auto token_res = lexer.Next();
  ASSERT_TRUE(token_res.IsOk());
  const auto token = *token_res.Ok();
  EXPECT_EQ(token.kind, kero::peg::TokenKind::kEndOfFile);
  EXPECT_EQ(token.value, "");
}

TEST(LexerTest, Space) {
  auto lexer{kero::peg::Lexer{" "}};
  auto token_res = lexer.Next();
  ASSERT_TRUE(token_res.IsOk());
  const auto token = *token_res.Ok();
  EXPECT_EQ(token.kind, kero::peg::TokenKind::kEndOfFile);
  EXPECT_EQ(token.value, "");
}

TEST(LexerTest, Space2) {
  auto lexer{kero::peg::Lexer{"  "}};
  auto token_res = lexer.Next();
  ASSERT_TRUE(token_res.IsOk());
  const auto token = *token_res.Ok();
  EXPECT_EQ(token.kind, kero::peg::TokenKind::kEndOfFile);
  EXPECT_EQ(token.value, "");
}

TEST(LexerTest, Tab) {
  auto lexer{kero::peg::Lexer{"\t"}};
  auto token_res = lexer.Next();
  ASSERT_TRUE(token_res.IsOk());
  const auto token = *token_res.Ok();
  EXPECT_EQ(token.kind, kero::peg::TokenKind::kEndOfFile);
  EXPECT_EQ(token.value, "");
}

TEST(LexerTest, Tab2) {
  auto lexer{kero::peg::Lexer{"\t\t"}};
  auto token_res = lexer.Next();
  ASSERT_TRUE(token_res.IsOk());
  const auto token = *token_res.Ok();
  EXPECT_EQ(token.kind, kero::peg::TokenKind::kEndOfFile);
  EXPECT_EQ(token.value, "");
}

TEST(LexerTest, SpaceTab) {
  auto lexer{kero::peg::Lexer{" \t"}};
  auto token_res = lexer.Next();
  ASSERT_TRUE(token_res.IsOk());
  const auto token = *token_res.Ok();
  EXPECT_EQ(token.kind, kero::peg::TokenKind::kEndOfFile);
  EXPECT_EQ(token.value, "");
}

TEST(LexerTest, TabSpace) {
  auto lexer{kero::peg::Lexer{"\t "}};
  auto token_res = lexer.Next();
  ASSERT_TRUE(token_res.IsOk());
  const auto token = *token_res.Ok();
  EXPECT_EQ(token.kind, kero::peg::TokenKind::kEndOfFile);
  EXPECT_EQ(token.value, "");
}

TEST(LexerTest, UnixNewLine) {
  auto lexer{kero::peg::Lexer{"\n"}};
  auto token_res = lexer.Next();
  ASSERT_TRUE(token_res.IsOk());
  const auto token = *token_res.Ok();
  EXPECT_EQ(token.kind, kero::peg::TokenKind::kEndOfFile);
  EXPECT_EQ(token.value, "");
}

TEST(LexerTest, UnixNewLine2) {
  auto lexer{kero::peg::Lexer{"\n\n"}};
  auto token_res = lexer.Next();
  ASSERT_TRUE(token_res.IsOk());
  const auto token = *token_res.Ok();
  EXPECT_EQ(token.kind, kero::peg::TokenKind::kEndOfFile);
  EXPECT_EQ(token.value, "");
}

TEST(LexerTest, WindowsNewLine) {
  auto lexer{kero::peg::Lexer{"\r\n"}};
  auto token_res = lexer.Next();
  ASSERT_TRUE(token_res.IsOk());
  const auto token = *token_res.Ok();
  EXPECT_EQ(token.kind, kero::peg::TokenKind::kEndOfFile);
  EXPECT_EQ(token.value, "");
}

TEST(LexerTest, WindowsNewLine2) {
  auto lexer{kero::peg::Lexer{"\r\n\r\n"}};
  auto token_res = lexer.Next();
  ASSERT_TRUE(token_res.IsOk());
  const auto token = *token_res.Ok();
  EXPECT_EQ(token.kind, kero::peg::TokenKind::kEndOfFile);
  EXPECT_EQ(token.value, "");
}

TEST(LexerTest, NonTerminal) {
  auto lexer{kero::peg::Lexer{"A"}};
  {
    auto token_res = lexer.Next();
    ASSERT_TRUE(token_res.IsOk());
    const auto token = *token_res.Ok();
    EXPECT_EQ(token.kind, kero::peg::TokenKind::kNonTerminal);
    EXPECT_EQ(token.value, "A");
  }
  {
    auto token_res = lexer.Next();
    ASSERT_TRUE(token_res.IsOk());
    const auto token = *token_res.Ok();
    EXPECT_EQ(token.kind, kero::peg::TokenKind::kEndOfFile);
    EXPECT_EQ(token.value, "");
  }
}

TEST(LexerTest, NonTerminalWithSpace) {
  auto lexer{kero::peg::Lexer{"A "}};
  {
    auto token_res = lexer.Next();
    ASSERT_TRUE(token_res.IsOk());
    const auto token = *token_res.Ok();
    EXPECT_EQ(token.kind, kero::peg::TokenKind::kNonTerminal);
    EXPECT_EQ(token.value, "A");
  }
  {
    auto token_res = lexer.Next();
    ASSERT_TRUE(token_res.IsOk());
    const auto token = *token_res.Ok();
    EXPECT_EQ(token.kind, kero::peg::TokenKind::kEndOfFile);
    EXPECT_EQ(token.value, "");
  }
}

TEST(LexerTest, NonTerminalWithTab) {
  auto lexer{kero::peg::Lexer{"A\t"}};
  {
    auto token_res = lexer.Next();
    ASSERT_TRUE(token_res.IsOk());
    const auto token = *token_res.Ok();
    EXPECT_EQ(token.kind, kero::peg::TokenKind::kNonTerminal);
    EXPECT_EQ(token.value, "A");
  }
  {
    auto token_res = lexer.Next();
    ASSERT_TRUE(token_res.IsOk());
    const auto token = *token_res.Ok();
    EXPECT_EQ(token.kind, kero::peg::TokenKind::kEndOfFile);
    EXPECT_EQ(token.value, "");
  }
}

TEST(LexerTest, NonTerminalWithNewLine) {
  auto lexer{kero::peg::Lexer{"A\n"}};
  {
    auto token_res = lexer.Next();
    ASSERT_TRUE(token_res.IsOk());
    const auto token = *token_res.Ok();
    EXPECT_EQ(token.kind, kero::peg::TokenKind::kNonTerminal);
    EXPECT_EQ(token.value, "A");
  }
  {
    auto token_res = lexer.Next();
    ASSERT_TRUE(token_res.IsOk());
    const auto token = *token_res.Ok();
    EXPECT_EQ(token.kind, kero::peg::TokenKind::kEndOfFile);
    EXPECT_EQ(token.value, "");
  }
}

TEST(LexerTest, NonTerminalAfterSpace) {
  auto lexer{kero::peg::Lexer{" A"}};
  {
    auto token_res = lexer.Next();
    ASSERT_TRUE(token_res.IsOk());
    const auto token = *token_res.Ok();
    EXPECT_EQ(token.kind, kero::peg::TokenKind::kNonTerminal);
    EXPECT_EQ(token.value, "A");
  }
  {
    auto token_res = lexer.Next();
    ASSERT_TRUE(token_res.IsOk());
    const auto token = *token_res.Ok();
    EXPECT_EQ(token.kind, kero::peg::TokenKind::kEndOfFile);
    EXPECT_EQ(token.value, "");
  }
}

TEST(LexerTest, NonTerminalAfterTab) {
  auto lexer{kero::peg::Lexer{"\tA"}};
  {
    auto token_res = lexer.Next();
    ASSERT_TRUE(token_res.IsOk());
    const auto token = *token_res.Ok();
    EXPECT_EQ(token.kind, kero::peg::TokenKind::kNonTerminal);
    EXPECT_EQ(token.value, "A");
  }
  {
    auto token_res = lexer.Next();
    ASSERT_TRUE(token_res.IsOk());
    const auto token = *token_res.Ok();
    EXPECT_EQ(token.kind, kero::peg::TokenKind::kEndOfFile);
    EXPECT_EQ(token.value, "");
  }
}

TEST(LexerTest, NonTerminalAfterNewLine) {
  auto lexer{kero::peg::Lexer{"\nA"}};
  {
    auto token_res = lexer.Next();
    ASSERT_TRUE(token_res.IsOk());
    const auto token = *token_res.Ok();
    EXPECT_EQ(token.kind, kero::peg::TokenKind::kNonTerminal);
    EXPECT_EQ(token.value, "A");
  }
  {
    auto token_res = lexer.Next();
    ASSERT_TRUE(token_res.IsOk());
    const auto token = *token_res.Ok();
    EXPECT_EQ(token.kind, kero::peg::TokenKind::kEndOfFile);
    EXPECT_EQ(token.value, "");
  }
}

TEST(LexerTest, LeftArrow) {
  auto lexer{kero::peg::Lexer{"<-"}};
  {
    auto token_res = lexer.Next();
    ASSERT_TRUE(token_res.IsOk());
    const auto token = *token_res.Ok();
    EXPECT_EQ(token.kind, kero::peg::TokenKind::kLeftArrow);
    EXPECT_EQ(token.value, "<-");
  }
  {
    auto token_res = lexer.Next();
    ASSERT_TRUE(token_res.IsOk());
    const auto token = *token_res.Ok();
    EXPECT_EQ(token.kind, kero::peg::TokenKind::kEndOfFile);
    EXPECT_EQ(token.value, "");
  }
}

TEST(LexerTest, Terminal) {
  auto lexer{kero::peg::Lexer{"\"A\""}};
  {
    auto token_res = lexer.Next();
    ASSERT_TRUE(token_res.IsOk());
    const auto token = *token_res.Ok();
    EXPECT_EQ(token.kind, kero::peg::TokenKind::kTerminal);
    EXPECT_EQ(token.value, "\"A\"");
  }
  {
    auto token_res = lexer.Next();
    ASSERT_TRUE(token_res.IsOk());
    const auto token = *token_res.Ok();
    EXPECT_EQ(token.kind, kero::peg::TokenKind::kEndOfFile);
    EXPECT_EQ(token.value, "");
  }
}

TEST(LexerTest, TerminalWithSpace) {
  auto lexer{kero::peg::Lexer{"\"A\" "}};
  {
    auto token_res = lexer.Next();
    ASSERT_TRUE(token_res.IsOk());
    const auto token = *token_res.Ok();
    EXPECT_EQ(token.kind, kero::peg::TokenKind::kTerminal);
    EXPECT_EQ(token.value, "\"A\"");
  }
  {
    auto token_res = lexer.Next();
    ASSERT_TRUE(token_res.IsOk());
    const auto token = *token_res.Ok();
    EXPECT_EQ(token.kind, kero::peg::TokenKind::kEndOfFile);
    EXPECT_EQ(token.value, "");
  }
}

TEST(LexerTest, TerminalWithTab) {
  auto lexer{kero::peg::Lexer{"\"A\"\t"}};
  {
    auto token_res = lexer.Next();
    ASSERT_TRUE(token_res.IsOk());
    const auto token = *token_res.Ok();
    EXPECT_EQ(token.kind, kero::peg::TokenKind::kTerminal);
    EXPECT_EQ(token.value, "\"A\"");
  }
  {
    auto token_res = lexer.Next();
    ASSERT_TRUE(token_res.IsOk());
    const auto token = *token_res.Ok();
    EXPECT_EQ(token.kind, kero::peg::TokenKind::kEndOfFile);
    EXPECT_EQ(token.value, "");
  }
}

TEST(LexerTest, TerminalWithNewLine) {
  auto lexer{kero::peg::Lexer{"\"A\"\n"}};
  {
    auto token_res = lexer.Next();
    ASSERT_TRUE(token_res.IsOk());
    const auto token = *token_res.Ok();
    EXPECT_EQ(token.kind, kero::peg::TokenKind::kTerminal);
    EXPECT_EQ(token.value, "\"A\"");
  }
  {
    auto token_res = lexer.Next();
    ASSERT_TRUE(token_res.IsOk());
    const auto token = *token_res.Ok();
    EXPECT_EQ(token.kind, kero::peg::TokenKind::kEndOfFile);
    EXPECT_EQ(token.value, "");
  }
}

TEST(LexerTest, TerminalAfterSpace) {
  auto lexer{kero::peg::Lexer{" \"A\""}};
  {
    auto token_res = lexer.Next();
    ASSERT_TRUE(token_res.IsOk());
    const auto token = *token_res.Ok();
    EXPECT_EQ(token.kind, kero::peg::TokenKind::kTerminal);
    EXPECT_EQ(token.value, "\"A\"");
  }
  {
    auto token_res = lexer.Next();
    ASSERT_TRUE(token_res.IsOk());
    const auto token = *token_res.Ok();
    EXPECT_EQ(token.kind, kero::peg::TokenKind::kEndOfFile);
    EXPECT_EQ(token.value, "");
  }
}

TEST(LexerTest, TerminalAfterTab) {
  auto lexer{kero::peg::Lexer{"\t\"A\""}};
  {
    auto token_res = lexer.Next();
    ASSERT_TRUE(token_res.IsOk());
    const auto token = *token_res.Ok();
    EXPECT_EQ(token.kind, kero::peg::TokenKind::kTerminal);
    EXPECT_EQ(token.value, "\"A\"");
  }
  {
    auto token_res = lexer.Next();
    ASSERT_TRUE(token_res.IsOk());
    const auto token = *token_res.Ok();
    EXPECT_EQ(token.kind, kero::peg::TokenKind::kEndOfFile);
    EXPECT_EQ(token.value, "");
  }
}

TEST(LexerTest, TerminalAfterNewLine) {
  auto lexer{kero::peg::Lexer{"\n\"A\""}};
  {
    auto token_res = lexer.Next();
    ASSERT_TRUE(token_res.IsOk());
    const auto token = *token_res.Ok();
    EXPECT_EQ(token.kind, kero::peg::TokenKind::kTerminal);
    EXPECT_EQ(token.value, "\"A\"");
  }
  {
    auto token_res = lexer.Next();
    ASSERT_TRUE(token_res.IsOk());
    const auto token = *token_res.Ok();
    EXPECT_EQ(token.kind, kero::peg::TokenKind::kEndOfFile);
    EXPECT_EQ(token.value, "");
  }
}

TEST(LexerTest, TerminalWithEscape) {
  auto lexer{kero::peg::Lexer{"\"\\\"\""}};
  {
    auto token_res = lexer.Next();
    ASSERT_TRUE(token_res.IsOk());
    const auto token = *token_res.Ok();
    EXPECT_EQ(token.kind, kero::peg::TokenKind::kTerminal);
    EXPECT_EQ(token.value, "\"\\\"");
  }
  {
    auto token_res = lexer.Next();
    ASSERT_TRUE(token_res.IsError());
    const auto error = *token_res.Error();
    EXPECT_EQ(error, kero::peg::LexerNextError::kMatchFailed);
  }
}

TEST(LexerTest, TerminalUsingSingleQuote) {
  auto lexer{kero::peg::Lexer{"'A'"}};
  {
    auto token_res = lexer.Next();
    ASSERT_TRUE(token_res.IsOk());
    const auto token = *token_res.Ok();
    EXPECT_EQ(token.kind, kero::peg::TokenKind::kTerminal);
    EXPECT_EQ(token.value, "'A'");
  }
  {
    auto token_res = lexer.Next();
    ASSERT_TRUE(token_res.IsOk());
    const auto token = *token_res.Ok();
    EXPECT_EQ(token.kind, kero::peg::TokenKind::kEndOfFile);
    EXPECT_EQ(token.value, "");
  }
}

TEST(LexerTest, TerminalUsingSingleQuoteWithSpace) {
  auto lexer{kero::peg::Lexer{"'A' "}};
  {
    auto token_res = lexer.Next();
    ASSERT_TRUE(token_res.IsOk());
    const auto token = *token_res.Ok();
    EXPECT_EQ(token.kind, kero::peg::TokenKind::kTerminal);
    EXPECT_EQ(token.value, "'A'");
  }
  {
    auto token_res = lexer.Next();
    ASSERT_TRUE(token_res.IsOk());
    const auto token = *token_res.Ok();
    EXPECT_EQ(token.kind, kero::peg::TokenKind::kEndOfFile);
    EXPECT_EQ(token.value, "");
  }
}

TEST(LexerTest, TerminalUsingSingleQuoteWithTab) {
  auto lexer{kero::peg::Lexer{"'A'\t"}};
  {
    auto token_res = lexer.Next();
    ASSERT_TRUE(token_res.IsOk());
    const auto token = *token_res.Ok();
    EXPECT_EQ(token.kind, kero::peg::TokenKind::kTerminal);
    EXPECT_EQ(token.value, "'A'");
  }
  {
    auto token_res = lexer.Next();
    ASSERT_TRUE(token_res.IsOk());
    const auto token = *token_res.Ok();
    EXPECT_EQ(token.kind, kero::peg::TokenKind::kEndOfFile);
    EXPECT_EQ(token.value, "");
  }
}

TEST(LexerTest, TerminalUsingSingleQuoteWithNewLine) {
  auto lexer{kero::peg::Lexer{"'A'\n"}};
  {
    auto token_res = lexer.Next();
    ASSERT_TRUE(token_res.IsOk());
    const auto token = *token_res.Ok();
    EXPECT_EQ(token.kind, kero::peg::TokenKind::kTerminal);
    EXPECT_EQ(token.value, "'A'");
  }
  {
    auto token_res = lexer.Next();
    ASSERT_TRUE(token_res.IsOk());
    const auto token = *token_res.Ok();
    EXPECT_EQ(token.kind, kero::peg::TokenKind::kEndOfFile);
    EXPECT_EQ(token.value, "");
  }
}

TEST(LexerTest, TerminalUsingSingleQuoteAfterSpace) {
  auto lexer{kero::peg::Lexer{" 'A'"}};
  {
    auto token_res = lexer.Next();
    ASSERT_TRUE(token_res.IsOk());
    const auto token = *token_res.Ok();
    EXPECT_EQ(token.kind, kero::peg::TokenKind::kTerminal);
    EXPECT_EQ(token.value, "'A'");
  }
  {
    auto token_res = lexer.Next();
    ASSERT_TRUE(token_res.IsOk());
    const auto token = *token_res.Ok();
    EXPECT_EQ(token.kind, kero::peg::TokenKind::kEndOfFile);
    EXPECT_EQ(token.value, "");
  }
}

TEST(LexerTest, TerminalUsingSingleQuoteAfterTab) {
  auto lexer{kero::peg::Lexer{"\t'A'"}};
  {
    auto token_res = lexer.Next();
    ASSERT_TRUE(token_res.IsOk());
    const auto token = *token_res.Ok();
    EXPECT_EQ(token.kind, kero::peg::TokenKind::kTerminal);
    EXPECT_EQ(token.value, "'A'");
  }
  {
    auto token_res = lexer.Next();
    ASSERT_TRUE(token_res.IsOk());
    const auto token = *token_res.Ok();
    EXPECT_EQ(token.kind, kero::peg::TokenKind::kEndOfFile);
    EXPECT_EQ(token.value, "");
  }
}

TEST(LexerTest, TerminalUsingSingleQuoteAfterNewLine) {
  auto lexer{kero::peg::Lexer{"\n'A'"}};
  {
    auto token_res = lexer.Next();
    ASSERT_TRUE(token_res.IsOk());
    const auto token = *token_res.Ok();
    EXPECT_EQ(token.kind, kero::peg::TokenKind::kTerminal);
    EXPECT_EQ(token.value, "'A'");
  }
  {
    auto token_res = lexer.Next();
    ASSERT_TRUE(token_res.IsOk());
    const auto token = *token_res.Ok();
    EXPECT_EQ(token.kind, kero::peg::TokenKind::kEndOfFile);
    EXPECT_EQ(token.value, "");
  }
}

TEST(LexerTest, TerminalUsingSingleQuoteWithEscape) {
  auto lexer{kero::peg::Lexer{"'\\''"}};
  {
    auto token_res = lexer.Next();
    ASSERT_TRUE(token_res.IsOk());
    const auto token = *token_res.Ok();
    EXPECT_EQ(token.kind, kero::peg::TokenKind::kTerminal);
    EXPECT_EQ(token.value, "'\\'");
  }
  {
    auto token_res = lexer.Next();
    ASSERT_TRUE(token_res.IsError());
    const auto error = *token_res.Error();
    EXPECT_EQ(error, kero::peg::LexerNextError::kMatchFailed);
  }
}

TEST(LexerTest, NonTerminalFromNonTerminal) {
  auto lexer{kero::peg::Lexer{"A <- B"}};
  {
    auto token_res = lexer.Next();
    ASSERT_TRUE(token_res.IsOk());
    const auto token = *token_res.Ok();
    EXPECT_EQ(token.kind, kero::peg::TokenKind::kNonTerminal);
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
    ASSERT_TRUE(token_res.IsOk());
    const auto token = *token_res.Ok();
    EXPECT_EQ(token.kind, kero::peg::TokenKind::kNonTerminal);
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

TEST(LexerTest, NonTerminalFromNonTerminalWithNewLine) {
  auto lexer{kero::peg::Lexer{"A <- B\n"}};
  {
    auto token_res = lexer.Next();
    ASSERT_TRUE(token_res.IsOk());
    const auto token = *token_res.Ok();
    EXPECT_EQ(token.kind, kero::peg::TokenKind::kNonTerminal);
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
    ASSERT_TRUE(token_res.IsOk());
    const auto token = *token_res.Ok();
    EXPECT_EQ(token.kind, kero::peg::TokenKind::kNonTerminal);
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

TEST(LexerTest, NonTerminalFromTerminal) {
  auto lexer{kero::peg::Lexer{"A <- \"B\""}};
  {
    auto token_res = lexer.Next();
    ASSERT_TRUE(token_res.IsOk());
    const auto token = *token_res.Ok();
    EXPECT_EQ(token.kind, kero::peg::TokenKind::kNonTerminal);
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
    ASSERT_TRUE(token_res.IsOk());
    const auto token = *token_res.Ok();
    EXPECT_EQ(token.kind, kero::peg::TokenKind::kTerminal);
    EXPECT_EQ(token.value, "\"B\"");
  }
  {
    auto token_res = lexer.Next();
    ASSERT_TRUE(token_res.IsOk());
    const auto token = *token_res.Ok();
    EXPECT_EQ(token.kind, kero::peg::TokenKind::kEndOfFile);
    EXPECT_EQ(token.value, "");
  }
}

TEST(LexerTest, NonTerminalExpression2) {
  auto lexer{kero::peg::Lexer{"A <- B\nC <- D"}};
  {
    auto token_res = lexer.Next();
    ASSERT_TRUE(token_res.IsOk());
    const auto token = *token_res.Ok();
    EXPECT_EQ(token.kind, kero::peg::TokenKind::kNonTerminal);
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
    ASSERT_TRUE(token_res.IsOk());
    const auto token = *token_res.Ok();
    EXPECT_EQ(token.kind, kero::peg::TokenKind::kNonTerminal);
    EXPECT_EQ(token.value, "B");
  }
  {
    auto token_res = lexer.Next();
    ASSERT_TRUE(token_res.IsOk());
    const auto token = *token_res.Ok();
    EXPECT_EQ(token.kind, kero::peg::TokenKind::kNonTerminal);
    EXPECT_EQ(token.value, "C");
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
    ASSERT_TRUE(token_res.IsOk());
    const auto token = *token_res.Ok();
    EXPECT_EQ(token.kind, kero::peg::TokenKind::kNonTerminal);
    EXPECT_EQ(token.value, "D");
  }
  {
    auto token_res = lexer.Next();
    ASSERT_TRUE(token_res.IsOk());
    const auto token = *token_res.Ok();
    EXPECT_EQ(token.kind, kero::peg::TokenKind::kEndOfFile);
    EXPECT_EQ(token.value, "");
  }
}

TEST(LexerTest, NonTerminalExpressionAndTerminalExpression) {
  auto lexer{kero::peg::Lexer{"A <- B\nC <- \"D\""}};
  {
    auto token_res = lexer.Next();
    ASSERT_TRUE(token_res.IsOk());
    const auto token = *token_res.Ok();
    EXPECT_EQ(token.kind, kero::peg::TokenKind::kNonTerminal);
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
    ASSERT_TRUE(token_res.IsOk());
    const auto token = *token_res.Ok();
    EXPECT_EQ(token.kind, kero::peg::TokenKind::kNonTerminal);
    EXPECT_EQ(token.value, "B");
  }
  {
    auto token_res = lexer.Next();
    ASSERT_TRUE(token_res.IsOk());
    const auto token = *token_res.Ok();
    EXPECT_EQ(token.kind, kero::peg::TokenKind::kNonTerminal);
    EXPECT_EQ(token.value, "C");
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
    ASSERT_TRUE(token_res.IsOk());
    const auto token = *token_res.Ok();
    EXPECT_EQ(token.kind, kero::peg::TokenKind::kTerminal);
    EXPECT_EQ(token.value, "\"D\"");
  }
  {
    auto token_res = lexer.Next();
    ASSERT_TRUE(token_res.IsOk());
    const auto token = *token_res.Ok();
    EXPECT_EQ(token.kind, kero::peg::TokenKind::kEndOfFile);
    EXPECT_EQ(token.value, "");
  }
}
