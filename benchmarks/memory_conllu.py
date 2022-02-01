from memory_profiler import profile

@profile
def load():
    import conllu

    data_file = open("cs-ud-train-l.conllu", "r", encoding="utf-8")
    # parse_incr is recommended for large files (more than 1 MB)
    # since it returns a generator, which is why a conversion to list
    tokenlist = list(conllu.parse_incr(data_file))

    for sentence in tokenlist:
        for token in sentence:
            form_lemma = token['form'] + token['lemma']

    for sentence in tokenlist:
        chain = []
        for token in sentence:
            if token['head']:
                parent = sentence[token['head'] - 1]
                if token['deprel'] == "case" and parent['deprel'] == "nmod":
                    chain.append(token)

    for sentence in tokenlist:
        for token in sentence:
            token['deprel'] = 'dep'

    with open('out.conllu', 'w', encoding='utf8') as f:
        f.writelines([sentence.serialize() + "\n" for sentence in tokenlist])


if __name__ == '__main__':
    load()
