#!/usr/bin/python

from cml_eval import cml_eval_tree, cml_isnum
from cml_log import cml_exit
from cml_tokens import CML_Token, CML_TokenKind, CML_Callable, cml_get_call
from mytree import *


def get_call_obj(src: str) -> CML_Callable:
    if not cml_isnum(src):
        cml_exit("SyntaxError: no Callable found for '{}'".format(src),1)

    return cml_get_call(int(src))

def get_ident_obj(src: str) -> CML_TokenKind:
    if cml_isnum(src):
        ident = int(src)
        if ident >= len(CML_TokenKind):
            cml_exit("SyntaxError: TokenKind id '{}' is out of bounds".format(ident),1)
    else:
        cml_exit("SyntaxError: no TokenKind found for '{}'".format(src),1)

    return CML_TokenKind(ident)

def deserialize_node_obj(src: str) -> CML_Token:
    if len(src) == 0:
        cml_exit("ValueError: empty/corrupted node", 2)

    g1, g2 = src.split(';', 1)

    kind = get_ident_obj(g1)
    call = get_call_obj(g2) if kind == CML_TokenKind.FUNCTION else None

    return CML_Token(kind=kind, label=g2, call=call)

if __name__ == '__main__':
    import sys

    if len(sys.argv) != 2:
        cml_exit("EntryError: missing entry", 2)

    entry = sys.argv[1]

    if len(entry) <= 2 or entry[0] != '(' or entry[-1] != ')' or entry.count(';') == 0:
        cml_exit("EntryError: malformed entry '{}'".format(entry), 2)

    ast = from_linear(entry, deserialize_node_obj)
    cml_eval_tree(ast)
    print(ast)

    sys.exit(0)
