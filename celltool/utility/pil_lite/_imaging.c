/*
 * The Python Imaging Library.
 * $Id: _imaging.c 2746 2006-06-18 17:33:46Z fredrik $
 *
 * the imaging library bindings
 *
 * history:
 * 1995-09-24 fl   Created
 * 1996-03-24 fl   Ready for first public release (release 0.0)
 * 1996-03-25 fl   Added fromstring (for Jack's "img" library)
 * 1996-03-28 fl   Added channel operations
 * 1996-03-31 fl   Added point operation
 * 1996-04-08 fl   Added new/new_block/new_array factories
 * 1996-04-13 fl   Added decoders
 * 1996-05-04 fl   Added palette hack
 * 1996-05-12 fl   Compile cleanly as C++
 * 1996-05-19 fl   Added matrix conversions, gradient fills
 * 1996-05-27 fl   Added display_mode
 * 1996-07-22 fl   Added getbbox, offset
 * 1996-07-23 fl   Added sequence semantics
 * 1996-08-13 fl   Added logical operators, point mode
 * 1996-08-16 fl   Modified paste interface
 * 1996-09-06 fl   Added putdata methods, use abstract interface
 * 1996-11-01 fl   Added xbm encoder
 * 1996-11-04 fl   Added experimental path stuff, draw_lines, etc
 * 1996-12-10 fl   Added zip decoder, crc32 interface
 * 1996-12-14 fl   Added modulo arithmetics
 * 1996-12-29 fl   Added zip encoder
 * 1997-01-03 fl   Added fli and msp decoders
 * 1997-01-04 fl   Added experimental sun_rle and tga_rle decoders
 * 1997-01-05 fl   Added gif encoder, getpalette hack
 * 1997-02-23 fl   Added histogram mask
 * 1997-05-12 fl   Minor tweaks to match the IFUNC95 interface
 * 1997-05-21 fl   Added noise generator, spread effect
 * 1997-06-05 fl   Added mandelbrot generator
 * 1997-08-02 fl   Modified putpalette to coerce image mode if necessary
 * 1998-01-11 fl   Added INT32 support
 * 1998-01-22 fl   Fixed draw_points to draw the last point too
 * 1998-06-28 fl   Added getpixel, getink, draw_ink
 * 1998-07-12 fl   Added getextrema
 * 1998-07-17 fl   Added point conversion to arbitrary formats
 * 1998-09-21 fl   Added support for resampling filters
 * 1998-09-22 fl   Added support for quad transform
 * 1998-12-29 fl   Added support for arcs, chords, and pieslices
 * 1999-01-10 fl   Added some experimental arrow graphics stuff
 * 1999-02-06 fl   Added draw_bitmap, font acceleration stuff
 * 2001-04-17 fl   Fixed some egcs compiler nits
 * 2001-09-17 fl   Added screen grab primitives (win32)
 * 2002-03-09 fl   Added stretch primitive
 * 2002-03-10 fl   Fixed filter handling in rotate
 * 2002-06-06 fl   Added I, F, and RGB support to putdata
 * 2002-06-08 fl   Added rankfilter
 * 2002-06-09 fl   Added support for user-defined filter kernels
 * 2002-11-19 fl   Added clipboard grab primitives (win32)
 * 2002-12-11 fl   Added draw context
 * 2003-04-26 fl   Tweaks for Python 2.3 beta 1
 * 2003-05-21 fl   Added createwindow primitive (win32)
 * 2003-09-13 fl   Added thread section hooks
 * 2003-09-15 fl   Added expand helper
 * 2003-09-26 fl   Added experimental LA support
 * 2004-02-21 fl   Handle zero-size images in quantize
 * 2004-06-05 fl   Added ptr attribute (used to access Imaging objects)
 * 2004-06-05 fl   Don't crash when fetching pixels from zero-wide images
 * 2004-09-17 fl   Added getcolors
 * 2004-10-04 fl   Added modefilter
 * 2005-10-02 fl   Added access proxy
 * 2006-06-18 fl   Always draw last point in polyline
 *
 * Copyright (c) 1997-2006 by Secret Labs AB 
 * Copyright (c) 1995-2006 by Fredrik Lundh
 *
 * See the README file for information on usage and redistribution.
 */


