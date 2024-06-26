#include "./lexer.h"

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
  NextOk(lexer, kero::peg::TokenKind::kEndOfInput, "");
}

TEST(LexerTest, Space) {
  auto lexer{kero::peg::Lexer{" "}};
  NextOk(lexer, kero::peg::TokenKind::kEndOfInput, "");
}

TEST(LexerTest, Space2) {
  auto lexer{kero::peg::Lexer{"  "}};
  NextOk(lexer, kero::peg::TokenKind::kEndOfInput, "");
}

TEST(LexerTest, Tab) {
  auto lexer{kero::peg::Lexer{"\t"}};
  NextOk(lexer, kero::peg::TokenKind::kEndOfInput, "");
}

TEST(LexerTest, Tab2) {
  auto lexer{kero::peg::Lexer{"\t\t"}};
  NextOk(lexer, kero::peg::TokenKind::kEndOfInput, "");
}

TEST(LexerTest, SpaceTab) {
  auto lexer{kero::peg::Lexer{" \t"}};
  NextOk(lexer, kero::peg::TokenKind::kEndOfInput, "");
}

TEST(LexerTest, TabSpace) {
  auto lexer{kero::peg::Lexer{"\t "}};
  NextOk(lexer, kero::peg::TokenKind::kEndOfInput, "");
}

TEST(LexerTest, UnixNewLine) {
  auto lexer{kero::peg::Lexer{"\n"}};
  NextOk(lexer, kero::peg::TokenKind::kNewLine, "\n");
  NextOk(lexer, kero::peg::TokenKind::kEndOfInput, "");
}

TEST(LexerTest, UnixNewLine2) {
  auto lexer{kero::peg::Lexer{"\n\n"}};
  NextOk(lexer, kero::peg::TokenKind::kNewLine, "\n");
  NextOk(lexer, kero::peg::TokenKind::kNewLine, "\n");
  NextOk(lexer, kero::peg::TokenKind::kEndOfInput, "");
}

TEST(LexerTest, WindowsNewLine) {
  auto lexer{kero::peg::Lexer{"\r\n"}};
  NextOk(lexer, kero::peg::TokenKind::kNewLine, "\r\n");
  NextOk(lexer, kero::peg::TokenKind::kEndOfInput, "");
}

TEST(LexerTest, WindowsNewLine2) {
  auto lexer{kero::peg::Lexer{"\r\n\r\n"}};
  NextOk(lexer, kero::peg::TokenKind::kNewLine, "\r\n");
  NextOk(lexer, kero::peg::TokenKind::kNewLine, "\r\n");
  NextOk(lexer, kero::peg::TokenKind::kEndOfInput, "");
}

TEST(LexerTest, NonTerminal) {
  auto lexer{kero::peg::Lexer{"A"}};
  NextOk(lexer, kero::peg::TokenKind::kNonTerminal, "A");
  NextOk(lexer, kero::peg::TokenKind::kEndOfInput, "");
}

TEST(LexerTest, NonTerminalWithSpace) {
  auto lexer{kero::peg::Lexer{"A "}};
  NextOk(lexer, kero::peg::TokenKind::kNonTerminal, "A");
  NextOk(lexer, kero::peg::TokenKind::kEndOfInput, "");
}

TEST(LexerTest, NonTerminalWithTab) {
  auto lexer{kero::peg::Lexer{"A\t"}};
  NextOk(lexer, kero::peg::TokenKind::kNonTerminal, "A");
  NextOk(lexer, kero::peg::TokenKind::kEndOfInput, "");
}

TEST(LexerTest, NonTerminalWithNewLine) {
  auto lexer{kero::peg::Lexer{"A\n"}};
  NextOk(lexer, kero::peg::TokenKind::kNonTerminal, "A");
  NextOk(lexer, kero::peg::TokenKind::kNewLine, "\n");
  NextOk(lexer, kero::peg::TokenKind::kEndOfInput, "");
}

