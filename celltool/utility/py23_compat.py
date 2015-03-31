try:
  set = set
except:
  import sets
  set = sets.Set

try:
  reversed = reversed
except:
  def reversed(iterable):
    l = list(iterable)
    l.reverse()
    return l

try:
  sorted = sorted
except:
  def sorted(iterable):
    l = list(iterable)
    l.sort()
    return l


try:
    #Python 2.4 has a deque
    from collections import deque
except:
    #Python 2.3 and earlier don't
    #
    # deque from Raymond Hettinger's recipe:
    #http://aspn.activestate.com/ASPN/Cookbook/Python/Recipe/259179
	class deque(object):

		def __init__(self, iterable=()):
			if not hasattr(self, 'data'):
				self.left = self.right = 0
				self.data = {}
			self.extend(iterable)         
				
		def append(self, x):
			self.data[self.right] = x
			self.right += 1
			
		def appendleft(self, x):
			self.left -= 1        
			self.data[self.left] = x
			
		def pop(self):
			if self.left == self.right:
				raise IndexError('cannot pop from empty deque')
			self.right -= 1
			elem = self.data[self.right]
			del self.data[self.right]         
			return elem
		
		def popleft(self):
			if self.left == self.right:
				raise IndexError('cannot pop from empty deque')
			elem = self.data[self.left]
			del self.data[self.left]
			self.left += 1
			return elem

		def clear(self):
			self.data.clear()
			self.left = self.right = 0

		def extend(self, iterable):
			for elem in iterable:
				self.append(elem)

		def extendleft(self, iterable):
			for elem in iterable:
				self.appendleft(elem)

		def rotate(self, n=1):
			if self:
				n %= len(self)
				for i in xrange(n):
					self.appendleft(self.pop())

		def __getitem__(self, i):
			if i < 0:
				i += len(self)
			try:
				return self.data[i + self.left]
			except KeyError:
				raise IndexError

		def __setitem__(self, i, value):
			if i < 0:
				i += len(self)        
			try:
				self.data[i + self.left] = value
			except KeyError:
				raise IndexError

		def __delitem__(self, i):
			size = len(self)
			if not (-size <= i < size):
				raise IndexError
			data = self.data
			if i < 0:
				i += size
			for j in xrange(self.left+i, self.right-1):
				data[j] = data[j+1]
			self.pop()
		
		def __len__(self):
			return self.right - self.left

		def __cmp__(self, other):
			if type(self) != type(other):
				return cmp(type(self), type(other))
			return cmp(list(self), list(other))
				
		def __repr__(self, _track=[]):
			if id(self) in _track:
				return '...'
			_track.append(id(self))
			r = 'deque(%r)' % (list(self),)
			_track.remove(id(self))
			return r
		
		def __getstate__(self):
			return (tuple(self),)
		
		def __setstate__(self, s):
			self.__init__(s[0])
			
		def __hash__(self):
			raise TypeError
		
		def __copy__(self):
			return self.__class__(self)
		
		def __deepcopy__(self, memo={}):
			from copy import deepcopy
			result = self.__class__()
			memo[id(self)] = result
			result.__init__(deepcopy(tuple(self), memo))
			return result
