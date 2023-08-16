#!/bin/bash

cmake -DARC_SIZE=8 ..
make
./practical -f ../result/random1000bel -g ../graph/bel.graph -m 1
./practical -f ../result/random1000bel -g ../graph/bel.graph -m 2
./practical -f ../result/random1000bel -g ../graph/bel.graph -m 3
./practical -f ../result/random1000bel -g ../graph/bel.graph -m 4

cmake -DARC_SIZE=16 ..
make
./practical -f ../result/random1000bel -g ../graph/bel.graph -m 2
./practical -f ../result/random1000bel -g ../graph/bel.graph -m 4

cmake -DARC_SIZE=32 ..
make
./practical -f ../result/random1000bel -g ../graph/bel.graph -m 2
./practical -f ../result/random1000bel -g ../graph/bel.graph -m 4

cmake -DARC_SIZE=64 ..
make
./practical -f ../result/random1000bel -g ../graph/bel.graph -m 2
./practical -f ../result/random1000bel -g ../graph/bel.graph -m 4

cmake -DARC_SIZE=128 ..
make
./practical -f ../result/random1000bel -g ../graph/bel.graph -m 2
./practical -f ../result/random1000bel -g ../graph/bel.graph -m 4

cmake -DARC_SIZE=256 ..
make
./practical -f ../result/random1000bel -g ../graph/bel.graph -m 2
./practical -f ../result/random1000bel -g ../graph/bel.graph -m 4

cmake -DARC_SIZE=512 ..
make
./practical -f ../result/random1000bel -g ../graph/bel.graph -m 2
./practical -f ../result/random1000bel -g ../graph/bel.graph -m 4


cmake -DARC_SIZE=8 ..
make
./practical -f ../result/random1000deu -g ../graph/deu.graph -m 1
./practical -f ../result/random1000deu -g ../graph/deu.graph -m 2
./practical -f ../result/random1000deu -g ../graph/deu.graph -m 3
./practical -f ../result/random1000deu -g ../graph/deu.graph -m 4

cmake -DARC_SIZE=16 ..
make
./practical -f ../result/random1000deu -g ../graph/deu.graph -m 2
./practical -f ../result/random1000deu -g ../graph/deu.graph -m 4

cmake -DARC_SIZE=32 ..
make
./practical -f ../result/random1000deu -g ../graph/deu.graph -m 2
./practical -f ../result/random1000deu -g ../graph/deu.graph -m 4

cmake -DARC_SIZE=64 ..
make
./practical -f ../result/random1000deu -g ../graph/deu.graph -m 2
./practical -f ../result/random1000deu -g ../graph/deu.graph -m 4

cmake -DARC_SIZE=128 ..
make
./practical -f ../result/random1000deu -g ../graph/deu.graph -m 2
./practical -f ../result/random1000deu -g ../graph/deu.graph -m 4

cmake -DARC_SIZE=256 ..
make
./practical -f ../result/random1000deu -g ../graph/deu.graph -m 2
./practical -f ../result/random1000deu -g ../graph/deu.graph -m 4

cmake -DARC_SIZE=512 ..
make
./practical -f ../result/random1000deu -g ../graph/deu.graph -m 2
./practical -f ../result/random1000deu -g ../graph/deu.graph -m 4
