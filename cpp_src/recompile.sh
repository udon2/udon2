rm -rf ../udon
rm core.o Util.o Node.o Tree.o ConllReader.o algorithms.o
make
mkdir udon
mv core.so udon/
mv algorithms.so udon/
mv udon ../
