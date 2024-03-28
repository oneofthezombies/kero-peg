#include "lexer.h"

#include "gtest/gtest.h"

auto NextOk(kero::peg::Lexer& lexer, const kero::peg::TokenKind kind,
            const std::string_view value) noexcept -> void {
  auto token_res = lexer.Next();
  ASSERT_TRUE(token_res.IsOk());
  const auto token = *token_res.Ok();
  EXPECT_EQ(token.kind, kind);
  EXPECT_EQ(token.value, value);
}

auto NextErr(kero::peg::Lexer& lexer,
             const kero::peg::TokenizeErrorCode error_code) noexcept -> void {
  auto token_res = lexer.Next();
  ASSERT_TRUE(token_res.IsErr());
  const auto error = *token_res.Err();
  EXPECT_EQ(error.code, error_code);
}

TEST(LexerTest, Empty) {
  auto lexer{kero::peg::Lexer{""}};
  NextOk(lexer, kero::peg::TokenKind::kEndOfFile, "");
}

TEST(LexerTest, Space) {
  auto lexer{kero::peg::Lexer{" "}};
  NextOk(lexer, kero::peg::TokenKind::kEndOfFile, "");
}

TEST(LexerTest, Space2) {
  auto lexer{kero::peg::Lexer{"  "}};
  NextOk(lexer, kero::peg::TokenKind::kEndOfFile, "");
}

TEST(LexerTest, Tab) {
  auto lexer{kero::peg::Lexer{"\t"}};
  NextOk(lexer, kero::peg::TokenKind::kEndOfFile, "");
}

TEST(LexerTest, Tab2) {
  auto lexer{kero::peg::Lexer{"\t\t"}};
  NextOk(lexer, kero::peg::TokenKind::kEndOfFile, "");
}

TEST(LexerTest, SpaceTab) {
  auto lexer{kero::peg::Lexer{" \t"}};
  NextOk(lexer, kero::peg::TokenKind::kEndOfFile, "");
}

TEST(LexerTest, TabSpace) {
  auto lexer{kero::peg::Lexer{"\t "}};
  NextOk(lexer, kero::peg::TokenKind::kEndOfFile, "");
}

TEST(LexerTest, UnixNewLine) {
  auto lexer{kero::peg::Lexer{"\n"}};
  NextOk(lexer, kero::peg::TokenKind::kEndOfFile, "");
}

TEST(LexerTest, UnixNewLine2) {
  auto lexer{kero::peg::Lexer{"\n\n"}};
  NextOk(lexer, kero::peg::TokenKind::kEndOfFile, "");
}

TEST(LexerTest, WindowsNewLine) {
  auto lexer{kero::peg::Lexer{"\r\n"}};
  NextOk(lexer, kero::peg::TokenKind::kEndOfFile, "");
}

TEST(LexerTest, WindowsNewLine2) {
  auto lexer{kero::peg::Lexer{"\r\n\r\n"}};
  NextOk(lexer, kero::peg::TokenKind::kEndOfFile, "");
}

TEST(LexerTest, NonTerminal) {
  auto lexer{kero::peg::Lexer{"A"}};
  NextOk(lexer, kero::peg::TokenKind::kNonTerminal, "A");
  NextOk(lexer, kero::peg::TokenKind::kEndOfFile, "");
}

TEST(LexerTest, NonTerminalWithSpace) {
  auto lexer{kero::peg::Lexer{"A "}};
  NextOk(lexer, kero::peg::TokenKind::kNonTerminal, "A");
  NextOk(lexer, kero::peg::TokenKind::kEndOfFile, "");
}

TEST(LexerTest, NonTerminalWithTab) {
  auto lexer{kero::peg::Lexer{"A\t"}};
  NextOk(lexer, kero::peg::TokenKind::kNonTerminal, "A");
  NextOk(lexer, kero::peg::TokenKind::kEndOfFile, "");
}

TEST(LexerTest, NonTerminalWithNewLine) {
  auto lexer{kero::peg::Lexer{"A\n"}};
  NextOk(lexer, kero::peg::TokenKind::kNonTerminal, "A");
  NextOk(lexer, kero::peg::TokenKind::kEndOfFile, "");
}

TEST(LexerTest, NonTerminalAfterSpace) {
  auto lexer{kero::peg::Lexer{" A"}};
  NextOk(lexer, kero::peg::TokenKind::kNonTerminal, "A");
  NextOk(lexer, kero::peg::TokenKind::kEndOfFile, "");
}