#include "Python.h"

#include "Imaging.h"

#define WITH_THREADING /* "friendly" threading support */
#define WITH_MAPPING

/* PIL Plus extensions */
#undef  WITH_CRACKCODE /* pil plus */

#undef	VERBOSE

#define CLIP(x) ((x) <= 0 ? 0 : (x) < 256 ? (x) : 255)

#define B16(p, i) ((((int)p[(i)]) << 8) + p[(i)+1])
#define L16(p, i) ((((int)p[(i)+1]) << 8) + p[(i)])
#define S16(v) ((v) < 32768 ? (v) : ((v) - 65536))

#if PY_VERSION_HEX < 0x01060000
#define PyObject_New PyObject_NEW
#define PyObject_Del PyMem_DEL
#endif

/* -------------------------------------------------------------------- */
/* OBJECT ADMINISTRATION						*/
/* -------------------------------------------------------------------- */

typedef struct {
    PyObject_HEAD
    Imaging image;
} ImagingObject;

staticforward PyTypeObject Imaging_Type;

PyObject* 
PyImagingNew(Imaging imOut)
{
    ImagingObject* imagep;

    if (!imOut)
	return NULL;

    imagep = PyObject_New(ImagingObject, &Imaging_Type);
    if (imagep == NULL) {
	ImagingDelete(imOut);
	return NULL;
    }

#ifdef VERBOSE
    printf("imaging %p allocated\n", imagep);
#endif

    imagep->image = imOut;

    return (PyObject*) imagep;
}

static void
_dealloc(ImagingObject* imagep)
{

#ifdef VERBOSE
    printf("imaging %p deleted\n", imagep);
#endif

    ImagingDelete(imagep->image);
    PyObject_Del(imagep);
}

#define PyImaging_Check(op) ((op)->ob_type == &Imaging_Type)

Imaging PyImaging_AsImaging(PyObject *op)
{
    if (!PyImaging_Check(op)) {
	PyErr_BadInternalCall();
	return NULL;
    }

    return ((ImagingObject *)op)->image;
}


/* -------------------------------------------------------------------- */
/* THREAD HANDLING                                                      */
/* -------------------------------------------------------------------- */

void ImagingSectionEnter(ImagingSectionCookie* cookie)
{
#ifdef WITH_THREADING
    *cookie = (PyThreadState *) PyEval_SaveThread();
#endif
}

void ImagingSectionLeave(ImagingSectionCookie* cookie)
{
#ifdef WITH_THREADING
    PyEval_RestoreThread((PyThreadState*) *cookie);
#endif
}

/* -------------------------------------------------------------------- */
/* EXCEPTION REROUTING                                                  */
/* -------------------------------------------------------------------- */

/* error messages */
static const char* wrong_mode = "unrecognized image mode";
static const char* wrong_raw_mode = "unrecognized raw mode";
static const char* outside_palette = "palette index out of range";
static const char* no_palette = "image has no palette";
/* static const char* no_content = "image has no content"; */

void *
ImagingError_IOError(void)
{
    PyErr_SetString(PyExc_IOError, "error when accessing file");
    return NULL;
}

void *
ImagingError_MemoryError(void)
{
    return PyErr_NoMemory();
}

void *
ImagingError_Mismatch(void)
{
    PyErr_SetString(PyExc_ValueError, "images do not match");
    return NULL;
}

void *
ImagingError_ModeError(void)
{
    PyErr_SetString(PyExc_ValueError, "image has wrong mode");
    return NULL;
}

void *
ImagingError_ValueError(const char *message)
{
    PyErr_SetString(
        PyExc_ValueError,
        (message) ? (char*) message : "unrecognized argument value"
        );
    return NULL;
}


/* -------------------------------------------------------------------- */
/* HELPERS								*/
/* -------------------------------------------------------------------- */

#define TYPE_UINT8 (0x100|sizeof(UINT8))
#define TYPE_INT32 (0x200|sizeof(INT32))
#define TYPE_FLOAT32 (0x300|sizeof(FLOAT32))
#define TYPE_DOUBLE (0x400|sizeof(double))

