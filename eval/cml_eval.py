from cml_log import cml_exit
from cml_tokens import CML_TokenKind, CML_Token
from mytree import Tree

def cml_ishexstr(s: str):
    if len(s) == 0:
        return False

    for c in s:
        if not c.isdigit() and c not in "ABCDEF":
            return False

    return True

def cml_isnum(s: str):
    if len(s) > 0 and s[0] in '-,+':
        return s[1:].isdigit()
    return s.isdigit()

def cml_eval_node(node: Tree) -> None:
    match node.key.kind:
        case CML_TokenKind.NULL:
            return

        case CML_TokenKind.OPERAND:
            if not cml_isnum(node.key.label):
                cml_exit("ValueError: Invalid int representation '{}'".format(node.key.label))

        case CML_TokenKind.FUNCTION:
            cml_callable = node.key.call

            if node.count() != cml_callable.argc:
                cml_exit("ValueError: argc mismatch for operation with id '{}' (got {})".format(node.key.label, node.count()))

            cml_eval_node(node.kids[0])
            cml_eval_node(node.kids[1])

            node.key = cml_callable.call(node.kids)
            node.kids = []

        case _:
            cml_exit("SyntaxError: Unknown token kind '{}'".format(node.key.kind))

def cml_eval_tree(ast: Tree) -> None:
        cml_eval_node(ast)

