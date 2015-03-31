// -*- C++ -*-

// geom/orq/Octree.ipp
// A class for an Octree in 3-D.

#if !defined(__geom_orq_Octree_ipp__)
#error This file is an implementation detail of the class Octree.
#endif

BEGIN_NAMESPACE_GEOM

//-----------------------------OctreeBranch-------------------------------


//
// OctreeBranch Constructors and Destructors
//


template<typename _Record,
	 typename _MultiKey,
	 typename _Key,
	 typename _MultiKeyAccessor,
	 typename _RecordOutputIterator>
inline
OctreeBranch<_Record,_MultiKey,_Key,_MultiKeyAccessor,_RecordOutputIterator>::
OctreeBranch(const SemiOpenInterval& domain) :
  _midpoint(domain.getLowerCorner() + 
	    (domain.getUpperCorner() - domain.getLowerCorner()) / 2.0) {
  Base::_domain = domain;
  for (int i = 0; i < 8; ++i) {
    _octant[i] = 0;
  }
}


template<typename _Record,
	 typename _MultiKey,
	 typename _Key,
	 typename _MultiKeyAccessor,
	 typename _RecordOutputIterator>
inline
OctreeBranch<_Record,_MultiKey,_Key,_MultiKeyAccessor,_RecordOutputIterator>::
~OctreeBranch() {
  for (int i = 0; i < 8; ++i) {
    if (_octant[i]) {
      delete _octant[i];
    }
  }
}


//
// Accesors
//


template<typename _Record,
	 typename _MultiKey,
	 typename _Key,
	 typename _MultiKeyAccessor,
	 typename _RecordOutputIterator>
inline
int 
OctreeBranch<_Record,_MultiKey,_Key,_MultiKeyAccessor,_RecordOutputIterator>::
getOctantIndex(Record record) const {
  const MultiKey& mk = getMultiKey(record);
  if (mk[2] < _midpoint[2]) {
    if (mk[1] < _midpoint[1]) {
      if (mk[0] < _midpoint[0])
	return 0;
      else
	return 1;
    }
    else {
      if (mk[0] < _midpoint[0])
	return 2;
      else
	return 3;
    }
  }
  else {
    if (mk[1] < _midpoint[1]) {
      if (mk[0] < _midpoint[0])
	return 4;
      else
	return 5;
    }
    else {
      if (mk[0] < _midpoint[0])
	return 6;
      else
	return 7;
    }
  }
}


template<typename _Record,
	 typename _MultiKey,
	 typename _Key,
	 typename _MultiKeyAccessor,
	 typename _RecordOutputIterator>
inline
typename OctreeBranch<_Record,_MultiKey,_Key,_MultiKeyAccessor,_RecordOutputIterator>::SemiOpenInterval
OctreeBranch<_Record,_MultiKey,_Key,_MultiKeyAccessor,_RecordOutputIterator>::
getOctantDomain(const int index) const {
  Point min, max;

  if (index & 1) {
    min[0] = _midpoint[0];
    max[0] = Base::_domain.getUpperCorner()[0];
  }
  else {
    min[0] = Base::_domain.getLowerCorner()[0];
    max[0] = _midpoint[0];
  }
    
  if (index & 2) {
    min[1] = _midpoint[1];
    max[1] = Base::_domain.getUpperCorner()[1];
  }
  else {
    min[1] = Base::_domain.getLowerCorner()[1];
    max[1] = _midpoint[1];
  }
    
  if (index & 4) {
    min[2] = _midpoint[2];
    max[2] = Base::_domain.getUpperCorner()[2];
  }
  else {
    min[2] = Base::_domain.getLowerCorner()[2];
    max[2] = _midpoint[2];
  }

  return SemiOpenInterval(min, max);
}


//
// Add grid elements.
//


template<typename _Record,
	 typename _MultiKey,
	 typename _Key,
	 typename _MultiKeyAccessor,
	 typename _RecordOutputIterator>
