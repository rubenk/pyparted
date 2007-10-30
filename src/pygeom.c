/*
 * pygeom.c
 *
 * Copyright (C) 2007  Red Hat, Inc.
 * All rights reserved.
 *
 * This copyrighted material is made available to anyone wishing to use,
 * modify, copy, or redistribute it subject to the terms and conditions of
 * the GNU General Public License v.2, or (at your option) any later version.
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY expressed or implied, including the implied warranties of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General
 * Public License for more details.  You should have received a copy of the
 * GNU General Public License along with this program; if not, write to the
 * Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA
 * 02110-1301, USA.  Any Red Hat trademarks that are incorporated in the
 * source code or documentation are not subject to the GNU General Public
 * License and may only be used or replicated with the express permission of
 * Red Hat, Inc.
 *
 * Red Hat Author(s): David Cantrell <dcantrell@redhat.com>
 *                    Chris Lumens <clumens@redhat.com>
 */

#include <Python.h>

#include "convert.h"
#include "pygeom.h"
#include "pynatmath.h"

/* _ped.Geometry functions */
void _ped_Geometry_dealloc(_ped_Geometry *self) {
    PyObject_Del(self);
}

PyObject *_ped_Geometry_new(PyTypeObject *type, PyObject *args,
                            PyObject *kwds) {
    _ped_Geometry *self;

    self = PyObject_New(_ped_Geometry, &_ped_Geometry_Type_obj);
    return (PyObject *) self;
}

int _ped_Geometry_init(_ped_Geometry *self, PyObject *args, PyObject *kwds) {
    /* XXX */
    return 0;
}

/* 1:1 function mappings for geom.h in libparted */
PyObject *py_ped_geometry_init(PyObject *s, PyObject *args) {
    int ret = -1;
    PyObject *in_geometry, *in_device, *in_start, *in_length;
    PedGeometry *out_geometry;
    PedDevice *out_device;
    PedSector out_start, out_length;

    if (!PyArg_ParseTuple(args, "OOOO", &in_geometry, &in_device,
                                        &in_start, &in_length)) {
        return NULL;
    }

    out_geometry = _ped_Geometry2PedGeometry(in_geometry);
    if (out_geometry == NULL) {
        return NULL;
    }

    out_device = _ped_Device2PedDevice(in_device);
    if (out_device == NULL) {
        return NULL;
    }

    out_start = _ped_Sector2PedSector(in_start);
    if (out_start == -1) {
        return NULL;
    }

    out_length = _ped_Sector2PedSector(in_length);
    if (out_length == -1) {
        return NULL;
    }

    ret = ped_geometry_init(out_geometry, out_device, out_start, out_length);
    ped_geometry_destroy(out_geometry);
    ped_device_destroy(out_device);
    return Py_BuildValue("i", ret);
}

PyObject *py_ped_geometry_new(PyObject *s, PyObject *args) {
    PyObject *in_device, *in_start, *in_length;
    PedGeometry *geom;
    PedDevice *out_device;
    PedSector out_start, out_length;
    _ped_Geometry *ret;

    if (!PyArg_ParseTuple(args, "OOO", &in_device, &in_start, &in_length)) {
        return NULL;
    }

    out_device = _ped_Device2PedDevice(in_device);
    if (out_device == NULL) {
        return NULL;
    }

    out_start = _ped_Sector2PedSector(in_start);
    if (out_start == -1) {
        return NULL;
    }

    out_length = _ped_Sector2PedSector(in_length);
    if (out_length == -1) {
        return NULL;
    }

    geom = ped_geometry_new(out_device, out_start, out_length);
    if (geom) {
        ret = PedGeometry2_ped_Geometry(geom);
    }

    ped_geometry_destroy(geom);
    ped_device_destroy(out_device);

    if (ret) {
        return (PyObject *) ret;
    } else {
        return NULL;
    }
}

PyObject *py_ped_geometry_duplicate(PyObject *s, PyObject *args) {
    PyObject *in_geometry;
    PedGeometry *out_geometry, *geometry, *geom;
    _ped_Geometry *ret;

    if (!PyArg_ParseTuple(args, "O", &in_geometry)) {
        return NULL;
    }

    out_geometry = _ped_Geometry2PedGeometry(in_geometry);
    if (out_geometry == NULL) {
        return NULL;
    }

    geom = ped_geometry_duplicate(out_geometry);
    if (geom) {
        ret = PedGeometry2_ped_Geometry(geom);
    }

    ped_geometry_destroy(out_geometry);
    ped_geometry_destroy(geom);

    if (ret) {
        return (PyObject *) ret;
    } else {
        return NULL;
    }
}

