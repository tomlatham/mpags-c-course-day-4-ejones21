// Standard library includes
#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <map>
#include <cctype>
#include <fstream>

// Out project headers
#include "PlayfairCipher.hpp"
#include "CipherMode.hpp"

PlayfairCipher::PlayfairCipher(const std::string key)
  : key_{key}
{
  if ( ! key.empty() ) {
    setkey(key);
  }
}

void PlayfairCipher::setkey ( const std::string& key ) 
{
  // store the original key
  key_ = key;

  // append the alphabet
  key_ += "ABCDEFGHIJKLMNOPQRSTUVWXYZ";

  // make sure the key is upper case
  std::transform( key_.begin(), key_.end(), key_.begin(), ::toupper);

  // remove non-alpha characters
  auto iter = std::remove_if(key_.begin(), key_.end(), [] (char a) {return ! std::isalpha(a);} );
  key_.erase (iter, key_.end());

  // change J->I
  auto transform = [] (char b) {
    if (b=='J'){
      return 'I';
    }
    else {
      return b;
    }
  };
  std::transform( key_.begin(), key_.end(), key_.begin(), transform);

  // remove duplicated letters
  std::string encounter{""};
  auto dup = [&] (char c) {
    size_t found = encounter.find(c);
    if (found==std::string::npos){
      encounter += c;
      return false;
    }
    else {
      return true;
    }
  };
  auto iter2 = std::remove_if(key_.begin(), key_.end(), dup);
  key_.erase (iter2, key_.end());

  // store the coords of each letter
  // store the playfair cipher key map
  for (size_t i {0}; i < key_.size(); ++i){
    std::pair<int,int> coords {i/5,i%5};
    std::pair<std::pair<int,int>, char> p0 {coords, key_[i]};
    std::pair<char, std::pair<int,int> > p1 {key_[i], coords};
    letter_.insert(p0);
    letter_coords_.insert(p1);
  }
}

std::string PlayfairCipher::applyCipher( const std::string& inputText, const CipherMode cipherMode ) const
{
  std::string input = inputText;
  std::string output {""};

  // - check if the mode is encrypting AND the input word ends with a Z. If both are true, output a message saying the cipher cannot work with words ending in Z
  if (cipherMode == CipherMode::Encrypt && input.back() == 'Z') {
    std::cout << "Cipher cannot encrypt words which end in a Z" << std::endl;
    std::cout << "Returning the user input" << std::endl;
    output = input;
  }
  
  // - check if the mode is decrypting AND the input word has an odd number of characters. If both are true, output a message saying the cipher cannot decrypt a word with an odd number of letters by definition. 
  else if (cipherMode == CipherMode::Decrypt && input.size() % 2 == 1) {
    std::cout << "Cipher cannot decrypt words with an odd number of letters" << std::endl;
    std::cout<< "Returning the user input" << std::endl;
    output = input;
  }

  else { 
    // change J->I
    auto transform = [] (char b) {
      if (b=='J'){
	return 'I';
      }
      else {
	return b;
      }
    };
    std::transform(input.begin(), input.end(), input.begin(), transform);
    
    // if repeated chars in a digraph add an X or Q if XX
    for (size_t i{1}; i<input.size(); ++i) {
      if (input[i]==input[i-1]){
	if (input[i]=='X'){
	  input.insert(i,"Q");
	}
	else {
	  input.insert(i,"X");
	}
      }
    }
    
    // if the size of input is odd, add a trailing Z
    if ( input.size() % 2 == 1) {
      input += 'Z';
    }
    
    // loop over the input in Digraphs
    for (auto initer{input.cbegin()}; initer != input.cend(); initer += 2) {
      
      //   - find the coords in the grid for each digraph
      switch (cipherMode) {
        case CipherMode::Encrypt : 
	  {
	    auto find = letter_coords_.find(*initer);
	    auto find2 = letter_coords_.find(*(initer+1));
	    std::pair<int,int> newcoords1 {0,0};
	    std::pair<int,int> newcoords2 {0,0};
	    
	    //   - apply the rules to these coords to get 'new' coords
	    if ((*find).second.first == (*find2).second.first) {
	      newcoords1.first = (*find).second.first;
	      newcoords1.second = ((*find).second.second + 1) % 5;
	      newcoords2.first = (*find).second.first;
	      newcoords2.second = ((*find2).second.second + 1) % 5;
	    }
	    else if ((*find).second.second == (*find2).second.second) {
	      newcoords1.first = ((*find).second.first + 1) % 5;
	      newcoords1.second = (*find).second.second;
	      newcoords2.first = ((*find2).second.first + 1) % 5;
	      newcoords2.second = (*find).second.second;
	    }
	    else {
	      newcoords1.first = (*find).second.first;
	      newcoords1.second = (*find2).second.second;
	      newcoords2.first = (*find2).second.first;
	      newcoords2.second = (*find).second.second;
	    }
	    
	    //   - find the letter associated with the new coords
	    auto refind = letter_.find(newcoords1);
	    auto refind2 = letter_.find(newcoords2);
	    output += (*refind).second;
	    output += (*refind2).second;
	    break;
	  }
	  
      case CipherMode::Decrypt :
	{
	  // check if there are double letters in the input, and if there are, output a message saying that the word to be decrypted should not contain double letters by definition
	  
	  // check if the size of input is odd, and if it is, output a message saying that the word to be decrypted should have an even number of characters by definition
	  /* if ( input.size() % 2 == 1) {
	     std::cout << "The word to be decrypted should have an even number of letters" << std::endl;
	     output = input;
	     }
	     
	     else { */
	  
	  //   - find the coords in the grid for each digraph
	  auto find = letter_coords_.find(*initer);
	  auto find2 = letter_coords_.find(*(initer+1));
	  std::pair<int,int> newcoords1 {0,0};
	  std::pair<int,int> newcoords2 {0,0};
	  
	  //   - apply the rules to these coords to get 'new' coords
	  if ((*find).second.first == (*find2).second.first) {
	    newcoords1.first = (*find).second.first;
	    newcoords1.second = ((*find).second.second + 4) % 5;
	    newcoords2.first = (*find).second.first;
	    newcoords2.second = ((*find2).second.second + 4) % 5;
	  }
	  else if ((*find).second.second == (*find2).second.second) {
	    newcoords1.first = ((*find).second.first + 4) % 5;
	    newcoords1.second = (*find).second.second;
	    newcoords2.first = ((*find2).second.first + 4) % 5;
	    newcoords2.second = (*find).second.second;
	  }
	  else {
	    newcoords1.first = (*find).second.first;
	    newcoords1.second = (*find2).second.second;
	    newcoords2.first = (*find2).second.first;
	    newcoords2.second = (*find).second.second;
	  }
	  
	  //   - find the letter associated with the new coords
	  auto refind = letter_.find(newcoords1);
	  auto refind2 = letter_.find(newcoords2);
	  output += (*refind).second;
	  output += (*refind2).second;
	  break;
	}
      }
    }  
  }
  // return the text

  return output;
}
    
  
