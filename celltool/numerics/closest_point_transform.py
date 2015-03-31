# Copyright 2007 Zachary Pincus
# This file is part of CellTool.
# 
# CellTool is free software; you can redistribute it and/or modify
# it under the terms of version 2 of the GNU General Public License as
# published by the Free Software Foundation.

import numpy
import _closest_point_transform

def cpt_2d(vertices, arcs = None, max_distance = None, domain = None, samples = None, find_closest_points = False, find_gradient = False):
  """Apply the closest-point transform to a shape defined by a set of vertices
     and arcs. This generates a signed distance map from the geometric data.
     
     Parameters:
       - vertices: shape (n, 2) array containing n unique vertices.
       - arcs: shape (k, 2) array containing (from_vertex, to_vertex) pairs
           of indices into the vertices array. If None, then the vertices are 
           assumed to form a cyclic (closed) polygon.
       - max_distance: maximum distance to calculate; if None then calculate
           all distance values. 
       - domain: (x_min, y_min, x_max, y_max) containing the spatial domain over
           which to compute the CPT. If None, then use the bounding box of the
           vertices.
       - samples: (x-size, y-size) tuple representing number of samples in each
           direction of the domain. This is the size of the output distance map
           array. If None, this is taken as the size of the domain.
       - find_closest_points: if True, calculate the closest points vector field.
       - find_gradient: if True, calculate the gradient vector field.
  """
  vertices, arcs, domain, samples = _prepare_data(vertices, arcs, domain, samples)
  if max_distance is None:
    xmin, ymin, xmax, ymax = domain
    max_distance = numpy.sqrt((xmax - xmin)**2 + (ymax - ymin)**2)
  return _closest_point_transform.cpt_2d(vertices, arcs, domain, max_distance, samples, find_closest_points, find_gradient)
  
def mask_2d(vertices, arcs = None, domain = None, samples = None):
  """Generate a binary mask from a set of vertices and arcs.
     
     Parameters:
       - vertices: shape (n, 2) array containing n unique vertices.
       - arcs: shape (k, 2) array containing (from_vertex, to_vertex) pairs
           of indices into the vertices array. If None, then the vertices are 
           assumed to form a cyclic (closed) polygon.
       - domain: (x_min, y_min, x_max, y_max) containing the spatial domain over
           which to compute the CPT. If None, then use the bounding box of the
           vertices.
       - samples: (x-size, y-size) tuple representing number of samples in each
           direction of the domain. This is the size of the output distance map
           array. If None, this is taken as the size of the domain.
  """
  vertices, arcs, domain, samples = _prepare_data(vertices, arcs, domain, samples)
  return _closest_point_transform.mask_2d(vertices, arcs, domain, samples).astype(bool)
  
def _prepare_data(vertices, arcs, domain, samples):
  vertices = numpy.asarray(vertices, dtype = numpy.double)
  if numpy.allclose(vertices[-1], vertices[0]):
    vertices = vertices[:-1]
  if arcs is None:
    # make simplest polygon from vertices.
    arc_parts = numpy.arange(vertices.shape[0])
    arcs = numpy.array([arc_parts, numpy.roll(arc_parts, -1)], dtype = numpy.int_).transpose()
  if domain is None:
    xmin, ymin = vertices.min(axis = 0)
    xmax, ymax = vertices.max(axis = 0)
    domain = (xmin, ymin, xmax, ymax)
  else:
    xmin, ymin, xmax, ymax = domain
  if samples is None:
    samples = (int(numpy.ceil(xmax - xmin)), int(numpy.ceil(ymax - ymin)))
  return vertices, arcs, domain, samples