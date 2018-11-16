//! Unit Tests for MPAGS Cipher PlayfairCipher Class
#define CATCH_CONFIG_MAIN
#include "catch.hpp"

#include "PlayfairCipher.cpp"

TEST_CASE("Playfair Cipher encryption", "[playfair]") {
  PlayfairCipher pc {"jam"};
  REQUIRE( pc.applyCipher("HELLOWORLD", CipherMode::Encrypt) == "DFNWNPYLWRHV");
}

TEST_CASE("Playfair Cipher decryption", "[playfair]") {
  PlayfairCipher pc {"jam"};
  REQUIRE( pc.applyCipher("DFNWNPYLWRHV", CipherMode::Decrypt) == "HELXLOWORLDZ");
}

TEST_CASE("Playfair Cipher encryption with XX", "[playfair]") { 
  PlayfairCipher pc {"jam"};
  REQUIRE( pc.applyCipher("ABXXD", CipherMode::Encrypt) == "MCVSVF");
}

TEST_CASE("Playfair Cipher encryption with AXX", "[playfair]") {
  PlayfairCipher pc {"jam"};
  REQUIRE( pc.applyCipher("AXXB", CipherMode::Encrypt) == "MWYM");
}

TEST_CASE("Encrypting words ending in Z", "[playfair]") {
  PlayfairCipher pc {"jam"};
  REQUIRE( pc.applyCipher("JAZZ", CipherMode::Encrypt) == "AMVYVY");
}

TEST_CASE("Decrypting words with an odd number of letters", "[playfair]") {
  PlayfairCipher pc {"jam"};
  REQUIRE( pc.applyCipher("ABCDE", CipherMode::Decrypt) == "IMIHHW");
}

