from typing import Tuple
import matplotlib.pyplot as plt
import numpy as np
import ctypes
import pdb


class Point(ctypes.Structure):
    _fields_ = [
        ("x", ctypes.c_int),  # x 是一个整数
        ("y", ctypes.c_int),  # y 是一个整数
    ]


def plotVector(x: int, y: int, size: Tuple[int, int]):
    x_ptr = ctypes.c_void_p(x)
    x_ptr = ctypes.cast(x_ptr, ctypes.POINTER(ctypes.c_double))
    x_array = np.ctypeslib.as_array(x_ptr, shape=(size[0],))
    if y is not None:
        y_ptr = ctypes.c_void_p(y)
        y_ptr = ctypes.cast(y_ptr, ctypes.POINTER(ctypes.c_double))
        y_array = np.ctypeslib.as_array(y_ptr, shape=(size[1],))
        plt.plot(x_array, y_array)
    else:
        plt.plot(x_array)


def fft(x: int, fft: int, size: Tuple[int, int]):
    # pdb.set_trace()
    x_ptr = ctypes.c_void_p(x)
    fft_ptr = ctypes.c_void_p(fft)
    x_ptr = ctypes.cast(x_ptr, ctypes.POINTER(ctypes.c_double))
    fft_ptr = ctypes.cast(fft_ptr, ctypes.POINTER(ctypes.c_double))
    x_array = np.ctypeslib.as_array(x_ptr, shape=(size[0],))
    y_array = np.ctypeslib.as_array(fft_ptr, shape=(size[1],))
    # plt.plot(x_array)
    seq = np.fft.fft(x_array)
    print(seq.shape)
    seq = np.abs(seq)
    print(seq.dtype)
    np.copyto(y_array, seq)


def print_point(pt: int):
    p_vptr = ctypes.c_void_p(pt)
    p_ptr = ctypes.cast(p_vptr, ctypes.POINTER(Point))
    point = p_ptr.contents
    print(f"(x:{point.x}, y:{point.y})")


def plotShow():
    plt.show()
