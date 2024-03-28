#include "lexer.h"

#include "gtest/gtest.h"

auto NextOk(kero::peg::grammar::Lexer& lexer,
            const kero::peg::grammar::TokenKind kind,
            const std::string_view value) noexcept -> void {
  auto token_res = lexer.Next();
  ASSERT_TRUE(token_res.IsOk());
  const auto token = *token_res.Ok();
  EXPECT_EQ(token.kind, kind);
  EXPECT_EQ(token.value, value);
}

auto NextErr(kero::peg::grammar::Lexer& lexer,
             const kero::peg::grammar::TokenizeErrorCode error_code) noexcept
    -> void {
  auto token_res = lexer.Next();
  ASSERT_TRUE(token_res.IsErr());
  const auto error = *token_res.Err();
  EXPECT_EQ(error.code, error_code);
}

TEST(LexerTest, Empty) {
  auto lexer{kero::peg::grammar::Lexer{""}};
  NextOk(lexer, kero::peg::grammar::TokenKind::kEndOfFile, "");
}

TEST(LexerTest, Space) {
  auto lexer{kero::peg::grammar::Lexer{" "}};
  NextOk(lexer, kero::peg::grammar::TokenKind::kEndOfFile, "");
}

TEST(LexerTest, Space2) {
  auto lexer{kero::peg::grammar::Lexer{"  "}};
  NextOk(lexer, kero::peg::grammar::TokenKind::kEndOfFile, "");
}

TEST(LexerTest, Tab) {
  auto lexer{kero::peg::grammar::Lexer{"\t"}};
  NextOk(lexer, kero::peg::grammar::TokenKind::kEndOfFile, "");
}

TEST(LexerTest, Tab2) {
  auto lexer{kero::peg::grammar::Lexer{"\t\t"}};
  NextOk(lexer, kero::peg::grammar::TokenKind::kEndOfFile, "");
}

TEST(LexerTest, SpaceTab) {
  auto lexer{kero::peg::grammar::Lexer{" \t"}};
  NextOk(lexer, kero::peg::grammar::TokenKind::kEndOfFile, "");
}

TEST(LexerTest, TabSpace) {
  auto lexer{kero::peg::grammar::Lexer{"\t "}};
  NextOk(lexer, kero::peg::grammar::TokenKind::kEndOfFile, "");
}

TEST(LexerTest, UnixNewLine) {
  auto lexer{kero::peg::grammar::Lexer{"\n"}};
  NextOk(lexer, kero::peg::grammar::TokenKind::kEndOfFile, "");
}

TEST(LexerTest, UnixNewLine2) {
  auto lexer{kero::peg::grammar::Lexer{"\n\n"}};
  NextOk(lexer, kero::peg::grammar::TokenKind::kEndOfFile, "");
}

TEST(LexerTest, WindowsNewLine) {
  auto lexer{kero::peg::grammar::Lexer{"\r\n"}};
  NextOk(lexer, kero::peg::grammar::TokenKind::kEndOfFile, "");
}

TEST(LexerTest, WindowsNewLine2) {
  auto lexer{kero::peg::grammar::Lexer{"\r\n\r\n"}};
  NextOk(lexer, kero::peg::grammar::TokenKind::kEndOfFile, "");
}

TEST(LexerTest, NonTerminal) {
  auto lexer{kero::peg::grammar::Lexer{"A"}};
  NextOk(lexer, kero::peg::grammar::TokenKind::kNonTerminal, "A");
  NextOk(lexer, kero::peg::grammar::TokenKind::kEndOfFile, "");
}

TEST(LexerTest, NonTerminalWithSpace) {
  auto lexer{kero::peg::grammar::Lexer{"A "}};
  NextOk(lexer, kero::peg::grammar::TokenKind::kNonTerminal, "A");
  NextOk(lexer, kero::peg::grammar::TokenKind::kEndOfFile, "");
}

TEST(LexerTest, NonTerminalWithTab) {
  auto lexer{kero::peg::grammar::Lexer{"A\t"}};
  NextOk(lexer, kero::peg::grammar::TokenKind::kNonTerminal, "A");
  NextOk(lexer, kero::peg::grammar::TokenKind::kEndOfFile, "");
}

