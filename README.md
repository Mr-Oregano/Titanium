# Titanium (Programming Language)
**Very early stage**, open-source, imperative programming language packaged and gift wrapped nicely with the *Titan* virtual machine backend (also a work in progress).
This language is still in development so most of it is purely conceptual currently and not very innovative yet. See below for a sample of what the syntax may look like.

## Thought Methodology

This language will be highly experimental and mostly pedagogical for the time being (maybe in the future I might make it useful beyond that in some way). However, I build upon the mistakes made in older languages: confusing semantics (C++), null is assignable to any type (Java, C#, JavaScript, Python), inheritance hell (Java), multithreading hell (Java, C++). My approaches to address these issues are inspired by work done in more recent languages (go, rust) as well as older languages that have strong, static typing and (haskell, OCaml, SML). I am not experimenting with anything new here, I plan on making other languages for such purposes. My *thought methodology* for the purposes of this language is that the problematic features that the aforementioned languages provide (which was not an exhaustive list for the record) give the programmer too much freedom in expressing what they want to do. This freedom, however, comes at the behest of bugs and tech debt rather than code that better expresses what the programmer wanted to do if given better direction. I will update this list as I get a chance to think about later features, but at the moment, the following highlights issues in other languages that I plan for this language to overcome:
  - [Null Hell](https://www.infoq.com/presentations/Null-References-The-Billion-Dollar-Mistake-Tony-Hoare/)
    - This variable (AKA *storage location*) you created might not have a value associated with it under some circumstances. What you *really* want is to encode this in the type *somehow*, and force the type system to make sure the way you access this data is consistent with when that data is actually available.
  - [Implementation Inheritance](https://www.infoworld.com/article/2073649/why-extends-is-evil.html)
    - You really just want to deduplicate code, but there might be *better* ways to do that!
  - [Optional Chaining](https://nwcalvank.dev/javascripts-optional-chaining-is-a-code-smell/)
    - Following from "Nulls whenever and wherever you want"... This *thing* you are accessing may or may not be there but you don't even know what your own code is doing so you'll just put these here and hope for the best!
  - Exceptions (they are often not documented or seldomly documented)
    - "Something went wrong and I'm too lazy to handle it here"
    - This is not a complete criticism of exceptions, they have their uses, but they're not often encoded in the types in any meaningful way and so it often is the case that you end up with programs that can have exceptions happen at any time.
  - Dynamic Typing or even Type Inference:
    - I shouldn't even have to explain this one. Dynamic typing is just plain dumb. I understand having it for small, isolated scripts, but that's the extent to which it should exist.
  - Implicit Conversions
    - These are sometimes not entirely bad, but they can get pretty horrible in some languages (like C++ which ***BY-DEFAULT*** has implicit constructors for any custom datatype).
  - Default Initialization
    - What do you want this *thing* to actually equal?
    - Why should the compiler give it a "default"?
    - What does that even mean? The compiler doesn't *know* what you want to *do* with it.
    - I'll admit, it's convenient for primitives to default to "`0`-values" and most programmers will operate under such an assumption when writing code, for that reason I'll still have this in my language, but *only* for primitives.

Let me be clear, these are not absolutisms (except for dynamic typing and nulls, those have an obvious solution). They're exactly what they are, *conveniences*. But they're exploitable and that's the problem. Project baselines become completely untrustworthy and unreliable when there's too much "voodoo" going on under-the-hood. Those that are aware of AND willing to prevent the pitfalls must walk on eggshells to ensure that they do not shoot themselves in the foot (e.g. using `Optional<T>` types provided by the language [Java, C#], designing around implementation inheritance, ensuring a valid control flow path exists for every circumstance, etc.)

With these considerations, this language will provide:
 - **Strong typing:** This language will be statically-typed and it will make you think about the types you are using (no type-inference unless the types are otherwise visible and clearly obvious from the context).
 - **Non-nullable objects ONLY:** Avoid those stupid null-pointer exceptions by making "absence of values" explicit in the program itself.
   - Use a `?` after a type to make it a `Maybe` type
   - `T` can be accessed from a `Maybe<T>` using `match` or built-in utility functions
   - `T` will **__never__** be accessible at compile time if it is not available in the context
 - **Result types in favour of exceptions:** For now, Functions that can, nevertheless, result in errors will instead wrap return values in a `Result` type, requiring the programmer to make explicit checks on the value.
   - This requires the programmer to explicitly define control flow paths for every circumstance.
 - **Non-void functions are "no-discard" by default**: Compiler will issue diagnostics when return values for non-void functions are implicitly discarded (unless a function is denoted "discardable" in an explicit manner).
 - *More soon...*