TEST(LexerTest, NonTerminalAfterSpace) {
  auto lexer{kero::peg::Lexer{" A"}};
  NextOk(lexer, kero::peg::TokenKind::kNonTerminal, "A");
  NextOk(lexer, kero::peg::TokenKind::kEndOfInput, "");
}

TEST(LexerTest, NonTerminalAfterTab) {
  auto lexer{kero::peg::Lexer{"\tA"}};
  NextOk(lexer, kero::peg::TokenKind::kNonTerminal, "A");
  NextOk(lexer, kero::peg::TokenKind::kEndOfInput, "");
}

TEST(LexerTest, NonTerminalAfterNewLine) {
  auto lexer{kero::peg::Lexer{"\nA"}};
  NextOk(lexer, kero::peg::TokenKind::kNewLine, "\n");
  NextOk(lexer, kero::peg::TokenKind::kNonTerminal, "A");
  NextOk(lexer, kero::peg::TokenKind::kEndOfInput, "");
}

TEST(LexerTest, LeftArrow) {
  auto lexer{kero::peg::Lexer{"<-"}};
  NextOk(lexer, kero::peg::TokenKind::kLeftArrow, "<-");
  NextOk(lexer, kero::peg::TokenKind::kEndOfInput, "");
}

TEST(LexerTest, Terminal) {
  auto lexer{kero::peg::Lexer{"\"A\""}};
  NextOk(lexer, kero::peg::TokenKind::kQuotedTerminal, "A");
  NextOk(lexer, kero::peg::TokenKind::kEndOfInput, "");
}

TEST(LexerTest, TerminalWithSpace) {
  auto lexer{kero::peg::Lexer{"\"A\" "}};
  NextOk(lexer, kero::peg::TokenKind::kQuotedTerminal, "A");
  NextOk(lexer, kero::peg::TokenKind::kEndOfInput, "");
}

TEST(LexerTest, TerminalWithTab) {
  auto lexer{kero::peg::Lexer{"\"A\"\t"}};
  NextOk(lexer, kero::peg::TokenKind::kQuotedTerminal, "A");
  NextOk(lexer, kero::peg::TokenKind::kEndOfInput, "");
}

TEST(LexerTest, TerminalWithNewLine) {
  auto lexer{kero::peg::Lexer{"\"A\"\n"}};
  NextOk(lexer, kero::peg::TokenKind::kQuotedTerminal, "A");
  NextOk(lexer, kero::peg::TokenKind::kNewLine, "\n");
  NextOk(lexer, kero::peg::TokenKind::kEndOfInput, "");
}

TEST(LexerTest, TerminalAfterSpace) {
  auto lexer{kero::peg::Lexer{" \"A\""}};
  NextOk(lexer, kero::peg::TokenKind::kQuotedTerminal, "A");
  NextOk(lexer, kero::peg::TokenKind::kEndOfInput, "");
}

TEST(LexerTest, TerminalAfterTab) {
  auto lexer{kero::peg::Lexer{"\t\"A\""}};
  NextOk(lexer, kero::peg::TokenKind::kQuotedTerminal, "A");
  NextOk(lexer, kero::peg::TokenKind::kEndOfInput, "");
}

TEST(LexerTest, TerminalAfterNewLine) {
  auto lexer{kero::peg::Lexer{"\n\"A\""}};
  NextOk(lexer, kero::peg::TokenKind::kNewLine, "\n");
  NextOk(lexer, kero::peg::TokenKind::kQuotedTerminal, "A");
  NextOk(lexer, kero::peg::TokenKind::kEndOfInput, "");
}

TEST(LexerTest, TerminalWithEscape) {
  auto lexer{kero::peg::Lexer{"\"\\\"\""}};
  NextOk(lexer, kero::peg::TokenKind::kQuotedTerminal, "\\");
  NextErr(lexer, kero::peg::TokenizeErrorCode::kMatchFailed);
}