TEST(LexerTest, NonTerminalWithNewLine) {
  auto lexer{kero::peg::grammar::Lexer{"A\n"}};
  NextOk(lexer, kero::peg::grammar::TokenKind::kNonTerminal, "A");
  NextOk(lexer, kero::peg::grammar::TokenKind::kEndOfFile, "");
}

TEST(LexerTest, NonTerminalAfterSpace) {
  auto lexer{kero::peg::grammar::Lexer{" A"}};
  NextOk(lexer, kero::peg::grammar::TokenKind::kNonTerminal, "A");
  NextOk(lexer, kero::peg::grammar::TokenKind::kEndOfFile, "");
}

TEST(LexerTest, NonTerminalAfterTab) {
  auto lexer{kero::peg::grammar::Lexer{"\tA"}};
  NextOk(lexer, kero::peg::grammar::TokenKind::kNonTerminal, "A");
  NextOk(lexer, kero::peg::grammar::TokenKind::kEndOfFile, "");
}

TEST(LexerTest, NonTerminalAfterNewLine) {
  auto lexer{kero::peg::grammar::Lexer{"\nA"}};
  NextOk(lexer, kero::peg::grammar::TokenKind::kNonTerminal, "A");
  NextOk(lexer, kero::peg::grammar::TokenKind::kEndOfFile, "");
}

TEST(LexerTest, LeftArrow) {
  auto lexer{kero::peg::grammar::Lexer{"<-"}};
  NextOk(lexer, kero::peg::grammar::TokenKind::kLeftArrow, "<-");
  NextOk(lexer, kero::peg::grammar::TokenKind::kEndOfFile, "");
}

TEST(LexerTest, Terminal) {
  auto lexer{kero::peg::grammar::Lexer{"\"A\""}};
  NextOk(lexer, kero::peg::grammar::TokenKind::kQuotedTerminal, "A");
  NextOk(lexer, kero::peg::grammar::TokenKind::kEndOfFile, "");
}

TEST(LexerTest, TerminalWithSpace) {
  auto lexer{kero::peg::grammar::Lexer{"\"A\" "}};
  NextOk(lexer, kero::peg::grammar::TokenKind::kQuotedTerminal, "A");
  NextOk(lexer, kero::peg::grammar::TokenKind::kEndOfFile, "");
}

TEST(LexerTest, TerminalWithTab) {
  auto lexer{kero::peg::grammar::Lexer{"\"A\"\t"}};
  NextOk(lexer, kero::peg::grammar::TokenKind::kQuotedTerminal, "A");
  NextOk(lexer, kero::peg::grammar::TokenKind::kEndOfFile, "");
}

TEST(LexerTest, TerminalWithNewLine) {
  auto lexer{kero::peg::grammar::Lexer{"\"A\"\n"}};
  NextOk(lexer, kero::peg::grammar::TokenKind::kQuotedTerminal, "A");
  NextOk(lexer, kero::peg::grammar::TokenKind::kEndOfFile, "");
}

TEST(LexerTest, TerminalAfterSpace) {
  auto lexer{kero::peg::grammar::Lexer{" \"A\""}};
  NextOk(lexer, kero::peg::grammar::TokenKind::kQuotedTerminal, "A");
  NextOk(lexer, kero::peg::grammar::TokenKind::kEndOfFile, "");
}

TEST(LexerTest, TerminalAfterTab) {
  auto lexer{kero::peg::grammar::Lexer{"\t\"A\""}};
  NextOk(lexer, kero::peg::grammar::TokenKind::kQuotedTerminal, "A");
  NextOk(lexer, kero::peg::grammar::TokenKind::kEndOfFile, "");
}

TEST(LexerTest, TerminalAfterNewLine) {
  auto lexer{kero::peg::grammar::Lexer{"\n\"A\""}};
  NextOk(lexer, kero::peg::grammar::TokenKind::kQuotedTerminal, "A");
  NextOk(lexer, kero::peg::grammar::TokenKind::kEndOfFile, "");
}

TEST(LexerTest, TerminalWithEscape) {
  auto lexer{kero::peg::grammar::Lexer{"\"\\\"\""}};
  NextOk(lexer, kero::peg::grammar::TokenKind::kQuotedTerminal, "\\");
  NextErr(lexer, kero::peg::grammar::TokenizeErrorCode::kMatchFailed);
}