/* -------------------------------------------------------------------- */
/* FACTORIES								*/
/* -------------------------------------------------------------------- */

static PyObject* 
_new(PyObject* self, PyObject* args)
{
    char* mode;
    int xsize, ysize;

    if (!PyArg_ParseTuple(args, "s(ii)", &mode, &xsize, &ysize))
	return NULL;

    return PyImagingNew(ImagingNew(mode, xsize, ysize));
}

static PyObject* 
_new_array(PyObject* self, PyObject* args)
{
    char* mode;
    int xsize, ysize;

    if (!PyArg_ParseTuple(args, "s(ii)", &mode, &xsize, &ysize))
	return NULL;

    return PyImagingNew(ImagingNewArray(mode, xsize, ysize));
}

static PyObject* 
_new_block(PyObject* self, PyObject* args)
{
    char* mode;
    int xsize, ysize;

    if (!PyArg_ParseTuple(args, "s(ii)", &mode, &xsize, &ysize))
	return NULL;

    return PyImagingNew(ImagingNewBlock(mode, xsize, ysize));
}

static PyObject* 
_getcount(PyObject* self, PyObject* args)
{
    if (!PyArg_ParseTuple(args, ":getcount"))
	return NULL;

    return PyInt_FromLong(ImagingNewCount);
}

/* -------------------------------------------------------------------- */
/* METHODS								*/
/* -------------------------------------------------------------------- */

static PyObject* 
_copy2(ImagingObject* self, PyObject* args)
{
    ImagingObject* imagep1;
    ImagingObject* imagep2;
    if (!PyArg_ParseTuple(args, "O!O!",
			  &Imaging_Type, &imagep1,
			  &Imaging_Type, &imagep2))
	return NULL;

    if (!ImagingCopy2(imagep1->image, imagep2->image))
        return NULL;

    Py_INCREF(Py_None);
    return Py_None;
}

static PyObject* 
_getpalette(ImagingObject* self, PyObject* args)
{
    PyObject* palette;
    int palettesize = 256;
    int bits;
    ImagingShuffler pack;

    char* mode = "RGB";
    char* rawmode = "RGB";
    if (!PyArg_ParseTuple(args, "|ss", &mode, &rawmode))
	return NULL;

    if (!self->image->palette) {
	PyErr_SetString(PyExc_ValueError, no_palette);
	return NULL;
    }

    pack = ImagingFindPacker(mode, rawmode, &bits);
    if (!pack) {
	PyErr_SetString(PyExc_ValueError, wrong_raw_mode);
	return NULL;
    }

    palette = PyString_FromStringAndSize(NULL, palettesize * bits / 8);
    if (!palette)
	return NULL;

    pack((UINT8*) PyString_AsString(palette),
	 self->image->palette->palette, palettesize);

    return palette;
}

static PyObject* 
_putpalette(ImagingObject* self, PyObject* args)
{
    ImagingShuffler unpack;
    int bits;

    char* rawmode;
    UINT8* palette;
    int palettesize;
    if (!PyArg_ParseTuple(args, "ss#", &rawmode, &palette, &palettesize))
	return NULL;

    if (strcmp(self->image->mode, "L") != 0 && strcmp(self->image->mode, "P")) {
	PyErr_SetString(PyExc_ValueError, wrong_mode);
	return NULL;
    }

    unpack = ImagingFindUnpacker("RGB", rawmode, &bits);
    if (!unpack) {
	PyErr_SetString(PyExc_ValueError, wrong_raw_mode);
	return NULL;
    }

    ImagingPaletteDelete(self->image->palette);

    strcpy(self->image->mode, "P");

    self->image->palette = ImagingPaletteNew("RGB");

    unpack(self->image->palette->palette, palette, palettesize * 8 / bits);

    Py_INCREF(Py_None);
    return Py_None;
}

