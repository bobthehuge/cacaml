from collections.abc import Callable
from dataclasses import dataclass
from enum import Enum

from cml_log import cml_exit

G_IOTA_COUNTER = 0

def iota(reset=False) -> int:
    global G_IOTA_COUNTER

    if reset:
        G_IOTA_COUNTER = 1
    else:
        G_IOTA_COUNTER += 1

    return G_IOTA_COUNTER - 1

class CML_TokenKind(Enum):
    NULL = iota(reset=True)
    I32 = iota()
    UNOP = iota()
    BINOP = iota()
    CALL = iota()

@dataclass
class CML_Callable:
    ident: int
    argc: int
    call: Callable

    def __str__(self):
        return '<{};{}>'.format(self.ident, self.argc)

@dataclass
class CML_Token:
    kind:   CML_TokenKind
    label:  str
    call:   CML_Callable

    def __str__(self):
        return '{};{}'.format(self.kind.value, self.label, self.call)

def djb2(key: str):
    res = 5381
    for c in s:
        res = ((res << 5) & 0xFFFFFFFF) + res + ord(c)
    return res

def cml_add(argv: list) -> CML_Token:
    return CML_Token(CML_TokenKind.I32,
                     str(int(argv[0].key.label) + int(argv[1].key.label)),
                     CML_Callable(ident=0, argc=0, call=None))

G_CML_CALLABLES = {
    CML_Callable(ident=hash("+"), argc=2, call=cml_add),
}

def cml_get_call(ident: int) -> CML_Callable:
    global G_CML_CALLABLES

    if ident >= len(G_CML_CALLABLES):
        cml_exit("SyntaxError: Callable id '{}' is out of bounds".format(ident),1)

    return G_CML_CALLABLES[ident]
