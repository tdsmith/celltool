// -*- C++ -*-

#if !defined(__ads_utility_ParseOptionsArguments_ipp__)
#error This file is an implementation detail of ParseOptionsArguments.
#endif

BEGIN_NAMESPACE_ADS


// Return true if the option was given.  Set the option value.
template<typename T>
inline
bool
ParseOptionsArguments::
getOption(const String& key, T* value) {
  String valueString;
  const bool result = getOption(key, &valueString);
  if (result) {
    std::istringstream iss(valueString);
    iss >> *value;
  }
  return result;
}


// Return true if the option was given.  Set the option value as a string.
inline
bool
ParseOptionsArguments::
getOption(const String& key, String* value) {
  OptionsWithValuesContainer::iterator i 
    = _optionsWithValues.find(key);
  // If the option was not specified.
  if (i == _optionsWithValues.end()) {
    return false;
  }
  *value = i->second;
  _optionsWithValues.erase(i);
  return true;
}


inline
void
ParseOptionsArguments::
parse(int argc, char* argv[]) {
  typedef OptionsWithValuesContainer::value_type KeyValuePair;

  // Get the program name.
  assert(argc > 0);
  _programName = *argv;
  --argc;
  ++argv;
    
  // Clear the options and arguments.
  _options.clear();
  _optionsWithValues.clear();
  _arguments = ArgumentsContainer();

  String string;
  // For each string in the argument array.
  for (; argc != 0; --argc, ++argv) {
    string = *argv;
    assert(string.length() != 0);
    // If this is an option.
    if (string[0] == _optionPrefix) {
      // Find the '=' character.
      const String::size_type equalsPosition = string.find('=');
      // If this is an option without a value.
      if (equalsPosition == String::npos) {
	// Insert the option, skip the option prefix.
	_options.insert(String(string, 1));
      }
      // Otherwise, this is an option with a value.
      else {
	const String key(string, 1, equalsPosition - 1);
	assert(key.length() != 0);
	const String value(string, equalsPosition + 1);
	assert(value.length() != 0);
	_optionsWithValues.insert(KeyValuePair(key, value));
      }
    }
    // This is an argument.
    else {
      _arguments.push_back(*argv);
    }
  }
}


// Print any remaining options (with and without values).
inline
void
ParseOptionsArguments::
printOptions(std::ostream& out) const {
  // For each option (without values).
  for (OptionsContainer::const_iterator i = _options.begin(); 
       i != _options.end(); ++i) {
    out << *i << "\n";
  }
  // For each option (with values).
  for (OptionsWithValuesContainer::const_iterator i = 
	 _optionsWithValues.begin(); 
       i != _optionsWithValues.end(); ++i) {
    out << i->first << "=" << i->second << "\n";
  }
}


// Print any remaining arguments.
inline
void
ParseOptionsArguments::
printArguments(std::ostream& out) const {
  // For each argument.
  for (ArgumentsContainer::const_iterator i = _arguments.begin(); 
       i != _arguments.end(); ++i) {
    out << *i << "\n";
  }
}


END_NAMESPACE_ADS