TEST(LexerTest, TerminalUsingSingleQuote) {
  auto lexer{kero::peg::grammar::Lexer{"'A'"}};
  NextOk(lexer, kero::peg::grammar::TokenKind::kQuotedTerminal, "A");
  NextOk(lexer, kero::peg::grammar::TokenKind::kEndOfFile, "");
}

TEST(LexerTest, TerminalUsingSingleQuoteWithSpace) {
  auto lexer{kero::peg::grammar::Lexer{"'A' "}};
  NextOk(lexer, kero::peg::grammar::TokenKind::kQuotedTerminal, "A");
  NextOk(lexer, kero::peg::grammar::TokenKind::kEndOfFile, "");
}

TEST(LexerTest, TerminalUsingSingleQuoteWithTab) {
  auto lexer{kero::peg::grammar::Lexer{"'A'\t"}};
  NextOk(lexer, kero::peg::grammar::TokenKind::kQuotedTerminal, "A");
  NextOk(lexer, kero::peg::grammar::TokenKind::kEndOfFile, "");
}

TEST(LexerTest, TerminalUsingSingleQuoteWithNewLine) {
  auto lexer{kero::peg::grammar::Lexer{"'A'\n"}};
  NextOk(lexer, kero::peg::grammar::TokenKind::kQuotedTerminal, "A");
  NextOk(lexer, kero::peg::grammar::TokenKind::kEndOfFile, "");
}

TEST(LexerTest, TerminalUsingSingleQuoteAfterSpace) {
  auto lexer{kero::peg::grammar::Lexer{" 'A'"}};
  NextOk(lexer, kero::peg::grammar::TokenKind::kQuotedTerminal, "A");
  NextOk(lexer, kero::peg::grammar::TokenKind::kEndOfFile, "");
}

TEST(LexerTest, TerminalUsingSingleQuoteAfterTab) {
  auto lexer{kero::peg::grammar::Lexer{"\t'A'"}};
  NextOk(lexer, kero::peg::grammar::TokenKind::kQuotedTerminal, "A");
  NextOk(lexer, kero::peg::grammar::TokenKind::kEndOfFile, "");
}

TEST(LexerTest, TerminalUsingSingleQuoteAfterNewLine) {
  auto lexer{kero::peg::grammar::Lexer{"\n'A'"}};
  NextOk(lexer, kero::peg::grammar::TokenKind::kQuotedTerminal, "A");
  NextOk(lexer, kero::peg::grammar::TokenKind::kEndOfFile, "");
}

TEST(LexerTest, TerminalUsingSingleQuoteWithEscape) {
  auto lexer{kero::peg::grammar::Lexer{"'\\''"}};
  NextOk(lexer, kero::peg::grammar::TokenKind::kQuotedTerminal, "\\");
  NextErr(lexer, kero::peg::grammar::TokenizeErrorCode::kMatchFailed);
}

TEST(LexerTest, NonTerminalFromNonTerminal) {
  auto lexer{kero::peg::grammar::Lexer{"A <- B"}};
  NextOk(lexer, kero::peg::grammar::TokenKind::kNonTerminal, "A");
  NextOk(lexer, kero::peg::grammar::TokenKind::kLeftArrow, "<-");
  NextOk(lexer, kero::peg::grammar::TokenKind::kNonTerminal, "B");
  NextOk(lexer, kero::peg::grammar::TokenKind::kEndOfFile, "");
}

TEST(LexerTest, NonTerminalFromNonTerminalWithNewLine) {
  auto lexer{kero::peg::grammar::Lexer{"A <- B\n"}};
  NextOk(lexer, kero::peg::grammar::TokenKind::kNonTerminal, "A");
  NextOk(lexer, kero::peg::grammar::TokenKind::kLeftArrow, "<-");
  NextOk(lexer, kero::peg::grammar::TokenKind::kNonTerminal, "B");
  NextOk(lexer, kero::peg::grammar::TokenKind::kEndOfFile, "");
}

TEST(LexerTest, NonTerminalFromTerminal) {
  auto lexer{kero::peg::grammar::Lexer{"A <- \"B\""}};
  NextOk(lexer, kero::peg::grammar::TokenKind::kNonTerminal, "A");
  NextOk(lexer, kero::peg::grammar::TokenKind::kLeftArrow, "<-");
  NextOk(lexer, kero::peg::grammar::TokenKind::kQuotedTerminal, "B");
  NextOk(lexer, kero::peg::grammar::TokenKind::kEndOfFile, "");
}

