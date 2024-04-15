# Titanium (Programming Language)
**Very early stage**, open-source, toy imperative programming language packaged and gift wrapped nicely with the *Titan* virtual machine backend (also a work in progress).
This language is still in development so most of it is purely conceptual currently and not very innovative yet. See below for a sample of what the syntax may look like.

## Features/Philosophies
 - **Strong typing:** This language will be statically-typed and it will make you think about the types you are using (no type-inference unless the types are otherwise visible and clearly obvious from the context).
 - **Non-nullable objects by default:** Avoid those pesky null-pointer exceptions by making "abscenes of values" explicit in the language itself.
   - Use a `?` after a type to make it "optional" or "nullable", this will require a safety check before using the value.
 - **Result types in favour of exceptions:** While exceptions can still be useful, simpler functions that can, nevertheless, result in errors will instead wrap return values in a `Result` type, requiring the programmer to make explicit checks on the value.
 - **Non-void functions are "no-discard" by default**: Compiler will issue diagnostics when return values for non-void functions are implicitly discarded (unless a function is denoted "discardable" in an explicit manner).
 - *More soon...*

## Sample
```c#
class Klazz
{
    private magic: string;
    private favorite_num: int32;

    // Strings are non-nullable! But, initialization can be defered by constructor   
    public ctor(magic: string, favorite_num: int32)
        : magic(magic), favorite_num(favorite_num)
    {}

    public display_info()
    {
        println(f"magic: {magic}, favorite_num: {favorite_num}");
    }
}

fun main()
{
    println("Hello, world!");

    for (i: int32 = 0; i < 10; ++i)
        println(i);

    klazz: Klazz = new("Some string", 69);
    klazz.display_info(); // Outputs "magic: Some string, favorite_num: 69"
}
```
