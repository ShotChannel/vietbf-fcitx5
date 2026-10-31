#include "../src/brainfuck.hpp"

#include <cassert>
#include <iostream>
#include <string>

int main() {
    vietbf::Brainfuck bf;
    assert(bf.valid());
    assert(bf.run("c0") == "c");
    assert(bf.run("a2") == "\xC3\xA0"); // à
    assert(bf.run("A1") == "\xE1\xBA\xA5"); // ấ
    assert(bf.run("D0") == "\xC4\x91"); // đ
    assert(bf.run("!0") == "!");
    assert(bf.run("c0h0a2o0") == "ch\xC3\xA0o");
    std::cout << "VietBF Brainfuck VM tests passed\n";
}