TEST(LexerTest, NonTerminalExpression2) {
  auto lexer{kero::peg::grammar::Lexer{"A <- B\nC <- D"}};
  NextOk(lexer, kero::peg::grammar::TokenKind::kNonTerminal, "A");
  NextOk(lexer, kero::peg::grammar::TokenKind::kLeftArrow, "<-");
  NextOk(lexer, kero::peg::grammar::TokenKind::kNonTerminal, "B");
  NextOk(lexer, kero::peg::grammar::TokenKind::kNonTerminal, "C");
  NextOk(lexer, kero::peg::grammar::TokenKind::kLeftArrow, "<-");
  NextOk(lexer, kero::peg::grammar::TokenKind::kNonTerminal, "D");
  NextOk(lexer, kero::peg::grammar::TokenKind::kEndOfFile, "");
}

TEST(LexerTest, NonTerminalExpressionAndTerminalExpression) {
  auto lexer{kero::peg::grammar::Lexer{"A <- B\nC <- \"D\""}};
  NextOk(lexer, kero::peg::grammar::TokenKind::kNonTerminal, "A");
  NextOk(lexer, kero::peg::grammar::TokenKind::kLeftArrow, "<-");
  NextOk(lexer, kero::peg::grammar::TokenKind::kNonTerminal, "B");
  NextOk(lexer, kero::peg::grammar::TokenKind::kNonTerminal, "C");
  NextOk(lexer, kero::peg::grammar::TokenKind::kLeftArrow, "<-");
  NextOk(lexer, kero::peg::grammar::TokenKind::kQuotedTerminal, "D");
  NextOk(lexer, kero::peg::grammar::TokenKind::kEndOfFile, "");
}

TEST(LexerTest, LeftParenthesis) {
  auto lexer{kero::peg::grammar::Lexer{"("}};
  NextOk(lexer, kero::peg::grammar::TokenKind::kLeftParenthesis, "(");
  NextOk(lexer, kero::peg::grammar::TokenKind::kEndOfFile, "");
}

TEST(LexerTest, RightParenthesis) {
  auto lexer{kero::peg::grammar::Lexer{")"}};
  NextOk(lexer, kero::peg::grammar::TokenKind::kRightParenthesis, ")");
  NextOk(lexer, kero::peg::grammar::TokenKind::kEndOfFile, "");
}

TEST(LexerTest, Asterisk) {
  auto lexer{kero::peg::grammar::Lexer{"*"}};
  NextOk(lexer, kero::peg::grammar::TokenKind::kAsterisk, "*");
  NextOk(lexer, kero::peg::grammar::TokenKind::kEndOfFile, "");
}

TEST(LexerTest, Plus) {
  auto lexer{kero::peg::grammar::Lexer{"+"}};
  NextOk(lexer, kero::peg::grammar::TokenKind::kPlus, "+");
  NextOk(lexer, kero::peg::grammar::TokenKind::kEndOfFile, "");
}

TEST(LexerTest, QuestionMark) {
  auto lexer{kero::peg::grammar::Lexer{"?"}};
  NextOk(lexer, kero::peg::grammar::TokenKind::kQuestionMark, "?");
  NextOk(lexer, kero::peg::grammar::TokenKind::kEndOfFile, "");
}

TEST(LexerTest, Ampersand) {
  auto lexer{kero::peg::grammar::Lexer{"&"}};
  NextOk(lexer, kero::peg::grammar::TokenKind::kAmpersand, "&");
  NextOk(lexer, kero::peg::grammar::TokenKind::kEndOfFile, "");
}

TEST(LexerTest, ExclamationMark) {
  auto lexer{kero::peg::grammar::Lexer{"!"}};
  NextOk(lexer, kero::peg::grammar::TokenKind::kExclamationMark, "!");
  NextOk(lexer, kero::peg::grammar::TokenKind::kEndOfFile, "");
}

TEST(LexerTest, Slash) {
  auto lexer{kero::peg::grammar::Lexer{"/"}};
  NextOk(lexer, kero::peg::grammar::TokenKind::kSlash, "/");
  NextOk(lexer, kero::peg::grammar::TokenKind::kEndOfFile, "");
}