static PyObject* 
_putpalettealpha(ImagingObject* self, PyObject* args)
{
    int index;
    int alpha = 0;
    if (!PyArg_ParseTuple(args, "i|i", &index, &alpha))
	return NULL;

    if (!self->image->palette) {
	PyErr_SetString(PyExc_ValueError, no_palette);
	return NULL;
    }

    if (index < 0 || index >= 256) {
	PyErr_SetString(PyExc_ValueError, outside_palette);
	return NULL;
    }

    strcpy(self->image->palette->mode, "RGBA");
    self->image->palette->palette[index*4+3] = (UINT8) alpha;

    Py_INCREF(Py_None);
    return Py_None;
}


#define IS_RGB(mode)\
    (!strcmp(mode, "RGB") || !strcmp(mode, "RGBA") || !strcmp(mode, "RGBX"))

static PyObject* 
im_setmode(ImagingObject* self, PyObject* args)
{
    /* attempt to modify the mode of an image in place */

    Imaging im;

    char* mode;
    int modelen;
    if (!PyArg_ParseTuple(args, "s#:setmode", &mode, &modelen))
	return NULL;

    im = self->image;

    /* FIXME: move this to a libImaging primitive (?) */
    /* FIXME: add support for 1->L and L->1 */

    if (!strcmp(im->mode, mode)) {
        ; /* same mode; always succeeds */
    } else if (IS_RGB(im->mode) && IS_RGB(mode)) {
        /* color to color */
        strcpy(im->mode, mode);
        im->bands = modelen;
        if (!strcmp(mode, "RGBA"))
            ImagingFillBand(im, 3, 255);
    } else
        return ImagingError_ModeError();

    Py_INCREF(Py_None);
    return Py_None;
}


/* -------------------------------------------------------------------- */

static PyObject* 
_isblock(ImagingObject* self, PyObject* args)
{
    return PyInt_FromLong((long) self->image->block);
}

static PyObject* 
_getband(ImagingObject* self, PyObject* args)
{
    int band;

    if (!PyArg_ParseTuple(args, "i", &band))
	return NULL;

    return PyImagingNew(ImagingGetBand(self->image, band));
}

static PyObject* 
_fillband(ImagingObject* self, PyObject* args)
{
    int band;
    int color;

    if (!PyArg_ParseTuple(args, "ii", &band, &color))
	return NULL;

    if (!ImagingFillBand(self->image, band, color))
        return NULL;
    
    Py_INCREF(Py_None);
    return Py_None;
}

static PyObject* 
_putband(ImagingObject* self, PyObject* args)
{
    ImagingObject* imagep;
    int band;
    if (!PyArg_ParseTuple(args, "O!i",
			  &Imaging_Type, &imagep,
			  &band))
	return NULL;

    if (!ImagingPutBand(self->image, imagep->image, band))
	return NULL;

    Py_INCREF(Py_None);
    return Py_None;
}

/* -------------------------------------------------------------------- */
/* UTILITIES								*/
/* -------------------------------------------------------------------- */

static PyObject* 
_crc32(PyObject* self, PyObject* args)
{
    unsigned char* buffer;
    int bytes;
    int hi, lo;
    UINT32 crc;

    hi = lo = 0;

    if (!PyArg_ParseTuple(args, "s#|(ii)", &buffer, &bytes, &hi, &lo))
	return NULL;

    crc = ((UINT32) (hi & 0xFFFF) << 16) + (lo & 0xFFFF);

    crc = ImagingCRC32(crc, (unsigned char *)buffer, bytes);

    return Py_BuildValue("ii", (crc >> 16) & 0xFFFF, crc & 0xFFFF);
}

static PyObject* 
_getcodecstatus(PyObject* self, PyObject* args)
{
    int status;
    char* msg;

    if (!PyArg_ParseTuple(args, "i", &status))
	return NULL;

    switch (status) {
    case IMAGING_CODEC_OVERRUN:
	msg = "buffer overrun."; break;
    case IMAGING_CODEC_BROKEN:
	msg = "broken data stream."; break;
    case IMAGING_CODEC_UNKNOWN:
	msg = "unrecognized data stream contents."; break;
    case IMAGING_CODEC_CONFIG:
	msg = "codec configuration error."; break;
    case IMAGING_CODEC_MEMORY:
	msg = "out of memory."; break;
    default:
	Py_INCREF(Py_None);
	return Py_None;
    }

    return PyString_FromString(msg);
}