TEST(LexerTest, TerminalUsingSingleQuote) {
  auto lexer{kero::peg::Lexer{"'A'"}};
  NextOk(lexer, kero::peg::TokenKind::kQuotedTerminal, "A");
  NextOk(lexer, kero::peg::TokenKind::kEndOfInput, "");
}

TEST(LexerTest, TerminalUsingSingleQuoteWithSpace) {
  auto lexer{kero::peg::Lexer{"'A' "}};
  NextOk(lexer, kero::peg::TokenKind::kQuotedTerminal, "A");
  NextOk(lexer, kero::peg::TokenKind::kEndOfInput, "");
}

TEST(LexerTest, TerminalUsingSingleQuoteWithTab) {
  auto lexer{kero::peg::Lexer{"'A'\t"}};
  NextOk(lexer, kero::peg::TokenKind::kQuotedTerminal, "A");
  NextOk(lexer, kero::peg::TokenKind::kEndOfInput, "");
}

TEST(LexerTest, TerminalUsingSingleQuoteWithNewLine) {
  auto lexer{kero::peg::Lexer{"'A'\n"}};
  NextOk(lexer, kero::peg::TokenKind::kQuotedTerminal, "A");
  NextOk(lexer, kero::peg::TokenKind::kNewLine, "\n");
  NextOk(lexer, kero::peg::TokenKind::kEndOfInput, "");
}

TEST(LexerTest, TerminalUsingSingleQuoteAfterSpace) {
  auto lexer{kero::peg::Lexer{" 'A'"}};
  NextOk(lexer, kero::peg::TokenKind::kQuotedTerminal, "A");
  NextOk(lexer, kero::peg::TokenKind::kEndOfInput, "");
}

TEST(LexerTest, TerminalUsingSingleQuoteAfterTab) {
  auto lexer{kero::peg::Lexer{"\t'A'"}};
  NextOk(lexer, kero::peg::TokenKind::kQuotedTerminal, "A");
  NextOk(lexer, kero::peg::TokenKind::kEndOfInput, "");
}

TEST(LexerTest, TerminalUsingSingleQuoteAfterNewLine) {
  auto lexer{kero::peg::Lexer{"\n'A'"}};
  NextOk(lexer, kero::peg::TokenKind::kNewLine, "\n");
  NextOk(lexer, kero::peg::TokenKind::kQuotedTerminal, "A");
  NextOk(lexer, kero::peg::TokenKind::kEndOfInput, "");
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
  NextOk(lexer, kero::peg::TokenKind::kEndOfInput, "");
}

TEST(LexerTest, NonTerminalFromNonTerminalWithNewLine) {
  auto lexer{kero::peg::Lexer{"A <- B\n"}};
  NextOk(lexer, kero::peg::TokenKind::kNonTerminal, "A");
  NextOk(lexer, kero::peg::TokenKind::kLeftArrow, "<-");
  NextOk(lexer, kero::peg::TokenKind::kNonTerminal, "B");
  NextOk(lexer, kero::peg::TokenKind::kNewLine, "\n");
  NextOk(lexer, kero::peg::TokenKind::kEndOfInput, "");
}

TEST(LexerTest, NonTerminalFromTerminal) {
  auto lexer{kero::peg::Lexer{"A <- \"B\""}};
  NextOk(lexer, kero::peg::TokenKind::kNonTerminal, "A");
  NextOk(lexer, kero::peg::TokenKind::kLeftArrow, "<-");
  NextOk(lexer, kero::peg::TokenKind::kQuotedTerminal, "B");
  NextOk(lexer, kero::peg::TokenKind::kEndOfInput, "");
}