TEST(LexerTest, Dot) {
  auto lexer{kero::peg::grammar::Lexer{"."}};
  NextOk(lexer, kero::peg::grammar::TokenKind::kDot, ".");
  NextOk(lexer, kero::peg::grammar::TokenKind::kEndOfFile, "");
}

TEST(LexerTest, BracketedTerminal) {
  auto lexer{kero::peg::grammar::Lexer{"[a]"}};
  NextOk(lexer, kero::peg::grammar::TokenKind::kBracketedTerminal, "a");
  NextOk(lexer, kero::peg::grammar::TokenKind::kEndOfFile, "");
}

TEST(LexerTest, BracketedTerminalEmpty) {
  auto lexer{kero::peg::grammar::Lexer{"[]"}};
  NextErr(lexer,
          kero::peg::grammar::TokenizeErrorCode::kBracketedTerminalEmpty);
  NextOk(lexer, kero::peg::grammar::TokenKind::kEndOfFile, "");
}

TEST(LexerTest, BracketedTerminalMinusSignLeftNotFound) {
  auto lexer{kero::peg::grammar::Lexer{"[-a]"}};
  NextErr(lexer, kero::peg::grammar::TokenizeErrorCode::
                     kBracketedTerminalMinusSignLeftNotFound);
  NextOk(lexer, kero::peg::grammar::TokenKind::kEndOfFile, "");
}

TEST(LexerTest, BracketedTerminalMinusSignRightNotFound) {
  auto lexer{kero::peg::grammar::Lexer{"[a-]"}};
  NextErr(lexer, kero::peg::grammar::TokenizeErrorCode::
                     kBracketedTerminalMinusSignRightNotFound);
  NextOk(lexer, kero::peg::grammar::TokenKind::kEndOfFile, "");
}

TEST(LexerTest, BracketedTerminalMinusSignRightNotNumber) {
  auto lexer{kero::peg::grammar::Lexer{"[0-a]"}};
  NextErr(lexer, kero::peg::grammar::TokenizeErrorCode::
                     kBracketedTerminalMinusSignRightNotNumber);
  NextOk(lexer, kero::peg::grammar::TokenKind::kEndOfFile, "");
}

TEST(LexerTest, BracketedTerminalMinusSignRightNumberEqualWithLeft) {
  auto lexer{kero::peg::grammar::Lexer{"[0-0]"}};
  NextErr(lexer, kero::peg::grammar::TokenizeErrorCode::
                     kBracketedTerminalMinusSignRightNumberEqualWithLeft);
  NextOk(lexer, kero::peg::grammar::TokenKind::kEndOfFile, "");
}

TEST(LexerTest, BracketedTerminalMinusSignRightNumberLessThanLeft) {
  auto lexer{kero::peg::grammar::Lexer{"[1-0]"}};
  NextErr(lexer, kero::peg::grammar::TokenizeErrorCode::
                     kBracketedTerminalMinusSignRightNumberLessThanLeft);
  NextOk(lexer, kero::peg::grammar::TokenKind::kEndOfFile, "");
}

TEST(LexerTest, BracketedTerminalMinusSignNumber) {
  auto lexer{kero::peg::grammar::Lexer{"[0-9]"}};
  NextOk(lexer, kero::peg::grammar::TokenKind::kBracketedTerminal, "0-9");
  NextOk(lexer, kero::peg::grammar::TokenKind::kEndOfFile, "");
}

TEST(LexerTest, BracketedTerminalMinusSignRightNotLowercase) {
  auto lexer{kero::peg::grammar::Lexer{"[a-A]"}};
  NextErr(lexer, kero::peg::grammar::TokenizeErrorCode::
                     kBracketedTerminalMinusSignRightNotLowercase);
  NextOk(lexer, kero::peg::grammar::TokenKind::kEndOfFile, "");
}

TEST(LexerTest, BracketedTerminalMinusSignRightLowercaseEqualWithLeft) {
  auto lexer{kero::peg::grammar::Lexer{"[a-a]"}};
  NextErr(lexer, kero::peg::grammar::TokenizeErrorCode::
                     kBracketedTerminalMinusSignRightLowercaseEqualWithLeft);
  NextOk(lexer, kero::peg::grammar::TokenKind::kEndOfFile, "");
}