/* -------------------------------------------------------------------- */

/* methods */

static struct PyMethodDef methods[] = {
    {"isblock", (PyCFunction)_isblock, 1},
    
    {"getband", (PyCFunction)_getband, 1},
    {"putband", (PyCFunction)_putband, 1},
    {"fillband", (PyCFunction)_fillband, 1},

    {"setmode", (PyCFunction)im_setmode, 1},
    
    {"getpalette", (PyCFunction)_getpalette, 1},
    {"putpalette", (PyCFunction)_putpalette, 1},
    {"putpalettealpha", (PyCFunction)_putpalettealpha, 1},
    /* Misc. */
    {"new_array", (PyCFunction)_new_array, 1},
    {"new_block", (PyCFunction)_new_block, 1},

    {NULL, NULL} /* sentinel */
};


/* attributes */

static PyObject*  
_getattr(ImagingObject* self, char* name)
{
    PyObject* res;

    res = Py_FindMethod(methods, (PyObject*) self, name);
    if (res)
	return res;
    PyErr_Clear();
    if (strcmp(name, "mode") == 0)
	return PyString_FromString(self->image->mode);
    if (strcmp(name, "size") == 0)
	return Py_BuildValue("ii", self->image->xsize, self->image->ysize);
    if (strcmp(name, "bands") == 0)
	return PyInt_FromLong(self->image->bands);
    if (strcmp(name, "id") == 0)
	return PyInt_FromLong((long) self->image);
    if (strcmp(name, "ptr") == 0)
        return PyCObject_FromVoidPtrAndDesc(self->image, IMAGING_MAGIC, NULL);
    PyErr_SetString(PyExc_AttributeError, name);
    return NULL;
}



/* type description */

statichere PyTypeObject Imaging_Type = {
    PyObject_HEAD_INIT(NULL)
    0,				/*ob_size*/
    "ImagingCore",		/*tp_name*/
    sizeof(ImagingObject),	/*tp_size*/
    0,				/*tp_itemsize*/
    /* methods */
    (destructor)_dealloc,	/*tp_dealloc*/
    0,				/*tp_print*/
    (getattrfunc)_getattr,	/*tp_getattr*/
    0,				/*tp_setattr*/
    0,				/*tp_compare*/
    0,				/*tp_repr*/
    0,                          /*tp_as_number */
    0,                          /*tp_as_sequence */
    0,                          /*tp_as_mapping */
    0                           /*tp_hash*/
};

/* -------------------------------------------------------------------- */

/* FIXME: this is something of a mess.  Should replace this with
   pluggable codecs, but not before PIL 1.1 */

/* Decoders (in decode.c) */
extern PyObject* PyImaging_BitDecoderNew(PyObject* self, PyObject* args);
extern PyObject* PyImaging_FliDecoderNew(PyObject* self, PyObject* args);
extern PyObject* PyImaging_GifDecoderNew(PyObject* self, PyObject* args);
extern PyObject* PyImaging_HexDecoderNew(PyObject* self, PyObject* args);
extern PyObject* PyImaging_JpegDecoderNew(PyObject* self, PyObject* args);
extern PyObject* PyImaging_TiffLzwDecoderNew(PyObject* self, PyObject* args);
extern PyObject* PyImaging_MspDecoderNew(PyObject* self, PyObject* args);
extern PyObject* PyImaging_PackbitsDecoderNew(PyObject* self, PyObject* args);
extern PyObject* PyImaging_PcdDecoderNew(PyObject* self, PyObject* args);
extern PyObject* PyImaging_PcxDecoderNew(PyObject* self, PyObject* args);
extern PyObject* PyImaging_RawDecoderNew(PyObject* self, PyObject* args);
extern PyObject* PyImaging_SunRleDecoderNew(PyObject* self, PyObject* args);
extern PyObject* PyImaging_TgaRleDecoderNew(PyObject* self, PyObject* args);
extern PyObject* PyImaging_XbmDecoderNew(PyObject* self, PyObject* args);
extern PyObject* PyImaging_ZipDecoderNew(PyObject* self, PyObject* args);