TEST(LexerTest, NonTerminalExpression2) {
  auto lexer{kero::peg::Lexer{"A <- B\nC <- D"}};
  NextOk(lexer, kero::peg::TokenKind::kNonTerminal, "A");
  NextOk(lexer, kero::peg::TokenKind::kLeftArrow, "<-");
  NextOk(lexer, kero::peg::TokenKind::kNonTerminal, "B");
  NextOk(lexer, kero::peg::TokenKind::kNewLine, "\n");
  NextOk(lexer, kero::peg::TokenKind::kNonTerminal, "C");
  NextOk(lexer, kero::peg::TokenKind::kLeftArrow, "<-");
  NextOk(lexer, kero::peg::TokenKind::kNonTerminal, "D");
  NextOk(lexer, kero::peg::TokenKind::kEndOfInput, "");
}

TEST(LexerTest, NonTerminalExpressionAndTerminalExpression) {
  auto lexer{kero::peg::Lexer{"A <- B\nC <- \"D\""}};
  NextOk(lexer, kero::peg::TokenKind::kNonTerminal, "A");
  NextOk(lexer, kero::peg::TokenKind::kLeftArrow, "<-");
  NextOk(lexer, kero::peg::TokenKind::kNonTerminal, "B");
  NextOk(lexer, kero::peg::TokenKind::kNewLine, "\n");
  NextOk(lexer, kero::peg::TokenKind::kNonTerminal, "C");
  NextOk(lexer, kero::peg::TokenKind::kLeftArrow, "<-");
  NextOk(lexer, kero::peg::TokenKind::kQuotedTerminal, "D");
  NextOk(lexer, kero::peg::TokenKind::kEndOfInput, "");
}

TEST(LexerTest, LeftParenthesis) {
  auto lexer{kero::peg::Lexer{"("}};
  NextOk(lexer, kero::peg::TokenKind::kLeftParenthesis, "(");
  NextOk(lexer, kero::peg::TokenKind::kEndOfInput, "");
}

TEST(LexerTest, RightParenthesis) {
  auto lexer{kero::peg::Lexer{")"}};
  NextOk(lexer, kero::peg::TokenKind::kRightParenthesis, ")");
  NextOk(lexer, kero::peg::TokenKind::kEndOfInput, "");
}

TEST(LexerTest, Asterisk) {
  auto lexer{kero::peg::Lexer{"*"}};
  NextOk(lexer, kero::peg::TokenKind::kAsterisk, "*");
  NextOk(lexer, kero::peg::TokenKind::kEndOfInput, "");
}

TEST(LexerTest, Plus) {
  auto lexer{kero::peg::Lexer{"+"}};
  NextOk(lexer, kero::peg::TokenKind::kPlus, "+");
  NextOk(lexer, kero::peg::TokenKind::kEndOfInput, "");
}

TEST(LexerTest, QuestionMark) {
  auto lexer{kero::peg::Lexer{"?"}};
  NextOk(lexer, kero::peg::TokenKind::kQuestionMark, "?");
  NextOk(lexer, kero::peg::TokenKind::kEndOfInput, "");
}

TEST(LexerTest, Ampersand) {
  auto lexer{kero::peg::Lexer{"&"}};
  NextOk(lexer, kero::peg::TokenKind::kAmpersand, "&");
  NextOk(lexer, kero::peg::TokenKind::kEndOfInput, "");
}

TEST(LexerTest, ExclamationMark) {
  auto lexer{kero::peg::Lexer{"!"}};
  NextOk(lexer, kero::peg::TokenKind::kExclamationMark, "!");
  NextOk(lexer, kero::peg::TokenKind::kEndOfInput, "");
}

TEST(LexerTest, Slash) {
  auto lexer{kero::peg::Lexer{"/"}};
  NextOk(lexer, kero::peg::TokenKind::kSlash, "/");
  NextOk(lexer, kero::peg::TokenKind::kEndOfInput, "");
}

TEST(LexerTest, Dot) {
  auto lexer{kero::peg::Lexer{"."}};
  NextOk(lexer, kero::peg::TokenKind::kDot, ".");
  NextOk(lexer, kero::peg::TokenKind::kEndOfInput, "");
}

