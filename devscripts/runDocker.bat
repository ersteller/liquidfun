
docker run -it -p 80:80 -p 2345:2345 -p 9229:9229 -v %~dp0/../:/build/project -v liquidfun_cache:/root/.emscripten_cache --name=liquidfunbuilder liquidfunbuilder:latest /bin/bash -c "source ./emsdk/emsdk_env.sh && cd project/liquidfun/Box2D/lfjs && bash"
rem docker run -it -p 80:80 -p 2345:2345 -p 9229:9229 -v %~dp0/../:/build/project -v liquidfun_cache:/root/.emscripten_cache --name=liquidfunbuilder liquidfunbuilder:latest /bin/bash -c "source ./emsdk/emsdk_env.sh && cd project/liquidfun/Box2D/lfjs && bash"



