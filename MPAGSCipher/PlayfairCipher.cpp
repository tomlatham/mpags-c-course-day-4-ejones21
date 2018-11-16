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

PlayfairCipher::PlayfairCipher(const std::string& key)
{
  setKey(key);
}

void PlayfairCipher::setKey( const std::string& key ) 
{
  // store the original key
  key_ = key;

  // append the alphabet
  key_ += "ABCDEFGHIJKLMNOPQRSTUVWXYZ";

  // make sure the key is upper case
  std::transform( key_.begin(), key_.end(), key_.begin(), ::toupper);

  // remove non-alpha characters
  auto iter = std::remove_if(key_.begin(), key_.end(), [] (char a) {return ! std::isalpha(a);} );
  key_.erase(iter, key_.end());

  // change J->I
  auto jtoi = [] (char b) {
    if (b=='J'){
      return 'I';
    }
    else {
      return b;
    }
  };
  std::transform( key_.begin(), key_.end(), key_.begin(), jtoi);

  // remove duplicated letters
  std::string encounter{""};
  auto dup = [&] (char c) {
    if (encounter.find(c) == std::string::npos){
      encounter += c;
      return false;
    }
    else {
      return true;
    }
  };
  auto iter2 = std::remove_if(key_.begin(), key_.end(), dup);
  key_.erase(iter2, key_.end());

  // store the coords of each letter
  // store the playfair cipher key map
  for (size_t i {0}; i < key_.size(); ++i){
    PlayfairCoords coords {i/5,i%5};
    coordsToLetter_.insert( std::make_pair(coords, key_[i]) );
    letterToCoords_.insert( std::make_pair(key_[i], coords) );
  }
}

std::string PlayfairCipher::applyCipher( const std::string& inputText, const CipherMode cipherMode ) const
{
  std::string input = inputText;
  std::string output {""};

    // change J->I
    auto jtoi = [] (char b) {
      if (b=='J'){
	return 'I';
      }
      else {
	return b;
      }
    };
    std::transform(input.begin(), input.end(), input.begin(), jtoi);
    
    // if repeated chars in a digraph add an X or Q if XX
    for (size_t i{1}; i<input.size(); i+=2) {
      if (input[i]==input[i-1]){
	if (input[i]=='X'){
	  input.insert(i,"Q");
	}
	else {
	  input.insert(i,"X");
	}
      }
    }
    
    // if the size of input is odd, add a trailing Z (or an X if the input already ends in Z)
    if ( input.size() % 2 == 1) {
      input += (input.back() == 'Z') ? 'X' : 'Z';
    }

    // set whether to shift right/down or left/up depending on the cipher mode
    const size_t shift { (cipherMode == CipherMode::Encrypt) ? 1u : 4u };
    
    // loop over the input in Digraphs
    for (auto initer{input.cbegin()}; initer != input.cend(); initer += 2) {
      
      //   - find the coords in the grid for each digraph
	    auto find1 = letterToCoords_.find(*initer);
	    auto find2 = letterToCoords_.find(*(initer+1));

	    PlayfairCoords newcoords1 {0,0};
	    PlayfairCoords newcoords2 {0,0};
	    
	    //   - apply the rules to these coords to get 'new' coords
	    if ((*find1).second.first == (*find2).second.first) {
	      newcoords1.first = (*find1).second.first;
	      newcoords1.second = ((*find1).second.second + shift) % 5;
	      newcoords2.first = (*find2).second.first;
	      newcoords2.second = ((*find2).second.second + shift) % 5;
	    }
	    else if ((*find1).second.second == (*find2).second.second) {
	      newcoords1.first = ((*find1).second.first + shift) % 5;
	      newcoords1.second = (*find1).second.second;
	      newcoords2.first = ((*find2).second.first + shift) % 5;
	      newcoords2.second = (*find2).second.second;
	    }
	    else {
	      newcoords1.first = (*find1).second.first;
	      newcoords1.second = (*find2).second.second;
	      newcoords2.first = (*find2).second.first;
	      newcoords2.second = (*find1).second.second;
	    }
	    
	    //   - find the letter associated with the new coords
	    auto refind1 = coordsToLetter_.find(newcoords1);
	    auto refind2 = coordsToLetter_.find(newcoords2);
	    output += (*refind1).second;
	    output += (*refind2).second;
	  
      }
  // return the text
  return output;
}