TEST(LexerTest, BracketedTerminal) {
  auto lexer{kero::peg::Lexer{"[a]"}};
  NextOk(lexer, kero::peg::TokenKind::kBracketedTerminal, "a");
  NextOk(lexer, kero::peg::TokenKind::kEndOfInput, "");
}

TEST(LexerTest, BracketedTerminalEmpty) {
  auto lexer{kero::peg::Lexer{"[]"}};
  NextErr(lexer, kero::peg::TokenizeErrorCode::kBracketedTerminalEmpty);
  NextOk(lexer, kero::peg::TokenKind::kEndOfInput, "");
}

TEST(LexerTest, BracketedTerminalMinusSignLeftNotFound) {
  auto lexer{kero::peg::Lexer{"[-a]"}};
  NextErr(
      lexer,
      kero::peg::TokenizeErrorCode::kBracketedTerminalMinusSignLeftNotFound);
  NextOk(lexer, kero::peg::TokenKind::kEndOfInput, "");
}

TEST(LexerTest, BracketedTerminalMinusSignRightNotFound) {
  auto lexer{kero::peg::Lexer{"[a-]"}};
  NextErr(
      lexer,
      kero::peg::TokenizeErrorCode::kBracketedTerminalMinusSignRightNotFound);
  NextOk(lexer, kero::peg::TokenKind::kEndOfInput, "");
}

TEST(LexerTest, BracketedTerminalMinusSignRightNotNumber) {
  auto lexer{kero::peg::Lexer{"[0-a]"}};
  NextErr(
      lexer,
      kero::peg::TokenizeErrorCode::kBracketedTerminalMinusSignRightNotNumber);
  NextOk(lexer, kero::peg::TokenKind::kEndOfInput, "");
}

TEST(LexerTest, BracketedTerminalMinusSignRightNumberEqualWithLeft) {
  auto lexer{kero::peg::Lexer{"[0-0]"}};
  NextErr(lexer, kero::peg::TokenizeErrorCode::
                     kBracketedTerminalMinusSignRightNumberEqualWithLeft);
  NextOk(lexer, kero::peg::TokenKind::kEndOfInput, "");
}

TEST(LexerTest, BracketedTerminalMinusSignRightNumberLessThanLeft) {
  auto lexer{kero::peg::Lexer{"[1-0]"}};
  NextErr(lexer, kero::peg::TokenizeErrorCode::
                     kBracketedTerminalMinusSignRightNumberLessThanLeft);
  NextOk(lexer, kero::peg::TokenKind::kEndOfInput, "");
}

TEST(LexerTest, BracketedTerminalMinusSignNumber) {
  auto lexer{kero::peg::Lexer{"[0-9]"}};
  NextOk(lexer, kero::peg::TokenKind::kBracketedTerminal, "0-9");
  NextOk(lexer, kero::peg::TokenKind::kEndOfInput, "");
}

TEST(LexerTest, BracketedTerminalMinusSignRightNotLowercase) {
  auto lexer{kero::peg::Lexer{"[a-A]"}};
  NextErr(lexer, kero::peg::TokenizeErrorCode::
                     kBracketedTerminalMinusSignRightNotLowercase);
  NextOk(lexer, kero::peg::TokenKind::kEndOfInput, "");
}

TEST(LexerTest, BracketedTerminalMinusSignRightLowercaseEqualWithLeft) {
  auto lexer{kero::peg::Lexer{"[a-a]"}};
  NextErr(lexer, kero::peg::TokenizeErrorCode::
                     kBracketedTerminalMinusSignRightLowercaseEqualWithLeft);
  NextOk(lexer, kero::peg::TokenKind::kEndOfInput, "");
}

TEST(LexerTest, BracketedTerminalMinusSignRightLowercaseLessThanLeft) {
  auto lexer{kero::peg::Lexer{"[b-a]"}};
  NextErr(lexer, kero::peg::TokenizeErrorCode::
                     kBracketedTerminalMinusSignRightLowercaseLessThanLeft);
  NextOk(lexer, kero::peg::TokenKind::kEndOfInput, "");
}

