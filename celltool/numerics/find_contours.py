# Copyright 2007 Zachary Pincus
# This file is part of CellTool.
# 
# CellTool is free software; you can redistribute it and/or modify
# it under the terms of version 2 of the GNU General Public License as
# published by the Free Software Foundation.

import numpy
import _find_contours

from celltool.utility.py23_compat import reversed, sorted, deque


def find_contours(array, level, fully_connected = 'low', positive_orientation = 'low'):
  '''Find iso-valued contours in a 2D array for a given level value.
  
  Uses the "marching squares" method to compute a the iso-valued contours of the
  input 2D array for a particular level value. Array values are linearly 
  interpolated to provide better precision for the output contours.
  
  Inputs: 
  'array' should be convertible to a 2D numpy array object.
  'levels' should be a single value or a list of values along which to find
     the array's contours.
  'fully_connected' must be either 'low' or 'high', and indicates whether 
     array elements below the given level value are to be considered fully-
     connected (and hence elements above the value will only be face connected),
     or vice-versa. (See below for details.) 
  'positive_orientation' must be either 'low' or 'high', and indicates whether
     the output contours will produce positively-oriented polygons around islands
     of low- or high-valued elements. If 'low' then contours will wind counter-
     clockwise around elements below the iso-value. Alternately, this means
     that low-valued elements are always on the left of the contour. (See 
     below for details.)
     
  Output: A list of contours will be produced for each input level specified.
  
  The marching squares algorithm is a special case of the marching cubes 
  algorithm (Lorensen, William and Harvey E. Cline. Marching Cubes: A High 
  Resolution 3D Surface Construction Algorithm. Computer Graphics (SIGGRAPH 87 
  Proceedings) 21(4) July 1987, p. 163-170). A simple explanation is available
  here: http://www.essi.fr/~lingrand/MarchingCubes/algo.html
  
  There is a single ambiguous case in the marching squares algorithm: when a 
  given 2x2-element square has two high-valued and two low-valued elements, each
  pair diagonally adjacent. (Where high- and low-valued is with respect to the
  contour value sought.) In this case, either the high-valued elements can be
  'connected together' via a thin isthmus that separates the low-valued elements,
  or vice-versa. When elements are connected together across a diagonal, they
  are considered 'fully connected' (also known as 'face+vertex-connected' or 
  '8-connected'). Only high-valued or low-valued elements can be fully-connected,
  the other set will be considred as 'face-connected' or '4-connected'.  By
  default, low-valued elements are considered fully-connected.
  
  Output contours are not guaranteed to be closed: contours which intersect the 
  array edge will be left open. All other contours will be closed. (The closed-ness
  of a contours can be tested by checking whether the beginning point is the same as
  the end point.)
  
  Contours are oriented. By default, array values lower than the contour value
  are to the left of the contour and values greater than the contour value are
  to the right. This means that contours will wind counter-clockwise (i.e. in
  'positive orientation') around islands of low-valued pixels.'''
  
  array = numpy.asarray(array)
  if array.ndim != 2:
    raise RuntimeError('Only 2D arrays are supported.')
  level = float(level)
  point_list = _find_contours.iterate_and_store(array, level, fully_connected == 'high')
  contours = _assemble_contours(_take_2(point_list))
  if positive_orientation == 'high':
    for c in contours: c.reverse()
  return contours
    
def _take_2(seq):
    iterator = iter(seq)
    while(True):
      n1 = iterator.next()
      n2 = iterator.next()
      yield (n1, n2)

def _assemble_contours(points_iterator):
  current_index = 0
  contours = {}
  starts = {}
  ends = {}
  for from_point, to_point in points_iterator:
    # Ignore degenerate segments. This happens when (and only when) one vertex
    # of the square is exactly the contour level, and the rest are above or 
    # below. This degnerate vertex will be picked up later by neighboring squares.
    if from_point == to_point: continue
  
    tail_data = starts.get(to_point)
    head_data = ends.get(from_point)
  
    if tail_data is not None and head_data is not None:
      tail, tail_num = tail_data
      head, head_num = head_data
      # We need to connect these two contours. 
      if tail is head:
        # We need to closed a contour. Add the end point, and remove the contour
        # from the 'starts' and 'ends' dicts.
        head.append(to_point)
        del starts[to_point]
        del ends[from_point]
      else: # tail is head
        # We need to join two distinct contours. We want to keep the first contour
        # segment created, so that the final contours are ordered in left-right,
        # top-bottom order.
        if tail_num > head_num:
          # tail was created second. Append tail to head.
          head.extend(tail)
          # remove all traces of tail:
          del starts[to_point]
          del ends[tail[-1]]
          del contours[tail_num]
          # remove the old end of head and add the new end.
          del ends[from_point]
          ends[head[-1]] = (head, head_num)
        else: # tail_num > head_num
          # head was created second. Prepend head to tail.
          tail.extendleft(reversed(head))
          # remove all traces of head:
          del starts[head[0]]
          del ends[from_point]
          del contours[head_num]
          # remove the old start of tail and add the new start.
          del starts[to_point]
          starts[tail[0]] = (tail, tail_num)
    elif tail_data is None and head_data is None:
      # we need to add a new contour
      current_index += 1
      new_num = current_index
      new_contour = deque((from_point, to_point))
      contours[new_num] = new_contour
      starts[from_point] = (new_contour, new_num)
      ends[to_point] = (new_contour, new_num)
    elif tail_data is not None and head_data is None:
      tail, tail_num = tail_data
      # we've found a single contour to which the new segment should be prepended
      tail.appendleft(from_point)
      del starts[to_point]
      starts[from_point] = (tail, tail_num)
    elif tail_data is None and head_data is not None:
      head, head_num = head_data
      # we've found a single contour to which the new segment should be appended
      head.append(to_point)
      del ends[from_point]
      ends[to_point] = (head, head_num)
  # end iteration over from_ and to_ points
  
  return [list(contour) for (num, contour) in sorted(contours.items())]
  