/* Encoders (in encode.c) */
extern PyObject* PyImaging_EpsEncoderNew(PyObject* self, PyObject* args);
extern PyObject* PyImaging_GifEncoderNew(PyObject* self, PyObject* args);
extern PyObject* PyImaging_JpegEncoderNew(PyObject* self, PyObject* args);
extern PyObject* PyImaging_PcxEncoderNew(PyObject* self, PyObject* args);
extern PyObject* PyImaging_RawEncoderNew(PyObject* self, PyObject* args);
extern PyObject* PyImaging_XbmEncoderNew(PyObject* self, PyObject* args);
extern PyObject* PyImaging_ZipEncoderNew(PyObject* self, PyObject* args);

extern PyObject* PyImaging_Mapper(PyObject* self, PyObject* args);
extern PyObject* PyImaging_MapBuffer(PyObject* self, PyObject* args);

static PyMethodDef functions[] = {

    /* Object factories */
    {"new", (PyCFunction)_new, 1},

    {"getcount", (PyCFunction)_getcount, 1},

    /* Functions */
    {"copy", (PyCFunction)_copy2, 1},

    /* Codecs */
    {"bit_decoder", (PyCFunction)PyImaging_BitDecoderNew, 1},
    {"eps_encoder", (PyCFunction)PyImaging_EpsEncoderNew, 1},
    {"fli_decoder", (PyCFunction)PyImaging_FliDecoderNew, 1},
    {"gif_decoder", (PyCFunction)PyImaging_GifDecoderNew, 1},
    {"gif_encoder", (PyCFunction)PyImaging_GifEncoderNew, 1},
    {"hex_decoder", (PyCFunction)PyImaging_HexDecoderNew, 1},
    {"hex_encoder", (PyCFunction)PyImaging_EpsEncoderNew, 1}, /* EPS=HEX! */
#ifdef HAVE_LIBJPEG
    {"jpeg_decoder", (PyCFunction)PyImaging_JpegDecoderNew, 1},
    {"jpeg_encoder", (PyCFunction)PyImaging_JpegEncoderNew, 1},
#endif
    {"tiff_lzw_decoder", (PyCFunction)PyImaging_TiffLzwDecoderNew, 1},
    {"msp_decoder", (PyCFunction)PyImaging_MspDecoderNew, 1},
    {"packbits_decoder", (PyCFunction)PyImaging_PackbitsDecoderNew, 1},
    {"pcd_decoder", (PyCFunction)PyImaging_PcdDecoderNew, 1},
    {"pcx_decoder", (PyCFunction)PyImaging_PcxDecoderNew, 1},
    {"pcx_encoder", (PyCFunction)PyImaging_PcxEncoderNew, 1},
    {"raw_decoder", (PyCFunction)PyImaging_RawDecoderNew, 1},
    {"raw_encoder", (PyCFunction)PyImaging_RawEncoderNew, 1},
    {"sun_rle_decoder", (PyCFunction)PyImaging_SunRleDecoderNew, 1},
    {"tga_rle_decoder", (PyCFunction)PyImaging_TgaRleDecoderNew, 1},
    {"xbm_decoder", (PyCFunction)PyImaging_XbmDecoderNew, 1},
    {"xbm_encoder", (PyCFunction)PyImaging_XbmEncoderNew, 1},
#ifdef HAVE_LIBZ
    {"zip_decoder", (PyCFunction)PyImaging_ZipDecoderNew, 1},
    {"zip_encoder", (PyCFunction)PyImaging_ZipEncoderNew, 1},
#endif

    /* Memory mapping */
#ifdef WITH_MAPPING
#ifdef WIN32
    {"map", (PyCFunction)PyImaging_Mapper, 1},
#endif
    {"map_buffer", (PyCFunction)PyImaging_MapBuffer, 1},
#endif

    /* Utilities */
    {"crc32", (PyCFunction)_crc32, 1},
    {"getcodecstatus", (PyCFunction)_getcodecstatus, 1},
    
    {NULL, NULL} /* sentinel */
};

DL_EXPORT(void)
init_imaging(void)
{
    /* Patch object type */
    Imaging_Type.ob_type = &PyType_Type;
    
    Py_InitModule("_imaging", functions);
}