PyObject *py_ped_geometry_intersect(PyObject *s, PyObject *args) {
    PyObject *in_a, *in_b;
    PedGeometry *out_a, *out_b, *geom;
    _ped_Geometry *ret;

    if (!PyArg_ParseTuple(args, "OO", &in_a, &in_b)) {
        return NULL;
    }

    out_a = _ped_Geometry2PedGeometry(in_a);
    if (out_a == NULL) {
        return NULL;
    }

    out_b = _ped_Geometry2PedGeometry(in_b);
    if (out_b == NULL) {
        return NULL;
    }

    geom = ped_geometry_intersect (out_a, out_b);
    if (geom) {
        ret = PedGeometry2_ped_Geometry(geom);
    }

    if (ret) {
        return (PyObject *) ret;
    } else {
        return NULL;
    }
}

PyObject *py_ped_geometry_destroy(PyObject *s, PyObject *args) {
    PyObject *in_geometry;
    PedGeometry *out_geometry;

    if (!PyArg_ParseTuple(args, "O", &in_geometry)) {
        return NULL;
    }

    out_geometry = _ped_Geometry2PedGeometry(in_geometry);
    if (out_geometry == NULL) {
        return NULL;
    }

    ped_geometry_destroy(out_geometry);

    Py_INCREF(Py_None);
    return Py_None;
}

PyObject *py_ped_geometry_set(PyObject *s, PyObject *args) {
    int ret = -1;
    PyObject *in_geom, *in_start, *in_length;
    PedGeometry *out_geom;
    PedSector out_start, out_length;

    if (!PyArg_ParseTuple(args, "OOO", &in_geom, &in_start, &in_length)) {
        return NULL;
    }

    out_geom = _ped_Geometry2PedGeometry(in_geom);
    if (out_geom == NULL) {
        return NULL;
    }

    out_start = _ped_Sector2PedSector(in_start);
    if (out_start == -1) {
        return NULL;
    }

    out_length = _ped_Sector2PedSector(in_length);
    if (out_length == -1) {
        return NULL;
    }

    ret = ped_geometry_set(out_geom, out_start, out_length);
    ped_geometry_destroy(out_geom);
    return PyBool_FromLong(ret);
}

PyObject *py_ped_geometry_set_start(PyObject *s, PyObject *args) {
    int ret = -1;
    PyObject *in_geom, *in_start;
    PedGeometry *out_geom;
    PedSector out_start;

    if (!PyArg_ParseTuple(args, "OO", &in_geom, &in_start)) {
        return NULL;
    }

    out_geom = _ped_Geometry2PedGeometry(in_geom);
    if (out_geom == NULL) {
        return NULL;
    }

    out_start = _ped_Sector2PedSector(in_start);
    if (out_start == -1) {
        return NULL;
    }

    ret = ped_geometry_set_start(out_geom, out_start);
    ped_geometry_destroy(out_geom);
    return PyBool_FromLong(ret);
}

PyObject *py_ped_geometry_set_end(PyObject *s, PyObject *args) {
    int ret = -1;
    PyObject *in_geom, *in_end;
    PedGeometry *out_geom;
    PedSector out_end;

    if (!PyArg_ParseTuple(args, "OO", &in_geom, &in_end)) {
        return NULL;
    }

    out_geom = _ped_Geometry2PedGeometry(in_geom);
    if (out_geom == NULL) {
        return NULL;
    }

    out_end = _ped_Sector2PedSector(in_end);
    if (out_end == -1) {
        return NULL;
    }

    ret = ped_geometry_set_end(out_geom, out_end);
    ped_geometry_destroy(out_geom);
    return PyBool_FromLong(ret);
}

PyObject *py_ped_geometry_test_overlap(PyObject *s, PyObject *args) {
    int ret = -1;
    PyObject *in_a, *in_b;
    PedGeometry *out_a, *out_b;

    if (!PyArg_ParseTuple(args, "OO", &in_a, &in_b)) {
        return NULL;
    }

    out_a = _ped_Geometry2PedGeometry(in_a);
    if (out_a == NULL) {
        return NULL;
    }

    out_b = _ped_Geometry2PedGeometry(in_b);
    if (out_b == NULL) {
        return NULL;
    }

    ret = ped_geometry_test_overlap(out_a, out_b);
    ped_geometry_destroy(out_a);
    ped_geometry_destroy(out_b);
    return PyBool_FromLong(ret);
}

