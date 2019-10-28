
/**@constructor*/
function b2MutualForceController() {
    this.body = null;
    this.buffer = null;
    this.ptr = null;
    this.shape = null;
  }
  
var b2MutualForceController_Step =
    Module.cwrap('b2MutualForceController_Step', 'number', ['number', 'number', 'number']);
var b2MutualForceController_AddGroup =
    Module.cwrap('b2MutualForceController_AddGroup', 'null', ['number']);

b2MutualForceController.prototype.Step = function() {
    b2MutualForceController_Step(this.ptr);
};

b2MutualForceController.prototype.AddGroup = function(p) {
    return b2MutualForceController_AddGroup(this.ptr, p.x, p.y);
};
  