TEST(LexerTest, BracketedTerminalMinusSignRightLowercaseLessThanLeft) {
  auto lexer{kero::peg::grammar::Lexer{"[b-a]"}};
  NextErr(lexer, kero::peg::grammar::TokenizeErrorCode::
                     kBracketedTerminalMinusSignRightLowercaseLessThanLeft);
  NextOk(lexer, kero::peg::grammar::TokenKind::kEndOfFile, "");
}

TEST(LexerTest, BracketedTerminalMinusSignLowercase) {
  auto lexer{kero::peg::grammar::Lexer{"[a-z]"}};
  NextOk(lexer, kero::peg::grammar::TokenKind::kBracketedTerminal, "a-z");
  NextOk(lexer, kero::peg::grammar::TokenKind::kEndOfFile, "");
}

TEST(LexerTest, BracketedTerminalMinusSignRightNotUppercase) {
  auto lexer{kero::peg::grammar::Lexer{"[A-a]"}};
  NextErr(lexer, kero::peg::grammar::TokenizeErrorCode::
                     kBracketedTerminalMinusSignRightNotUppercase);
  NextOk(lexer, kero::peg::grammar::TokenKind::kEndOfFile, "");
}

TEST(LexerTest, BracketedTerminalMinusSignRightUppercaseEqualWithLeft) {
  auto lexer{kero::peg::grammar::Lexer{"[A-A]"}};
  NextErr(lexer, kero::peg::grammar::TokenizeErrorCode::
                     kBracketedTerminalMinusSignRightUppercaseEqualWithLeft);
  NextOk(lexer, kero::peg::grammar::TokenKind::kEndOfFile, "");
}

TEST(LexerTest, BracketedTerminalMinusSignRightUppercaseLessThanLeft) {
  auto lexer{kero::peg::grammar::Lexer{"[B-A]"}};
  NextErr(lexer, kero::peg::grammar::TokenizeErrorCode::
                     kBracketedTerminalMinusSignRightUppercaseLessThanLeft);
  NextOk(lexer, kero::peg::grammar::TokenKind::kEndOfFile, "");
}

TEST(LexerTest, BracketedTerminalMinusSignUppercase) {
  auto lexer{kero::peg::grammar::Lexer{"[A-Z]"}};
  NextOk(lexer, kero::peg::grammar::TokenKind::kBracketedTerminal, "A-Z");
  NextOk(lexer, kero::peg::grammar::TokenKind::kEndOfFile, "");
}

TEST(LexerTest, BracketedTerminalAlphabet) {
  auto lexer{kero::peg::grammar::Lexer{"[a-zA-Z]"}};
  NextOk(lexer, kero::peg::grammar::TokenKind::kBracketedTerminal, "a-zA-Z");
  NextOk(lexer, kero::peg::grammar::TokenKind::kEndOfFile, "");
}

TEST(LexerTest, BracketedTerminalAlphabetNumber) {
  auto lexer{kero::peg::grammar::Lexer{"[a-zA-Z0-9]"}};
  NextOk(lexer, kero::peg::grammar::TokenKind::kBracketedTerminal, "a-zA-Z0-9");
  NextOk(lexer, kero::peg::grammar::TokenKind::kEndOfFile, "");
}

TEST(LocationTest, Print) {
  auto location{kero::peg::grammar::Location{1, 2, 3}};
  std::ostringstream oss;
  oss << location;
  EXPECT_EQ(oss.str(), "Location{position=1, line=2, column=3}");
}

TEST(TokenTest, Print) {
  auto token{kero::peg::grammar::Token{
      kero::peg::grammar::Location{1, 2, 3},
      kero::peg::grammar::Location{4, 5, 6}, "original", "value",
      kero::peg::grammar::TokenKind::kNonTerminal}};
  std::ostringstream oss;
  oss << token;
  EXPECT_EQ(oss.str(),
            "Token{kind=NonTerminal, value=value, original=original, "
            "original_start=Location{position=1, line=2, column=3}, "
            "original_end=Location{position=4, line=5, column=6}}");
}

TEST(TokenizeErrorTest, Print) {
  auto error{kero::peg::grammar::TokenizeError{
      kero::peg::grammar::TokenizeErrorCode::kMatchFailed,
      kero::peg::grammar::Location{1, 2, 3}}};
  std::ostringstream oss;
  oss << error;
  EXPECT_EQ(oss.str(), "TokenizeError{code=MatchFailed, location=Location{"
                       "position=1, line=2, column=3}}");
}
