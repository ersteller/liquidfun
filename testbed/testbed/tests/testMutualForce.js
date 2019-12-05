function TestMutualForce() {
  camera.position.y = 2;
  camera.position.z = 6;

  var bodyDef = new b2BodyDef();
  var ground = world.CreateBody(bodyDef);

  this.g = new b2Vec2(0.0, -1.0);
  this.og = new b2Vec2(0.0, 0.0);
  this.currg = this.og;
  world.SetGravity(this.currg);

  /* create fence */
  var chainShape = new b2ChainShape();
  chainShape.vertices.push(new b2Vec2(-2-2, 0-2));
  chainShape.vertices.push(new b2Vec2(2+2, 0-2));
  chainShape.vertices.push(new b2Vec2(2+2, 4+2));
  chainShape.vertices.push(new b2Vec2(-2-2, 4+2));

  chainShape.CreateLoop();
  ground.CreateFixtureFromShape(chainShape, 0);

  /* particel properties */
  var psd = new b2ParticleSystemDef();
  psd.radius = 0.025;
  psd.dampingStrength = 0.2;
  var particleSystem = world.CreateParticleSystem(psd);
  var pd = new b2ParticleGroupDef();

  /* shared linear velocity */
  var dt = 0.27;

  /* large body blob bottom left right move up */
  var shape = new b2PolygonShape;
  shape.SetAsBoxXYCenterAngle(0.8, 0.8, new b2Vec2(-0.0, 2.0), 0);
  pd.shape = shape;
  pd.linearVelocity = new b2Vec2(0.00e+00, dt);
  var group = particleSystem.CreateParticleGroup(pd);

  /* second body 8 times smaller blob top right move down */
  var shape2 = new b2PolygonShape;
  shape2.SetAsBoxXYCenterAngle(0.2, 0.4, new b2Vec2(1.8, 2.0), 0);
  pd.shape = shape2;
  pd.linearVelocity = new b2Vec2(0.00e+00, -dt * 8);
  var group2 = particleSystem.CreateParticleGroup(pd);


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

      if (this.currg == this.og) // world.GetGravity() == og) { not implemented
      {
          world.SetGravity(this.g);
          this.currg = this.g;
      } else {
          world.SetGravity(this.og);
          this.currg = this.og
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
