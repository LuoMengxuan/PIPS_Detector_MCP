set -e
cd /mnt/hgfs/share/PIPS_Detector_MCP
rm -rf ./build/*
mkdir -p  build
cd build/
cmake ..
make -j4
cp ../run1.mac ./

#beta==================
./exampleB1 -A 450 -J 531 -N e- -L 2 -D 24 run1.mac
./exampleB1 -A 450 -J 531 -N e- -L 4 -D 24 run1.mac
./exampleB1 -A 450 -J 531 -N e- -L 6 -D 24 run1.mac
./exampleB1 -A 450 -J 531 -N e- -L 8 -D 24 run1.mac

./exampleB1 -A 1700 -J 1900 -N e- -L 2 -D 42 run1.mac
./exampleB1 -A 1700 -J 1900 -N e- -L 4 -D 42 run1.mac
./exampleB1 -A 1700 -J 1900 -N e- -L 6 -D 42 run1.mac
./exampleB1 -A 1700 -J 1900 -N e- -L 8 -D 42 run1.mac

#alpha==================
./exampleB1 -A 450 -J 531 -N alpha -L 2 -D 24 run1.mac
./exampleB1 -A 450 -J 531 -N alpha -L 4 -D 24 run1.mac
./exampleB1 -A 450 -J 531 -N alpha -L 6 -D 24 run1.mac
./exampleB1 -A 450 -J 531 -N alpha -L 8 -D 24 run1.mac

./exampleB1 -A 1700 -J 1900 -N alpha -L 2 -D 42 run1.mac
./exampleB1 -A 1700 -J 1900 -N alpha -L 4 -D 42 run1.mac
./exampleB1 -A 1700 -J 1900 -N alpha -L 6 -D 42 run1.mac
./exampleB1 -A 1700 -J 1900 -N alpha -L 8 -D 42 run1.mac
