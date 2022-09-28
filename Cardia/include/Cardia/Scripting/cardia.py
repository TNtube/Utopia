import inspect

import cardia_native as _cd

import enum
import functools
from types import MethodType


class Serializable:
    def __init__(self, t: type):
        self.t = t


class Key(enum.IntEnum):
    Space = 32
    Apostrophe = 39
    Comma = 44
    Minus = 45
    Period = 46
    Slash = 47
    N0 = 48
    N1 = 49
    N2 = 50
    N3 = 51
    N4 = 52
    N5 = 53
    N6 = 54
    N7 = 55
    N8 = 56
    N9 = 57
    Semicolon = 59
    Equal = 61
    A = 65
    B = 66
    C = 67
    D = 68
    E = 69
    F = 70
    G = 71
    H = 72
    I = 73
    J = 74
    K = 75
    L = 76
    M = 77
    N = 78
    O = 79
    P = 80
    Q = 81
    R = 82
    S = 83
    T = 84
    U = 85
    V = 86
    W = 87
    X = 88
    Y = 89
    Z = 90
    LBracket = 91
    BackSlash = 92
    RBracket = 93
    GraveAccent = 96
    Right = 262
    Left = 263
    Down = 264
    Up = 265
    LeftAlt = 342
    RightAlt = 346
    LeftCtrl = 341
    RightCtrl = 345


class Input:
    @staticmethod
    def is_key_pressed(key: Key) -> bool:
        return _cd.is_key_pressed(int(key))


class DeltaTime:
    def __init__(self):
        self.dt = 0.0
        self.mldt = 0.0

    def seconds(self) -> float:
        self.dt = _cd.get_delta_time_seconds()
        return self.dt

    def milliseconds(self) -> float:
        self.mldt = _cd.get_delta_time_milliseconds()
        return self.mldt

    def __float__(self):
        return self.dt


class Time:
    delta_time = DeltaTime()


class Behavior:
    def __init__(self):
        self.id = None

    @property
    def transform(self):
        return _cd.get_native_transform(self.id)

    @transform.setter
    def transform(self, t):
        _cd.set_native_transform(self.id, t)

    def on_create(self):
        ...

    def on_update(self):
        ...


def on_key_pressed(key: Key):
    class Inner:
        def __init__(self, func):
            functools.update_wrapper(self, func)
            args = inspect.getfullargspec(func).args
            if not len(args) or args[0] != "self":
                _cd.register_update_function(self.__call__)
            self.func = func
            self.cls = None

        def __set_name__(self, cls, __):
            self.cls = cls
            _cd.register_update_method(cls, self.func.__name__)

        def __call__(self, *args, **kwargs):
            if _cd.is_key_pressed(int(key)):
                self.func(*args, **kwargs)

        def __get__(self, inst, _):  # Thanks @horus-4ever
            return MethodType(self, inst)
    return Inner
