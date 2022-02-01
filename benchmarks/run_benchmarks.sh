for lib in "pyconll" "conllu" "udapi" "udon2"
do
    echo $lib
    python3 speed_$lib.py
    python3 -m memory_profiler memory_$lib.py
    echo "Done\n"
done
