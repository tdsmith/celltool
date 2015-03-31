// Copyright 2007 Zachary Pincus
// This file is part of CellTool.
// 
// CellTool is free software; you can redistribute it and/or modify
// it under the terms of version 2 of the GNU General Public License as
// published by the Free Software Foundation.


#include <Python.h>
#include "numpy/arrayobject.h"
// classes from Sean Mauch's Closest Point Transform
#include "cpt.h"
static int BOOL_TYPE;


static char _closest_point_transform_doc[] = 
"This module calls the closest point transform for a list of 2D points";

static char cpt_2d_doc[] = 
"cpt_2d(vertex_array, arc_array, domain, max_distance, extents, find_closest_points, find_gradient) -> \n\
   (distance_map, closest_points [or None], gradient [or None])\n\
\n\
vertex_array: shape (n, 2) array containing n unique vertices.\n\
arc_array: shape (k, 2) array containing (from_vertex, to_vertex) pairs.\n\
domain: (x_min, y_min, x_max, y_max) containing the spatial domain.\n\
max_distance: maximum distance to calculate.\n\
extents: (x, y) shape of array to store output distance map.\n\
find_closest_points: if True, calculate the closest points vector field.\n\
find_gradient: if True, calculate the gradient vector field.";

static PyObject*
cpt_2d(PyObject *self, PyObject *args)
{
	cpt::State<2, double> state;
	PyObject* vertex_array = NULL;
	PyObject* arc_array = NULL;
	double x_min, y_min, x_max, y_max;
	double max_distance;
  int x_extent, y_extent;
  int find_closest_points, find_gradient;
  
  npy_intp* vertex_dims;
  npy_intp* arc_dims;
  npy_intp extents[2];
  npy_intp vector_extents[3];
  int int_extents[2];
  
  PyObject* distance_map_array = NULL;
  PyObject* closest_points_array = NULL;
  PyObject* gradient_array = NULL;
  
  double domain[4];
  int origin[2] = {0, 0};
  double* vertex_data = NULL;
  int* arc_data = NULL;
  double* dma_data = NULL;
  double* ga_data = NULL;
  double* cpa_data = NULL;
  
  PyObject* return_tuple;
  
	if (!PyArg_ParseTuple(args, "OO(dddd)d(ii)ii:cpt_2d", &vertex_array, &arc_array,
	    &x_min, &y_min, &x_max, &y_max, &max_distance, &x_extent, &y_extent, 
	    &find_closest_points, &find_gradient)) goto fail;

  vector_extents[0] = 2;
  int_extents[0] = extents[0] = vector_extents[1] = x_extent;
  int_extents[1] = extents[1] = vector_extents[2] = y_extent;
  domain[0] = x_min;
  domain[1] = y_min;
  domain[2] = x_max;
  domain[3] = y_max;
  
  vertex_array = PyArray_FromAny(vertex_array, PyArray_DescrFromType(NPY_DOUBLE), 
    2, 2, NPY_CARRAY, NULL);
  if (!vertex_array) goto fail;
  vertex_dims = PyArray_DIMS(vertex_array);
  if (vertex_dims[1] != 2) {
    PyErr_SetString(PyExc_ValueError, "vertex_array must be Nx2-dimensional.");
    goto fail;
  }  
  vertex_data = (double *) PyArray_DATA(vertex_array);
  
  arc_array = PyArray_FromAny(arc_array, PyArray_DescrFromType(NPY_INT), 
    2, 2, NPY_CARRAY, NULL);
  if (!arc_array) goto fail;
  arc_dims = PyArray_DIMS(arc_array);
  if (arc_dims[1] != 2) {
    PyErr_SetString(PyExc_ValueError, "arc_array must be Nx2-dimensional.");
    goto fail;
  }  
  arc_data = (int *) PyArray_DATA(arc_array);

  // the '1' below means 'create fortran-order array'
  distance_map_array = PyArray_EMPTY(2, extents, NPY_DOUBLE, 1);
  if (!distance_map_array) goto fail;
  dma_data = (double *) PyArray_DATA(distance_map_array);
  
  if (find_closest_points) {
    closest_points_array = PyArray_EMPTY(3, vector_extents, NPY_DOUBLE, 1);
    if (!closest_points_array) goto fail;
    cpa_data = (double *) PyArray_DATA(closest_points_array);
  }
  
  if (find_gradient) {
    gradient_array = PyArray_EMPTY(3, vector_extents, NPY_DOUBLE, 1);
    if (!gradient_array) goto fail;
    ga_data = (double *) PyArray_DATA(gradient_array);
  }
      
  // Now compute the CPT
  state.setParameters(domain, max_distance);
  state.setBRepWithNoClipping(vertex_dims[0], vertex_data, arc_dims[0], arc_data);
  state.setLattice(extents, domain);
  
  // The CPT code allows for grids to inhabit sub-regions of the lattice
  // defined above. We want to use the entire lattice, so we pass the first
  // two parameters as the origin and the full extents.
  // That last NULL is for the "closest face" array which is an int* that holds
  // the index of the arc closest to each point. No need for this.
  state.insertGrid(origin, int_extents, dma_data, ga_data, cpa_data, NULL);
  state.computeClosestPointTransform();
  state.clearGrids();
  
  return_tuple = Py_BuildValue("(OOO)", distance_map_array, 
    closest_points_array ? closest_points_array : Py_None, 
    gradient_array ? gradient_array : Py_None);
  if (!return_tuple) goto fail;
    
  // Now clean up the arrays we own and return the return_tuple (which will be
  // null if we're bailing out early.)
  Py_XDECREF(gradient_array);
  Py_XDECREF(closest_points_array);
  Py_DECREF(distance_map_array);
	Py_DECREF(arc_array);
	Py_DECREF(vertex_array);
	return return_tuple;
	
	fail:
  Py_XDECREF(gradient_array);
  Py_XDECREF(closest_points_array);
  Py_XDECREF(distance_map_array);
	Py_XDECREF(arc_array);
	Py_XDECREF(vertex_array);
  return NULL;
}