TEST(LexerTest, NonTerminalAfterTab) {
  auto lexer{kero::peg::Lexer{"\tA"}};
  NextOk(lexer, kero::peg::TokenKind::kNonTerminal, "A");
  NextOk(lexer, kero::peg::TokenKind::kEndOfFile, "");
}

TEST(LexerTest, NonTerminalAfterNewLine) {
  auto lexer{kero::peg::Lexer{"\nA"}};
  NextOk(lexer, kero::peg::TokenKind::kNonTerminal, "A");
  NextOk(lexer, kero::peg::TokenKind::kEndOfFile, "");
}

TEST(LexerTest, LeftArrow) {
  auto lexer{kero::peg::Lexer{"<-"}};
  NextOk(lexer, kero::peg::TokenKind::kLeftArrow, "<-");
  NextOk(lexer, kero::peg::TokenKind::kEndOfFile, "");
}

TEST(LexerTest, Terminal) {
  auto lexer{kero::peg::Lexer{"\"A\""}};
  NextOk(lexer, kero::peg::TokenKind::kQuotedTerminal, "A");
  NextOk(lexer, kero::peg::TokenKind::kEndOfFile, "");
}

TEST(LexerTest, TerminalWithSpace) {
  auto lexer{kero::peg::Lexer{"\"A\" "}};
  NextOk(lexer, kero::peg::TokenKind::kQuotedTerminal, "A");
  NextOk(lexer, kero::peg::TokenKind::kEndOfFile, "");
}

TEST(LexerTest, TerminalWithTab) {
  auto lexer{kero::peg::Lexer{"\"A\"\t"}};
  NextOk(lexer, kero::peg::TokenKind::kQuotedTerminal, "A");
  NextOk(lexer, kero::peg::TokenKind::kEndOfFile, "");
}

TEST(LexerTest, TerminalWithNewLine) {
  auto lexer{kero::peg::Lexer{"\"A\"\n"}};
  NextOk(lexer, kero::peg::TokenKind::kQuotedTerminal, "A");
  NextOk(lexer, kero::peg::TokenKind::kEndOfFile, "");
}

TEST(LexerTest, TerminalAfterSpace) {
  auto lexer{kero::peg::Lexer{" \"A\""}};
  NextOk(lexer, kero::peg::TokenKind::kQuotedTerminal, "A");
  NextOk(lexer, kero::peg::TokenKind::kEndOfFile, "");
}

TEST(LexerTest, TerminalAfterTab) {
  auto lexer{kero::peg::Lexer{"\t\"A\""}};
  NextOk(lexer, kero::peg::TokenKind::kQuotedTerminal, "A");
  NextOk(lexer, kero::peg::TokenKind::kEndOfFile, "");
}

TEST(LexerTest, TerminalAfterNewLine) {
  auto lexer{kero::peg::Lexer{"\n\"A\""}};
  NextOk(lexer, kero::peg::TokenKind::kQuotedTerminal, "A");
  NextOk(lexer, kero::peg::TokenKind::kEndOfFile, "");
}

TEST(LexerTest, TerminalWithEscape) {
  auto lexer{kero::peg::Lexer{"\"\\\"\""}};
  NextOk(lexer, kero::peg::TokenKind::kQuotedTerminal, "\\");
  NextErr(lexer, kero::peg::TokenizeErrorCode::kMatchFailed);
}

TEST(LexerTest, TerminalUsingSingleQuote) {
  auto lexer{kero::peg::Lexer{"'A'"}};
  NextOk(lexer, kero::peg::TokenKind::kQuotedTerminal, "A");
  NextOk(lexer, kero::peg::TokenKind::kEndOfFile, "");
}

TEST(LexerTest, TerminalUsingSingleQuoteWithSpace) {
  auto lexer{kero::peg::Lexer{"'A' "}};
  NextOk(lexer, kero::peg::TokenKind::kQuotedTerminal, "A");
  NextOk(lexer, kero::peg::TokenKind::kEndOfFile, "");
}

TEST(LexerTest, TerminalUsingSingleQuoteWithTab) {
  auto lexer{kero::peg::Lexer{"'A'\t"}};
  NextOk(lexer, kero::peg::TokenKind::kQuotedTerminal, "A");
  NextOk(lexer, kero::peg::TokenKind::kEndOfFile, "");
}

TEST(LexerTest, TerminalUsingSingleQuoteWithNewLine) {
  auto lexer{kero::peg::Lexer{"'A'\n"}};
  NextOk(lexer, kero::peg::TokenKind::kQuotedTerminal, "A");
  NextOk(lexer, kero::peg::TokenKind::kEndOfFile, "");
}