inline
OctreeNode<_Record,_MultiKey,_Key,_MultiKeyAccessor,_RecordOutputIterator>*
OctreeBranch<_Record,_MultiKey,_Key,_MultiKeyAccessor,_RecordOutputIterator>::
insert(Record record, const int leafSize) {
  int index = getOctantIndex(record);
  if (_octant[index] == 0) {
    _octant[index] = new Leaf(getOctantDomain(index));
  }
  _octant[index] = _octant[index]->insert(record, leafSize);
  return this;
}


//
// Mathematical member functions
//


template<typename _Record,
	 typename _MultiKey,
	 typename _Key,
	 typename _MultiKeyAccessor,
	 typename _RecordOutputIterator>
inline
typename OctreeBranch<_Record,_MultiKey,_Key,_MultiKeyAccessor,_RecordOutputIterator>::SizeType
OctreeBranch<_Record,_MultiKey,_Key,_MultiKeyAccessor,_RecordOutputIterator>::
report(RecordOutputIterator iter) const {
  SizeType count = 0;
  for (int i = 0; i < 8; ++i) {
    if (_octant[i]) {
      count += _octant[i]->report(iter);
    }
  }
  return count;
}


template<typename _Record,
	 typename _MultiKey,
	 typename _Key,
	 typename _MultiKeyAccessor,
	 typename _RecordOutputIterator>
inline
typename OctreeBranch<_Record,_MultiKey,_Key,_MultiKeyAccessor,_RecordOutputIterator>::SizeType
OctreeBranch<_Record,_MultiKey,_Key,_MultiKeyAccessor,_RecordOutputIterator>::
computeWindowQuery(RecordOutputIterator iter, const BBox& window) const {
  SizeType count = 0;
  if (doOverlap(window, Base::_domain)) {
    for (int i = 0; i < 8; ++i) {
      if (_octant[i]) {
	count += _octant[i]->computeWindowQuery(iter, window);
      }
    }
  }
  return count;
}


template<typename _Record,
	 typename _MultiKey,
	 typename _Key,
	 typename _MultiKeyAccessor,
	 typename _RecordOutputIterator>
inline
typename OctreeBranch<_Record,_MultiKey,_Key,_MultiKeyAccessor,_RecordOutputIterator>::SizeType
OctreeBranch<_Record,_MultiKey,_Key,_MultiKeyAccessor,_RecordOutputIterator>::
computeWindowQueryCheckDomain(RecordOutputIterator iter, const BBox& window) 
  const {
  SizeType count = 0;
  if (window.isIn(Base::_domain)) {
    for (int i = 0; i < 8; ++i) {
      if (_octant[i]) {
	count += _octant[i]->report(iter);
      }
    }
  }
  else if (doOverlap(window, Base::_domain)) {
    for (int i = 0; i < 8; ++i) {
      if (_octant[i]) {
	count += _octant[i]->computeWindowQueryCheckDomain(iter, window);
      }
    }
  }
  return count;
}


//
// File IO
//


template<typename _Record,
	 typename _MultiKey,
	 typename _Key,
	 typename _MultiKeyAccessor,
	 typename _RecordOutputIterator>
inline
void
OctreeBranch<_Record,_MultiKey,_Key,_MultiKeyAccessor,_RecordOutputIterator>::
put(std::ostream& out) const {
  for (int i = 0; i < 8; ++i) {
    if (_octant[i]) {
      _octant[i]->put(out);
    }
  }
}


template<typename _Record,
	 typename _MultiKey,
	 typename _Key,
	 typename _MultiKeyAccessor,
	 typename _RecordOutputIterator>
inline
void 
OctreeBranch<_Record,_MultiKey,_Key,_MultiKeyAccessor,_RecordOutputIterator>::
print(std::ostream& out, std::string tabbing) const {
  out << tabbing << Base::_domain << '\n';
  std::string newTabbing(tabbing);
  newTabbing.append("  ");
  for (int i = 0; i < 8; ++i) {
    if (_octant[i]) {
      _octant[i]->print(out, newTabbing);
    }
  }
}


