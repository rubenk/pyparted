/*
 * pyunit.c
 *
 * Copyright (C) 2007  Red Hat, Inc.
 * All rights reserved.
 *
 * This copyrighted material is made available to anyone wishing to use,
 * modify, copy, or redistribute it subject to the terms and conditions of
 * the GNU General Public License v.2, or (at your option) any later version.
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY expressed or implied, including the implied warranties of
 * MERCHANTABILITY or FITNESS FOR A * PARTICULAR PURPOSE.  See the GNU General
 * Public License for more details.  You should have received a copy of the
 * GNU General Public License along with this program; if not, write to the
 * Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA
 * 02110-1301, USA.  Any Red Hat trademarks that are incorporated in the
 * source code or documentation are not subject to the GNU General Public
 * License and may only be used or replicated with the express permission of
 * Red Hat, Inc.
 *
 * Red Hat Author(s): David Cantrell <dcantrell@redhat.com>
 */

#include <Python.h>

#include "convert.h"
#include "pydevice.h"
#include "pyunit.h"

/* 1:1 function mappings for unit.h in libparted */
PyObject *py_ped_unit_set_default(PyObject *s, PyObject *args) {
    PedUnit unit;

    if (!PyArg_ParseTuple(args, "L", &unit)) {
        return NULL;
    }

    if (unit < PED_UNIT_FIRST || unit > PED_UNIT_LAST) {
        /* XXX: some sort of range exception */
        return NULL;
    }

    ped_unit_set_default(unit);

    Py_INCREF(Py_None);
    return Py_None;
}

PyObject *py_ped_unit_get_default(PyObject *s, PyObject *args) {
    return PyLong_FromLongLong(ped_unit_get_default());
}

PyObject *py_ped_unit_get_size(PyObject *s, PyObject *args) {
    long long ret = -1;
    long long in_unit;
    PyObject *in_dev;
    PedDevice *out_dev;
    PedUnit out_unit;

    if (!PyArg_ParseTuple(args, "lO", &in_unit, &in_dev)) {
        return NULL;
    }

    out_dev = _ped_Device2PedDevice(in_dev);
    if (out_dev == NULL) {
        return NULL;
    }

    out_unit = _ped_Unit2PedUnit(in_unit);
    if (out_unit == -1) {
        return NULL;
    }

    ret = ped_unit_get_size(out_dev, out_unit);
    ped_device_destroy(out_dev);

    return PyLong_FromLongLong(ret);
}

PyObject *py_ped_unit_get_name(PyObject *s, PyObject *args) {
    PedUnit unit;

    if (!PyArg_ParseTuple(args, "L", &unit)) {
        return NULL;
    }

    return Py_BuildValue("s", ped_unit_get_name(unit));
}

PyObject *py_ped_unit_get_by_name(PyObject *s, PyObject *args) {
    char *name;

    if (!PyArg_ParseTuple(args, "s", &name)) {
        return NULL;
    }

    return PyLong_FromLongLong(ped_unit_get_by_name(name));
}

PyObject *py_ped_unit_format_custom_byte(PyObject *s, PyObject *args) {
    char *ret = NULL;
    PyObject *in_dev, *in_sector;
    PedUnit unit;
    PedDevice *out_dev;
    PedSector out_sector;

    if (!PyArg_ParseTuple(args, "OOL", &in_dev, &in_sector, &unit)) {
        return NULL;
    }

    out_dev = _ped_Device2PedDevice(in_dev);
    if (out_dev == NULL) {
        return NULL;
    }

    out_sector = _ped_Sector2PedSector(in_sector);
    if (out_sector == -1) {
        return NULL;
    }

    ret = ped_unit_format_custom_byte(out_dev, out_sector, unit);
    ped_device_destroy(out_dev);

    return Py_BuildValue("s", ret);
}

PyObject *py_ped_unit_format_byte(PyObject *s, PyObject *args) {
    char *ret = NULL;
    PyObject *in_dev, *in_sector;
    PedDevice *out_dev;
    PedSector out_sector;

    if (!PyArg_ParseTuple(args, "OO", &in_dev, &in_sector)) {
        return NULL;
    }

    out_dev = _ped_Device2PedDevice(in_dev);
    if (out_dev == NULL) {
        return NULL;
    }

    out_sector = _ped_Sector2PedSector(in_sector);
    if (out_sector == -1) {
        return NULL;
    }

    ret = ped_unit_format_byte(out_dev, out_sector);
    ped_device_destroy(out_dev);

    return Py_BuildValue("s", ret);
}

PyObject *py_ped_unit_format_custom(PyObject *s, PyObject *args) {
    char *ret = NULL;
    PyObject *in_dev, *in_sector;
    PedDevice *out_dev;
    PedSector out_sector;
    PedUnit unit;

    if (!PyArg_ParseTuple(args, "OOL", &in_dev, &in_sector, &unit)) {
        return NULL;
    }

    out_dev = _ped_Device2PedDevice(in_dev);
    if (out_dev == NULL) {
        return NULL;
    }

    out_sector = _ped_Sector2PedSector(in_sector);
    if (out_sector == -1) {
        return NULL;
    }

    ret = ped_unit_format_custom(out_dev, out_sector, unit);
    ped_device_destroy(out_dev);

    return Py_BuildValue("s", ret);
}

PyObject *py_ped_unit_format(PyObject *s, PyObject *args) {
    char *ret = NULL;
    PyObject *in_dev, *in_sector;
    PedDevice *out_dev;
    PedSector out_sector;

    if (!PyArg_ParseTuple(args, "OO", &in_dev, &in_sector)) {
        return NULL;
    }

    out_dev = _ped_Device2PedDevice(in_dev);
    if (out_dev == NULL) {
        return NULL;
    }

    out_sector = _ped_Sector2PedSector(in_sector);
    if (out_sector == -1) {
        return NULL;
    }

    ret = ped_unit_format(out_dev, out_sector);
    ped_device_destroy(out_dev);

    return Py_BuildValue("s", ret);
}

PyObject *py_ped_unit_parse(PyObject *s, PyObject *args) {
    /* XXX */
    PyErr_SetString(PyExc_NotImplementedError, NULL);
    return NULL;
}

PyObject *py_ped_unit_parse_custom(PyObject *s, PyObject *args) {
    /* XXX */
    PyErr_SetString(PyExc_NotImplementedError, NULL);
    return NULL;
}