PyObject *py_ped_geometry_test_inside(PyObject *s, PyObject *args) {
    int ret = -1;
    PyObject *in_a, *in_b;
    PedGeometry *out_a, *out_b;

    if (!PyArg_ParseTuple(args, "OO", &in_a, &in_b)) {
        return NULL;
    }

    out_a = _ped_Geometry2PedGeometry(in_a);
    if (out_a == NULL) {
        return NULL;
    }

    out_b = _ped_Geometry2PedGeometry(in_b);
    if (out_b == NULL) {
        return NULL;
    }

    ret = ped_geometry_test_inside(out_a, out_b);
    ped_geometry_destroy(out_a);
    ped_geometry_destroy(out_b);
    return PyBool_FromLong(ret);
}

PyObject *py_ped_geometry_test_equal(PyObject *s, PyObject *args) {
    int ret = -1;
    PyObject *in_a, *in_b;
    PedGeometry *out_a, *out_b;

    if (!PyArg_ParseTuple(args, "OO", &in_a, &in_b)) {
        return NULL;
    }

    out_a = _ped_Geometry2PedGeometry(in_a);
    if (out_a == NULL) {
        return NULL;
    }

    out_b = _ped_Geometry2PedGeometry(in_b);
    if (out_b == NULL) {
        return NULL;
    }

    ret = ped_geometry_test_equal(out_a, out_b);
    ped_geometry_destroy(out_a);
    ped_geometry_destroy(out_b);
    return PyBool_FromLong(ret);
}

PyObject *py_ped_geometry_test_sector_inside(PyObject *s, PyObject *args) {
    int ret = -1;
    PyObject *in_geom, *in_sector;
    PedGeometry *out_geom;
    PedSector out_sector;

    if (!PyArg_ParseTuple(args, "OO", &in_geom, &in_sector)) {
        return NULL;
    }

    out_geom = _ped_Geometry2PedGeometry(in_geom);
    if (out_geom == NULL) {
        return NULL;
    }

    out_sector = _ped_Sector2PedSector(in_sector);
    if (out_sector == -1) {
        return NULL;
    }

    ret = ped_geometry_test_sector_inside(out_geom, out_sector);
    ped_geometry_destroy(out_geom);
    return PyBool_FromLong(ret);
}

PyObject *py_ped_geometry_read(PyObject *s, PyObject *args) {
    int ret = -1;
    PyObject *in_geom, *in_buf, *in_offset, *in_count;
    PedGeometry *out_geom;
    void *out_buf;
    PedSector out_offset, out_count;

    if (!PyArg_ParseTuple(args, "OOOO", &in_geom, &in_buf, &in_offset, &in_count)) {
        return NULL;
    }

    out_geom = _ped_Geometry2PedGeometry(in_geom);
    if (out_geom == NULL) {
        return NULL;
    }

    out_offset = _ped_Sector2PedSector(in_offset);
    if (out_offset == -1) {
        return NULL;
    }

    out_count = _ped_Sector2PedSector(in_count);
    if (out_count == -1) {
        return NULL;
    }

    out_buf = PyCObject_AsVoidPtr(in_buf);
    if (out_buf == NULL) {
        return NULL;
    }

    ret = ped_geometry_read(out_geom, out_buf, out_offset, out_count);
    ped_geometry_destroy(out_geom);
    return PyBool_FromLong(ret);
}

PyObject *py_ped_geometry_sync(PyObject *s, PyObject *args) {
    int ret = -1;
    PyObject *in_geom;
    PedGeometry *out_geom;

    if (!PyArg_ParseTuple(args, "O", &in_geom)) {
        return NULL;
    }

    out_geom = _ped_Geometry2PedGeometry(in_geom);
    if (out_geom == NULL) {
        return NULL;
    }

    ret = ped_geometry_sync(out_geom);
    ped_geometry_destroy(out_geom);
    return PyBool_FromLong(ret);
}