//
// Memory usage.
//

    
template<typename _Record,
	 typename _MultiKey,
	 typename _Key,
	 typename _MultiKeyAccessor,
	 typename _RecordOutputIterator>
inline
typename OctreeBranch<_Record,_MultiKey,_Key,_MultiKeyAccessor,_RecordOutputIterator>::SizeType
OctreeBranch<_Record,_MultiKey,_Key,_MultiKeyAccessor,_RecordOutputIterator>::
getMemoryUsage() const {
  int count = sizeof(OctreeBranch);
  for (int i = 0; i < 8; ++i) {
    if (_octant[i]) {
      count += _octant[i]->getMemoryUsage();
    }
  }
  return count;
}


//
// Validity check.
//


template<typename _Record,
	 typename _MultiKey,
	 typename _Key,
	 typename _MultiKeyAccessor,
	 typename _RecordOutputIterator>
inline
bool
OctreeBranch<_Record,_MultiKey,_Key,_MultiKeyAccessor,_RecordOutputIterator>::
isValid() const {
  for (int i = 0; i < 8; ++i) {
    if (_octant[i]) {
      if (! _octant[i]->isValid()) {
	return false;
      }
    }
  }
  return true;
}


//-----------------------------OctreeLeaf-------------------------------


//
// Add grid elements.
//
      

template<typename _Record,
	 typename _MultiKey,
	 typename _Key,
	 typename _MultiKeyAccessor,
	 typename _RecordOutputIterator>
inline
OctreeNode<_Record,_MultiKey,_Key,_MultiKeyAccessor,_RecordOutputIterator>*
OctreeLeaf<_Record,_MultiKey,_Key,_MultiKeyAccessor,_RecordOutputIterator>::
insert(Record record, const int leafSize) {
  // If this leaf is full.
  if (static_cast<int>(_records.size()) == leafSize) {
    // Replace the leaf with a branch.
    Branch* branch = new Branch(Base::_domain);
    for (ConstIterator i = _records.begin(); i != _records.end(); ++i){
      branch->insert(*i, leafSize);
    }
    branch->insert(record, leafSize);
    delete this;
    return branch;
  }
  _records.push_back(record);
  return this;
}


//
// Mathematical member functions
//


template<typename _Record,
	 typename _MultiKey,
	 typename _Key,
	 typename _MultiKeyAccessor,
	 typename _RecordOutputIterator>
inline
typename OctreeLeaf<_Record,_MultiKey,_Key,_MultiKeyAccessor,_RecordOutputIterator>::SizeType
OctreeLeaf<_Record,_MultiKey,_Key,_MultiKeyAccessor,_RecordOutputIterator>::
report(RecordOutputIterator iter) const {
  for (ConstIterator i = _records.begin(); i != _records.end(); ++i) {
    *(iter++) = (*i);
  }
  return _records.size();
}


template<typename _Record,
	 typename _MultiKey,
	 typename _Key,
	 typename _MultiKeyAccessor,
	 typename _RecordOutputIterator>
inline
typename OctreeLeaf<_Record,_MultiKey,_Key,_MultiKeyAccessor,_RecordOutputIterator>::SizeType
OctreeLeaf<_Record,_MultiKey,_Key,_MultiKeyAccessor,_RecordOutputIterator>::
computeWindowQuery(RecordOutputIterator iter, const BBox& window) const {
  // Copy the window into keys for faster access.
  const Key windowXMin = window.getLowerCorner()[0];
  const Key windowYMin = window.getLowerCorner()[1];
  const Key windowZMin = window.getLowerCorner()[2];
  const Key windowXMax = window.getUpperCorner()[0];
  const Key windowYMax = window.getUpperCorner()[1];
  const Key windowZMax = window.getUpperCorner()[2];
  ConstIterator recordsEnd = _records.end(); 
  SizeType count = 0;
  for (ConstIterator i = _records.begin(); i != recordsEnd; ++i) {
    const MultiKey& mk = getMultiKey(*i);
    if (mk[0] >= windowXMin &&
	mk[0] <= windowXMax &&
	mk[1] >= windowYMin &&
	mk[1] <= windowYMax &&
	mk[2] >= windowZMin &&
	mk[2] <= windowZMax) {
      *(iter++) = (*i);
      ++count;
    }
  }
  return count;
}


