function TestMutualForce() {
    camera.position.y = 2;
    camera.position.z = 3;
    var bodyDef = new b2BodyDef();
    var ground = world.CreateBody(bodyDef);
  
    var chainShape = new b2ChainShape();
    chainShape.vertices.push(new b2Vec2(-2, 0));
    chainShape.vertices.push(new b2Vec2(2, 0));
    chainShape.vertices.push(new b2Vec2(2, 4));
    chainShape.vertices.push(new b2Vec2(-2, 4));
  
    chainShape.CreateLoop();
    ground.CreateFixtureFromShape(chainShape, 0);
  
    var shape = new b2PolygonShape;
    shape.SetAsBoxXYCenterAngle(0.8, 1, new b2Vec2(-1.2, 1.01), 0);
  
    var psd = new b2ParticleSystemDef();
    psd.radius = 0.025;
    psd.dampingStrength = 0.2;
  
    var particleSystem = world.CreateParticleSystem(psd);
  
    var pd = new b2ParticleGroupDef();
    pd.shape = shape;
    var group = particleSystem.CreateParticleGroup(pd);

    var og = new b2Vec2(0.000000000000000e+00, 0.000000000000000e+01);
    world.SetGravity(og);

    //add it to the mutual force contorller 
    console.log("init mfc is called");
    var mfc = new b2MutualForceController();

    this.mfc = mfc;
    
    this.mfc.AddGroup(particleSystem);
    console.log("done with init is called");

    // somewhere step needs to be called

  }
  
TestMutualForce.prototype.Keyboard = function(char) {
    console.log("Keyboard is called");
    switch (char) {
      case 'a':
 
        break;
      case 's':

        break;
      case 'g':
        var g = new b2Vec2(0.000000000000000e+00, -1.000000000000000e+01);
        var og = new b2Vec2(0.000000000000000e+00, 0.000000000000000e+01);
        if (world.GetGravity() == og) {
            world.SetGravity(g);
        } else {
            world.SetGravity(og);
        }

        break;
      case 'd':

        break;
      case 'q':
        this.hz = Math.max(0, this.hz - 1);

        break;
      case 'e':
        this.hz += 1;
  
        break;
    }
};
  
TestMutualForce.prototype.Step = function() {
    console.log("step is called");

    this.mfc.Step();
    Step();
};
  