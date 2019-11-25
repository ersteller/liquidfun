docker run -v $(realpath $(dirname $0)):/build/project -v liquidfun_cache:/root/.emscripten_cache -it liquidfunbuilder:latest /bin/bash -c "cd project/liquidfun/Box2D/lfjs && make && ./uglify.sh"