TEST(LexerTest, TerminalUsingSingleQuoteAfterSpace) {
  auto lexer{kero::peg::Lexer{" 'A'"}};
  NextOk(lexer, kero::peg::TokenKind::kQuotedTerminal, "A");
  NextOk(lexer, kero::peg::TokenKind::kEndOfFile, "");
}

TEST(LexerTest, TerminalUsingSingleQuoteAfterTab) {
  auto lexer{kero::peg::Lexer{"\t'A'"}};
  NextOk(lexer, kero::peg::TokenKind::kQuotedTerminal, "A");
  NextOk(lexer, kero::peg::TokenKind::kEndOfFile, "");
}

TEST(LexerTest, TerminalUsingSingleQuoteAfterNewLine) {
  auto lexer{kero::peg::Lexer{"\n'A'"}};
  NextOk(lexer, kero::peg::TokenKind::kQuotedTerminal, "A");
  NextOk(lexer, kero::peg::TokenKind::kEndOfFile, "");
}

TEST(LexerTest, TerminalUsingSingleQuoteWithEscape) {
  auto lexer{kero::peg::Lexer{"'\\''"}};
  NextOk(lexer, kero::peg::TokenKind::kQuotedTerminal, "\\");
  NextErr(lexer, kero::peg::TokenizeErrorCode::kMatchFailed);
}

TEST(LexerTest, NonTerminalFromNonTerminal) {
  auto lexer{kero::peg::Lexer{"A <- B"}};
  NextOk(lexer, kero::peg::TokenKind::kNonTerminal, "A");
  NextOk(lexer, kero::peg::TokenKind::kLeftArrow, "<-");
  NextOk(lexer, kero::peg::TokenKind::kNonTerminal, "B");
  NextOk(lexer, kero::peg::TokenKind::kEndOfFile, "");
}

TEST(LexerTest, NonTerminalFromNonTerminalWithNewLine) {
  auto lexer{kero::peg::Lexer{"A <- B\n"}};
  NextOk(lexer, kero::peg::TokenKind::kNonTerminal, "A");
  NextOk(lexer, kero::peg::TokenKind::kLeftArrow, "<-");
  NextOk(lexer, kero::peg::TokenKind::kNonTerminal, "B");
  NextOk(lexer, kero::peg::TokenKind::kEndOfFile, "");
}

TEST(LexerTest, NonTerminalFromTerminal) {
  auto lexer{kero::peg::Lexer{"A <- \"B\""}};
  NextOk(lexer, kero::peg::TokenKind::kNonTerminal, "A");
  NextOk(lexer, kero::peg::TokenKind::kLeftArrow, "<-");
  NextOk(lexer, kero::peg::TokenKind::kQuotedTerminal, "B");
  NextOk(lexer, kero::peg::TokenKind::kEndOfFile, "");
}

TEST(LexerTest, NonTerminalExpression2) {
  auto lexer{kero::peg::Lexer{"A <- B\nC <- D"}};
  NextOk(lexer, kero::peg::TokenKind::kNonTerminal, "A");
  NextOk(lexer, kero::peg::TokenKind::kLeftArrow, "<-");
  NextOk(lexer, kero::peg::TokenKind::kNonTerminal, "B");
  NextOk(lexer, kero::peg::TokenKind::kNonTerminal, "C");
  NextOk(lexer, kero::peg::TokenKind::kLeftArrow, "<-");
  NextOk(lexer, kero::peg::TokenKind::kNonTerminal, "D");
  NextOk(lexer, kero::peg::TokenKind::kEndOfFile, "");
}

TEST(LexerTest, NonTerminalExpressionAndTerminalExpression) {
  auto lexer{kero::peg::Lexer{"A <- B\nC <- \"D\""}};
  NextOk(lexer, kero::peg::TokenKind::kNonTerminal, "A");
  NextOk(lexer, kero::peg::TokenKind::kLeftArrow, "<-");
  NextOk(lexer, kero::peg::TokenKind::kNonTerminal, "B");
  NextOk(lexer, kero::peg::TokenKind::kNonTerminal, "C");
  NextOk(lexer, kero::peg::TokenKind::kLeftArrow, "<-");
  NextOk(lexer, kero::peg::TokenKind::kQuotedTerminal, "D");
  NextOk(lexer, kero::peg::TokenKind::kEndOfFile, "");
}

