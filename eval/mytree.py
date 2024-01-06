from graphviz import Graph
# import regex

class Tree:
    def __init__(self, key, kids=None):
        self.key = key
        if kids == None:
            self.kids = []
        else:
            self.kids = kids

    def __str__(self):
        res = '('

        if self.key is None:
            return res + ')'

        res += str(self.key)

        for k in self.kids:
            res += str(k)

        return res + ')'

    def __eq__(self, other):
        if (self.key != other.key or
            self.count != other.count):
            return False

        return self.kids == other.kids

    def count(self):
        return len(self.kids)

    def export(self, file: str):
        g = Graph(file)
        g.attr('node', height='.2')
        q = []
        q.append(self)

        while q != []:
            elt = q.pop(0)
            g.body += str(elt.key)
            for i in elt.kids:
                g.body += f" {elt.key}--{i.key} "
                q.append(i)

        g.render(file)

def to_linear(tree: Tree):
    res = '('

    if tree.key is None:
        return res + ')'

    res += str(tree.key)

    for k in tree.kids:
        res += to_linear(k)

    return res + ')'

def from_linear(src: str, op):
    src = src[1:-1]

    bal = 0
    idx = 0
    length = len(src)
    groups = []
    tmp = ''

    for c in src:
        if c == '(':
            break
        tmp += c
        idx += 1

    groups.append(tmp)
    tmp = '('

    for c in src[idx:]:
        if c == ')':
            bal -= 1
        if bal == 0 and len(tmp) > 0 and tmp != '(':
            groups.append(tmp+')')
            tmp = '('
        if bal > 0:
            tmp += c
        if c == '(':
            bal += 1

    if bal == 0 and len(tmp) > 0 and tmp != '(':
        groups.append(tmp)
        tmp = ''

    if bal != 0:
        raise Exception('Unbalanced Parenthesis')

    if len(groups) == 0:
        return None

    key = op(groups[0])

    return Tree(key, [from_linear(x,op) for x in groups[1:]])