PyObject *py_ped_geometry_sync_fast(PyObject *s, PyObject *args) {
    int ret = -1;
    PyObject *in_geom;
    PedGeometry *out_geom;

    if (!PyArg_ParseTuple(args, "O", &in_geom)) {
        return NULL;
    }

    out_geom = _ped_Geometry2PedGeometry(in_geom);
    if (out_geom == NULL) {
        return NULL;
    }

    ret = ped_geometry_sync_fast(out_geom);
    ped_geometry_destroy(out_geom);
    return PyBool_FromLong(ret);
}

PyObject *py_ped_geometry_write(PyObject *s, PyObject *args) {
    int ret = -1;
    PyObject *in_geom, *in_buf, *in_offset, *in_count;
    PedGeometry *out_geom;
    void *out_buf;
    PedSector out_offset, out_count;

    if (!PyArg_ParseTuple(args, "OOOO", &in_geom, &in_buf, &in_offset, &in_count)) {
        return NULL;
    }

    out_geom = _ped_Geometry2PedGeometry(in_geom);
    if (out_geom == NULL) {
        return NULL;
    }

    out_offset = _ped_Sector2PedSector(in_offset);
    if (out_offset == -1) {
        return NULL;
    }

    out_count = _ped_Sector2PedSector(in_count);
    if (out_count == -1) {
        return NULL;
    }

    out_buf = PyCObject_AsVoidPtr(in_buf);
    if (out_buf == NULL) {
        return NULL;
    }

    ret = ped_geometry_write(out_geom, out_buf, out_offset, out_count);
    ped_geometry_destroy(out_geom);
    return PyBool_FromLong(ret);
}

PyObject *py_ped_geometry_check(PyObject *s, PyObject *args) {
    PyObject *in_geom, *in_buf, *in_buffer_size, *in_offset, *in_granularity;
    PyObject *in_count, *in_timer;
    PedGeometry *out_geom;
    void *out_buf;
    PedSector out_buffer_size, out_offset, out_granularity, out_count, sector;
    PedTimer *out_timer;
    _ped_Sector *ret;

    if (!PyArg_ParseTuple(args, "OOOOOOO", &in_geom, &in_buf, &in_buffer_size,
                                           &in_offset, &in_granularity,
                                           &in_count, &in_timer)) {
        return NULL;
    }

    out_geom = _ped_Geometry2PedGeometry(in_geom);
    if (out_geom == NULL) {
        return NULL;
    }

    out_buffer_size = _ped_Sector2PedSector(in_buffer_size);
    if (out_buffer_size == -1) {
        return NULL;
    }

    out_offset = _ped_Sector2PedSector(in_offset);
    if (out_offset == -1) {
        return NULL;
    }

    out_granularity = _ped_Sector2PedSector(in_granularity);
    if (out_granularity == -1) {
        return NULL;
    }

    out_count = _ped_Sector2PedSector(in_count);
    if (out_count == -1) {
        return NULL;
    }

    out_timer = _ped_Timer2PedTimer(in_timer);
    if (out_timer == NULL) {
        return NULL;
    }

    out_buf = PyCObject_AsVoidPtr(in_buf);
    if (out_buf == NULL) {
        return NULL;
    }

    sector = ped_geometry_check(out_geom, out_buf, out_buffer_size, out_offset,
                                out_granularity, out_count, out_timer);
    ped_geometry_destroy(out_geom);
    ped_timer_destroy(out_timer);

    ret = PedSector2_ped_Sector(sector);

    if (ret) {
        return (PyObject *) ret;
    } else {
        return NULL;
    }
}

PyObject *py_ped_geometry_map(PyObject *s, PyObject *args) {
    int ret = -1;
    PyObject *in_dst, *in_src, *in_sector;
    PedGeometry *out_dst, *out_src;
    PedSector out_sector;

    if (!PyArg_ParseTuple(args, "OOO", &in_dst, &in_src, &in_sector)) {
        return NULL;
    }

    out_dst = _ped_Geometry2PedGeometry(in_dst);
    if (out_dst == NULL) {
        return NULL;
    }

    out_src = _ped_Geometry2PedGeometry(in_src);
    if (out_src == NULL) {
        return NULL;
    }

    out_sector = _ped_Sector2PedSector(in_sector);
    if (out_sector == -1) {
        return NULL;
    }

    ret = ped_geometry_map(out_dst, out_src, out_sector);
    ped_geometry_destroy(out_dst);
    ped_geometry_destroy(out_src);
    return Py_BuildValue("I", ret);
}