TEST(LexerTest, LeftParenthesis) {
  auto lexer{kero::peg::Lexer{"("}};
  NextOk(lexer, kero::peg::TokenKind::kLeftParenthesis, "(");
  NextOk(lexer, kero::peg::TokenKind::kEndOfFile, "");
}

TEST(LexerTest, RightParenthesis) {
  auto lexer{kero::peg::Lexer{")"}};
  NextOk(lexer, kero::peg::TokenKind::kRightParenthesis, ")");
  NextOk(lexer, kero::peg::TokenKind::kEndOfFile, "");
}

TEST(LexerTest, Asterisk) {
  auto lexer{kero::peg::Lexer{"*"}};
  NextOk(lexer, kero::peg::TokenKind::kAsterisk, "*");
  NextOk(lexer, kero::peg::TokenKind::kEndOfFile, "");
}

TEST(LexerTest, Plus) {
  auto lexer{kero::peg::Lexer{"+"}};
  NextOk(lexer, kero::peg::TokenKind::kPlus, "+");
  NextOk(lexer, kero::peg::TokenKind::kEndOfFile, "");
}

TEST(LexerTest, QuestionMark) {
  auto lexer{kero::peg::Lexer{"?"}};
  NextOk(lexer, kero::peg::TokenKind::kQuestionMark, "?");
  NextOk(lexer, kero::peg::TokenKind::kEndOfFile, "");
}

TEST(LexerTest, Ampersand) {
  auto lexer{kero::peg::Lexer{"&"}};
  NextOk(lexer, kero::peg::TokenKind::kAmpersand, "&");
  NextOk(lexer, kero::peg::TokenKind::kEndOfFile, "");
}

TEST(LexerTest, ExclamationMark) {
  auto lexer{kero::peg::Lexer{"!"}};
  NextOk(lexer, kero::peg::TokenKind::kExclamationMark, "!");
  NextOk(lexer, kero::peg::TokenKind::kEndOfFile, "");
}

TEST(LexerTest, Slash) {
  auto lexer{kero::peg::Lexer{"/"}};
  NextOk(lexer, kero::peg::TokenKind::kSlash, "/");
  NextOk(lexer, kero::peg::TokenKind::kEndOfFile, "");
}

TEST(LexerTest, Dot) {
  auto lexer{kero::peg::Lexer{"."}};
  NextOk(lexer, kero::peg::TokenKind::kDot, ".");
  NextOk(lexer, kero::peg::TokenKind::kEndOfFile, "");
}

TEST(LexerTest, BracketedTerminal) {
  auto lexer{kero::peg::Lexer{"[a]"}};
  NextOk(lexer, kero::peg::TokenKind::kBracketedTerminal, "a");
  NextOk(lexer, kero::peg::TokenKind::kEndOfFile, "");
}

TEST(LexerTest, BracketedTerminalEmpty) {
  auto lexer{kero::peg::Lexer{"[]"}};
  NextErr(lexer, kero::peg::TokenizeErrorCode::kBracketedTerminalEmpty);
  NextOk(lexer, kero::peg::TokenKind::kEndOfFile, "");
}

TEST(LexerTest, BracketedTerminalMinusSignLeftNotFound) {
  auto lexer{kero::peg::Lexer{"[-a]"}};
  NextErr(
      lexer,
      kero::peg::TokenizeErrorCode::kBracketedTerminalMinusSignLeftNotFound);
  NextOk(lexer, kero::peg::TokenKind::kEndOfFile, "");
}

TEST(LexerTest, BracketedTerminalMinusSignRightNotFound) {
  auto lexer{kero::peg::Lexer{"[a-]"}};
  NextErr(
      lexer,
      kero::peg::TokenizeErrorCode::kBracketedTerminalMinusSignRightNotFound);
  NextOk(lexer, kero::peg::TokenKind::kEndOfFile, "");
}

TEST(LexerTest, BracketedTerminalMinusSignRightNotNumber) {
  auto lexer{kero::peg::Lexer{"[0-a]"}};
  NextErr(
      lexer,
      kero::peg::TokenizeErrorCode::kBracketedTerminalMinusSignRightNotNumber);
  NextOk(lexer, kero::peg::TokenKind::kEndOfFile, "");
}

TEST(LexerTest, BracketedTerminalMinusSignRightNumberEqualWithLeft) {
  auto lexer{kero::peg::Lexer{"[0-0]"}};
  NextErr(lexer, kero::peg::TokenizeErrorCode::
                     kBracketedTerminalMinusSignRightNumberEqualWithLeft);
  NextOk(lexer, kero::peg::TokenKind::kEndOfFile, "");
}

