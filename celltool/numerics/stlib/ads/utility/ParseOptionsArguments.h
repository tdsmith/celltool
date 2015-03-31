// -*- C++ -*-

/*! 
  \file ParseOptionsArguments.h
  \brief Class for parsing command line options and arguments.
*/

#if !defined(__ads_utility_ParseOptionsArguments_h__)
#define __ads_utility_ParseOptionsArguments_h__

#include "../defs.h"

#include <set>
#include <map>
#include <list>
#include <string>
#include <sstream>

#include <cassert>

BEGIN_NAMESPACE_ADS

//! Class for parsing command line options and arguments.
class ParseOptionsArguments {
  //
  // Public types.
  //

public:

  //! The string type.
  typedef std::string String;

  //
  // Private types.
  //

private:

  //! The container for options (without values).
  typedef std::set<String> OptionsContainer;
  //! The container for options with values.
  typedef std::map<String,String> OptionsWithValuesContainer;
  //! The container arguments.
  typedef std::list<String> ArgumentsContainer;

  //
  // Member data.
  //

private:

  // The character that prefixes an option.
  char _optionPrefix;
  // The program name.
  String _programName;
  // The options (without values).
  OptionsContainer _options;
  // The options with values.
  OptionsWithValuesContainer _optionsWithValues;
  // The arguments.
  ArgumentsContainer _arguments;

  //
  // Not implemented.
  //

private:

  // Copy constructor not implemented.
  ParseOptionsArguments(const ParseOptionsArguments& other);
  
  // Assignment operator not implemented.
  ParseOptionsArguments&
  operator=(const ParseOptionsArguments& other);
  

public:

  //--------------------------------------------------------------------------
  //! \name Constructors etc.
  //@{

  //! Default constructor.
  ParseOptionsArguments() :
    // Default value.
    _optionPrefix('-'),
    // Empty string.
    _programName(),
    // Empty.
    _options(),
    // Empty.
    _optionsWithValues(),
    // Empty.
    _arguments()
  {}
  
  //! Construct from the command line arguments.
  ParseOptionsArguments(const int argc, char* argv[], 
			const char optionPrefix = '-') :
    // Default value.
    _optionPrefix(optionPrefix),
    // Empty string.
    _programName(),
    // Empty.
    _options(),
    // Empty.
    _optionsWithValues(),
    // Empty.
    _arguments() {
    // Parse the options and arguments.
    parse(argc, argv);
  }
 
  //! Trivial destructor.
  ~ParseOptionsArguments()
  {}

  //@}
  //--------------------------------------------------------------------------
  //! \name Accessors.
  //@{

  //! Get the program name.
  const String&
  getProgramName() const {
    return _programName;
  }

  //! Get the option prefix character.
  char
  getOptionPrefix() const {
    return _optionPrefix;
  }

  //! Get the number of options (with and without values) remaining.
  int
  getNumberOfOptions() const {
    return int(_options.size() + _optionsWithValues.size());
  }

  //! Return true if there are no more options (with and without values) remaining.
  bool
  areOptionsEmpty() const {
    return getNumberOfOptions() == 0;
  }

  //! Get the number of arguments remaining.
  int
  getNumberOfArguments() const {
    return int(_arguments.size());
  }

  //! Return true if there are no more arguments remaining.
  bool
  areArgumentsEmpty() const {
    return _arguments.empty();
  }

  //@}
  //--------------------------------------------------------------------------
  //! \name Manipulators.
  //@{

  //! Parse the options and arguments.
  void
  parse(int argc, char* argv[]);

  //! Set the option prefix character.
  void
  setOptionPrefix(const char optionPrefix) {
    _optionPrefix = optionPrefix;
  }

  //! Return true if the option was given.  Erase the option.
  bool
  getOption(const String& key) {
    OptionsContainer::iterator i = _options.find(key);
    if (i == _options.end()) {
      return false;
    }
    _options.erase(i);
    return true;
  }

  //! Return true if the option was given.  Erase the option.
  /*!
    This is a convenience function for single character option keys.
  */
  bool
  getOption(const char key) {
    return getOption(String(1, key));
  }

  //! Return true if the option was given.  Set the option value and erase the option.
  template<typename T>
  bool
  getOption(const String& key, T* value);

  //! Return true if the option was given.  Set the option value and erase the option.
  /*!
    This is a convenience function for single character option keys.
  */
  template<typename T>
  bool
  getOption(const char key, T* value) {
    return getOption(String(1,key), value);
  }

  //! Return true if the option was given.  Set the option value as a string and erase the option.
  bool
  getOption(const String& key, String* value);

  //! Get the current argument and erase it.  
  /*!
    \pre The number of remaining arguments must not be zero.
  */
  String
  getArgument() {
    assert(! _arguments.empty());
    String argument = _arguments.front();
    _arguments.pop_front();
    return argument;
  }

  //! Get the current argument and erase it.  
  /*!
    \pre The number of remaining arguments must not be zero.
    
    The object of type T must be able to read its' state from a stream 
    with \c operator>>().

    \return true if the argument could be read.
  */
  template<typename T>
  bool
  getArgument(T* x) {
    assert(! _arguments.empty());
    // Make an input string stream from the current argument.
    std::istringstream in(_arguments.front());
    // Pop the current argument.
    _arguments.pop_front();
    // Read from the input string stream.
    in >> *x;
    // Check the state of the stream.
    if (in) {
      return true;
    }
    return false;
  }

  //@}
  //--------------------------------------------------------------------------
  //! \name File I/O.
  //@{

  //! Print any remaining options (with and without values).
  void
  printOptions(std::ostream& out) const;

  //! Print any remaining arguments.
  void
  printArguments(std::ostream& out) const;

  //@}
};

END_NAMESPACE_ADS

#define __ads_utility_ParseOptionsArguments_ipp__
#include "ParseOptionsArguments.ipp"
#undef __ads_utility_ParseOptionsArguments_ipp__

#endif
