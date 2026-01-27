# expr-eval

A small C++ expression evaluator with an interactive REPL. Parses and evaluates expressions over numbers, strings, booleans, and variables using a lexer, recursive-descent parser, and tree-walking interpreter.

## Requirements

- CMake ≥ 3.22  
- C++20 compiler (GCC, Clang, or MSVC)

## Build

```bash
mkdir build && cd build
cmake ..
cmake --build .
```

Executable: `build/expr-eval`.

## Usage

Run the REPL:

```bash
./expr-eval
```

Type expressions at the `>>>` prompt. Use `exit` to quit.

**Examples:**

```
>>> 2 + 3 * 4
ans: 14

>>> x + 10
ans: 33.45

>>> f_name + " " + l_name
ans: John Doe

>>> PI * 2
ans: 6.28
```

Predefined variables: `f_name`, `l_name`, `x`, `PI` (see `src/main.cpp` to change or add more).

## Architecture

| Layer | Component | Role |
|-------|-----------|------|
| **Frontend** | `Lexer` | Tokenizes input (numbers, strings, identifiers, `+ - * /`, parens) |
| | `Parser` | Builds an AST from tokens via recursive descent |
| | `ast.h` | AST nodes: `Program`, `BinaryExpr`, literals (`NumberLiteral`, `StringLiteral`, etc.) |
| **Backend** | `RuntimeVar` | Typed runtime value (string, number, bool, nil) with `+ - * /` |
| | `Interpreter` | Wires parser and AST evaluation; holds variable scope |

Evaluation is **left-to-right** for additive operators, **factors before additives** for precedence (e.g. `*` before `+`). The interpreter walks the AST and uses `RuntimeVar` for type coercion and arithmetic.

## Project layout

```
include/expr-eval/
  frontend/   lexer.h, parser.h, ast.h
  backend/    interpreter.h, runtime.h
  picojson.h  (bundled header-only JSON; used for AST dumping)
src/
  frontend/   lexer.cpp, ast.cpp, parser.cpp
  backend/    interpreter.cpp, runtime.cpp
  main.cpp    REPL entrypoint
```

## License

Project code is under your chosen license. `picojson.h` is BSD-licensed (see its header).