TEST(LexerTest, BracketedTerminalMinusSignRightNumberLessThanLeft) {
  auto lexer{kero::peg::Lexer{"[1-0]"}};
  NextErr(lexer, kero::peg::TokenizeErrorCode::
                     kBracketedTerminalMinusSignRightNumberLessThanLeft);
  NextOk(lexer, kero::peg::TokenKind::kEndOfFile, "");
}

TEST(LexerTest, BracketedTerminalMinusSignNumber) {
  auto lexer{kero::peg::Lexer{"[0-9]"}};
  NextOk(lexer, kero::peg::TokenKind::kBracketedTerminal, "0-9");
  NextOk(lexer, kero::peg::TokenKind::kEndOfFile, "");
}

TEST(LexerTest, BracketedTerminalMinusSignRightNotLowercase) {
  auto lexer{kero::peg::Lexer{"[a-A]"}};
  NextErr(lexer, kero::peg::TokenizeErrorCode::
                     kBracketedTerminalMinusSignRightNotLowercase);
  NextOk(lexer, kero::peg::TokenKind::kEndOfFile, "");
}

TEST(LexerTest, BracketedTerminalMinusSignRightLowercaseEqualWithLeft) {
  auto lexer{kero::peg::Lexer{"[a-a]"}};
  NextErr(lexer, kero::peg::TokenizeErrorCode::
                     kBracketedTerminalMinusSignRightLowercaseEqualWithLeft);
  NextOk(lexer, kero::peg::TokenKind::kEndOfFile, "");
}

TEST(LexerTest, BracketedTerminalMinusSignRightLowercaseLessThanLeft) {
  auto lexer{kero::peg::Lexer{"[b-a]"}};
  NextErr(lexer, kero::peg::TokenizeErrorCode::
                     kBracketedTerminalMinusSignRightLowercaseLessThanLeft);
  NextOk(lexer, kero::peg::TokenKind::kEndOfFile, "");
}

TEST(LexerTest, BracketedTerminalMinusSignLowercase) {
  auto lexer{kero::peg::Lexer{"[a-z]"}};
  NextOk(lexer, kero::peg::TokenKind::kBracketedTerminal, "a-z");
  NextOk(lexer, kero::peg::TokenKind::kEndOfFile, "");
}

TEST(LexerTest, BracketedTerminalMinusSignRightNotUppercase) {
  auto lexer{kero::peg::Lexer{"[A-a]"}};
  NextErr(lexer, kero::peg::TokenizeErrorCode::
                     kBracketedTerminalMinusSignRightNotUppercase);
  NextOk(lexer, kero::peg::TokenKind::kEndOfFile, "");
}

TEST(LexerTest, BracketedTerminalMinusSignRightUppercaseEqualWithLeft) {
  auto lexer{kero::peg::Lexer{"[A-A]"}};
  NextErr(lexer, kero::peg::TokenizeErrorCode::
                     kBracketedTerminalMinusSignRightUppercaseEqualWithLeft);
  NextOk(lexer, kero::peg::TokenKind::kEndOfFile, "");
}

TEST(LexerTest, BracketedTerminalMinusSignRightUppercaseLessThanLeft) {
  auto lexer{kero::peg::Lexer{"[B-A]"}};
  NextErr(lexer, kero::peg::TokenizeErrorCode::
                     kBracketedTerminalMinusSignRightUppercaseLessThanLeft);
  NextOk(lexer, kero::peg::TokenKind::kEndOfFile, "");
}

TEST(LexerTest, BracketedTerminalMinusSignUppercase) {
  auto lexer{kero::peg::Lexer{"[A-Z]"}};
  NextOk(lexer, kero::peg::TokenKind::kBracketedTerminal, "A-Z");
  NextOk(lexer, kero::peg::TokenKind::kEndOfFile, "");
}

TEST(LexerTest, BracketedTerminalAlphabet) {
  auto lexer{kero::peg::Lexer{"[a-zA-Z]"}};
  NextOk(lexer, kero::peg::TokenKind::kBracketedTerminal, "a-zA-Z");
  NextOk(lexer, kero::peg::TokenKind::kEndOfFile, "");
}

TEST(LexerTest, BracketedTerminalAlphabetNumber) {
  auto lexer{kero::peg::Lexer{"[a-zA-Z0-9]"}};
  NextOk(lexer, kero::peg::TokenKind::kBracketedTerminal, "a-zA-Z0-9");
  NextOk(lexer, kero::peg::TokenKind::kEndOfFile, "");
}