TEST(LexerTest, BracketedTerminalMinusSignLowercase) {
  auto lexer{kero::peg::Lexer{"[a-z]"}};
  NextOk(lexer, kero::peg::TokenKind::kBracketedTerminal, "a-z");
  NextOk(lexer, kero::peg::TokenKind::kEndOfInput, "");
}

TEST(LexerTest, BracketedTerminalMinusSignRightNotUppercase) {
  auto lexer{kero::peg::Lexer{"[A-a]"}};
  NextErr(lexer, kero::peg::TokenizeErrorCode::
                     kBracketedTerminalMinusSignRightNotUppercase);
  NextOk(lexer, kero::peg::TokenKind::kEndOfInput, "");
}

TEST(LexerTest, BracketedTerminalMinusSignRightUppercaseEqualWithLeft) {
  auto lexer{kero::peg::Lexer{"[A-A]"}};
  NextErr(lexer, kero::peg::TokenizeErrorCode::
                     kBracketedTerminalMinusSignRightUppercaseEqualWithLeft);
  NextOk(lexer, kero::peg::TokenKind::kEndOfInput, "");
}

TEST(LexerTest, BracketedTerminalMinusSignRightUppercaseLessThanLeft) {
  auto lexer{kero::peg::Lexer{"[B-A]"}};
  NextErr(lexer, kero::peg::TokenizeErrorCode::
                     kBracketedTerminalMinusSignRightUppercaseLessThanLeft);
  NextOk(lexer, kero::peg::TokenKind::kEndOfInput, "");
}

TEST(LexerTest, BracketedTerminalMinusSignUppercase) {
  auto lexer{kero::peg::Lexer{"[A-Z]"}};
  NextOk(lexer, kero::peg::TokenKind::kBracketedTerminal, "A-Z");
  NextOk(lexer, kero::peg::TokenKind::kEndOfInput, "");
}

TEST(LexerTest, BracketedTerminalAlphabet) {
  auto lexer{kero::peg::Lexer{"[a-zA-Z]"}};
  NextOk(lexer, kero::peg::TokenKind::kBracketedTerminal, "a-zA-Z");
  NextOk(lexer, kero::peg::TokenKind::kEndOfInput, "");
}

TEST(LexerTest, BracketedTerminalAlphabetNumber) {
  auto lexer{kero::peg::Lexer{"[a-zA-Z0-9]"}};
  NextOk(lexer, kero::peg::TokenKind::kBracketedTerminal, "a-zA-Z0-9");
  NextOk(lexer, kero::peg::TokenKind::kEndOfInput, "");
}

TEST(LocationTest, Print) {
  auto location{kero::peg::Location{1, 2, 3}};
  std::ostringstream oss;
  oss << location;
  EXPECT_EQ(oss.str(), "Location{position=1, line=2, column=3}");
}

TEST(TokenTest, Print) {
  auto token{kero::peg::Token{kero::peg::Location{1, 2, 3},
                              kero::peg::Location{4, 5, 6}, "original", "value",
                              kero::peg::TokenKind::kNonTerminal}};
  std::ostringstream oss;
  oss << token;
  EXPECT_EQ(oss.str(),
            "Token{kind=NonTerminal, value=value, original=original, "
            "original_start=Location{position=1, line=2, column=3}, "
            "original_end=Location{position=4, line=5, column=6}}");
}

TEST(TokenizeErrorTest, Print) {
  auto error{
      kero::peg::TokenizeError{kero::peg::TokenizeErrorCode::kMatchFailed,
                               kero::peg::Location{1, 2, 3}}};
  std::ostringstream oss;
  oss << error;
  EXPECT_EQ(oss.str(), "TokenizeError{code=MatchFailed, location=Location{"
                       "position=1, line=2, column=3}}");
}

