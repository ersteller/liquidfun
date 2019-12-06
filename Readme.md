
<img src="liquidfun/Box2D/Documentation/Programmers-Guide/html/liquidfun-logo-square-small.png"
alt="LiquidFun logo" style="float:right;" />

forked from LiquidFun Version [1.1.0][]

# About this fork
## N-body 
This is an old project of mine to play around with gravity,n-body problem and some approximations.
in particular this contains an implementation of a barnes-hut algorythm using a quadtree written in c++.
## webassembly 
This is a proof of concept to see if and how slow the c++ code would run in broswer to evaluate/inhibit any later game concept ideas. There is an example in the [landing page][]. Just keep in mind there is a third dimension and maybe VR. 
Liquidfun is single threaded and the barnes-hut tree is called in the step callback synchronously.

# Welcome to LiquidFun!
LiquidFun is a 2D physics engine for games.  Go to our
[landing page][] to browse our documentation and see some examples.

LiquidFun is an extension of [Box2D][]. It adds a particle based fluid and soft
body simulation to the rigid body functionality of [Box2D][]. LiquidFun can be
built for many different systems, including Android, iOS, Windows, OS X, Linux,
and JavaScript.

File issues on the [LiquidFun Issues Tracker][]

To build this code (javascript and c++ bindings) use docker! It contains all dependancies.

In any case please see [Box2D/Documentation/Building/][] to learn how to build LiquidFun and
run the testbed.

LiquidFun has a logo that you can use, in your splash screens or documentation,
for example. Please see the [Programmer's Guide][] for the graphics and further
details.

For applications on Google Play that integrate this tool, usage is tracked.
This tracking is done automatically using the embedded version string
(`b2_liquidFunVersionString`), and helps us continue to optimize it. Aside from
consuming a few extra bytes in your application binary, it shouldn't affect
your application at all. We use this information to let us know if LiquidFun
is useful and if we should continue to invest in it. Since this is open
source, you are free to remove the version string but we would appreciate if
you would leave it in.

  [LiquidFun Google Group]: https://groups.google.com/forum/#!forum/liquidfun
  [LiquidFun Issues Tracker]: http://github.com/ersteller/liquidfun/issues
  [stackoverflow.com]: http://www.stackoverflow.com
  [landing page]: http://ersteller.github.io/liquidfun
  [1.1.0]: http://google.github.io/liquidfun/ReleaseNotes.html
  [Box2D]: http://box2d.org
  [Box2D/Documentation/Building/]: http://google.github.io/liquidfun/Building.html
  [Programmer's Guide]: http://google.github.io/liquidfun/Programmers-Guide.html


# Build
## Build docker image for JavaScript build
```
docker build -f Dockerfile -t liquidfunbuilder:latest . 
```
### Run build
In indows10 run ```.\devscripts\buildjsandlaunchserver.bat```

In Linux run ```devscripts/buildjs.sh```(untested: might need some minor updates) 

This generates new bindings for javascript and also may run uglify if not for debugging disabled
after build is done a webserver is started for the testbed.

### Test
Then browse to localhost:80 and visit the testbed 

## C++ on windows
you need cmake and some MS Visual Studio to build the c++ app for windows
'''
mkdir build
cd build
cmake ..\liquidfun\Box2D -G "Visual Studio 15 2017 Win64"
cmake --build . --config Release
'''

# TODO: coolstuff
## for lighting
http://www.java-gaming.org/topics/libgdx-trying-to-get-my-head-around-box2d-lights/38424/view.html
### shader
and the shader ```lightingshader.sdrsrc``` found there: 
found on youtube usr:Šklopec https://youtu.be/fsbECSpwtig
and absolutly like so https://github.com/vanderlin/Box2dLights

## for sound
add https://www.sfml-dev.org/
like this https://github.com/SonarSystems/SFML-Box2D-Tutorials or like this https://gist.github.com/jmcgill/1396667
