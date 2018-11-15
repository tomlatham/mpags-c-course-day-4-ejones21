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
  explicit PlayfairCipher( const std::string key );

  /**
   * Assign the key with the set key function
   *
   * \param key the key given by the user
   */
  void setkey(const std::string& key);


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
  std::map<std::pair<int, int>, char> letter_ ;
  std::map<char, std::pair<int,int> > letter_coords_ ;
  };

#endif
