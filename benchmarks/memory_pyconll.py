from memory_profiler import profile

@profile
def load():
    import pyconll
    train = pyconll.load_from_file('cs-ud-train-l.conllu')

    for sentence in train:
        for token in sentence:
            if not token.is_multiword():
                form_lemma = token.form + token.lemma

    for sentence in train:
        chain = []
        for token in sentence:
            if not token.is_multiword():
                if token.head == '0': continue
                parent = sentence[token.head]
                if token.deprel == "case" and parent.deprel == "nmod":
                    chain.append(token)

    for sentence in train:
        for token in sentence:
            if not token.is_multiword():
                token.deprel = 'dep'

    train.write(open('out.conllu', 'w'))

if __name__ == '__main__':
    load()