TEST(LexerTest, Sample) {
  auto lexer{kero::peg::Lexer{"Expr <- Sum\n"
                              "Sum <- Product (('+' / '-') Product)*\n"
                              "Product <- Power (('*' / '/') Power)*\n"
                              "Power <- Value ('^' Power)?\n"
                              "Value <- [0-9]+ / '(' Expr ')'"}};
  std::stringstream ss;
  while (true) {
    auto next_res = lexer.Next();
    if (next_res.IsErr()) {
      break;
    }
    const auto token = *next_res.Ok();
    ss << token.kind << " " << token.value << std::endl;
    if (token.kind == kero::peg::TokenKind::kEndOfInput) {
      break;
    }
  }
  EXPECT_EQ(ss.str(), "NonTerminal Expr\n"
                      "LeftArrow <-\n"
                      "NonTerminal Sum\n"
                      "NewLine \n\n"
                      "NonTerminal Sum\n"
                      "LeftArrow <-\n"
                      "NonTerminal Product\n"
                      "LeftParenthesis (\n"
                      "LeftParenthesis (\n"
                      "QuotedTerminal +\n"
                      "Slash /\n"
                      "QuotedTerminal -\n"
                      "RightParenthesis )\n"
                      "NonTerminal Product\n"
                      "RightParenthesis )\n"
                      "Asterisk *\n"
                      "NewLine \n\n"
                      "NonTerminal Product\n"
                      "LeftArrow <-\n"
                      "NonTerminal Power\n"
                      "LeftParenthesis (\n"
                      "LeftParenthesis (\n"
                      "QuotedTerminal *\n"
                      "Slash /\n"
                      "QuotedTerminal /\n"
                      "RightParenthesis )\n"
                      "NonTerminal Power\n"
                      "RightParenthesis )\n"
                      "Asterisk *\n"
                      "NewLine \n\n"
                      "NonTerminal Power\n"
                      "LeftArrow <-\n"
                      "NonTerminal Value\n"
                      "LeftParenthesis (\n"
                      "QuotedTerminal ^\n"
                      "NonTerminal Power\n"
                      "RightParenthesis )\n"
                      "QuestionMark ?\n"
                      "NewLine \n\n"
                      "NonTerminal Value\n"
                      "LeftArrow <-\n"
                      "BracketedTerminal 0-9\n"
                      "Plus +\n"
                      "Slash /\n"
                      "QuotedTerminal (\n"
                      "NonTerminal Expr\n"
                      "QuotedTerminal )\n"
                      "EndOfInput \n");
}

