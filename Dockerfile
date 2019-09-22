FROM ubuntu
RUN apt-get update && apt-get install -y \
    cmake \
    curl \
    default-jre \
    gcc g++ \
    git-core \
    libglapi-mesa \
    libglu1-mesa-dev \
    nodejs \
    python \
    unzip

WORKDIR /build

# install emsdk
RUN git clone https://github.com/emscripten-core/emsdk.git && \
    cd emsdk && \
    ./emsdk install latest  && \
    ./emsdk activate latest 

#RUN /bin/bash -c source ./emsdk_env.sh
ENV PATH="$PATH:/build/emsdk/fastcomp/emscripten:/build/emsdk/node/12.9.1_64bit/bin" \
    EMSDK="/build/emsdk" \
    EM_CONFIG="/root/.emscripten" \
    EMSDK_NODE="/build/emsdk/node/12.9.1_64bit/bin/node"

# LLVM 
#RUN emcc && build-js.sh
RUN emcc -v
RUN echo "LLVM_ROOT = os.path.expanduser('/build/emsdk/fastcomp/bin')\n" >> ~/.emscripten && \
    echo "NODE_JS = os.path.expanduser('/build/emsdk/node/12.9.1_64bit/bin/node')\n" >> ~/.emscripten 

# get closure compiler
RUN mkdir closure-compiler && \
    curl https://dl.google.com/closure-compiler/compiler-latest.zip > ./closure-compiler/compiler-latest.zip
RUN unzip ./closure-compiler/compiler-latest.zip -d ./closure-compiler/

# test of toolchain
#RUN echo "/* A simple function. */ function hello(longName) {  alert('Hello, ' + longName);} hello('New User'); " > hello.js 
#RUN java -jar ./closure-compiler/closure-compiler-v20190909.jar --js hello.js --js_output_file hello-compiled.js
#RUN echo "<html><head><title>Hello World</title></head> <body><script src=\"hello-compiled.js\"></script></body></html>" > test.html

ENV EMSCRIPTEN="/build/emsdk/fastcomp/emscripten" \
    CLOSURE_JAR="/build/closure-compiler/closure-compiler-v20190909.jar"

RUN mkdir project


#COPY freeglut ./project/freeglut
#COPY googletest ./project/googletest
#COPY liquidfun ./project/liquidfun
#RUN cd project/liquidfun/Box2D/lfjs \
#    make \
#    ./uglify.sh

# copy build with: docker cp <container>:/build/project/liquidfun/Box2D/lfjs
#`Box2D/lfjs/index.html' in your browser
