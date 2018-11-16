#ifndef MPAGSCIPHER_PLAYFAIRCIPHER_HPP
#define MPAGSCIPHER_PLAYFAIRCIPHER_HPP

// Standard library includes
#include <string>
#include <vector>
#include <map>

// Our project headers
#include "CipherMode.hpp"

/**
 * \file PlayfairCipher.hpp
 * \brief Contains the declaration of the PlayfairCipher class
 */

/**
 * \class PlayfairCipher
 * \brief Encrypt or decrypt text using the Playfair cipher with the given key
 */
class PlayfairCipher {
public:
  /**                                                                                                                                                                            
   * Create a new PlayfairCipher with the given key
   *                                                                                                                                                                             
   * \param key the key to use in the cipher                                                                                                                                     
   */
  explicit PlayfairCipher( const std::string& key );

  /**
   * Assign the key with the set key function
   *
   * \param key the key given by the user
   */
  void setKey(const std::string& key);


  /**                                                                                                                                                                            
   * Apply the cipher to the provided text                                                                                                                                       
   *                                                                                                                                                                             
   * \param inputText the text to encrypt or decrypt                                                                                                                             
   * \param cipherMode whether to encrypt or decrypt the input text                                                                                                              
   * \return the result of applying the cipher to the input text                                                                                                                 
   */
  std::string applyCipher( const std::string& inputText, const CipherMode cipherMode ) const;

private: 
  /// The cipher key
  std::string key_ ;

  /// Type definition for the coordinates in the 5x5 table
  using PlayfairCoords = std::pair<size_t,size_t>;

  /// Lookup table to go from the coordinates to the character
  std::map<PlayfairCoords, char> coordsToLetter_ ;

  /// Lookup table to go from the character to the coordinates
  std::map<char, PlayfairCoords> letterToCoords_ ;
};

#endif