TEST(LexerTest, Sample2) {
  auto lexer{kero::peg::Lexer{
      "SingleQuotedTerminal <- 'a'\n"
      "DoubleQuotedTerminal <- \"b\"\n"
      "BracketedTerminal <- [c]\n"
      "NonTerminal <- BracketedTerminal\n"
      "Sequence <- SingleQuotedTerminal DoubleQuotedTerminal NonTerminal\n"
      "OrderedChoice <- SingleQuotedTerminal / DoubleQuotedTerminal / "
      "NonTerminal\n"
      "ZeroOrMore <- Sequence*\n"
      "OneOrMore <- Sequence+\n"
      "Optional <- Sequence?\n"
      "Group <- (Sequence)\n"
      "Group2 <- (SingleQuotedTerminal DoubleQuotedTerminal NonTerminal)\n"
      "Group3 <- (SingleQuotedTerminal / DoubleQuotedTerminal / NonTerminal)\n"
      "AndPredicate <- &Sequence\n"
      "NotPredicate <- !Sequence\n"
      "AndPredicate2 <- SingleQuotedTerminal &DoubleQuotedTerminal\n"
      "NotPredicate2 <- SingleQuotedTerminal !DoubleQuotedTerminal\n"}};
  std::stringstream ss;
  while (true) {
    auto next_res = lexer.Next();
    if (next_res.IsErr()) {
      break;
    }
    const auto token = *next_res.Ok();
    ss << token.kind << " " << token.value << std::endl;
    if (token.kind == kero::peg::TokenKind::kEndOfInput) {
      break;
    }
  }
  EXPECT_EQ(ss.str(), "NonTerminal SingleQuotedTerminal\n"
                      "LeftArrow <-\n"
                      "QuotedTerminal a\n"
                      "NewLine \n\n"
                      "NonTerminal DoubleQuotedTerminal\n"
                      "LeftArrow <-\n"
                      "QuotedTerminal b\n"
                      "NewLine \n\n"
                      "NonTerminal BracketedTerminal\n"
                      "LeftArrow <-\n"
                      "BracketedTerminal c\n"
                      "NewLine \n\n"
                      "NonTerminal NonTerminal\n"
                      "LeftArrow <-\n"
                      "NonTerminal BracketedTerminal\n"
                      "NewLine \n\n"
                      "NonTerminal Sequence\n"
                      "LeftArrow <-\n"
                      "NonTerminal SingleQuotedTerminal\n"
                      "NonTerminal DoubleQuotedTerminal\n"
                      "NonTerminal NonTerminal\n"
                      "NewLine \n\n"
                      "NonTerminal OrderedChoice\n"
                      "LeftArrow <-\n"
                      "NonTerminal SingleQuotedTerminal\n"
                      "Slash /\n"
                      "NonTerminal DoubleQuotedTerminal\n"
                      "Slash /\n"
                      "NonTerminal NonTerminal\n"
                      "NewLine \n\n"
                      "NonTerminal ZeroOrMore\n"
                      "LeftArrow <-\n"
                      "NonTerminal Sequence\n"
                      "Asterisk *\n"
                      "NewLine \n\n"
                      "NonTerminal OneOrMore\n"
                      "LeftArrow <-\n"
                      "NonTerminal Sequence\n"
                      "Plus +\n"
                      "NewLine \n\n"
                      "NonTerminal Optional\n"
                      "LeftArrow <-\n"
                      "NonTerminal Sequence\n"
                      "QuestionMark ?\n"
                      "NewLine \n\n"
                      "NonTerminal Group\n"
                      "LeftArrow <-\n"
                      "LeftParenthesis (\n"
                      "NonTerminal Sequence\n"
                      "RightParenthesis )\n"
                      "NewLine \n\n"
                      "NonTerminal Group2\n"
                      "LeftArrow <-\n"
                      "LeftParenthesis (\n"
                      "NonTerminal SingleQuotedTerminal\n"
                      "NonTerminal DoubleQuotedTerminal\n"
                      "NonTerminal NonTerminal\n"
                      "RightParenthesis )\n"
                      "NewLine \n\n"
                      "NonTerminal Group3\n"
                      "LeftArrow <-\n"
                      "LeftParenthesis (\n"
                      "NonTerminal SingleQuotedTerminal\n"
                      "Slash /\n"
                      "NonTerminal DoubleQuotedTerminal\n"
                      "Slash /\n"
                      "NonTerminal NonTerminal\n"
                      "RightParenthesis )\n"
                      "NewLine \n\n"
                      "NonTerminal AndPredicate\n"
                      "LeftArrow <-\n"
                      "Ampersand &\n"
                      "NonTerminal Sequence\n"
                      "NewLine \n\n"
                      "NonTerminal NotPredicate\n"
                      "LeftArrow <-\n"
                      "ExclamationMark !\n"
                      "NonTerminal Sequence\n"
                      "NewLine \n\n"
                      "NonTerminal AndPredicate2\n"
                      "LeftArrow <-\n"
                      "NonTerminal SingleQuotedTerminal\n"
                      "Ampersand &\n"
                      "NonTerminal DoubleQuotedTerminal\n"
                      "NewLine \n\n"
                      "NonTerminal NotPredicate2\n"
                      "LeftArrow <-\n"
                      "NonTerminal SingleQuotedTerminal\n"
                      "ExclamationMark !\n"
                      "NonTerminal DoubleQuotedTerminal\n"
                      "NewLine \n\n"
                      "EndOfInput \n");
}
