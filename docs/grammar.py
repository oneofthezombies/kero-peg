# PEG for parsing PEG

"""
EndOfInput <- !.
NewLine <- "\n" / "\r" / "\r\n"
EndOfLine <- NewLine / EndOfInput
Comment <- "#" (!EndOfLine .)* EndOfLine
Literal <- "'" (!"'" .)* "'" / "\"" (!"\"" .)* "\""
Bracket <- "[" (!"]" .)* "]"
AnyChar <- "."
Name <- [a-zA-Z] [a-zA-Z0-9_]*

RuleSet <- Comment* (Rule Comment*)* EndOfInput
Rule <- Name "<-" Expression (Comment / EndOfLine)
Expression <- Sequence ("/" Sequence)*
Sequence <- Prefix*
Prefix <- ("&" / "!")? Suffix
Suffix <- Primary ("*" / "+" / "?")?
Primary <- Group / Name / Literal / Bracket / AnyChar
Group <- "(" Expression ")"
"""