static char mask_2d_doc[] = 
"mask_2d(vertex_array, arc_array, domain, extents) -> \n\
   boolean_mask\n\
\n\
vertex_array: shape (n, 2) array containing n unique vertices.\n\
arc_array: shape (k, 2) array containing (from_vertex, to_vertex) pairs.\n\
domain: (x_min, y_min, x_max, y_max) containing the spatial domain.\n\
extents: (x, y) shape of array to store output distance map.";

static PyObject*
mask_2d(PyObject *self, PyObject *args)
{
  if (BOOL_TYPE == -1) {
    PyErr_SetString(PyExc_RuntimeError, "Cannot determine the bit-width of the c++ boolean data type on this machine, which is required for mask_2d.");
    return NULL;
  }
	static cpt::State<2, double> state;
	PyObject* vertex_array = NULL;
	PyObject* arc_array = NULL;
	double x_min, y_min, x_max, y_max;
  int x_extent, y_extent;
  
  npy_intp* vertex_dims;
  npy_intp* arc_dims;
  npy_intp extents[2];
  int int_extents[2];
  
  PyObject* mask_array = NULL;
  
  double domain[4];
    
	if (!PyArg_ParseTuple(args, "OO(dddd)(ii):cpt_2d", &vertex_array, &arc_array,
	    &x_min, &y_min, &x_max, &y_max, &x_extent, &y_extent)) goto fail;

  int_extents[0] = extents[0] = x_extent;
  int_extents[1] = extents[1] = y_extent;
  domain[0] = x_min;
  domain[1] = y_min;
  domain[2] = x_max;
  domain[3] = y_max;
  
  vertex_array = PyArray_FromAny(vertex_array, PyArray_DescrFromType(NPY_DOUBLE), 
    2, 2, NPY_CARRAY, NULL);
  if (!vertex_array) goto fail;
  vertex_dims = PyArray_DIMS(vertex_array);
  if (vertex_dims[1] != 2) {
    PyErr_SetString(PyExc_ValueError, "vertex_array must be Nx2-dimensional.");
    goto fail;
  }  
  
  arc_array = PyArray_FromAny(arc_array, PyArray_DescrFromType(NPY_INT), 
    2, 2, NPY_CARRAY, NULL);
  if (!arc_array) goto fail;
  arc_dims = PyArray_DIMS(arc_array);
  if (arc_dims[1] != 2) {
    PyErr_SetString(PyExc_ValueError, "arc_array must be Nx2-dimensional.");
    goto fail;
  }  

  // the '1' below means 'create fortran-order array'
  mask_array = PyArray_EMPTY(2, extents, BOOL_TYPE, 1);
  if (!mask_array) goto fail;
  
  // Now compute the CPT
  state.setBRepWithNoClipping(vertex_dims[0], (double *) PyArray_DATA(vertex_array),
                              arc_dims[0], (int *) PyArray_DATA(arc_array));
  state.determinePointsInside(domain, int_extents, (bool *) PyArray_DATA(mask_array));

  Py_DECREF(arc_array);
  Py_DECREF(vertex_array);
  return mask_array;
  
  // Now clean up the arrays we own and return the return_tuple (which will be
  // null if we're bailing out early.)
	fail:
	Py_XDECREF(arc_array);
	Py_XDECREF(vertex_array);
	Py_XDECREF(mask_array);
  return NULL;
}


static PyMethodDef _closest_point_transform_methods[] = {
	{"cpt_2d", cpt_2d, METH_VARARGS, cpt_2d_doc},
  {"mask_2d", mask_2d, METH_VARARGS, mask_2d_doc},
	{NULL, NULL, 0, NULL}
};

PyMODINIT_FUNC
init_closest_point_transform(void)
{
	Py_InitModule3("_closest_point_transform", _closest_point_transform_methods, _closest_point_transform_doc);
	import_array();
	switch (sizeof(bool)) {
	  case 1:
	    BOOL_TYPE = NPY_UINT8;
	    break;
	  case 2:
      BOOL_TYPE = NPY_UINT16;
	    break;
	  case 4:
      BOOL_TYPE = NPY_UINT32;
	    break;
	  case 8:
      BOOL_TYPE = NPY_UINT64;
	    break;
	  default:
	    BOOL_TYPE = -1;
	}
}