template<typename _Record,
	 typename _MultiKey,
	 typename _Key,
	 typename _MultiKeyAccessor,
	 typename _RecordOutputIterator>
inline
typename OctreeLeaf<_Record,_MultiKey,_Key,_MultiKeyAccessor,_RecordOutputIterator>::SizeType
OctreeLeaf<_Record,_MultiKey,_Key,_MultiKeyAccessor,_RecordOutputIterator>::
computeWindowQueryCheckDomain(RecordOutputIterator iter, const BBox& window) 
  const {
  SizeType count = 0;
  if (window.isIn(Base::_domain)) {
    for (ConstIterator i = _records.begin(); i != _records.end(); ++i) {
      *(iter++) = (*i);
    }
    count += _records.size();
  }
  else {
    // copy the window into keys for faster access.
    const Key windowXMin = window.getLowerCorner()[0];
    const Key windowYMin = window.getLowerCorner()[1];
    const Key windowZMin = window.getLowerCorner()[2];
    const Key windowXMax = window.getUpperCorner()[0];
    const Key windowYMax = window.getUpperCorner()[1];
    const Key windowZMax = window.getUpperCorner()[2];
    ConstIterator recordsEnd = _records.end(); 
    for (ConstIterator i = _records.begin(); i != recordsEnd; ++i) {
      const MultiKey& mk = getMultiKey(*i);
      if (mk[0] >= windowXMin &&
	  mk[0] <= windowXMax &&
	  mk[1] >= windowYMin &&
	  mk[1] <= windowYMax &&
	  mk[2] >= windowZMin &&
	  mk[2] <= windowZMax) {
	*(iter++) = (*i);
	++count;
      }
    }
  }
  return count;
}


//
// File IO
//


template<typename _Record,
	 typename _MultiKey,
	 typename _Key,
	 typename _MultiKeyAccessor,
	 typename _RecordOutputIterator>
inline
void
OctreeLeaf<_Record,_MultiKey,_Key,_MultiKeyAccessor,_RecordOutputIterator>::
put(std::ostream& out) const {
  for (ConstIterator i = _records.begin(); i != _records.end(); ++i) {
    out << **i << '\n';
  }
}


template<typename _Record,
	 typename _MultiKey,
	 typename _Key,
	 typename _MultiKeyAccessor,
	 typename _RecordOutputIterator>
inline
void 
OctreeLeaf<_Record,_MultiKey,_Key,_MultiKeyAccessor,_RecordOutputIterator>::
print(std::ostream& out, std::string tabbing) const {
  out << tabbing << Base::_domain << '\n';
  for (ConstIterator i = _records.begin(); i != _records.end(); ++i) {
    out << tabbing << **i << '\n';
  }
}


//-----------------------------Octree-----------------------------------


//
// File I/O
//


template<typename _Record,
	 typename _MultiKey,
	 typename _Key,
	 typename _MultiKeyAccessor,
	 typename _RecordOutputIterator>
inline
void
Octree<_Record,_MultiKey,_Key,_MultiKeyAccessor,_RecordOutputIterator>::
put(std::ostream& out) const {
  out << getSize() << " records" 
      << '\n'
      << "domain = " << getDomain()
      << '\n';
  _root->put(out);
}


template<typename _Record,
	 typename _MultiKey,
	 typename _Key,
	 typename _MultiKeyAccessor,
	 typename _RecordOutputIterator>
inline
void
Octree<_Record,_MultiKey,_Key,_MultiKeyAccessor,_RecordOutputIterator>::
print(std::ostream& out) const {
  out << getSize() << " records" << '\n';
  out << "domain = " << getDomain() << '\n';

  std::string empty;
  _root->print(out, empty);
}

END_NAMESPACE_GEOM

// End of file.
