sy = ["+", "+.", "-", "-.", "*", "*.", "/", "/.", "mod"]

def hash(s):
    res = 5381
    for c in s:
        res = ((res << 5) & 0xFFFFFFFF) + res + ord(c)
    return res

def test(x):
    samples = []
        
    for s in sy:
        h = hash(s) % x
        if h in samples:
            return False
        samples.append(h)

    print(sy)
    print(samples)
    return True

l = 8

while not test(l):
    l *= 2

print(l)
