rm -rf ../udon
rm core.o Util.o Node.o Tree.o ConllReader.o algorithms.o
make
mkdir udon
cp init_template.py udon/__init__.py
mv core.so udon/
mv algorithms.so udon/
mv udon ../
